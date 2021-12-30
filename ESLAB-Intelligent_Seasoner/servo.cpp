#include "mbed.h"
#include "./servo.h"

Servo::Servo(PinName pin){
    servo = new PwmOut(pin);
}
Servo::~Servo(){
    delete servo;
}

void Servo::writeAngle(float angle){
    servo->period(2.0f);
    servo->write(angle/180);
}

void Servo::writeSpeed(float speed){
    servo->period(2.0f);
    servo->write(speed/180);
}