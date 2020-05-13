/**
 * Connects to a BLE device, updates the UI, and starts a timer
 * to take continuous measurements every 5 seconds.
 */
async function connect() {
    if (!this.connected) {
        try {
          this.device = await navigator.bluetooth.requestDevice({
            filters: [
              {
                namePrefix: "Papa"
              }
            ],
            optionalServices: [this.serviceUuid]
          });
          this.device_name = this.device.name;
          const server = await this.device.gatt.connect();
          this.connected = true;

          service = await server.getPrimaryService(this.serviceUuid);
          characteristic = await service.getCharacteristic(this.txUuid);
          await characteristic.startNotifications();
          characteristic.addEventListener(
            "characteristicvaluechanged",
            this.value_update
          );
          characteristic = await service.getCharacteristic(this.rxUuid);
          await characteristic.writeValue(encoder.encode("papa"));
          await new Promise(resolve => setTimeout(resolve, 10));

          this.measure();

          setInterval(this.measure, 5000);
        } catch (error) {
          this.switchConnected = false;
          alert(error);
        }
      } else {
        this.disconnect();
        this.switchConnected = false;
        characteristic = await service.getCharacteristic(this.rxUuid);
        await characteristic.writeValue(encoder.encode("disconnect"));
      }
}

/**
 * Disconnects from the BLE device and resets the UI
 */
function disconnect() {
  app.device.gatt.disconnect();
  this.device = null;
  this.connected = false;
  this.device_name = "";
  this.papa = "-";
  this.temp = "-";
  this.status = "not connected";
  this.papa_low_ref = "-";
  this.papa_low_read = "-";
  this.papa_high_ref = "-";
  this.papa_high_read = "-";
  this.papa_offset = "-";
  this.papa_temp_constant = "-";
  this.papa_temp_coefficient = "-";
  this.papa_connected = false;
  this.switchConnected = false;
}