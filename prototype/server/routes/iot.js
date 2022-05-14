let express = require('express');
[ mqtt_client, topics, allowed_cmds ] = require('../mqtt_client');

let router = express.Router();

router.get('/', (req, res) => {
    let cmd = req.query.cmd;

    if (allowed_cmds.includes(cmd)) {
        mqtt_client.publish(topics.topic_pub, cmd);
        res.send(`> ${topics.topic_pub} : ${cmd}`)
    } else {
        res.send("> Invalid command");
    }
    
});

module.exports = router;