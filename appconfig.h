#ifndef APPCONFIG_H_
#define APPCONFIG_H_

#define HOST_NAME "GN02475inv"

#include "../secrets.h"
#include <stdint.h>
#include <Arduino.h>
#include <driver/gpio.h>

struct Settings
{
  const gpio_num_t led = (gpio_num_t)2; // status led
#define ListenChannelsCount 0
  const char *listenChannels[ListenChannelsCount] = {};
};

struct Intervals
{
  int statusPublish = 1000; // interval at which status is published to MQTT
};

extern Settings settings;
extern Intervals intervals;

#endif /* APPCONFIG_H_ */
