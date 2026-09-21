#include <Arduino.h>
#include "lvgl.h"
#include "src/bsp/board.h"
#include "src/bsp/lvgl_port.h"
#include "ui_manager.h"
#include "time_manager.h"
#include "hardware_manager.h"

// Inkludera den nya WiFi-modulen
#include "wifi_manager.h"

static unsigned long last_hardware_check = 0;

void setup() {
    Serial.begin(115200);

    // 1. Initiera Waveshare-hårdvaran via BSP
    board_init();
    
    // 2. Initiera LVGL grafikporten
    lvgl_port_init();

    // 3. Initiera I2C, CAN och batterihanteraren
    HardwareManager::initPeripherals();

    // 4. Initiera klockhanteringen (Ansluter till PCF85063A)
    TimeManager::init();

    // 5. INAKTIVERAD: Initiera WiFi och starta NTP-bakgrundssynk
    // WiFiManager::init();

    // 6. Bygg och visa gränssnittet
    UIManager::init();
}

void loop() {
    // Kör realtidsuppdatering av tid och UI
    TimeManager::update();
    UIManager::update();

    // Läs av inkommande CAN-meddelanden kontinuerligt (Non-blocking)
    HardwareManager::processCANBus();

    // 7. INAKTIVERAD: Hantera WiFi-status och kolla NTP-status i bakgrunden
    // WiFiManager::update();

    // Utför diagnostik och I2C-skanning var 5:e sekund för att inte blockera UI
    unsigned long current_time = millis();
    if (current_time - last_hardware_check >= 5000) {
        last_hardware_check = current_time;
        
        HardwareManager::scanI2CBus();
        Serial.printf("[SYSTEM] Batterinivå: %.2f V (%d%%)\n", 
                      HardwareManager::getLipoVoltage(), 
                      HardwareManager::getLipoPercentage());
    }

    delay(5);
}
