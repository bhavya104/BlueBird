#include <ArduinoJson.h>

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

BLEServer* pServer = NULL;
BLECharacteristic* pCharacteristic = NULL;
BLECharacteristic* pCharacteristicR = NULL;

bool deviceConnected = false;
bool oldDeviceConnected = false;

#define SERVICE_UUID        "afa2f203-348f-4718-9cf3-f7ff0de38472"
#define CHARACTERISTIC_UUID "450fcc81-dad5-46a6-959f-c5f41e37b669"

class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
      BLEDevice::startAdvertising();
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
    }
};

#define CHARACTERISTIC_UUID_RX "10fd2313-62c2-4053-b3af-7e6ca407bf21"
class MyCallbacks: public BLECharacteristicCallbacks {

    void onWrite(BLECharacteristic *pCharacteristic) {
      std::string rxValue = pCharacteristic->getValue();
      Serial.println(rxValue[0]);

      if (rxValue.length() > 0) {
        Serial.println("*********");
        Serial.print("Received Value: ");

        for (int i = 0; i < rxValue.length(); i++) {
          Serial.print(rxValue[i]);
        }
        Serial.println();
        Serial.println("*********");
      }

    }
};

// Simulate Payload
int simulate = 1;

void setup()
{
  Serial.begin(115200);
  setupBlueTooth();
}

/**
   setupBlueTooth
*/
void setupBlueTooth()
{
  // Create the BLE Device
  BLEDevice::init("BlueBird");

  // Create the BLE Server
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE Service
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Create a BLE Characteristic
  pCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID,
                      BLECharacteristic::PROPERTY_READ   |
                      BLECharacteristic::PROPERTY_WRITE  |
                      BLECharacteristic::PROPERTY_NOTIFY |
                      BLECharacteristic::PROPERTY_INDICATE
                    );

  // https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.descriptor.gatt.client_characteristic_configuration.xml
  // Create a BLE Descriptor
  pCharacteristic->addDescriptor(new BLE2902());

  /*
    pCharacteristicR = pService->createCharacteristic(
                                         CHARACTERISTIC_UUID_RX,
                                         BLECharacteristic::PROPERTY_WRITE
                                       );
    pCharacteristicR->setCallbacks(new MyCallbacks());
  */

  pCharacteristic->setCallbacks(new MyCallbacks());

  // Start the service
  pService->start();

  // Start advertising
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(false);
  pAdvertising->setMinPreferred(0x0);  // set value to 0x00 to not advertise this parameter
  BLEDevice::startAdvertising();
  Serial.println("Waiting a client connection to notify...");
}


void loop()
{

  if (simulate != 1)
  {
    //Functions for the future
  }
  else
  {
    jsonSimulation();
  }
}

// Simulating Sending JSON Data to BlueBird Platform
void jsonSimulation()
{
  const int bufferSize = JSON_OBJECT_SIZE(1) + JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3) + 2 * JSON_OBJECT_SIZE(4);
  DynamicJsonBuffer jsonBuffer(4000);

  JsonObject& root = jsonBuffer.createObject();

  JsonObject& civilian = root.createNestedObject("civilian");

  JsonObject& civilian_info   = civilian.createNestedObject("info");
  civilian_info["name"]       = "John Doe";
  civilian_info["phone"]      = random(1000000000, 9999999999);
  civilian_info["location"]   = "2725 E 14th St";
  civilian_info["occupants"]  = random(1, 10);

  int danger = random(0, 2);
  int vac    = 0;
  if (danger == 0)
  {
    vac++;
  }

  JsonObject& civilian_status = civilian.createNestedObject("status");
  civilian_status["danger"]   = danger;
  civilian_status["vacant"]   = vac;

  JsonObject& civilian_need   = civilian.createNestedObject("needs");
  civilian_need["first-aid"]  = random(0, 2);
  civilian_need["water"]      = random(0, 2);
  civilian_need["food"]       = random(0, 2);
  civilian["message"]         = "Please send help";

  String jsonData;
  root.printTo(jsonData);
  root.prettyPrintTo(Serial);

  size_t dataLen = jsonData.length();
  pCharacteristic->setValue((uint8_t*)&jsonData[0], dataLen);
  pCharacteristic->notify();

  delay(10000);
}
