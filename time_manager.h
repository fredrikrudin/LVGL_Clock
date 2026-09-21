#ifndef TIME_MANAGER_H
#define TIME_MANAGER_H

namespace TimeManager {
    void init();
    void update();
    void setTime(int h, int m, int s);
    int getHour();
    int getMinute();
    int getSecond();
}

#endif
