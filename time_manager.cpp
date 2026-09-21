#include "time_manager.h"
#include <Arduino.h>

namespace TimeManager {
    static int hour = 12;
    static int minute = 0;
    static int second = 0;
    static unsigned long last_tick = 0;

    void init() {
        last_tick = millis();
    }

    void update() {
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
                    if (hour >= 24) {
                        hour = 0;
                    }
                }
            }
        }
    }

    void setTime(int h, int m, int s) {
        hour = h;
        minute = m;
        second = s;
        last_tick = millis(); // Återställ tidsreferensen
    }

    int getHour()   { return hour; }
    int getMinute() { return minute; }
    int getSecond() { return second; }
}
