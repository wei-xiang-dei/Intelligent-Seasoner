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
    return p_control + i_control + d_control;
}