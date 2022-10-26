let mqttClient;
module.exports = {
  init: function () {
    // start mqtt client and cache io value
    let broker_addr = `mqtt://${process.env.MQTT_ADDRESS}`;
    const mqtt = require("mqtt");
    mqttClient = mqtt.connect(broker_addr);
    return mqttClient;
  },
  get: function () {
    // return previously cached value
    if (!mqttClient) {
      throw new Error("must call .init() before you can call .get()");
    }
    return mqttClient;
  },
};
