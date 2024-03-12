/*
 * Digital Thermometer 
 * 
 * This code is an exampleto show you how to made a single thermometer with: 
 * - ESP32 development module
 * - LED with its resistance for 3.3V operation
 * - 0.91 Inch OLED Display I2C SSD1306 Chip 128 x 32 resolution
 * - DS18B20 digital 1-wire temperature sensor
 * 
 * 
 * You can use and modify this code under your own responsibility
 * 
 * by Adrian Gallardo, Cristian Orrego
 * CEAZA
 * March 2024
 */
#include <Wire.h>                 // communication protocol with OLED module
#include <Adafruit_GFX.h>         // OLED graphics
#include <Adafruit_SSD1306.h>     // OLED driver
#include <OneWire.h>              // communication protocol with temperature sensor
#include <DallasTemperature.h>    // temperature sensor driver


//****************************************************************************************
// Global variables and constants definitions
//****************************************************************************************
const int oneWireBus = 16;        // GPIO where the DS18B20 is connected to
const int LED_PIN = 2;            // GPIO where the LED is connected to
float oldTemp;                    // previous temperature measurement used for field display clean


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
  display.display();


  // Start the DS18B20 sensor
  sensors.begin();

  // Set controller pin for LED
  pinMode(LED_PIN,OUTPUT); digitalWrite(LED_PIN,LOW);
}



//****************************************************************************************
// Main Loop
//****************************************************************************************
void loop() {
  float newTemp = readTemp();     // update temperature data
  
  updateDisplay(newTemp);         // update data on OLED module
  Serial.print(newTemp);Serial.println("*C");   // update data by serial communcation
  
  oldTemp = newTemp;              // update oldTemp varialbe
  delay(5000);                    // wait until next loop cicle
  
  // and start the loop again...
}
