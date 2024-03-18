/*
 * Digital IoT Thermometer 
 * 
 * This code is an exampleto show you how to connect your DIY thermometer to internet: 
 * - ESP32 development module
 * - LED with its resistance for 3.3V operation
 * - 0.91 Inch OLED Display I2C SSD1306 Chip 128 x 32 resolution
 * - DS18B20 digital 1-wire temperature sensor
 * 
 * You will need to create a thinkspeak account on https://thingspeak.com/
 * or post your data on an existing channel.
 * 
 * 
 * You can use and modify this code under your own responsibility
 * 
 * by Adrian Gallardo, Cristian Orrego
 * CEAZA
 * March 2024
 */
#define TS_ENABLE_SSL // For HTTPS SSL connection
 
#include <Wire.h>                 // communication protocol with OLED module
#include <Adafruit_GFX.h>         // OLED graphics
#include <Adafruit_SSD1306.h>     // OLED driver
#include <OneWire.h>              // communication protocol with temperature sensor
#include <DallasTemperature.h>    // temperature sensor driver
#include <WiFiManager.h>          // Wifi network manager
#include <WiFi.h>
#include "ThingSpeak.h"
#include "oled.h"                 // graphics
#include "secretData.h"

WiFiClient  client;


//****************************************************************************************
// Global variables and constants definitions
//****************************************************************************************
const int oneWireBus = 16;        // GPIO where the DS18B20 is connected to
const int LED_PIN = 2;            // GPIO where the LED is connected to
const unsigned long myChannelNumber = MY_SECRET_CHANNEL_ID;
const char * myWriteAPIKey = MY_SECRET_API_KEY;
float oldTemp;                    // previous temperature measurement used for field display clean
unsigned long stamp = millis();
const unsigned long samplingPeriod = 5000;
unsigned long thingSpeakStamp = millis();
const unsigned long thingSpeakPeriod = 60000;
bool lastWifiStatus = false;


//****************************************************************************************
// OLED module definitions
//****************************************************************************************
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);



//****************************************************************************************
// DS18B20 Temperature sensor definitions
//****************************************************************************************
OneWire oneWire(oneWireBus);          // Setup a oneWire instance to communicate with any OneWire devices
DallasTemperature sensors(&oneWire);  // Pass our oneWire reference to Dallas Temperature sensor 


//****************************************************************************************
// wifi manager class
//****************************************************************************************
WiFiManager wm;


//****************************************************************************************
// program Functions
//****************************************************************************************
float readTemp(void){
  digitalWrite(LED_PIN,HIGH);         // Turn on the LED diring temperature reading
  sensors.requestTemperatures();      // Request temperature from sensor
  float temperatureC = sensors.getTempCByIndex(0);  // pass temperature data to a float variable
  digitalWrite(LED_PIN,LOW);          // Turn off the LED
  return temperatureC;                // return temperature data to end the function
}


void updateDisplay(float inData){
  // delete previous data field on display
  display.setCursor(0,15);
  display.setTextSize(2);
  display.setTextColor(SSD1306_BLACK);
  display.print(oldTemp);
  display.print(" ");
  display.write(0xF8);
  display.print("C");

  // write new data on display
  display.setCursor(0,15);
  display.setTextColor(SSD1306_WHITE);
  display.print(inData);
  display.print(" ");
  display.write(0xF8);
  display.print("C");

  // show updates on screen
  display.display();
}


void ActiveWaitMs(unsigned long delayMsTime, float number){
  while(millis() - stamp < delayMsTime){
    wm.process();
    if ( WiFi.status() == WL_CONNECTED ){
      if(!lastWifiStatus){
        display.drawBitmap(108, 0, wifiDisable, 20, 17, BLACK);
        display.drawBitmap(109, 0, wifiEnable, 18, 17, WHITE);
        display.display();
        Serial.println("WIFI connected");
        lastWifiStatus = true;
      }

      if(millis() - thingSpeakStamp > thingSpeakPeriod){
        thingSpeakStamp = millis();
        
        // Write to ThingSpeak. There are up to 8 fields in a channel, allowing you to store up to 8 different
        // pieces of information in a channel.  Here, we write to field 1.
        int x = ThingSpeak.writeField(myChannelNumber, 1, number, myWriteAPIKey);
        if(x == 200){
          Serial.println("Channel update successful.");
        }else{
          Serial.println("Problem updating channel. HTTP error code " + String(x));
        }
        
      }
    }else{
      if(lastWifiStatus){
        display.drawBitmap(109, 0, wifiEnable, 18, 17, BLACK);
        display.drawBitmap(108, 0, wifiDisable, 20, 17, WHITE);
        display.display();
        Serial.println("WIFI connection lost!");
        lastWifiStatus = false;
      }
    }

    
  }
  stamp = millis();
}





//****************************************************************************************
// Program begin (this part of the program its executed only one time at the beginning)
//****************************************************************************************
void setup() {
  delay(100);           // to avoid ESP32 crash on start

  Serial.begin(115200); // Enable serial communication with the host computer
  while(!Serial){}
  Serial.println("OLED display Test!");
  
  // Start the OLED module
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32 OLED module
    Serial.println(F("SSD1306 allocation failed"));
    while(true) {}  // If there is a problem with the OLED module, then the program stop forever
  }
  display.cp437(true);
  display.clearDisplay();
  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(0,0);             // Start at top-left corner
  display.println(F("Temperature"));
  display.drawBitmap(108, 0, wifiDisable, 20, 17, WHITE);
  display.display();


  // Start the DS18B20 sensor
  sensors.begin();

  // Set controller pin for LED
  pinMode(LED_PIN,OUTPUT); digitalWrite(LED_PIN,LOW);



  // Wifi manager start
  WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP
  ThingSpeak.begin(client);  // Initialize ThingSpeak
  bool res;
  wm.setConfigPortalBlocking(false);  // allows the program continue running paralel to wifi manager
  //automatically connect using saved credentials if they exist
  //If connection fails it starts an access point with the specified name
  res = wm.autoConnect(); // auto generated AP name from chipid
  if(!res) {
        Serial.println("Failed to connect");
    } 
    else {
        //if you get here you have connected to the WiFi    
        Serial.println("connected...yeey :)");
        display.drawBitmap(108, 0, wifiDisable, 20, 17, BLACK);
        display.drawBitmap(109, 0, wifiEnable, 18, 17, WHITE);
        display.display();
        //lastWifiStatus = true;
    }
}



//****************************************************************************************
// Main Loop
//****************************************************************************************
void loop() {
  float newTemp;            // float variable to store new temperature data
  newTemp = readTemp();     // update temperature data
  wm.process();             // update wifi manager status
  
  updateDisplay(newTemp);                       // update temperature data on OLED module
  Serial.print(newTemp);Serial.println("*C");   // update temperature data by serial communcation 
  
  
  oldTemp = newTemp;              // update oldTemp varialbe
  ActiveWaitMs(samplingPeriod, newTemp);   // wait meanwhile updating wifi manager
  // and start the loop again...
}
