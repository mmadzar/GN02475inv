// FCEC8C33F14FCDC64BCCE53E365E7
// {[w$FeAAkf{aov/
// ZzFyTv8/gsm0qf8+1YF9 - open-ssl rand 15 chars base64 encoded - total 20 chars
// Vem6G0L/RfeQtl5k0BLB

#include <WiFi.h>
#include <WiFiMulti.h>
#include <esp_wifi.h>
#include <ESPmDNS.h>
#include <ArduinoOTA.h>
#include "WiFiOTA.h"

WiFiMulti WiFiMulti;
wifi_sta_list_t wifi_sta_list;
tcpip_adapter_sta_list_t adapter_sta_list;
long lastReconnect = 0;
bool connectionInProgress = false;
char *buff = new char[50];
char *buff2 = new char[50];
IPAddress ipA;
IPAddress ipAG;
WiFiOTA::WiFiOTA()
{
}

void WiFiOTA::setupWiFi()
{
  esp_wifi_set_storage(WIFI_STORAGE_RAM);
  for (int i = 0; i < wifiSettings.APsCount; i++)
  {
    Serial.println(wifiSettings.APs[i][0]);
    WiFiMulti.addAP(wifiSettings.APs[i][0], wifiSettings.APs[i][1], wifiSettings.APs[i][2]);
  }

  Serial.print("Conecting to WiFi... ");
  WiFi.disconnect(false, false);

  WiFi.onEvent(WiFiEvent);
  ReconnectWiFi();
}

void WiFiOTA::ReconnectWiFi()
{
  WiFiMulti.run();
}

void WiFiOTA::handleWiFi()
{
  wl_status_t ws;
  ws = WiFi.status();
  if ((ws == WL_DISCONNECTED || ws == WL_CONNECTION_LOST) && !connectionInProgress && status.currentMillis - lastReconnect > 5000)
  {
    lastReconnect = status.currentMillis;
    connectionInProgress = true;
    if (ws != WL_DISCONNECTED)
    {
      Serial.print("disconnecting...");
      WiFi.disconnect(false);
    }
    WiFi.persistent(false);
    Serial.print("reconnecting wifi... timeout: ");
    Serial.println(ws);
    Serial.println(status.currentMillis - lastReconnect > 5000);
    ReconnectWiFi();
  }
}

void WiFiOTA::setupOTA()
{
  // OTA configuration
  // Port defaults to 3232
  // ArduinoOTA.setPort(3232);
  ArduinoOTA.setHostname(wifiSettings.hostname);
  ArduinoOTA.setPassword(wifiSettings.passwordOTA);
  ArduinoOTA
      .onStart([]()
               {
                 String type;
                 if (ArduinoOTA.getCommand() == U_FLASH)
                   type = "sketch";
                 else // U_SPIFFS
                   type = "filesystem";

                 // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
                 Serial.println("Start updating " + type); })
      .onEnd([]()
             { Serial.println("\nEnd. Restarting..."); })
      .onProgress([](unsigned int progress, unsigned int total)
                  { Serial.printf("Progress: %u%%\r", (progress / (total / 100))); })
      .onError([](ota_error_t error)
               {
                 Serial.printf("Error[%u]: ", error);
                 if (error == OTA_AUTH_ERROR)
                   Serial.println("Auth Failed");
                 else if (error == OTA_BEGIN_ERROR)
                   Serial.println("Begin Failed");
                 else if (error == OTA_CONNECT_ERROR)
                   Serial.println("Connect Failed");
                 else if (error == OTA_RECEIVE_ERROR)
                   Serial.println("Receive Failed");
                 else if (error == OTA_END_ERROR)
                   Serial.println("End Failed"); });

  ArduinoOTA.begin();

  // //blink for success
  // digitalWrite(pinsSett.led, HIGH);
  // delay(100);
  // digitalWrite(pinsSett.led, LOW);
}

void WiFiOTA::handleOTA()
{
  if ((status.currentMillis - lastOTAmillis) > 1000)
  {
    ArduinoOTA.handle();
    lastOTAmillis = status.currentMillis;
  }
}

