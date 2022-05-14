# Ubuntu instructions



### Install mosquitto https://mosquitto.org/download/
```
sudo apt-add-repository ppa:mosquitto-dev/mosquitto-ppa
apt install mosquitto
```

### Start mosquitto broker
```
cd prototype/mosquitto
./start_broker.sh
```


### To interact with mosquitto from terminal, install mosquitto-client
```
apt get install mosqutto-client
```

### Subscribe to topic
`mosquitto_sub -h <ip> -t <topic>`
```
mosquitto_sub -h localhost -t esp/data`
```

### Publish to topic
`mosquitto_pub -h <ip> -t <topic> -m "Hello"`
```
mosquitto_pub -h localhost -t esp/cmd -m SIG_ON
```


### Accepted parameters in this prototype
* topic `esp/cmd`
    * publish messages to iot device
        * `SIG_ON`
            * turns the builtin LED on
        * `SIG_OFF`
            * turns the builtin LED off
        * `SIG_STATUS`
            * iot device to publish led status to topic esp/data
        * `SIG_RESTART`
            * reboot iot device
        * `SIG_RESET`
            * clear data (wifi credentials) and restart
* topic `esp/data`
    * subscribe to recieve data from iot device