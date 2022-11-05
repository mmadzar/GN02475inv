#include "Arduino.h"
#include "status.h"
#include "MqttMessageHandler.h"

MqttMessageHandler::MqttMessageHandler()
{
}

void MqttMessageHandler::HandleMessage(const char *command, const char *message)
{
  if (strcmp(command, "inverter") == 0)
  {
    //store into status variable for sending to inverter in next loop cycle
    String msg(message);
    msg.toCharArray(status.inverterSend, msg.length() + 1);
  }
}
