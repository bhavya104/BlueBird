let decoder = new TextDecoder("utf-8");
let encoder = new TextEncoder("utf-8");

var app = new Vue({
  el: "#app",
  data: {
    device: 0,
    service: 0,
    characteristic: 0,
    connected: false,
   papa: "-",
   papa_unit: "test",
    device_name: "Papa",
   papa_connected: false,
   papa_low_ref: "-",
   papa_low_read: "-",
    switchConnected: false, 
    serviceUuid: "7549e55b-b314-4923-aa27-1403720b9cf4",
    txUuid: "b6ab8bdf-c704-49ec-83b1-bfca810c35f6",
    rxUuid: "b6ab8bdf-c704-49ec-83b1-bfca810c35f6"
  },
  mounted: function () {
    if ('serviceWorker' in navigator) {
      navigator.serviceWorker.register('service-worker.js');
    }
  },
  methods: {
    connect,
    measure: async function(event){
      characteristic = await service.getCharacteristic(this.rxUuid);
      if (this.papa_connected){
        await characteristic.writeValue(encoder.encode("papa"));
      }


    },
    value_update: async function(event) {
      let value = event.target.value;
      value = decoder.decode(value);
      let obj = JSON.parse(value);

      if (obj.hasOwnProperty("papa")) {
        this.papa_connected = obj.papa;
      }
    },
     papa_config,
     papa_set_offset,
     papa_low,
     papa_reset,
    disconnect
  }
});
