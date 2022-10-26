#ifndef APPCONFIG_H_
#define APPCONFIG_H_

#define HOST_NAME "GN02475inv"

#include "../secrets.h"
#include <stdint.h>
#include <Arduino.h>
#include <driver/gpio.h>
#include "..\include\configtypes\enums.h"
#include "..\include\configtypes\configtypes.h"

struct PinsSettings
{
  const gpio_num_t led = (gpio_num_t)2;      // status led
  const gpio_num_t can0_rx = (gpio_num_t)32; // can0 transciever rx line
  const gpio_num_t can0_tx = (gpio_num_t)22; // can0 transciever tx line

#define SwitchCount 0
  SwitchConfig switches[SwitchCount] = {};

#define SensorCount 0
  SensorConfig sensors[SensorCount] = {};
  //     {devicet::adc_motor1, "tempm1", 32, sensort::adc}, // motor temp sensor 1
  //     {devicet::adc_motor2, "tempm2", 33, sensort::adc}  // motor temp sensor 2
  // };

  int getSensorIndex(const char *name)
  {
    for (size_t i = 0; i < SensorCount; i++)
    {
      if (strcmp(sensors[i].name, name) == 0)
        return i;
    }
  }

  int getSensorIndex(devicet device)
  {
    for (size_t i = 0; i < SensorCount; i++)
    {
      if (sensors[i].device == device)
        return i;
    }
  }

  int getSwitchIndex(devicet device)
  {
    for (size_t i = 0; i < SwitchCount; i++)
    {
      if (switches[i].device == device)
        return i;
    }
  }
};

struct Intervals
{
  int statusPublish = 1000;     // interval at which status is published to MQTT
  int Can2Mqtt = 1000;          // send CAN messages to MQTT every n secconds. Accumulate messages until. Set this to 0 for forwarding all CAN messages to MQTT as they are received.
  int CANsend = 10;             // interval at which to send CAN Messages to car bus network (milliseconds)
  int outChannelsPublish = 500; // interval at which status is published to MQTT out channels
  int click_onceDelay = 1000;   // milliseconds
};

struct BrakesSettings
{
  uint16_t vacuum_min = 2519; // ADC value - turn pump on cca 1.7V
  uint16_t vacuum_max = 1350; // ADC value - turn pump off cca 1.4V
};

extern PinsSettings pinsSettings;
extern Intervals intervals;
extern BrakesSettings brakesSettings;

#endif /* APPCONFIG_H_ */
