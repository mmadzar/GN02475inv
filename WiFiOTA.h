#ifndef WIFIOTA_H_
#define WIFIOTA_H_

#include <WiFi.h>
#include <ArduinoOTA.h>
#include "appconfig.h"
#include "status.h"

class WiFiOTA
{
private:
  long lastOTAmillis = 0;
  static void WiFiEvent(WiFiEvent_t event);
  void checkConnectedDevice();
  void checkDisconnectedDevice();

public:
  WiFiOTA();
  void setupWiFi();
  void ReconnectWiFi();
  void setupOTA();
  void handleWiFi();
  void handleOTA();
};

#endif /* WIFIOTA_H_ */
