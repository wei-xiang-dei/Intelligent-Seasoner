#include "mbed.h"

class PID_controller {
    public:
        PID_controller(float target_value, float kp, float ki, float kd);
        float output_control(float measurement);
    private:
        float kp, ki, kd;
        float target;
        float now_error;
        float prev_error;
        float sum_error;
};