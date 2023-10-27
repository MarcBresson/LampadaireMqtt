#ifndef class_Lamp
#define class_Lamp

#include "HAMqtt.h"
#include <button.h>

typedef std::function<void(const String &message)> MessageReceivedCallback;

class Lamp: public HAMqttEntity {
    public:
        using HAMqttEntity::HAMqttEntity;

        void subscribe(MessageReceivedCallback callback);
        void sendState();

        void setButtonPin(int pin);
        void setRelayPin(int pin);
        void checkButton();

        void toggle();
        void turnOn();
        void turnOff();
        const char* getState();

        enum State {Off, On};
        State state;

        Button button;
        int relayPin;
};
#endif
