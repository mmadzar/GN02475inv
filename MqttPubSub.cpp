#include "MqttPubSub.h"

MqttPubSub::MqttPubSub()
{
}

void MqttPubSub::setup()
{
  hostname = wifiSettings.hostname;
  strcat(channelStatus, hostname);
  strcat(channelStatus, "/status");
  strcat(channelIn, hostname);
  strcat(channelIn, "/in/#");
  strcat(channelOut, hostname);
  strcat(channelOut, "/out");

  client.setClient(espClient);
  client.setBufferSize(1024);
  client.setKeepAlive(30);
  client.setCallback(callback);
}

void MqttPubSub::callback(char *topic, byte *message, unsigned int length)
{
  char m[length];
  for (size_t i = 0; i < length; i++)
    m[i] = (char)message[i];

  String msg = String(m);
  String const cmd = String(topic).substring(String(wifiSettings.hostname).length() + 4, String(topic).length());
  if (length > 0)
  {
    if (cmd == "inverter" && length > 0)
    {
      // TODO
      Serial.println("Inverter command... send...");
      char msg[length + 1];
      for (size_t cnt = 0; cnt < length + 1; cnt++)
      {
        // convert byte to its ascii representation
        sprintf(&msg[cnt], "%C", message[cnt]);
      }

      // convert hex representation of message to bytes
      // Ex. 41 41 41 41 41 -> AAAAA
      status.inverterSend = String(msg);
      Serial.println(status.inverterSend);
      // char tc[2] = {0x00, 0x00};
      // for (size_t i = 0; i < (length) / 3; i++)
      // {
      //   tc[0] = char(message[i * 3]);
      //   tc[1] = char(message[(i * 3) + 1]);
      //   status.inverterSend[i] = strtol(tc, 0, 16);
      //   Serial.println(status.inverterSend[i]);
      // }
    }
    else
    {
      // for (size_t i = 0; i < SwitchCount; i++)
      // {
      //   SwitchConfig *sc = &pinsSettings.switches[i];
      //   // find switch in settings and set status value by index
      //   if (String(sc->name).equals(cmd))
      //   {
      //     status.switches[i] = msg.toInt();
      //     break;
      //   }
      // }
    }
    // Serial.println(msg);
    // Serial.println(cmd);
    status.receivedCount++;
  }
}

bool MqttPubSub::reconnect()
{
  if (WiFi.SSID() != currentMqttSettings[0])
  {
    for (size_t i = 0; i < wifiSettings.APsCount; i++)
    {
      if (WiFi.SSID() == mqttSettings.Servers[i][0])
      {
        currentMqttSettings[0] = mqttSettings.Servers[i][0];
        currentMqttSettings[1] = mqttSettings.Servers[i][1];
        if (currentMqttSettings[1] == "gateway")
          currentMqttSettings[1] = status.gatewayAddress;
        currentMqttSettings[2] = mqttSettings.Servers[i][2];
        currentMqttSettings[3] = mqttSettings.Servers[i][3];
        currentMqttSettings[4] = mqttSettings.Servers[i][4];
        break;
      }
    }
  }

  if (currentMqttSettings[1] != "")
  {
    Serial.println("Reconnecting to MQTT...");
    digitalWrite(2, HIGH);
    // Attempt to connect
    client.setServer(currentMqttSettings[1], atoi(currentMqttSettings[2]));
    if (connect(hostname, currentMqttSettings[3], currentMqttSettings[4]))
    {
      Serial.println("Connected to MQTT.");
      digitalWrite(2, LOW);
      client.subscribe(channelIn);
      Serial.print("Listening: ");
      Serial.println(channelIn);

      publishStatus(false);
    }
  }
  return client.connected();
}

bool MqttPubSub::connect(const char *id, const char *username, const char *password)
{
  if (username == "")
  {
    return client.connect(id);
  }
  else
  {
    return client.connect(id, username, password);
  }
}

void MqttPubSub::publishStatus(bool waitForInterval) // TODO pass additional status values
{
  if (!waitForInterval || ((millis() - lastMillis) > intervals.statusPublish))
  {
    lastMillis = millis();
    JsonObject root = doc.to<JsonObject>();

    root["uptime"] = (status.currentMillis - status.bootedMillis) / 1000;
    root["loops"] = status.loops;
    root["currentMillis"] = status.currentMillis;
    root["freeMem"] = status.freeMem;
    root["minFreeMem"] = status.minFreeMem;
    root["ipAddress"] = status.ipAddress;
    root["gateway"] = status.gatewayAddress;
    root["location"] = "car";
    root["RSSI"] = WiFi.RSSI(); // status.rssi;
    root["receivedCount"] = status.receivedCount;

    JsonObject sensors = root.createNestedObject("sensors");
    for (size_t i = 0; i < SensorCount; i++)
    {
      sensors[pinsSettings.sensors[i].name] = status.sensors[i];
    }

    serializeJson(doc, tempBuffer);
    client.publish(channelStatus, tempBuffer);

    status.missedSend = 0;
  }
  else
  {
    status.missedSend++;
  }
}

void MqttPubSub::handle()
{
  if (WiFi.status() == WL_CONNECTED)
  {
    if (!client.connected())
    {
      long currentMillis = millis();
      if (currentMillis - lastReconnectAttempt > 10000) // reconnect to MQTT every 10 seconds
      {
        Serial.println("Mqtt - Client not connected!");
        lastReconnectAttempt = currentMillis;
        // Attempt to reconnect
        if (reconnect())
        {
          lastReconnectAttempt = 0;
          reconnectAttemptsFailed = 0;
          Serial.print("mqtt connected."); // Expired waiting on 0inactive connection...");
        }
        else
        {
          reconnectAttemptsFailed++;
          // if (reconnectAttemptsFailed > 5)
          // { //no MQTT broker available, try with other WiFi network
          //   //wait 30 secs before restarting
          //   //ESP.restart();
          Serial.print("No MQTT broker available. ");
          Serial.println(reconnectAttemptsFailed);
          // }
        }
      }
    }
    else
    {
      // Client connected
      client.loop();
    }
  }
  else
  {
    // Serial.println("Mqtt - WiFi not connected!");
  }
}

void MqttPubSub::sendMessage(String message)
{
  char msg[255];
  message.toCharArray(msg, 255);
  client.publish(channelOut, msg);
}

void MqttPubSub::sendMessage(String message, String channel)
{
  char msg[255];
  message.toCharArray(msg, 255);
  char chnl[255];
  channel.toCharArray(chnl, 255);

  client.publish(chnl, msg);
}

void MqttPubSub::sendMesssageToTopic(const char *topic, String message)
{
  char msg[255];
  message.toCharArray(msg, 255);
  client.publish(topic, msg);
}