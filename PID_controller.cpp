#include "./PID_controller.h"

PID_controller::PID_controller(float target_value, float kp, float ki, float kd)
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
    float p_control = now_error * kp;
    sum_error += now_error;
    float i_control = sum_error * ki;
    float d_control = (now_error - prev_error) * kd;
    prev_error = now_error;
    // printf("p_control %f\t i_controld %f\t d_control %f \n", p_control, i_control, d_control);
    float total = p_control + i_control + d_control;
    
    float output;
    float low_thr = 0;
    float high_thr = 50;
    float fullWater = 0.09;
    float noWater = 0.12;
    if (abs(total) < low_thr){
        output = noWater;
    }
    else if (abs(total) > high_thr){
        output = fullWater;
    }
    else{
        output = abs(total) * (-fullWater/(high_thr-low_thr)) + (noWater - fullWater * low_thr / (high_thr-low_thr));
    }
    if (total >= 0){
        return output;
    }
    else{
        return -output;
    }
}