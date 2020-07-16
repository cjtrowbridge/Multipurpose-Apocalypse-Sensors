//LCD includes
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define backlightKeyPin 5
#define lightsPin 3

int LCDLastUpdated = -1;
int displaySlide = 0;
int previousDisplaySlide = -1;
int backlightState = 0;

long screenPeek = 0;
int peekLength = 10;

//Initialize variables for LCD
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 2);

void backlightOn(){
  backlightState = 1;
  lcd.backlight();
  digitalWrite(lightsPin,HIGH);
}

void backlightOff(){
  backlightState = 0;
  lcd.noBacklight();
  digitalWrite(lightsPin,LOW);
}

void backlightToggle(){
  if(backlightState == 1){
    backlightOff();
  }else{
    backlightOn();
  }
}

void backlightPeek(){
  backlightOn();
  screenPeek = millis();
}

void setupLCD(){
  lcd.init();
  backlightOff();
  lcd.clear();

  pinMode(lightsPin, OUTPUT);
  pinMode(backlightKeyPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(backlightKeyPin), backlightPeek, CHANGE);
}

void loopLCD(){
  if(LCDLastUpdated<0){
    LCDLastUpdated = millis();
  }

  if(screenPeek > 0){
    if( screenPeek < (millis() - 1000 * peekLength) ){
      backlightOff();
    }
  }
  
  //How long to show each value on the LCD
  int displayLength = 5;
  
  //This is a counter which gives us a value for a switch statement. 
  //This lets us cycle through all the sensors and display them on the LCD.
  int displaySlide = floor((millis() - LCDLastUpdated)/1000/displayLength);

  if(displaySlide != previousDisplaySlide){
    previousDisplaySlide = displaySlide;
    lcd.clear();
    switch (displaySlide) {
      case 0:
        lcd.setCursor(0, 0);
        lcd.print(SensorPM);
        lcd.print("ugm3 ");
        lcd.print(SensorGeigerCPM);
        lcd.print("cpm");
        lcd.setCursor(0, 1);
        lcd.print((int)SensorTemperatureF);
        lcd.print("f ");
        lcd.print((int)SensorHumidity);
        lcd.print("%h");
        break;
      /*
      case 0:
        lcd.setCursor(0, 0);
        lcd.print("PM2.5(ug/m^3)");
        lcd.setCursor(0, 1);
        lcd.print(SensorPM);
        lcd.setCursor(0, 0);
        break;
      case 1:
        lcd.setCursor(0, 0);
        lcd.print("Temp(F): ");
        lcd.print(SensorTemperatureF);
        lcd.setCursor(0, 1);
        lcd.print("Temp(C): ");
        lcd.print(SensorTemperatureC);
        break;
      case 2:
        lcd.setCursor(0, 0);
        lcd.print("Smoke(PPM): ");
        lcd.print(SensorSmoke);
        lcd.setCursor(0, 1);
        lcd.print("Smoke(T/F): ");
        lcd.print(SensorMQ2);
        break;
      case 3:
        lcd.setCursor(0, 0);
        lcd.print("LPG(PPM): ");
        lcd.print(SensorLP);
        lcd.setCursor(0, 1);
        lcd.print("CO(PPM): ");
        lcd.print(SensorCO);
        break;
      */
      default:
        LCDLastUpdated = millis();
        break;
    }
  }
}
