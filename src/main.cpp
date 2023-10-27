#include <Arduino.h>
#include "HAMqtt.h"
#include "lamp.h"
#include "Credentials.h"

#define MQTT_KEEPALIVE 60

HAMqttDevice lampadaireDevice("Lampadaire");

String client_name = lampadaireDevice.getIdentifier();
String lastWillTopic = lampadaireDevice.getAvailabilityTopic();
EspMQTTClient client(
  WIFI_SSID,
  WIFI_PASSWORD,
  MQTT_IP,
  MQTT_USERNAME,
  MQTT_PASSWORD,
  client_name.c_str()
);

Lamp MainLamp(lampadaireDevice, "lampe principale", HAMqttEntity::LIGHT);
Lamp SmallLamp(lampadaireDevice, "petite lampe", HAMqttEntity::LIGHT);

void setup() {
    Serial.begin(9600);

    lampadaireDevice.setClient(client);
    lampadaireDevice.addConfig("sw_version", "1.0");
    lampadaireDevice.addConfig("manufacturer", "Marc Bresson");
    lampadaireDevice.addConfig("model", "Lampadaire V1.0");
    
    client.setKeepAlive(MQTT_KEEPALIVE);
    client.setMaxPacketSize(750);
    client.enableDebuggingMessages();

    client.enableLastWillMessage(lastWillTopic.c_str(), "offline");

    MainLamp.addStateTopic();
    MainLamp.addCommandTopic();
    MainLamp.setButtonPin(14);
    MainLamp.setRelayPin(5);
    MainLamp.turnOff();
    MainLamp.button.update();

    SmallLamp.addStateTopic();
    SmallLamp.addCommandTopic();
    SmallLamp.setButtonPin(12);
    SmallLamp.setRelayPin(4);
    SmallLamp.turnOff();
    SmallLamp.button.update();
}

void sendConfig(){
  client.publish(MainLamp.getDiscoveryTopic(), MainLamp.getConfigPayload(), true);
  client.publish(SmallLamp.getDiscoveryTopic(), SmallLamp.getConfigPayload(), true);
}

bool callbackMain(const String message){
  if(message == "ON"){MainLamp.turnOn();}
  if(message == "OFF"){MainLamp.turnOff();}

  return true;
}

bool callbackSmall(const String message){
  if(message == "ON"){SmallLamp.turnOn();}
  if(message == "OFF"){SmallLamp.turnOff();}

  return true;
}

bool isFirstConnection = true;
void onConnectionEstablished(){
  if(isFirstConnection){
    sendConfig();
    isFirstConnection = false;
  }

  MainLamp.subscribe(callbackMain);
  MainLamp.sendState();
  SmallLamp.subscribe(callbackSmall);
  SmallLamp.sendState();

  lampadaireDevice.sendAvailable();
}

void loop() {
    client.loop();

    lampadaireDevice.manageAvailability(MQTT_KEEPALIVE);

    MainLamp.checkButton();
    SmallLamp.checkButton();
}
