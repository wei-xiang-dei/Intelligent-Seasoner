#include "mbed.h"

#ifndef TDS_METER
#define TDS_METER

class TDS {
    public:
        TDS(PinName pin, float voltage, float temp);
        ~TDS();
        void listening_to_data() volatile;
        float getSensorValue() volatile;
        float getFilteredValue() volatile;
    private:
        AnalogIn* ain;
        float VREF;
        float sensorValue;
        float filteredValue;
        float* sensor_buffer;
        int BUFFER_SIZE = 30;
        float temperature;
};

#endif