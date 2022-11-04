#ifndef MQTTMESSAGEHANDLER_H_
#define MQTTMESSAGEHANDLER_H_

class MqttMessageHandler
{

public:
    MqttMessageHandler();
    void static HandleMessage(const char *command, const char *message);
};

#endif /* MQTTMESSAGEHANDLER_H_ */
