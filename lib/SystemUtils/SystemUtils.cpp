#include <Arduino.h>
#include <Wire.h>
#include "SystemUtils.h"
extern "C" {
    #include "user_interface.h"
}

void SystemUtils::printAllI2C(){
    byte error, address;
    int nDevices;
 
    Serial.println("Scanning...");
 
    nDevices = 0;
    for(address = 8; address < 127; address++ ){
        Wire.beginTransmission(address);
        error = Wire.endTransmission();
 
        if (error == 0){
            Serial.print("I2C device found at address 0x");
            if (address<16)
                Serial.print("0");
            Serial.print(address,HEX);
            Serial.println(" !");
 
            nDevices++;
        }
        else if (error==4) {
            Serial.print("Unknow error at address 0x");
            if (address<16)
                Serial.print("0");
            Serial.println(address,HEX);
        } 
    }
    if (nDevices == 0)
        Serial.println("No I2C devices found\n");
    else
        Serial.println("done\n");
}

bool SystemUtils::isI2CDeviceConnected(uint8_t address){
    Wire.beginTransmission(address);
    uint8_t error = Wire.endTransmission();

    if (error == 0) return true;
    return false;
}

int SystemUtils::getFreeRAMBytes() {
    return ESP.getFreeHeap();
}

int SystemUtils::getTotalRAMBytes() {
    return 81920;
}

uint8_t SystemUtils::getCPUSpeedMHz() {
    return system_get_cpu_freq();
}

float SystemUtils::getVoltageFromAnalogPin(uint8_t pin, int r1, int r2){
    float Tvoltage=0.0;
    float Vvalue=0.0,Rvalue=0.0;
    float RatioFactor=(float)r2/(float)(r1+r2);

    for(unsigned int i=0;i<10;i++){
        Vvalue=Vvalue+analogRead(pin);         //Read analog Voltage
        delay(5);                              //ADC stable
    }

    Vvalue=(float)Vvalue/10.0;            //Find average of 10 values
    Rvalue=(float)(Vvalue/1023.0)*3.3;      //Convert Voltage in 3.3v factor
    Tvoltage=Rvalue/RatioFactor;          //Find original voltage by multiplying with factor

    return Tvoltage;
}