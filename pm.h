//Sharp PM2.5 sensor
// Arduino pin numbers.
const int sharpLEDPin = 0;   // Arduino digital pin 7 connect to sensor LED.
const int sharpVoPin = A1;   // Arduino analog pin 5 connect to sensor Vo.
/// For averaging last N raw voltage readings.
#define N 100
static unsigned long VoRawTotal = 0;
static int VoRawCount = 0;
static float Voc = 0.6; // Set the typical output voltage in Volts when there is zero dust. 
const float K = 0.5;    // Use the typical sensitivity in units of V per 100ug/m3.


void updatePM(){
  Serial.println("Updating PM2.5...");

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
  if(dustDensity>0){
    SensorPM = dustDensity;
  }else{
    Serial.println("Failed. Trying again...");
    updatePM();
    return;
  }  
  
  Serial.print(SensorPM);
  Serial.println("ug/m3");
}
