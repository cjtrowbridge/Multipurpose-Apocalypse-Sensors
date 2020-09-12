#include <SPI.h>
#include <WiFiNINA.h>
#include <RTCZero.h>
#include <ArduinoOTA.h>

#define SECRET_SSID "The Lodge"
#define SECRET_PASS "azula1969"

char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;                 // your network key Index number (needed only for WEP)

WiFiServer server(80);

void setupWifi(){
  // attempt to connect to Wifi network:
  int status = WL_IDLE_STATUS;
  while (status != WL_CONNECTED) {
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Connecting Wifi");
    lcd.setCursor(0, 1);
    lcd.print("\"");
    lcd.print(ssid);
    lcd.print("\"");
    status = WiFi.begin(ssid, pass);
    // wait 10 seconds for connection:
    delay(100);
  }

  //Start OTA listener
  ArduinoOTA.begin(WiFi.localIP(), "Arduino", "791345682", InternalStorage);
  
  server.begin();
  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  String ipStr = String(ip[0])+"."+String(ip[1])+"."+String(ip[2])+"."+String(ip[3]);
  //Check which sensor array this is
  String host = "unkown";
  if(ipStr=="192.168.86.10"){
    host="Inside Sensors";
    Ro           = .04; // (Inside)
    //backlightOn();
  }else if(ipStr=="192.168.86.11"){
    host="Outside Sensors";
    Ro           = .06; // (Outside)
    backlightOn();
  }
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(host);
  delay(100);
}

void checkWifi(){
  int status = WiFi.status();
  // attempt to connect to Wifi network:
  while ( status != WL_CONNECTED) {

    NVIC_SystemReset();

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Reconnecting");
    lcd.setCursor(0, 1);
    lcd.print(ssid);
    
    Serial.print("Reconnecting");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);
    delay(100);
  }
}

void loopWifi(){

  //Check if wifi is still connected
  checkWifi();
  
  //Listen for OTA
  ArduinoOTA.poll();

  //Listen for client requests
  WiFiClient client = server.available();   // listen for incoming clients
  if (client) {                             // if you get a client,
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:application/json");
            client.println("");
            client.println(getJSON());
            break;
          }else{      // if you got a newline, then clear currentLine:
            // break out of the while loop:
            currentLine = "";
          }
        }
        else if (c != '\r') {    // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

        if (currentLine.endsWith("GET /now")) {
          updateSensorData();
          client.println("HTTP/1.1 200 OK");
          client.println("Content-type:application/json");
          client.println("");
          client.println(getJSON());
          break;
        }

        //Endpoint for the PM2.5 laser sensor
        if (currentLine.endsWith("GET /pm2.5")) {
          updatePMl();
          client.println("HTTP/1.1 200 OK");
          client.println("Content-type:text/html");
          client.println("");
          client.println(SensorPMl25);
          break;
        }

        //Endpoints for the DHT22 sensor
        if (currentLine.endsWith("GET /temperaturef")) {
          updateDHT22();
          client.println("HTTP/1.1 200 OK");
          client.println("Content-type:text/html");
          client.println("");
          client.println(SensorTemperatureF);
          break;
        }
        if (currentLine.endsWith("GET /temperaturec")) {
          updateDHT22();
          client.println("HTTP/1.1 200 OK");
          client.println("Content-type:text/html");
          client.println("");
          client.println(SensorTemperatureC);
          break;
        }
        if (currentLine.endsWith("GET /humidity")) {
          updateDHT22();
          client.println("HTTP/1.1 200 OK");
          client.println("Content-type:text/html");
          client.println("");
          client.println(SensorHumidity);
          break;
        }
/*
        //Endpoints for the MQ-2 sensor
        if (currentLine.endsWith("GET /co")) {
          updateMQ2();
          client.println("HTTP/1.1 200 OK");
          client.println("Content-type:text/html");
          client.println("");
          client.println(SensorCO);
          break;
        }
        if (currentLine.endsWith("GET /lp")) {
          updateMQ2();
          client.println("HTTP/1.1 200 OK");
          client.println("Content-type:text/html");
          client.println("");
          client.println(SensorLP);
          break;
        }
        if (currentLine.endsWith("GET /smoke")) {
          updateMQ2();
          client.println("HTTP/1.1 200 OK");
          client.println("Content-type:text/html");
          client.println("");
          client.println(SensorSmoke);
          break;
        }
        if (currentLine.endsWith("GET /mq2")) {
          updateMQ2();
          client.println("HTTP/1.1 200 OK");
          client.println("Content-type:text/html");
          client.println("");
          client.println(SensorMQ2);
          break;
        }
        */
        if (currentLine.endsWith("GET /?backlight=on")) {
          backlightOn();
          client.println("HTTP/1.1 200 OK");
          client.println("Content-type:text/html");
          client.println("");
          client.println("Ok");
          break;
        }
        if (currentLine.endsWith("GET /?backlight=off")) {
          backlightOff();
          client.println("HTTP/1.1 200 OK");
          client.println("Content-type:text/html");
          client.println("");
          client.println("Ok");
          break;
        }
        if (currentLine.endsWith("GET /?backlight=toggle")) {
          backlightToggle();
          client.println("HTTP/1.1 200 OK");
          client.println("Content-type:text/html");
          client.println("");
          client.println("Ok");
          break;
        }
        if (currentLine.endsWith("GET /?backlight=peek")) {
          backlightPeek();
          client.println("HTTP/1.1 200 OK");
          client.println("Content-type:text/html");
          client.println("");
          client.println("Ok");
          break;
        }
        
      }
    }
    // close the connection:
    client.stop();
    Serial.println("client disconnected");
  }
}
