#include <Arduino.h>
#include <PowerManager.h>
#include <config.h>
#include <ESP8266WiFi.h>

//#define DEBUG

PowerManager::PowerManager(){
    powerRF(false);
}

void PowerManager::deepSleep(int seconds){
    #ifdef DEBUG
    Serial.print("PowerManager::deepSleep ");Serial.println(seconds);
    #endif

    ESP.deepSleep(seconds * 1000000 /*, WAKE_RF_DISABLED*/);
}

void PowerManager::powerRF(bool turnOn){
    if(turnOn){
        WiFi.forceSleepWake();
        delay(1);
    }
    else {
        WiFi.mode(WIFI_OFF);
        WiFi.forceSleepBegin();
        delay(1);
    }
}