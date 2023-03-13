#ifndef STATUS_H_
#define STATUS_H_

#include "appconfig.h"
#include "shared/status_base.h"

class Status : public StatusBase
{
public:
  double tempm1 = 0;               // motor temperature 1
  double tempm2 = 0;               // motor temperature 2
  int baudRate = 9600;             // inverter comms baudrate
  int queryInverterInterval = 333; // 300 milliseconds between inverter queries - 0 disabled
  char inverterSend[128];
  int wifiPortEnabled = 0;
  int collectors[CollectorCount];

  JsonObject GenerateJson()
  {

    JsonObject root = this->PrepareRoot();
    root["queryInverterInterval"] = queryInverterInterval;
    root["baudRate"] = baudRate;
    root["wifiPortEnabled"] = wifiPortEnabled;
    
    JsonObject sensors = root.createNestedObject("sensors");
    sensors["tempm1"] = tempm1;
    sensors["tempm2"] = tempm2;

    JsonObject jcollectors = root.createNestedObject("collectors");
    for (size_t i = 0; i < CollectorCount; i++)
      jcollectors[settings.collectors[i].name] = collectors[i];

    return root;
  }
};

extern Status status;

#endif /* STATUS_H_ */
