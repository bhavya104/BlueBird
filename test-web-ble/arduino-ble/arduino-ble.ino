#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

BLEServer* pServer = NULL;
BLECharacteristic* pCharacteristic = NULL;
BLECharacteristic* pCharacteristicR = NULL;
bool deviceConnected = false;
bool oldDeviceConnected = false;
uint32_t value = 0;

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

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


void setup() {
  Serial.begin(115200);

  // Create the BLE Device
  BLEDevice::init("ESP32");

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

  //pCharacteristicR->addDescriptor(new BLE2902());
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

uint8_t level = 97;

void loop() {
  
  String bleStatus = "status";
  size_t dataLen = bleStatus.length();
  pCharacteristic->setValue((uint8_t*)&bleStatus[0], dataLen);
  pCharacteristic->notify(); 
//  
  
   
  pCharacteristic->setValue(&level, 2); // size : 8 bit -> 1 byte, this only accepts uint8_t
  pCharacteristic->notify(); // send first 20 bytes
  level++;

  String longString = "luwheri38iuwhh13fh38r4chfwiho132dh83hfobjbjbjbjbjbjbjbjbjjbjbjbjbjbjbjbjbj";
  pCharacteristic->setValue((uint8_t*)&longString[0], longString.length()); // size : 8 bit -> 1 byte, this only accepts uint8_t
  pCharacteristic->notify();

  delay(1000); 

}