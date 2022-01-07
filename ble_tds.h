#include "ble/BLE.h"
#include "ble/Gap.h"
#include<iostream>
#include <cstdint>
#include <events/mbed_events.h>
#include "mbed.h"

#include "TDSP_service.h"
#include "pretty_printer.h"




const static char DEVICE_NAME[] = "intelligent seasoner";
static EventQueue event_queue(/* event count */ 10 * EVENTS_EVENT_SIZE);




class tdsdemo: ble::Gap::EventHandler {

public:
    tdsdemo(BLE &ble, events::EventQueue &event_queue, volatile float& target_ppm, volatile float& kp, volatile float& ki, volatile float& kd, volatile bool& butt) :
        _ble(ble),
        _event_queue(event_queue),
        _tdspservice(NULL),
        _tdsp_uuid(TdspService::TDSP_SERVICE_UUID),
        _adv_data_builder(_adv_buffer),
        target_ppm(target_ppm),
        kp(kp),
        ki(ki),
        kd(kd),
        butt(butt)
        {};
    volatile uint64_t target_ppm_int;
    volatile float&  target_ppm;
    volatile float&  kp;
    volatile float&  ki;
    volatile float&  kd;
    volatile bool&  butt;




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
            std::cout << params->data << std::endl;
            target_ppm_int = *(params->data);
            target_ppm = float(target_ppm_int);
            printf("target_ppm is %f",target_ppm);

        }else if ((params->handle == _tdspservice->getValueHandle1())) {
            
            std::cout << params->data<< std::endl;
            kp = *(params->data);
            printf("kp is %f",kp);

        }else if ((params->handle == _tdspservice->getValueHandle2())) {
            
            std::cout << params->data<< std::endl;
            ki = *(params->data);
            printf("ki is %f",ki);
        
        }else if ((params->handle == _tdspservice->getValueHandle3())) {
            
            std::cout << params->data<< std::endl;
        // statements
            kd = *(params->data);
            printf("kd is %f",kd);
        }else if ((params->handle == _tdspservice->getValueHandle4())){
            
            std::cout << params->data<< std::endl;
            butt = *(params->data);
            printf("butt is %d",butt);

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

void schedule_ble_events(BLE::OnEventsToProcessCallbackContext *context) {
    event_queue.call(Callback<void()>(&context->ble, &BLE::processEvents));
}




