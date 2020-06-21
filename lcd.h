//LCD includes
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

int LCDLastUpdated = -1;
int displaySlide = 0;
int previousDisplaySlide = -1;

//Initialize variables for LCD
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 2);

void setupLCD(){
  lcd.init();
  lcd.backlight();
  lcd.clear();
}

void loopLCD(){
  if(LCDLastUpdated<0){
    LCDLastUpdated = millis();
  }
  
  //How long to show each value on the LCD
  int displayLength = 3;
  
  //This is a counter which gives us a value for a switch statement. 
  //This lets us cycle through all the sensors and display them on the LCD.
  int displaySlide = floor((millis() - LCDLastUpdated)/1000/displayLength);

  if(displaySlide != previousDisplaySlide){
    previousDisplaySlide = displaySlide;
    lcd.clear();
    switch (displaySlide) {
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
        //lcd.print("Temp(C): ");
        //lcd.print(SensorTemperatureC);
        lcd.print("Hum(%): ");
        lcd.print(SensorHumidity);
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
      default:
        LCDLastUpdated = millis();
        break;
    }
  }
}
