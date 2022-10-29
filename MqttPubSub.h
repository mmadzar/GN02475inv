#ifndef MQTTPUBSUB_H_
#define MQTTPUBSUB_H_

#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "appconfig.h"
#include "status.h"

class MqttPubSub
{
public:
  PubSubClient client;
  PinsSettings pinsSettings;
  int lastMillis = 0;
  MqttPubSub();
  void publishStatus(bool waitForInterval);
  void setup();
  void handle();
  void sendMessage(String message);
  void sendMesssageToTopic(const char *topic, const char *message);
  void sendMessage(String message, String channel);
  void sendMesssageToTopic(const char *topic, String message);

private:
  StaticJsonDocument<512> doc;
  char tempBuffer[512];

  long lastReconnectAttempt = -10000;
  char channelStatus[50];
  char channelRaw[50];
  char channelIn[50];
  char channelOut[50];
  int idx1;
  int idx2;
  long bytesCount = 0;

  const char *hostname;
  const char *currentMqttSettings[5];
  WiFiClient espClient;
  bool connect(const char *id, const char *username, const char *password);
  bool reconnect();
  static void callback(char *topic, byte *message, unsigned int length);
};

#endif /* MQTTPUBSUB_H_ */
