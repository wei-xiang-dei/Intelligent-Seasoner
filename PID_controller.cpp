#include "./PID_controller.h"

PID_controller::PID_controller(volatile float& target_value, volatile float& kp, volatile float& ki, volatile float& kd)
:
target(target_value),
kp(kp),
ki(ki),
kd(kd),
now_error(0),
sum_error(0),
prev_error(0)
{}

float PID_controller::output_control(float measurement){
    now_error = target - measurement;
    p_control = now_error * kp;
    sum_error += now_error;
    i_control = sum_error * ki;
    d_control = (now_error - prev_error) * kd;
    prev_error = now_error;
    // printf("p_control %f\t i_controld %f\t d_control %f \n", p_control, i_control, d_control);
    float total = p_control + i_control + d_control;
    
    float output;
    float low_thr = 0;
    float high_thr = 50;
    float fullWater = 0.096; //0.09
    float noWater = 0.108; //0.12
    if (abs(total) < low_thr){
        output = noWater;
    }
    else if (abs(total) > high_thr){
        output = fullWater;
    }
    else{
        float a = (fullWater-noWater)/(high_thr-low_thr);
        float b = noWater - a * low_thr;
        output = abs(total) * a + b;
    }
    if (total >= 0){
        return output+0.002;
    }
    else{
        return -output+0.009;
    }
}