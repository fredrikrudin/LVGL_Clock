#include "wifi_manager.h"
#include "time_manager.h"
#include <WiFi.h>
#include "esp_sntp.h"

// --- ÄNDRA DESSA TILL DINA EGNA UPPGIFTER NÄR DU AKTIVERAR ---
const char* WIFI_SSID     = "DITT_WIFI_NAMN";
const char* WIFI_PASSWORD = "DITT_WIFI_LÖSENORD";

// NTP Konfiguration
const char* NTP_SERVER    = "pool.ntp.org";
// Tidszonssträng för Sverige (CET/CEST med automatisk övergång för sommar/vintertid)
const char* TZ_INFO       = "CET-1CEST,M3.5.0,M10.5.0"; 

namespace WiFiManager {
    static bool time_synced = false;
    static unsigned long last_wifi_check = 0;

    // Callback som triggas automatiskt av ESP32 när NTP-servern svarar
    void timeSyncNotificationCallback(struct timeval *tv) {
        Serial.println("[WiFi/NTP] Tidssynkronisering slutförd mot NTP-server!");
        
        // Hämta den lokala tiden som ESP32 just räknat ut baserat på tidszonen
        time_t now;
        struct tm timeinfo;
        time(&now);
        localtime_r(&now, &timeinfo);

        // Skriv den synkroniserade tiden direkt till ditt hårdvaru-RTC (PCF85063A)
        TimeManager::setTime(timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
        
        time_synced = true;
    }

    void init() {
        Serial.println("[WiFi] Startar asynkron anslutning...");
        
        // Sätt WiFi i klientläge
        WiFi.mode(WIFI_STA);
        WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
        
        // Konfigurera inbyggd SNTP-klient och tidszon
        configTzTime(TZ_INFO, NTP_SERVER);
        
        // Sätt callback-funktion för när synkningen är klar
        sntp_set_time_sync_notification_cb(timeSyncNotificationCallback);
        
        last_wifi_check = millis();
    }

    void update() {
        unsigned long current_time = millis();
        
        // Kontrollera WiFi-anslutningens status var 10:e sekund (ickeblockerande)
        if (current_time - last_wifi_check >= 10000) {
            last_wifi_check = current_time;
            
            if (WiFi.status() == WL_CONNECTED) {
                if (!time_synced) {
                    Serial.println("[WiFi] Ansluten! Väntar på NTP-svar...");
                }
            } else {
                Serial.println("[WiFi] Försöker ansluta eller återansluta...");
                // Försök återansluta automatiskt om anslutningen brutits
                if (WiFi.status() == WL_DISCONNECTED) {
                    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
                }
            }
        }
    }

    bool isTimeSynced() {
        return time_synced;
    }
}
