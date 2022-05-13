let mqtt = require('mqtt');

let broker_addr = 'mqtt://localhost'

let base_topic = 'esp'

let topic_sub = `${base_topic}/data`
let topic_pub = `${base_topic}/cmd`

let client = mqtt.connect(broker_addr)

client.on('connect', () => {
    console.log(`connnected to: ${broker_addr}`);

    client.subscribe(topic_sub, err => {
        if (err) console.log(err);
        else console.log(`subscribed to: ${topic_sub}`);
    });
    
});

client.on('message', (topic, message) => {
    console.log(message.toString());
});


// reference
// https://dev.t-matix.com/blog/platform/itwo-way-communication-with-iot-devices-using-mqtt/