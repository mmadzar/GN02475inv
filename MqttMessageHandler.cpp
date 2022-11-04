#include "Arduino.h"
#include "MqttMessageHandler.h"

MqttMessageHandler::MqttMessageHandler()
{
}

void MqttMessageHandler::HandleMessage(const char *topic, const char *message)
{
  Serial.printf("%s %s", topic, message);
}
