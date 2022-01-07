#include "ble/BLE.h"
#include "ble/Gap.h"

#ifndef _TDSP_SERVICE_H__
#define _TDSP_SERVICE_H__

const static uint16_t DIGITAL_USERDESCRIPTION_UUID  = 0x2901;
uint8_t kpname[] = "kp value";
uint8_t kiname[] = "ki value";
uint8_t kdname[] = "kd value";
uint8_t target_ppm_name[] = "target ppm value";
uint8_t the_BUTTONSTATE[] = "Button State";

class TdspService{
public:

    const static uint16_t TDSP_SERVICE_UUID =0x9000;
    const static uint16_t TDS_VALUE_CHARACTERISTIC_UUID =0x9001;
    const static uint16_t TDS_KP_CHARACTERISTIC_UUID =0x9002;
    const static uint16_t TDS_KI_CHARACTERISTIC_UUID =0x9003;
    const static uint16_t TDS_KD_CHARACTERISTIC_UUID =0x9004;
    const static uint16_t TDS_BUTTON_CHARACTERISTIC_UUID = 0xA001;
    TdspService(BLE &_ble, uint64_t tdsp_init, float kp_init, float ki_init, float kd_init, bool button_init):
        ble(_ble), 
        Tdsp(TDS_VALUE_CHARACTERISTIC_UUID, &tdsp_init, GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY, tdsp_des, 1),
        Tds_kp(TDS_KP_CHARACTERISTIC_UUID, &kp_init, GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY, kp_des, 1),
        Tds_ki(TDS_KI_CHARACTERISTIC_UUID, &ki_init, GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY, ki_des, 1),
        Tds_kd(TDS_KD_CHARACTERISTIC_UUID, &kd_init, GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY, kd_des, 1),
        Tds_button(TDS_BUTTON_CHARACTERISTIC_UUID, &button_init, GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY, button_state_des, 1)
        
    {
        tdsp_des[0] = new GattAttribute(DIGITAL_USERDESCRIPTION_UUID, target_ppm_name, sizeof(target_ppm_name), 20, true);
        kp_des[0] = new GattAttribute(DIGITAL_USERDESCRIPTION_UUID, kpname, sizeof(kpname), 20, true);
        ki_des[0] = new GattAttribute(DIGITAL_USERDESCRIPTION_UUID, kiname, sizeof(kiname), 20, true);
        kd_des[0] = new GattAttribute(DIGITAL_USERDESCRIPTION_UUID, kdname, sizeof(kdname), 20, true);
        button_state_des[0] = new GattAttribute(DIGITAL_USERDESCRIPTION_UUID, the_BUTTONSTATE, sizeof(the_BUTTONSTATE), 20, true);
        GattCharacteristic *charTable[] = {&Tdsp, &Tds_kp, &Tds_ki, &Tds_kd, &Tds_button};
        GattService         TdspService(TdspService::TDSP_SERVICE_UUID, charTable, sizeof(charTable) / sizeof(GattCharacteristic *));
        ble.gattServer().addService(TdspService);    		
    }
    // void updateTdsp(uint64_t newTdsp) {
    //     ble.gattServer().write(Tdsp.getValueHandle(), (uint8_t *)&newTdsp, sizeof(uint64_t));
    // }

    GattAttribute::Handle_t getValueHandle() const
    {
        return Tdsp.getValueHandle();
    }
   GattAttribute::Handle_t getValueHandle1() const
    {
        return Tds_kp.getValueHandle();
    }
   GattAttribute::Handle_t getValueHandle2() const
    {
        return Tds_ki.getValueHandle();
    }
   GattAttribute::Handle_t getValueHandle3() const
    {
        return Tds_kd.getValueHandle();
    }
   GattAttribute::Handle_t getValueHandle4() const
    {
        return Tds_button.getValueHandle();
    }




private:
    BLE &ble;
    GattAttribute **tdsp_des = new GattAttribute*[1];
    GattAttribute **kp_des = new GattAttribute*[1];
    GattAttribute **ki_des = new GattAttribute*[1];
    GattAttribute **kd_des = new GattAttribute*[1];
    GattAttribute **button_state_des = new GattAttribute*[1];

    ReadWriteGattCharacteristic<uint64_t> Tdsp;
    ReadWriteGattCharacteristic<float> Tds_kp;
    ReadWriteGattCharacteristic<float> Tds_ki;
    ReadWriteGattCharacteristic<float> Tds_kd;
    ReadWriteGattCharacteristic<bool> Tds_button;
};

#endif 