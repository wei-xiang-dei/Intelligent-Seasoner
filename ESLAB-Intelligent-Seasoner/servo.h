#include "mbed.h"

#ifndef SERVO
#define SERVO

class Servo{
    public:
        Servo(PinName pin);
        ~Servo();
        void writeAngle(float angle);
        void writeSpeed(float speed);
    private:
        PwmOut* servo;
};


#endif