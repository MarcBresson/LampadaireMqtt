#include "lamp.h"
#include <button.h>

void Lamp::subscribe(MessageReceivedCallback callback){
    getClient()->subscribe(getCommandTopic(false), callback);
}

void Lamp::sendState(){
    getClient()->publish(getStateTopic(), getState());
}

const char* Lamp::getState(){
    if(state == On){return "ON";}
    else{return "OFF";}
}

void Lamp::setRelayPin(int pin){
    relayPin = pin;
    pinMode(relayPin, OUTPUT);
}

void Lamp::setButtonPin(int pin){
    button = Button(pin);
    pinMode(pin, OUTPUT);
}

void Lamp::checkButton(){
    button.update();

    if(button.onRelease() && button.getPressDuration() > 100){
        toggle();
    }
}

void Lamp::toggle(){
    if(state == On){
        turnOff();
    } else {
        turnOn();
    }
}

void Lamp::turnOff(){
    Serial.println("turn off");
    digitalWrite(relayPin, HIGH);
    state = Off;
    sendState();
}

void Lamp::turnOn(){
    Serial.println("turn on");
    digitalWrite(relayPin, LOW);
    state = On;
    sendState();
}
