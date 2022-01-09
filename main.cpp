#include "mbed.h"
#include "ble/BLE.h"
#include "ble/Gap.h"

#include "./TDS_Meter.h"
#include "./servo.h"
#include "./PID_controller.h"
#include "./ble_tds.h"

// TDS Parameter
#define VREF 5.0
#define temperature 25.0
#define tdsPin A1
// #define target_ppm 45.0

// Servo Parameter
#define p_valve1 PA_7
#define p_valve2 PA_2
#define p_stirringRod PB_1
#define p_liftingArm PB_4

// Controller Paramter
// #define kp 0.3
// #define ki 0.005
// #define kd 0.2
volatile float target_ppm=60; 
volatile float kp=0.4;
volatile float ki=0.01;
volatile float kd=0.0;
volatile bool button_state = 0;

InterruptIn button(BUTTON1);
DigitalOut stateLED0(LED1);
DigitalOut stateLED1(LED3);
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

void schedule_ble_events(BLE::OnEventsToProcessCallbackContext *context) {
    event_queue.call(Callback<void()>(&context->ble, &BLE::processEvents));
}
void ble_trun(){
    BLE &ble = BLE::Instance();
    ble.onEventsToProcess(schedule_ble_events);
    tdsdemo demo(ble, event_queue, target_ppm, kp, ki, kd, button_state, lock);
    demo.start();   
}
int main()
{
    Thread  ble_t;
    ble_t.start(callback(ble_trun));
    
    // Start a thread to concurrently fetching and filtering the data.
    // Then in the main function, call getSensorValue and getFilteredValue to fetch the current state.
    Thread thread_tds;
    volatile TDS* tds = new TDS(tdsPin, VREF, temperature);
    thread_tds.start(callback(tds, &TDS::listening_to_data));

    // Servo
    Servo* s_valve1 = new Servo(p_valve1); // clean water
    Servo* s_valve2 = new Servo(p_valve2); // dirty waer
    Servo* s_stirring = new Servo(p_stirringRod);
    Servo* s_lifting = new Servo(p_liftingArm);

    // PID, specify target, kp, ki, kd
    PID_controller* pid_control = new PID_controller(target_ppm, kp, ki, kd);
    float output_control = 0;

    // Button Control
    button.fall(&button_pressed);
    bool readyToRun = 0;

    while (true) {
        // printf("sensor %f\t filtered %f\n", tds->getSensorValue(), tds->getFilteredValue());
        if (button_state == 0){
            stateLED0 = 1;
            stateLED1 = 0;
        ////////////////////////////// State 0: Ready State /////////////////////////////////
            s_valve1->openClaw(); //closed
            s_valve2->openClaw(); // closed

            // Hard-Code
            s_stirring->stopStir(); // Stop stirring
            s_lifting->lift(); // Lifting
            readyToRun = 0;
        }

        else if (button_state == 1){
            stateLED0 = 0;
            stateLED1 = 1;
            
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
            ///////////////// Servo Test //////////////////////////
            // s_valve1->writeAngle(0.09);
            // s_valve2->writeAngle(0.09);
            // continue;
            //////////////////////////////////////////////////////
            output_control = pid_control->output_control(tds->getFilteredValue());
            if (output_control >= 0){
                // Need to be denser (target is more than measurement)
                s_valve2->writeAngle(output_control);
                s_valve1->openClaw(); // closed
            }
            else{
                // Need more clean water (target is less than measurement)
                s_valve2->openClaw(); // closed
                s_valve1->writeAngle(-output_control);
            }
        }

        // for print purpose
        float p_control = pid_control->now_error * pid_control->kp;
        float i_control = pid_control->sum_error * pid_control->ki;
        float d_control = (pid_control->now_error - pid_control->prev_error) * pid_control->kd;
        float total_control = p_control + i_control + d_control;
        printf("%3.5f\t%3.5f\t%3.5f\t%d\t%3.5f\t%3.5f\t%3.5f\t%3.5f\t%3.5f\t%3.5f\t%3.5f\t%3.5f\n", 
            tds->getSensorValue(), 
            tds->getFilteredValue(), 
            target_ppm, 
            button_state, 
            kp, 
            ki,
            kd,
            p_control,
            i_control,
            d_control,
            total_control,
            output_control
        );
        ThisThread::sleep_for(30ms);   
    }
}

