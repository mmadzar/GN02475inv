#ifndef APPCONFIG_H_
#define APPCONFIG_H_

#define HOST_NAME "GN02475inv"

#include "../secrets.h"
#include <stdint.h>
#include <Arduino.h>

struct PinsSettings
{
  const gpio_num_t led = (gpio_num_t)2;      // status led
};

struct Intervals
{
  int statusPublish = 1000;     // interval at which status is published to MQTT
};

extern PinsSettings pinsSettings;
extern Intervals intervals;

#endif /* APPCONFIG_H_ */
