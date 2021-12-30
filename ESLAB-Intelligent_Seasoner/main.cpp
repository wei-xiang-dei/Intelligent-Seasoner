#include "mbed.h"
#include "./TDS_Meter.h"
#include "./servo.h"
#include "./PID_controller.h"

// TDS Parameter
#define VREF 5.0
#define temperature 25.0
#define tdsPin A1
#define target_ppm 200.0

// Servo Parameter
#define p_valve1 PA_7
#define p_valve2 PA_2
#define p_stirringRod PB_1
#define p_liftingArm PB_4

// Controller Paramter
#define kp 1.0
#define ki 1.0
#define kd 1.0


// Button Control
InterruptIn button(BUTTON1);
volatile bool button_state = 0;
DigitalOut state0(LED1);
DigitalOut state1(LED3);
volatile bool lock = 0;

/////////////////////////////////////////////// Serial Monitor ///////////////////////////////////////////////
// Specify different pins to test printing on UART other than the console UART.
#define TARGET_TX_PIN                                                     USBTX
#define TARGET_RX_PIN                                                     USBRX
// Create a BufferedSerial object to be used by the system I/O retarget code.
static UnbufferedSerial serial_port(TARGET_TX_PIN, TARGET_RX_PIN, 9600);

FileHandle *mbed::mbed_override_console(int fd)
{
    return &serial_port;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

void button_pressed(){
    if (lock == 0){
        //if not sleeping, we can update
        button_state = !button_state;
    }
}

int main()
{
    // Start a thread to concurrently fetching and filtering the data.
    // Then in the main function, call getSensorValue and getFilteredValue to fetch the current state.
    Thread thread;
    volatile TDS* tds = new TDS(tdsPin, VREF, temperature);
    thread.start(callback(tds, &TDS::listening_to_data));

    // Servo
    Servo* s_valve1 = new Servo(p_valve1);
    Servo* s_valve2 = new Servo(p_valve2);
    Servo* s_stirring = new Servo(p_stirringRod);
    Servo* s_lifting = new Servo(p_liftingArm);

    // PID, specify target, kp, ki, kd
    PID_controller* pid_control = new PID_controller(target_ppm, kp, ki, kd);

    // Button Control
    button.fall(&button_pressed);
    bool readyToRun = 0;

    while (true) {
        // printf("sensor %f\t filtered %f\n", tds->getSensorValue(), tds->getFilteredValue());
        
        if (button_state == 0){
            state0 = 1;
            state1 = 0;
        ////////////////////////////// State 0: Ready State /////////////////////////////////
            s_valve1->openClaw(); //closed
            s_valve2->openClaw(); // closed

            // Hard-Code
            s_stirring->stopStir(); // Stop stirring
            s_lifting->lift(); // Lifting
            readyToRun = 0;
        }



        else if (button_state == 1){
            state0 = 0;
            state1 = 1;
        ////////////////////////////// State 1: Running State /////////////////////////////////
            lock = 1;
            if (readyToRun == 0){
                // Hard-Code
                s_lifting->decline(); // Declining
                ThisThread::sleep_for(2s);   
                s_stirring->stir(); // Start stirring
                ThisThread::sleep_for(2s);  
                readyToRun = 1;
            }
            lock = 0;
            float output_control = pid_control->output_control(tds->getFilteredValue());
            if (output_control >= 0){
                // Need to be denser (target is more than measurement)
                s_valve1->writeAngle(output_control);
                s_valve2->openClaw(); // closed
            }
            else{
                // Need more clean water (target is less than measurement)
                s_valve1->openClaw(); // closed
                s_valve2->writeAngle(output_control);
            }
        }
        printf("%3.0f\t%3.0f\t%3.0f\t%d\n", tds->getSensorValue(), tds->getFilteredValue(), target_ppm, button_state);
        ThisThread::sleep_for(30ms);   
    }
}
