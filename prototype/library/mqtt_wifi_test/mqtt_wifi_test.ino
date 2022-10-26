
#include <WiFiManager.h>
#include <PubSubClient.h>

// configiure settings
const char* apSSID = "Setup IoT Device";
const char* mqttServer = "192.168.0.3";
const int mqttPort = 1883;
// const char* mqttUser = "";
// const char* mqttPassword = "";


bool is_led_on = false;

void callback(char* topic, byte* payload, unsigned int length);

WiFiClient wifiClient;
PubSubClient client(wifiClient);


void setup() {

  // Initialize pins
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  // Initialize WiFi and serial
  WiFi.mode(WIFI_STA);
  Serial.begin(115200);
  WiFiManager wm;
  
  
  // Connect to WiFi
  bool res;
  res = wm.autoConnect(apSSID);
  if (!res) Serial.println("Failed to connect WiFi");
  else Serial.println("WiFi connected");

  // Connect to MQTT
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);

  while (!client.connected()) {
    Serial.print("Connecting to MQTT...");
    if (client.connect("ESP8266Client")) {
      Serial.println(" OK.");  
    } else {
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
    }
  }

  client.publish("esp/data", "ESP8266_CONNECTED");
  client.subscribe("esp/cmd");
}


void loop() {
  client.loop();
}


// handles MQTT subscription
void callback(char* topic, byte* payload, unsigned int length) {

  Serial.print("> ");
  Serial.print(topic);

  // convert payload to String
  char* cmd_raw = new char[length+1];
  strncpy(cmd_raw, (char*)payload, length);
  cmd_raw[length] = '\0';
  String cmd = String(cmd_raw);
  
  Serial.print(" : " + cmd);

  // handle cmd
  if (cmd.equals("SIG_ON")) {
    digitalWrite(LED_BUILTIN, LOW);
    is_led_on = true;
  }
  else if (cmd.equals("SIG_OFF")) {
    digitalWrite(LED_BUILTIN, HIGH);
    is_led_on = false;
  }
  else if (cmd.equals("SIG_STATUS")) {
    if (is_led_on) client.publish("esp/data", "LED_STATUS: ON");
    else client.publish("esp/data", "LED_STATUS: OFF");
  }
  else if (cmd.equals("SIG_RESTART")) {
    ESP.restart();
  }
  else if (cmd.equals("SIG_RESET")) {
    WiFiManager wm;
    wm.resetSettings();
    ESP.restart();
  }
  else {
      Serial.print(" - ERROR: Undefined");
  }
 
  Serial.println();
  delete[] cmd_raw;
}


// reference https://techtutorialsx.com/2017/04/09/esp8266-connecting-to-mqtt-broker/
