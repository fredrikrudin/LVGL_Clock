#include <Arduino.h>
#include "lvgl.h"
#include "src/bsp/board.h"       // Waveshares officiella BSP-sökväg
#include "src/bsp/lvgl_port.h"   // Waveshares LVGL wrapper-port
#include "ui_manager.h"
#include "time_manager.h"

void setup() {
    Serial.begin(115200);

    // 1. Initiera Waveshare-hårdvaran (Skärm, Touch, Expander, etc.)
    board_init();
    
    // 2. Initiera Waveshare LVGL-porten
    lvgl_port_init();

    // 3. Initiera tidsfunktionen
    TimeManager::init();

    // 4. Initiera Gränssnittet (Mörkt tema + Sidor)
    UIManager::init();
}

void loop() {
    // Uppdatera den interna klockan
    TimeManager::update();
    
    // Uppdatera UI-element (t.ex. klocktexten) om vi är på startskärmen
    UIManager::update();

    // LVGL-hantering sköts automatiskt via Waveshares lvgl_port i bakgrunden,
    // men en kort delay förhindrar att processorn går på högvarv.
    delay(5);
}
