//Adafruit Laser PM Sensor
#include "Adafruit_PM25AQI.h"
Adafruit_PM25AQI aqi = Adafruit_PM25AQI();

//Sharp PM2.5 sensor
// Arduino pin numbers.
const int sharpLEDPin = 0;   // Arduino digital pin 7 connect to sensor LED.
const int sharpVoPin = A1;   // Arduino analog pin 5 connect to sensor Vo.
/// For averaging last N raw voltage readings.
#define N 200
static unsigned long VoRawTotal = 0;
static int VoRawCount = 0;
static float Voc = 0.6; // Set the typical output voltage in Volts when there is zero dust. 
const float K = 0.5;    // Use the typical sensitivity in units of V per 100ug/m3.

void setupPMi(){
  pinMode(sharpLEDPin,OUTPUT);
  pinMode(sharpVoPin, INPUT);
}

void updatePMid(){
  int samplingTime = 280;
  int deltaTime = 40;
  int sleepTime = 9680;
  float voMeasured = 0;
  float calcVolt = 0;
  float dustDensity = 0;
  float temp = 0;
  float vMin = 1; //1v as intercept
  float initPM = 34; //initial PM where the device turns on
  
  temp = 0;
  //measure 1000 times and average the result
  for (int i=0; i<1000; i++) {
    digitalWrite(sharpLEDPin,LOW); // power on the LED
    delayMicroseconds(samplingTime);
    voMeasured = analogRead(sharpVoPin); // read the dust value
    digitalWrite(sharpLEDPin,HIGH); // turn the LED off
    //Serial.println(voMeasured);
    temp = temp + voMeasured;
    delayMicroseconds(deltaTime);
    delayMicroseconds(sleepTime);
    //delay(1000);
  }
  voMeasured = temp/1000;
  calcVolt = voMeasured/1023*5.0;
  
  if(calcVolt<vMin){
    vMin = calcVolt;
  }
  dustDensity = (int)((calcVolt - (vMin + 0.01 - 0.05*initPM/100))/0.05*100);
  SensorPMid = dustDensity;
  
  Serial.print("PM2.5 (Method D): ");
  Serial.println(SensorPMid);
  delay(100);
}
void updatePMic(){
  //Serial.println("Updating PM2.5 (Method C)...");

  int samplingTime = 280;
  int deltaTime = 40;
  int sleepTime = 9680;
  float voMeasured = 0;
  float calcVoltage = 0;
  float dustDensity = 0;

  digitalWrite(sharpLEDPin,LOW); // power on the LED
  delayMicroseconds(samplingTime);
  voMeasured = analogRead(sharpVoPin); // read the dust value
  delayMicroseconds(deltaTime);
  digitalWrite(sharpLEDPin,HIGH); // turn the LED off
  delayMicroseconds(sleepTime);
  // 0 - 5V mapped to 0 - 1023 integer values
  // recover voltage
  calcVoltage = voMeasured * (5.0 / 1024.0);
  // linear eqaution taken from http://www.howmuchsnow.com/arduino/airquality/
  // Chris Nafis (c) 2012
  SensorPMic = 0.17 * calcVoltage - 0.1;
  
  Serial.print("PM 2.5 (Method C): ");
  Serial.print(SensorPMic);
  Serial.println("ug/m3");
}

void updatePMib(){
  //Serial.println("Updating PM2.5 (Method B)...");

  float dustVal=0; 
  int delayTime=280;
  int delayTime2=40;
  float offTime=9680;

  digitalWrite(sharpLEDPin,LOW); 
  delayMicroseconds(delayTime);
  dustVal=analogRead(sharpVoPin); 
  delayMicroseconds(delayTime2);
  digitalWrite(sharpLEDPin,HIGH); 
  delayMicroseconds(offTime);
   
  delay(1000);
  //if (dustVal>36.455){
    SensorPMib = (float(dustVal/1024)-0.0356)*120000*0.035;
  //}else{
    //Serial.println("PM2.5 B Failed. Trying again...");
    //updatePMib();
  //}
  Serial.print("PM 2.5 (Method B): ");
  Serial.print(SensorPMib);
  Serial.println("ug/m3");
}

