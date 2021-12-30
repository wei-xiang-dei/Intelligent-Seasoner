#include "mbed.h"
#include "./TDS_Meter.h"


TDS::TDS(PinName pin, float voltage, float temp){
    ain = new AnalogIn(pin);
    VREF = voltage;
    temperature = temp;
    ain->set_reference_voltage(VREF);
    sensorValue = 0.0;
    filteredValue = 0.0;
    sensor_buffer = new float[BUFFER_SIZE];
    for(int i=0; i< BUFFER_SIZE; i++){
        sensor_buffer[i] = 0;
    }
};

TDS::~TDS(){
    delete[] sensor_buffer;
    delete ain;
};

void TDS::listening_to_data() volatile{
    float buffer_sum = 0;
    while (1) {
        float averageVoltage = ain->read() * VREF;
        float compensationCoefficient=1.0+0.02*(temperature-25.0);    //temperature compensation formula: fFinalResult(25^C) = fFinalResult(current)/(1.0+0.02*(fTP-25.0));
        float compensationVolatge=averageVoltage/compensationCoefficient;  //temperature compensation
        float tdsValue=(133.42*compensationVolatge*compensationVolatge*compensationVolatge - 255.86*compensationVolatge*compensationVolatge + 857.39*compensationVolatge)*0.5; //convert voltage value to tds value

        buffer_sum -= sensor_buffer[0];
        for (int i = 0; i < BUFFER_SIZE; i++){
            if (i == BUFFER_SIZE - 1 ){
                sensor_buffer[i] = tdsValue;
            }
            else{
                sensor_buffer[i] = sensor_buffer[i+1];
            }
        }
        buffer_sum += sensor_buffer[BUFFER_SIZE-1];
        // printf("%3.0f\n", buffer_sum/BUFFER_SIZE);

        //assign the member state.
        sensorValue = tdsValue;
        filteredValue = buffer_sum/BUFFER_SIZE;

        ThisThread::sleep_for(30ms); // may affect the filtering performance.
    }
}

float TDS::getSensorValue() volatile{
    return sensorValue;
}

float TDS::getFilteredValue() volatile{
    return filteredValue;
}
