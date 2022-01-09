#include "mbed.h"

class PID_controller {
    public:
        PID_controller(volatile float& target_value, volatile float& kp, volatile float& ki, volatile float& kd);
        float output_control(float measurement);
    public:
        volatile float& kp, ki, kd;
        volatile float& target;
        float now_error;
        float prev_error;
        float sum_error;
};