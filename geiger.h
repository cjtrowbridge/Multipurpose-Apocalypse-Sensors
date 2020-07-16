/*
 * Geiger counter Kit could get on:  https://www.aliexpress.com            search: geiger counter kit
* --------------------------------------------------------------------------------------
* WHAT IS CPM?
* CPM (or counts per minute) is events quantity from Geiger Tube you get during one minute. Usually it used to 
* calculate a radiation level. Different GM Tubes has different quantity of CPM for background. Some tubes can produce
* about 10-50 CPM for normal background, other GM Tube models produce 50-100 CPM or 0-5 CPM for same radiation level.
* Please refer your GM Tube datasheet for more information. Just for reference here, J305 and SBM-20 can generate 
* about 10-50 CPM for normal background. 
* --------------------------------------------------------------------------------------
* HOW TO CONNECT GEIGER KIT?
* The kit 3 wires that should be connected to Arduino UNO board: 5V, GND and INT. PullUp resistor is included on
* kit PCB. Connect INT wire to Digital Pin#2 (INT0), 5V to 5V, GND to GND. Then connect the Arduino with
* USB cable to the computer and upload this sketch. 
* 
 * Author:JiangJie Zhang * If you have any questions, please connect cajoetech@qq.com
 * 
 * License: MIT License
 * 
 * Please use freely with attribution. Thank you!
*/

//#include <SPI.h>
#define GeigerPin 4
#define SensorGeigerLOGPERIOD 15000  //Logging period in milliseconds, recommended value 15000-60000.
#define SensorGeigerMAXPERIOD 60000  //Maximum logging period without modifying this sketch

unsigned long SensorGeigerCounts;          //variable for GM Tube events
unsigned long SensorGeigerCPM;           //variable for CPM
unsigned int SensorGeigerMultiplier;       //variable for calculation CPM in this sketch
unsigned long SensorGeigerPreviousMillis;  //variable for time measurement

void tube_impulse(){ //subprocedure for capturing events from Geiger Kit
  SensorGeigerCounts++;
}

void setupGeiger(){ //setup subprocedure
  SensorGeigerCounts = 0;
  SensorGeigerCPM = 0;
  SensorGeigerMultiplier = SensorGeigerMAXPERIOD / SensorGeigerLOGPERIOD; //calculating multiplier, depend on your log period
  attachInterrupt(GeigerPin, tube_impulse, FALLING); //define external interrupts 
}

void updateGeiger(){ //main cycle
  unsigned long currentMillis = millis();
  if((currentMillis - SensorGeigerPreviousMillis) > SensorGeigerLOGPERIOD){
    SensorGeigerPreviousMillis = currentMillis;
    SensorGeigerCPM = SensorGeigerCounts * SensorGeigerMultiplier;
    Serial.print("CPM: ");
    Serial.println(SensorGeigerCPM);
    SensorGeigerCounts = 0;
  }
}
