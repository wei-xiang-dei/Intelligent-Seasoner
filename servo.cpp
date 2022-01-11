#include "mbed.h"
#include "./servo.h"
#include "mbed.h"

Servo::Servo(PinName pin){
    servo = new PwmOut(pin);
}
Servo::~Servo(){
    delete servo;
}

void Servo::writeAngle(float angle){
    if (angle != prevValue){
        // has not set before. 
        servo->period(0.02);
        servo->write(angle);
    }
    prevValue = angle;
}

void Servo::writeSpeed(float speed){
    if (speed != prevValue){
        // has not set before. 
        servo->period(0.02);
        servo->write(speed);
    }
    prevValue = speed;
}

void Servo::openClaw(){
    writeAngle(0.12);
}

void Servo::stir(){
    writeSpeed(0.1);
}

void Servo::stopStir(){
    writeSpeed(0);
}

void Servo:: lift(){
    writeAngle(0.12);
}

void Servo::middleMove(){
}

void Servo::decline(){
    writeAngle(0.075);
}