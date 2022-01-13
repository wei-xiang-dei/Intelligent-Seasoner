#include "ble/BLE.h"
#include "ble/Gap.h"
#include<iostream>
#include <cstdint>
#include <events/mbed_events.h>
#include "mbed.h"

#include "TDSP_service.h"
#include "pretty_printer.h"




const static char DEVICE_NAME[] = "Intelligent Seasoner";
static EventQueue event_queue(/* event count */ 10 * EVENTS_EVENT_SIZE);




class tdsdemo: ble::Gap::EventHandler {

public:
    tdsdemo(BLE &ble, events::EventQueue &event_queue, volatile float& target_ppm, volatile float& kp, volatile float& ki, volatile float& kd, volatile bool& butt, volatile bool& lock, volatile bool& middleMoved) :
        _ble(ble),
        _event_queue(event_queue),
        _tdspservice(NULL),
        _tdsp_uuid(TdspService::TDSP_SERVICE_UUID),
        _adv_data_builder(_adv_buffer),
        target_ppm(target_ppm),
        kp(kp),
        ki(ki),
        kd(kd),
        butt(butt),
        lock(lock),
        middleMoved(middleMoved)
        {};
    volatile float&  target_ppm;
    volatile float&  kp;
    volatile float&  ki;
    volatile float&  kd;
    volatile bool&  butt;
    volatile bool& lock;
    volatile bool& middleMoved;




   void start(){
        _ble.gap().setEventHandler(this);

        _ble.init(this, &tdsdemo::on_init_complete);

        _event_queue.dispatch_forever();
    }




private:
    /** Callback triggered when the ble initialization process has finished */
    void on_init_complete(BLE::InitializationCompleteCallbackContext *params) {
        if (params->error != BLE_ERROR_NONE) {
            printf("Ble initialization failed.");
            return;
        }

        print_mac_address();

        /* Setup primary service. */
        _tdspservice= new TdspService(_ble, target_ppm, kp, ki, kd, butt);
        _ble.gattServer().onDataWritten(this, &tdsdemo::on_data_written);
        start_advertising();
    }
    void start_advertising() {
        /* Create advertising parameters and payload */

        ble::AdvertisingParameters adv_parameters(
            ble::advertising_type_t::CONNECTABLE_UNDIRECTED,
            ble::adv_interval_t(ble::millisecond_t(1000))
        );

        _adv_data_builder.setFlags();
        _adv_data_builder.setLocalServiceList(mbed::make_Span(&_tdsp_uuid, 1));
        _adv_data_builder.setName(DEVICE_NAME);

        /* Setup advertising */

        ble_error_t error = _ble.gap().setAdvertisingParameters(
            ble::LEGACY_ADVERTISING_HANDLE,
            adv_parameters
        );

        if (error) {
            print_error(error, "_ble.gap().setAdvertisingParameters() failed");
            return;
        }

        error = _ble.gap().setAdvertisingPayload(
            ble::LEGACY_ADVERTISING_HANDLE,
            _adv_data_builder.getAdvertisingData()
        );

        if (error) {
            print_error(error, "_ble.gap().setAdvertisingPayload() failed");
            return;
        }

        /* Start advertising */

        error = _ble.gap().startAdvertising(ble::LEGACY_ADVERTISING_HANDLE);

        if (error) {
            print_error(error, "_ble.gap().startAdvertising() failed");
            return;
        }
    }

     void on_data_written(const GattWriteCallbackParams *params) {
        if ((params->handle == _tdspservice->getValueHandle()) ){
            // data: 60, ppm: 60
            target_ppm = float(*(params->data));
            printf("target_ppm is %3.3f\n",target_ppm);

        }else if ((params->handle == _tdspservice->getValueHandle1())) {
            // data: 30, kp = data/100 = 0.3
            kp = (float(*(params->data)))/100;
            printf("kp is %3.3f\n",kp);

        }else if ((params->handle == _tdspservice->getValueHandle2())) {
            // data: 50, ki = data/10000 = 0.005
            ki = (float(*(params->data)))/10000;
            printf("ki is %3.3f\n",ki);
        
        }else if ((params->handle == _tdspservice->getValueHandle3())) {
            // data: 20, ppm = data/100 = 0.2
            kd = (float(*(params->data)))/100;
            printf("kd is %3.3f\n",kd);

        }else if ((params->handle == _tdspservice->getValueHandle4())){
            if (lock == 0){
                //if not sleeping, we can update
                butt = !butt;
                middleMoved = !middleMoved;
            }

        }
    }


private:
    virtual void onDisconnectionComplete(const ble::DisconnectionCompleteEvent&) {
        _ble.gap().startAdvertising(ble::LEGACY_ADVERTISING_HANDLE);
    }



private:
    BLE &_ble;
    events::EventQueue &_event_queue;
    TdspService *_tdspservice;

    UUID _tdsp_uuid;

    uint8_t _adv_buffer[ble::LEGACY_ADVERTISING_MAX_SIZE];
    ble::AdvertisingDataBuilder _adv_data_builder;
    
};





