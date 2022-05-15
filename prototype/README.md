# Ubuntu instructions

### 1. Install prerequisites
* NodeJs & NPM: https://nodejs.org/en/download/
* Mosquitto MQTT Broker: https://mosquitto.org/download/
* Arduino IDE: https://www.arduino.cc/en/software

### 2. Launch MQTT broker
1. Open a new terminal and change directory to `prototype/mosquitto`
2. Run `./start_broker.sh`
    * or `mosquitto -c config.txt -v`

### 3. Launch NodeJs server
1. Open a new terminal and change directory to `prototype/server`
2. Run `npm install` to download dependencies
3. Run `npm run start` to launch server
4. Connect to the web client in your browser on `http://localhost:8080`

### 4. Upload sketch to microcontroller
Tested with ESP8266. I assume ESP32 will also work, not sure about arduino boards.
1. Launch Arduino IDE
2. Open the `mqtt_wifi_test.ino` file located in `prototype/library/mqtt_wifi_test/` 
3. Edit line 7 with the ip address of your MQTT server. 
    * If the microcontroller and server are on the same network, use the local ip of the computer that hosts mqtt
4. Upload sketch to the microcontroller

### 5. Configure microcontroller WiFi
1. Open WiFi settings on your phone
2. Connect to WiFi network with name `Setup IoT Device`
3. Open the 'sign in' web portal
    * If it doesnt automatially open or theres no button to open it, go to `192.168.4.1` in a browser 
4. Press `Configure WiFi` and select the WiFi you want the microcontroler to use

### 6. Send commands to the micrcontroller using web portal
1. In the web portal, check if the `esp/data` log shows a connection from the micrcontroller
2. Press the buttons on the left to send commands to the micrcontroller
    * `SIG_ON`
        * Turns on the builtin LED
    * `SIG_OFF`
        * Turns off the builtin LED
    * `SIG_STATUS`
        * Requests LED state to be published in topic `esp/data`
    * `SIG_RESTART`
        * Reboot device
    * `SIG_RESET`
        * Erase stored data (wifi credentials) and restart