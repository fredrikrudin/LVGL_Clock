#include "hardware_manager.h"
#include <Wire.h>
#include "driver/twai.h" // Espressifs inbyggda CAN-drivrutin

#define I2C_SDA_PIN      8    // Hårdvaru I2C1 SDA för externa enheter/expanders
#define I2C_SCL_PIN      9    // Hårdvaru I2C1 SCL
#define CAN_TX_PIN       20   // TJA1051T CAN TX-pinne
#define CAN_RX_PIN       19   // TJA1051T CAN RX-pinne
#define SW6106_I2C_ADDR  0x3C // I2C-adress för LiPo laddningschippet SW6106

namespace HardwareManager {

    void initPeripherals() {
        // 1. Startar sekundär I2C-buss för externa sensorer och LiPo-chip
        Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN, 400000);

        // 2. Konfigurerar ESP32-S3 inbyggda CAN (TWAI) till 500kbps standardhastighet
        twai_general_config_t g_config = TWAI_GENERAL_CONFIG_DEFAULT((gpio_num_t)CAN_TX_PIN, (gpio_num_t)CAN_RX_PIN, TWAI_MODE_NORMAL);
        twai_timing_config_t t_config = TWAI_TIMING_CONFIG_500KBITS();
        twai_filter_config_t f_config = TWAI_FILTER_CONFIG_ACCEPT_ALL();

        if (twai_driver_install(&g_config, &t_config, &f_config) == ESP_OK) {
            twai_start(); 
        }
    }

    void scanI2CBus() {
        Serial.println("--- Skannar I2C-buss ---");
        byte error, address;
        int devicesFound = 0;

        for (address = 1; address < 127; address++) {
            Wire.beginTransmission(address);
            error = Wire.endTransmission();

            if (error == 0) {
                Serial.printf("I2C-enhet funnen på adress 0x%02X\n", address);
                devicesFound++;
            }
        }
        if (devicesFound == 0) Serial.println("Inga I2C-enheter hittades.\n");
    }

    void processCANBus() {
        twai_message_t rx_msg;
        if (twai_receive(&rx_msg, 0) == ESP_OK) {
            Serial.printf("CAN-meddelande mottaget ID: 0x%03X, DLC: %d, Data: ", rx_msg.identifier, rx_msg.data_length_code);
            for (int i = 0; i < rx_msg.data_length_code; i++) {
                Serial.printf("%02X ", rx_msg.data[i]);
            }
            Serial.println();
        }
    }

    float getLipoVoltage() {
        Wire.beginTransmission(SW6106_I2C_ADDR);
        Wire.write(0x32); 
        if (Wire.endTransmission() != 0) return 0.0f;

        Wire.requestFrom(SW6106_I2C_ADDR, 2);
        if (Wire.available() == 2) {
            uint16_t raw = (Wire.read() << 8) | Wire.read();
            return raw * 0.001f; 
        }
        return 0.0f;
    }

    int getLipoPercentage() {
        Wire.beginTransmission(SW6106_I2C_ADDR);
        Wire.write(0x3A); 
        if (Wire.endTransmission() != 0) return 0;

        Wire.requestFrom(SW6106_I2C_ADDR, 1);
        if (Wire.available() == 1) {
            return Wire.read(); 
        }
        return 0;
    }
}
