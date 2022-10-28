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
  int8_t rssi = 0;

  long receivedCount = 0;

  int sensors[SensorCount];
  int switches[SwitchCount];

  char inverterSend[512] = {};
  const char *response = ""; // response to above command
};

extern Status status;

#endif /* STATUS_H_ */
