#include "time_manager.h"
#include <Arduino.h>
#include <Wire.h> 

#define RTC_I2C_ADDRESS 0x51 // Hårdvaruadress för PCF85063A-chippet

namespace TimeManager {
    static int hour = 12;
    static int minute = 0;
    static int second = 0;
    static unsigned long last_tick = 0;

    void init() {
        last_tick = millis();
        
        // Kontrollera om RTC svarar på I2C
        Wire.beginTransmission(RTC_I2C_ADDRESS);
        if (Wire.endTransmission() == 0) {
            Serial.println("[RTC] PCF85063A hittad och aktiverad!");
        } else {
            Serial.println("[RTC] VARNING: Hittade inte PCF85063A via I2C!");
        }
    }

    void update() {
        // Hämta realtid direkt från hårdvaru-RTC
        Wire.beginTransmission(RTC_I2C_ADDRESS);
        Wire.write(0x04); // Startregister för tid (sekunder) hos PCF85063
        if (Wire.endTransmission() == 0) {
            Wire.requestFrom(RTC_I2C_ADDRESS, 3); // Begär sekunder, minuter, timmar
            if (Wire.available() == 3) {
                // Konvertera BCD-format (Binary Coded Decimal) till vanliga heltal
                uint8_t raw_sec = Wire.read() & 0x7F;
                uint8_t raw_min = Wire.read() & 0x7F;
                uint8_t raw_hr  = Wire.read() & 0x3F;
                
                second = ((raw_sec >> 4) * 10) + (raw_sec & 0x0F);
                minute = ((raw_min >> 4) * 10) + (raw_min & 0x0F);
                hour   = ((raw_hr  >> 4) * 10) + (raw_hr  & 0x0F);
                return; // Avbryt mjukvarubaserad tidräkning
            }
        }

        // Fallback om I2C-kommunikationen skulle svikta temporärt
        unsigned long now = millis();
        if (now - last_tick >= 1000) {
            last_tick += 1000;
            second++;
            if (second >= 60) {
                second = 0;
                minute++;
                if (minute >= 60) {
                    minute = 0;
                    hour++;
                    if (hour >= 24) hour = 0;
                }
            }
        }
    }

    void setTime(int h, int m, int s) {
        hour = h;
        minute = m;
        second = s;
        last_tick = millis();

        // Skriv den nya inställda tiden till hårdvaru-RTC-chippet
        uint8_t bcd_sec = ((s / 10) << 4) | (s % 10);
        uint8_t bcd_min = ((m / 10) << 4) | (m % 10);
        uint8_t bcd_hr  = ((h / 10) << 4) | (h % 10);

        Wire.beginTransmission(RTC_I2C_ADDRESS);
        Wire.write(0x04); // Sekundregister
        Wire.write(bcd_sec);
        Wire.write(bcd_min);
        Wire.write(bcd_hr);
        Wire.endTransmission();
        Serial.printf("[RTC] Ny tid sparad i hårdvaran: %02d:%02d:%02d\n", h, m, s);
    }

    int getHour()   { return hour; }
    int getMinute() { return minute; }
    int getSecond() { return second; }
}
