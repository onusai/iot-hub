#include <FS.h>
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiManager.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>
#include "base64.hpp"
#include "Fetch.h"
#include <string>

#ifndef IoTHub_h
#define IoTHub_h

class IoTHub
{
    public:
        WiFiClient wifiClient;
        PubSubClient mqttClient;

        char hubAddress[50];
        char collectionID[50];
        char deviceID[50];
        char* connectionString;
        char mqttAddress[50];
        char mqttPort[10];
        String mqttDataTopic;
        String mqttCMDTopic;
        bool _resetWiFi;
        bool _resetFS;

        // function prototypes
        IoTHub(bool resetWiFi=false, bool resetFS=false);
        void start();
        void loop();
        void parseConnectionString();
        static void mqttCallback(char* topic, byte* payload, unsigned int length);
        void loadConfig();
        void saveConfig();
        void connectToHub();
        void connectToMQTT();
        void setCallback(void (*pFunc)(String cmd, String data));
        void mqttPublish(char* payload);
        void reset();
};

#endif