#include "mbed.h"
#include <cmath>
AnalogIn   ain(A1);
#define VREF 5.0
#define temperature 25.0
// Specify different pins to test printing on UART other than the console UART.
#define TARGET_TX_PIN                                                     USBTX
#define TARGET_RX_PIN                                                     USBRX
#define BUFFER_SIZE 30
// Create a BufferedSerial object to be used by the system I/O retarget code.
static UnbufferedSerial serial_port(TARGET_TX_PIN, TARGET_RX_PIN, 9600);

FileHandle *mbed::mbed_override_console(int fd)
{
    return &serial_port;
}

int main(void)
{
    float tds_value_array[BUFFER_SIZE];
    float tds_value_sum = 0;
    int i=0;
    
    ain.set_reference_voltage(VREF);
    for (i = 0; i < BUFFER_SIZE; i++){
        tds_value_array[i] = 0;
    }
    while (1) {
        // test the voltage on the initialized analog pin
        //  and if greater than 0.3 * VCC set the digital pin
        //  to a logic 1 otherwise a logic 0

        float averageVoltage = ain.read() * VREF;
        float compensationCoefficient=1.0+0.02*(temperature-25.0);    //temperature compensation formula: fFinalResult(25^C) = fFinalResult(current)/(1.0+0.02*(fTP-25.0));
        float compensationVolatge=averageVoltage/compensationCoefficient;  //temperature compensation
        float tdsValue=(133.42*compensationVolatge*compensationVolatge*compensationVolatge - 255.86*compensationVolatge*compensationVolatge + 857.39*compensationVolatge)*0.5; //convert voltage value to tds value
        
        // print the percentage and 16 bit normalized values
        // printf("percentage: %3.3f\n", ain.read_voltage());
        // printf("normalized: 0x%04X \n", ain.read_u16());
        tds_value_sum -= tds_value_array[0];
        for (i = 0; i < BUFFER_SIZE; i++){
            if (i == BUFFER_SIZE - 1 ){
                tds_value_array[i] = tdsValue;
            }
            else{
                tds_value_array[i] = tds_value_array[i+1];
            }
        }
        tds_value_sum += tds_value_array[BUFFER_SIZE-1];
        printf("%3.0f\n", tds_value_sum/BUFFER_SIZE);
        ThisThread::sleep_for(30ms);
    }

}