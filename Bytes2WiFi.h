#ifndef BYTES2WIFI_H_
#define BYTES2WIFI_H_

#define SERIAL_TCP_PORT 23 
#define MAX_NMEA_CLIENTS 1
#include <WiFi.h>
#include <WiFiClient.h>

class Bytes2WiFi
{
public:
    Bytes2WiFi();
    void setup();
    void send();
    void handle();
    void addBuffer(byte b);
    void addBuffer(const char *buffer, size_t size);

private:
#define BUFFER_SIZE 512
    byte content[BUFFER_SIZE];
    int position = 0;
    uint32_t lastMicros;
    uint32_t currentMicros;
    WiFiUDP wifiUDPServer;
    uint32_t lastBroadcast;
};

#endif /* BYTES2WIFI_H_ */
