#ifndef STATUS_H_
#define STATUS_H_

#include <stdint.h>
#include <Arduino.h>

struct Status
{
  long loops = 0;
  long missedSend = 0;
  long bootedMillis = 0;
  long currentMillis = 0;
  uint32_t freeMem = 0;
  uint32_t minFreeMem = 0;
  const char *ipAddress = "255.255.255.255";
  const char *gatewayAddress = "255.255.255.255";
  String SSID = "";
  int8_t rssi = 0;
  double tempm1 = 0; // motor temperature 1
  double tempm2 = 0; // motor temperature 2
  long receivedCount = 0;

  int sensors[SensorCount];
  int switches[SwitchCount];

  char inverterSend[128] = {};
  const char *response = ""; // response to above command
};

extern Status status;

#endif /* STATUS_H_ */
