#include <IoTHub.h>

// variables
bool is_led_on = false;

// setup
void mqttCallback(String cmd, String data);
IoTHub iotClient;

void setup()
{
  Serial.begin(115200);

  // initilize IoTHub
  iotClient.setCallback(mqttCallback);
  iotClient.start();
  
  // initilize pins
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  // other code goes here
}

void loop()
{
  // needs to be called every loop
  iotClient.loop();

  // other code goes here
}

// recieves MQTT commands
void mqttCallback(String cmd, String data)
{
  Serial.println("> " + cmd);

  if (cmd.equals("SIG_ON")) {
    digitalWrite(LED_BUILTIN, LOW);
    is_led_on = true;
    iotClient.mqttPublish("SIG_STATUS/ON");
  }

  else if (cmd.equals("SIG_OFF")) {
    digitalWrite(LED_BUILTIN, HIGH);
    is_led_on = false;
    iotClient.mqttPublish("SIG_STATUS/OFF");
  }

  else if (cmd.equals("SIG_STATUS")) {
    if (is_led_on) iotClient.mqttPublish("SIG_STATUS/ON");
    else iotClient.mqttPublish("SIG_STATUS/OFF");
  }

  else if (cmd.equals("SIG_RESTART")) {
    ESP.restart();
  }

  else if (cmd.equals("SIG_RESET")) {
    iotClient.reset();
  }

  else if (cmd.equals("SIG_BRIGHTNESS")) {
    int brightness = (int)(255-((data.toFloat()/100.0f)*255.0f));
    Serial.println("- Set Brightness: " + String(brightness));
    analogWrite(LED_BUILTIN, brightness);
    if (data.toInt() > 0) {
      is_led_on = true;
      iotClient.mqttPublish("SIG_STATUS/ON");
    } else {
      is_led_on = false;
      iotClient.mqttPublish("SIG_STATUS/OFF");
    }
  }

  else {
    Serial.println(" - ERROR: Undefined Command");
  }
}
