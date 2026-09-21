#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

namespace WiFiManager {
    // Startar WiFi-processen i bakgrunden (Asynkron anslutning)
    void init();
    
    // Hanterar WiFi-status och kontrollerar om NTP-synk har slutförts
    void update();
    
    // Returnerar true om klockan har synkroniserats mot internet framgångsrikt
    bool isTimeSynced();
}

#endif
