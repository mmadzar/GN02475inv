#ifndef STATUS_H_
#define STATUS_H_

#include "shared/status_base.h"

class Status : public StatusBase
{
public:
  double tempm1 = 0;               // motor temperature 1
  double tempm2 = 0;               // motor temperature 2
  int baudRate = 9600;             // inverter comms baudrate
  int queryInverterInterval = 500; // 300 milliseconds between inverter queries - 0 disabled
  char inverterSend[128];

  JsonObject GenerateJson()
  {

    JsonObject root = this->PrepareRoot();
    root["queryInverterInterval"] = queryInverterInterval;
    root["baudRate"] = baudRate;
    JsonObject sensors = root.createNestedObject("sensors");
    sensors["tempm1"] = tempm1;
    sensors["tempm2"] = tempm2;
    return root;
  }
};

extern Status status;

#endif /* STATUS_H_ */
