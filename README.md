# IoT Hub
IoT Hub is an end-to-end solution for creating and managing custom IoT devices over WiFi & Internet

<img src="https://user-images.githubusercontent.com/18707771/208439253-e7ee7e56-9975-456f-abb3-4e3fcca5c185.png" alt="IoTHub WebUI" width="70%">

## Features
* IoT Library
    * Supported hardware: ESP8266,  ESP32, Arduino
    * Provides a simple communication interface to the management server / web portal
    * Dynamic device registration
        * No hardcoding Wi-Fi credentials, server address, or device token
* Web Server 
    * Manages registration and communication with IoT devices
    * Managment UI / Web Portal
        * Monitor and control connected IoT devices
        * Define custom UI controls for your IoT project using UI template system  

## Server Requirements
- [NodeJS](https://nodejs.org/en/download/) installed on your system
- MQTT server
   - If you don't have one, install [Mosquitto](https://mosquitto.org/download/)
- MySQL server
   - If you dont have one, install [MariaDB](https://www.digitalocean.com/community/tutorials/how-to-install-mariadb-on-ubuntu-20-04)

## Installation (ubuntu)
Although this application should work on any platform that supports NodeJS, it was only tested on Ubuntu and WSL

1. Go to the [Releases](https://github.com/onusai/iot-hub/releases) page and download the latest version of the `iothub_vX.X.zip`
2. Extract the file and open the `iothub` folder
3. Edit server configuration
   - Open the `.env` file in a text editor
   - Fill in the empty variables and optionally edit the other variables to your needs
4. Install server dependencies
```bash
cd iothub/server
npm install
```
5. Ensure that your MySQL server is running
6. Ensure that your MQTT server is running
   - If you are using Mosquitto, open another terminal session and run the `start_mqtt_broker.sh` script
   - If you wish to change the port, you must change it in both `mqtt_config.txt` and `.env` files

## Start server
To start the server, execute the `run.sh` script

## Architecture
<img src="https://user-images.githubusercontent.com/18707771/208431701-96432654-76ed-4468-bc47-51bd30a672a5.png" alt="Architecture" width="35%">
