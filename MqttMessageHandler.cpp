#include "Arduino.h"
#include "status.h"
#include "MqttMessageHandler.h"

MqttMessageHandler::MqttMessageHandler()
{
}

void MqttMessageHandler::HandleMessage(const char *command, const char *message, int length)
{
  if (strcmp(command, "inverter") == 0)
  {
    // store into status variable for sending to inverter in next loop cycle
    String msg(message);
    msg.toCharArray(status.inverterSend, length);
  }
  else if (strcmp(command, "queryInverterInterval") == 0)
  {
    status.queryInverterInterval = String(message).toInt();
  }
}

void MqttMessageHandler::callback(char *topic, byte *message, unsigned int length)
{
}

void MqttMessageHandler::handle()
{
}