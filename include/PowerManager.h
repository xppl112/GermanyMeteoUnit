#pragma once

class PowerManager
{
public:
    PowerManager();
    void deepSleep(int seconds);
    void powerRF(bool turnOn);
};