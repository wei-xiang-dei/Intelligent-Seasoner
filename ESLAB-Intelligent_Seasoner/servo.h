#include "mbed.h"

#ifndef SERVO
#define SERVO

class Servo{
    public:
        Servo(PinName pin);
        ~Servo();
        void writeAngle(float angle);
        void writeSpeed(float speed);
        void openClaw();
        void stir();
        void stopStir();
        void lift();
        void decline();
    private:
        PwmOut* servo;
        bool prevValue;
};


#endif