void WiFiOTA::WiFiEvent(WiFiEvent_t event)
{
  Serial.printf("[WiFi-event] event: %d\n", event);

  switch (event)
  {
  case SYSTEM_EVENT_WIFI_READY:
    digitalWrite(2, HIGH);
    Serial.println("WiFi interface ready");
    break;
  case SYSTEM_EVENT_SCAN_DONE:
    Serial.println("Completed scan for access points");
    Serial.println(WiFi.SSID());
    connectionInProgress = false;
    break;
  case SYSTEM_EVENT_STA_START:
    Serial.println("WiFi client started");
    Serial.println(WiFi.SSID());
    break;
  case SYSTEM_EVENT_STA_STOP:
    Serial.println("WiFi clients stopped");
    WiFiMulti.run();
    Serial.println(WiFi.SSID());
    break;
  case SYSTEM_EVENT_STA_CONNECTED:
    Serial.print("Connected to access point ");
    Serial.println(WiFi.SSID());
    connectionInProgress = false;
    break;
  case SYSTEM_EVENT_STA_DISCONNECTED:
    Serial.println("Disconnected from WiFi access point");
    Serial.println(WiFi.SSID());
    digitalWrite(2, HIGH);
    status.ipAddress = "none";
    status.gatewayAddress = "255.255.255.255";
    connectionInProgress = false;
    // WiFi.persistent(false);
    // WiFi.disconnect(false, false);
    // WiFi.persistent(false);
    // for (int i = 0; i < wifiSettings.APsCount; i++)
    // {
    //   WiFiMulti.addAP(wifiSettings.APs[i][0], wifiSettings.APs[i][1]);
    // }
    // //Serial.println(wifiSett.APs[0][0]);
    // ReconnectWiFi();
    break;
  case SYSTEM_EVENT_STA_AUTHMODE_CHANGE:
    Serial.println("Authentication mode of access point has changed");
    Serial.println(WiFi.SSID());
    break;
  case SYSTEM_EVENT_STA_GOT_IP:
    Serial.print("IP address: ");
    ipA = WiFi.localIP();
    sprintf(buff, "%d.%d.%d.%d", ipA[0], ipA[1], ipA[2], ipA[3]);
    status.ipAddress = buff;
    Serial.println(status.ipAddress);
    
    ipAG = WiFi.gatewayIP();
    sprintf(buff2, "%d.%d.%d.%d", ipAG[0], ipAG[1], ipAG[2], ipAG[3]);
    status.gatewayAddress = buff2;
    Serial.print(status.ipAddress);
    Serial.print(" gateway: ");
    Serial.println(status.gatewayAddress);
    digitalWrite(2, LOW);
    status.rssi = WiFi.RSSI();
    Serial.print("SSID: ");
    Serial.print(WiFi.SSID());
    Serial.print(" RSSI: ");
    Serial.println(status.rssi);
    Serial.println("...");
    break;
  case SYSTEM_EVENT_STA_LOST_IP:
    Serial.println("Lost IP address and IP address is reset to 0");
    Serial.println(WiFi.SSID());
    connectionInProgress = false;
    break;
  case SYSTEM_EVENT_STA_WPS_ER_SUCCESS:
    Serial.println("WiFi Protected Setup (WPS): succeeded in enrollee mode");
    Serial.println(WiFi.SSID());
    break;
  case SYSTEM_EVENT_STA_WPS_ER_FAILED:
    Serial.println("WiFi Protected Setup (WPS): failed in enrollee mode");
    Serial.println(WiFi.SSID());
    break;
  case SYSTEM_EVENT_STA_WPS_ER_TIMEOUT:
    Serial.println("WiFi Protected Setup (WPS): timeout in enrollee mode");
    Serial.println(WiFi.SSID());
    break;
  case SYSTEM_EVENT_STA_WPS_ER_PIN:
    Serial.println("WiFi Protected Setup (WPS): pin code in enrollee mode");
    break;
  case SYSTEM_EVENT_AP_START:
    Serial.println("WiFi access point started");
    break;
  case SYSTEM_EVENT_AP_STOP:
    Serial.println("WiFi access point  stopped");
    break;
  case SYSTEM_EVENT_AP_STACONNECTED:
    Serial.println("Client connected");
    break;
  case SYSTEM_EVENT_AP_STADISCONNECTED:
    Serial.println("Client disconnected");
    break;
  case SYSTEM_EVENT_AP_STAIPASSIGNED:
    Serial.println("Assigned IP address to client");
    break;
  case SYSTEM_EVENT_AP_PROBEREQRECVED:
    Serial.println("Received probe request");
    break;
  case SYSTEM_EVENT_GOT_IP6:
    Serial.println("IPv6 is preferred");
    break;
  case SYSTEM_EVENT_ETH_START:
    Serial.println("Ethernet started");
    break;
  case SYSTEM_EVENT_ETH_STOP:
    Serial.println("Ethernet stopped");
    break;
  case SYSTEM_EVENT_ETH_CONNECTED:
    Serial.println("Ethernet connected");
    break;
  case SYSTEM_EVENT_ETH_DISCONNECTED:
    Serial.println("Ethernet disconnected");
    break;
  case SYSTEM_EVENT_ETH_GOT_IP:
    Serial.println("Obtained IP address");
    break;
  default:
    break;
  }
}
