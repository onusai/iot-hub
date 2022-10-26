let mqtt = require('mqtt');
const io = require('./socket.js').getio();

let broker_addr = 'mqtt://localhost'
let base_topic = 'esp'

let topic_sub = `${base_topic}/data`
let topic_pub = `${base_topic}/cmd`

let client = mqtt.connect(broker_addr)

let allowed_cmds = ['SIG_ON', 'SIG_OFF', 'SIG_STATUS', 'SIG_RESTART', 'SIG_RESET'];

client.on('connect', () => {
    console.log(`MQTT connnected on: ${broker_addr}`);

    client.subscribe(topic_sub, err => {
        if (err) console.log(err);
        else console.log(`MQTT subscribed to: ${topic_sub}`);
    });

    client.subscribe(topic_pub, err => {
        if (err) console.log(err);
        else console.log(`MQTT subscribed to: ${topic_pub}`);
    });
    
});

client.on('message', (topic, message) => {
    message = String(message);
    if (topic == "esp/cmd") {
        io.emit('esp/cmd', message);
    }
    else if (topic == "esp/data") {
        io.emit('esp/data', message);
    }
});

let topics = {topic_sub, topic_pub};

module.exports = [client, topics, allowed_cmds];



// reference: https://dev.t-matix.com/blog/platform/itwo-way-communication-with-iot-devices-using-mqtt/