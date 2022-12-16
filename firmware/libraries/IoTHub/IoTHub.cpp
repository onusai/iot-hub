#include <FS.h>
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiManager.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>
#include "base64.hpp"
#include "Fetch.h"
#include <string>
#include <IoTHub.h>

void saveConfigCallback();
bool shouldSaveConfig = false;
void (*callbackFunc)(String command, String data);

IoTHub::IoTHub(bool resetWiFi, bool resetFS)
  : mqttClient(wifiClient)
{
  _resetWiFi=resetWiFi;
  _resetFS=resetFS;
  //mqttClient = PubSubClient(wifiClient);
}

void IoTHub::start()
{   
  // init 
  WiFi.mode(WIFI_STA);
  
  // clean FS
  if (_resetFS) {
    SPIFFS.format();
  }

  loadConfig();

  WiFiManager wifiManager;
  wifiManager.setSaveConfigCallback(saveConfigCallback);
  if (_resetWiFi) wifiManager.resetSettings();
  
  // add custom parameters here
  WiFiManagerParameter param_connectionString("connectionString", "Connection String", connectionString, 200);
  wifiManager.addParameter(&param_connectionString);

  // set minimu quality of signal so it ignores AP's under that quality defaults to 8%
  //wifiManager.setMinimumSignalQuality();
  // sets timeout until configuration portal gets turned off useful to make it all retry or go to sleep in seconds
  //wifiManager.setTimeout(120);

  bool res;
  res = wifiManager.autoConnect("Setup IoT Device");
  if (!res) Serial.println("Failed to connect WiFi");
  else Serial.println("WiFi connected");
  
  if (shouldSaveConfig)
  {
    connectionString = (char*)param_connectionString.getValue();
    parseConnectionString();
    saveConfig();
  }

  Serial.println("LOADED DATA ============");
  Serial.println(hubAddress);
  Serial.println(collectionID);
  Serial.println(deviceID);

  connectToHub();
  connectToMQTT();
}

void IoTHub::loop()
{
    mqttClient.loop();
}

void saveConfigCallback()
{
  Serial.println("Should save config");
  shouldSaveConfig = true;
}

void IoTHub::loadConfig()
{
  // read configuration from FS json
  Serial.println("mounting FS...");
  if (SPIFFS.begin())
  {
    Serial.println("mounted file system");
    if (SPIFFS.exists("/config.json"))
    {
      // file exists, reading and loading
      Serial.println("reading config file");
      File configFile = SPIFFS.open("/config.json", "r");
      if (configFile)
      {
        Serial.println("opened config file");
        size_t size = configFile.size();
        // Allocate a buffer to store contents of the file.
        std::unique_ptr<char[]> buf(new char[size]);

        configFile.readBytes(buf.get(), size);
        DynamicJsonBuffer jsonBuffer;
        JsonObject &json = jsonBuffer.parseObject(buf.get());
        json.printTo(Serial);
        if (json.success())
        {
          Serial.println("\nparsed json");
          strcpy(hubAddress, json["hubAddress"]);
          strcpy(collectionID, json["collectionID"]);
          strcpy(deviceID, json["deviceID"]);
        }
        else
        {
          Serial.println("failed to load json config");
        }
      }
    }
  }
  else
  {
    Serial.println("failed to mount FS");
  }
}


void IoTHub::saveConfig()
{
  Serial.println("saving config");
  DynamicJsonBuffer jsonBuffer;
  JsonObject &json = jsonBuffer.createObject();
  json["hubAddress"] = hubAddress;
  json["collectionID"] = collectionID;
  json["deviceID"] = deviceID;

  File configFile = SPIFFS.open("/config.json", "w");
  if (!configFile)
  {
    Serial.println("failed to open config file for writing");
  }

  json.printTo(Serial);
  json.printTo(configFile);
  configFile.close();
  // end save
}


void IoTHub::parseConnectionString()
{
  std::string conStrB64full(connectionString);
  int len = atoi(conStrB64full.substr(0, 3).c_str());
  std::string conStrB64 = conStrB64full.substr(3, conStrB64full.length());
  //Serial.println(conStrB64full.c_str());

  uint8_t raw[BASE64::decodeLength(conStrB64.c_str())];
  BASE64::decode(conStrB64.c_str(), raw);
  std::string conStrJsonFull((char*)raw);
  std::string conStrJson = conStrJsonFull.substr(0, len);
  //Serial.println(conStrJson.c_str());

  DynamicJsonBuffer jsonBuffer;
  JsonObject &json = jsonBuffer.parseObject(conStrJson.c_str());
  json.printTo(Serial);
  if (json.success())
  {
    Serial.println("\nparsed json");
    strcpy(hubAddress, json["a"]);
    strcpy(collectionID, json["c"]);
    strcpy(deviceID, json["d"]);
  }
  else
  {
    Serial.println("failed to load json config");
  }
}


void IoTHub::connectToHub()
{
  RequestOptions options;
  options.method = "POST";
  options.headers["Content-Type"] = "application/json";

  String body = String("{\"collectionID\": \"") + collectionID + "\", \"deviceID\": \"" + deviceID + "\"}";
  options.body = body.c_str();
  Serial.println(body.c_str());
  
  String url = String("http://") + hubAddress + "/api/dev/register";
  Serial.println(url.c_str());
  
  Response response = fetch(url.c_str(), options);
  Serial.println(response);

  DynamicJsonBuffer jsonBuffer;
  JsonObject &json = jsonBuffer.parseObject(response.body.c_str());
  //json.printTo(Serial);
  if (json.success())
  {
    Serial.println("\nparsed json");
    strcpy(mqttAddress, json["MQTT_ADDRESS"]);
    strcpy(mqttPort, json["MQTT_PORT"]);
  }
  else
    Serial.println("failed to load json config");
}


void IoTHub::connectToMQTT()
{
  // Connect to MQTT
  mqttClient.setServer(mqttAddress, String(mqttPort).toInt());
  mqttClient.setCallback(mqttCallback);

  while (!mqttClient.connected()) {
    Serial.print("Connecting to MQTT...");
    if (mqttClient.connect("ESP8266Client")) {
      Serial.println(" OK.");  
    } else {
      Serial.print("failed with state ");
      Serial.print(mqttClient.state());
      delay(2000);
    }
  }

  mqttDataTopic = String("data/") + collectionID + "/" + deviceID;
  mqttCMDTopic = String("cmd/") + collectionID + "/" + deviceID;
  
  mqttClient.publish(mqttDataTopic.c_str(), "ESP8266_CONNECTED");
  mqttClient.subscribe(mqttCMDTopic.c_str());

  Serial.println(mqttDataTopic.c_str());  
  Serial.println(mqttCMDTopic.c_str());  
}

void IoTHub::mqttCallback(char* topic, byte* payload, unsigned int length) {
  char* cmd_raw = new char[length+1];
  strncpy(cmd_raw, (char*)payload, length);
  cmd_raw[length] = '\0';
  String cmd = String(cmd_raw);

  int idx = cmd.indexOf("/");

  String command = cmd;
  String data = "";
  if (idx != -1) {
    command = cmd.substring(0, idx);
    data = cmd.substring(idx+1, cmd.length());
  }
  
  callbackFunc(command, data);
  delete[] cmd_raw;
}

void IoTHub::setCallback(void (*pFunc)(String cmd, String data)){
  callbackFunc = pFunc;
}

void IoTHub::mqttPublish(char* payload)
{
  mqttClient.publish(mqttDataTopic.c_str(), payload);
}

void IoTHub::reset()
{
  WiFiManager wm;
  wm.resetSettings();
  ESP.restart();
}