void updatePMia(){
  //Serial.println("Updating PM2.5...");

    // Turn on the dust sensor LED by setting digital pin LOW.
  digitalWrite(sharpLEDPin, LOW);

  // Wait 0.28ms before taking a reading of the output voltage as per spec.
  delayMicroseconds(280);

  // Record the output voltage. This operation takes around 100 microseconds.
  int VoRaw = analogRead(sharpVoPin);
  
  // Turn the dust sensor LED off by setting digital pin HIGH.
  digitalWrite(sharpLEDPin, HIGH);

  // Wait for remainder of the 10ms cycle = 10000 - 280 - 100 microseconds.
  delayMicroseconds(9620);

  float Vo;
  
  // Use averaging to get a more accurate value.
  VoRawCount = 0;
  VoRawTotal = 0;
  while ( VoRawCount < N ) {
    Vo = VoRaw;
    VoRawTotal += VoRaw;
    VoRawCount++;
    delay(10);
  }
  Vo = 1.0 * VoRawTotal / N;
  
  // Compute the output voltage in Volts.
  Vo = Vo / 1024.0 * 5.0;

  // Convert to Dust Density in units of ug/m3.
  float dV = Vo - Voc;
  if ( dV < 0 ) {
    dV = 0;
    Voc = Vo;
  }
  float dustDensity = dV / K * 100.0;
  //if(dustDensity>0){
    SensorPMia = dustDensity;
  //}else{
    //Serial.println("PM2.5 A Failed. Trying again...");
    //updatePMia();
    //return;
  //}  

  Serial.print("PM 2.5 (Method A): ");
  Serial.print(SensorPMia);
  Serial.println("ug/m3");
  
}

void updatePMi(){
  updatePMia();
  updatePMib();
  updatePMic();
  updatePMid();
}



//BEGIN LASER SENSOR CODE

void setupPMl(){
  //Set up serial connection to sensor coming in on the RX pin
  Serial1.begin(9600);
  if (! aqi.begin_UART(&Serial1)) { // connect to the sensor over hardware serial
    Serial.println("Could not find Laser PM 2.5 sensor!");
    while (1) delay(10);
  }
  Serial.println("Laser PM Sensor Connected.");
}

void updatePMl(){
  PM25_AQI_Data data;

  if (! aqi.read(&data)) {
    //This is not an ideal solution. Sometimes it doesn't work and it has to try again. If there is some problem where it can't work, then it will get stuck here forever.
    updatePMl();
    return;
  }
 
  Serial.println();
  Serial.println(F("---------------------------------------"));
  Serial.println(F("Concentration Units (standard)"));
  Serial.println(F("---------------------------------------"));
  Serial.print(F("PM 1.0: ")); Serial.print(data.pm10_standard);
  Serial.print(F("\t\tPM 2.5: ")); Serial.print(data.pm25_standard);
  Serial.print(F("\t\tPM 10: ")); Serial.println(data.pm100_standard);
  Serial.println(F("Concentration Units (environmental)"));
  Serial.println(F("---------------------------------------"));
  Serial.print(F("PM 1.0: ")); Serial.print(data.pm10_env);
  Serial.print(F("\t\tPM 2.5: ")); Serial.print(data.pm25_env);
  Serial.print(F("\t\tPM 10: ")); Serial.println(data.pm100_env);
  Serial.println(F("---------------------------------------"));
  Serial.print(F("Particles > 0.3um / 0.1L air:")); Serial.println(data.particles_03um);
  Serial.print(F("Particles > 0.5um / 0.1L air:")); Serial.println(data.particles_05um);
  Serial.print(F("Particles > 1.0um / 0.1L air:")); Serial.println(data.particles_10um);
  Serial.print(F("Particles > 2.5um / 0.1L air:")); Serial.println(data.particles_25um);
  Serial.print(F("Particles > 5.0um / 0.1L air:")); Serial.println(data.particles_50um);
  Serial.print(F("Particles > 50 um / 0.1L air:")); Serial.println(data.particles_100um);
  Serial.println(F("---------------------------------------"));
  
  SensorPMl10 = data.pm10_standard;
  SensorPMl25 = data.pm25_standard;
  SensorPMl100 = data.pm100_standard;
}
