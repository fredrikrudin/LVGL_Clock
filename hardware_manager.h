#ifndef HARDWARE_MANAGER_H
#define HARDWARE_MANAGER_H

#include <Arduino.h>

namespace HardwareManager {
    // Initierar I2C, CAN (TWAI) samt LiPo-mätaren (SW6106)
    void initPeripherals();
    
    // Läser av data från externa I2C-enheter på bussen
    void scanI2CBus();
    
    // Läser och skickar testmeddelanden över CAN-bussen (TWAI)
    void processCANBus();
    
    // Hämtar batteristatus (Volt och %) från SW6106 via I2C
    float getLipoVoltage();
    int getLipoPercentage();
}

#endif
