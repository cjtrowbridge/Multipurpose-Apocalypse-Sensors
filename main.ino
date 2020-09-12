#include "battery.h"
#include "geiger.h" 
#include "sensors.h" 
#include "lcd.h"
#include "json.h"
#include "wifi.h" 

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  delay(2000);

  setupBattery();
  setupDHT();
  setupLCD();
  setupWifi();
  setupGeiger();
  setupPMi();
  setupPMl();

  updateSensorData();
}


void loop() {
  loopSensors();
  loopWifi();
  loopLCD();
}
