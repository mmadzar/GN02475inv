

#include "Bytes2WiFi.h"

WiFiServer serverWiFi(SERIAL_TCP_PORT);
WiFiClient TCPClient[MAX_NMEA_CLIENTS];
static IPAddress broadcastAddr(255, 255, 255, 255);

Bytes2WiFi::Bytes2WiFi()
{
}

void Bytes2WiFi::setup()
{
    serverWiFi.begin(); // start TCP server
    serverWiFi.setNoDelay(true);
}

void Bytes2WiFi::addBuffer(byte b)
{
    if (position > BUFFER_SIZE - 2)
    {
        position = 0;
    }
    content[position++] = b;
}

void Bytes2WiFi::addBuffer(const char *buffer, size_t size)
{
    for (size_t i = 0; i < size; i++)
    {
        addBuffer(buffer[i]);
    }
}

void Bytes2WiFi::send()
{
    if (serverWiFi.hasClient())
    {
        for (byte i = 0; i < MAX_NMEA_CLIENTS; i++)
        {
            // find free/disconnected spot
            if (!TCPClient[i] || !TCPClient[i].connected())
            {
                if (TCPClient[i])
                    TCPClient[i].stop();
                TCPClient[i] = serverWiFi.available();
                continue;
            }
        }
        // no free/disconnected spot so reject
        WiFiClient TmpserverClient = serverWiFi.available();
        TmpserverClient.stop();
    }

    // send bytes to WiFi
    if (position > 0)
    {
        bool sent = false;
        for (byte cln = 0; cln < MAX_NMEA_CLIENTS; cln++)
        {
            if (TCPClient[cln])
            {
                // send plain bytes
                uint8_t *buff = content;
                TCPClient[cln].write(buff, position);
                sent = true; // preserve position for next send cycle if there are clients
            }
        }
        if (sent || position > BUFFER_SIZE - 16) // reserve some space in buffer
            position = 0;
    }
}

void Bytes2WiFi::read()
{
    if (serverWiFi.hasClient())
    {
        for (byte i = 0; i < MAX_NMEA_CLIENTS; i++)
        {
            // find free/disconnected spot
            if (!TCPClient[i] || !TCPClient[i].connected())
            {
                if (TCPClient[i])
                    TCPClient[i].stop();
                TCPClient[i] = serverWiFi.available();
                continue;
            }
        }
        // no free/disconnected spot so reject
        WiFiClient TmpserverClient = serverWiFi.available();
        TmpserverClient.stop();
    }

    for (byte cln = 0; cln < MAX_NMEA_CLIENTS; cln++)
    {
        if (TCPClient[cln])
        {
            // collect command
            while (TCPClient[cln].available())
            {
                wifiCommand[wifiCmdPos] = TCPClient[cln].read(); // read char from client
                if (wifiCmdPos < 128 - 1)
                    wifiCmdPos++;
            }
            char cmd[wifiCmdPos + 1];
            String((char *)wifiCommand).toCharArray(cmd, wifiCmdPos + 1);
            if (wifiCmdPos > 1)
            {
                Serial.println(cmd);
                // execute command
                if (strcmp(cmd, "ping") == 0)
                {
                    addBuffer("pong", 4);
                }
                else if (strcmp(cmd, "restart") == 0)
                {
                    ESP.restart();
                }
                else if (strcmp(cmd, "reconnect") == 0)
                {
                    WiFi.disconnect(false, false);
                }

                wifiCmdPos = 0;
            }
            else
                wifiCmdPos = 0;
        }
    }
}

void Bytes2WiFi::handle()
{
    if (WiFi.isConnected())
    {
        read();
        currentMicros = micros();
        if ((currentMicros - lastBroadcast) > 1000000ul) // every second send out a broadcast ping
        {
            uint8_t buff[4] = {0x1C, 0xEF, 0xAC, 0xED};
            lastBroadcast = currentMicros;
            wifiUDPServer.beginPacket(broadcastAddr, 17222);
            wifiUDPServer.write(buff, 4);
            wifiUDPServer.endPacket();
        }
        if (position > 0)
        {
            send();
        }
    }
}
