#ifndef MQTTMESSAGEHANDLER_H_
#define MQTTMESSAGEHANDLER_H_

class MqttMessageHandler
{

public:
    MqttMessageHandler();
    void handle();
    static void HandleMessage(const char *topic, const char *message, int length);
    static void callback(char *topic, byte *message, unsigned int length);
};

#endif /* MQTTMESSAGEHANDLER_H_ */
