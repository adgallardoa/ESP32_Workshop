/*
 * Digital Thermometer 
 * 
 * This code is an exampleto show you how to 
 */


#include <Wire.h>                 // communication protocol with OLED module
#include <Adafruit_GFX.h>         // OLED graphics
#include <Adafruit_SSD1306.h>     // OLED driver
#include <OneWire.h>              // communication protocol with temperature sensor
#include <DallasTemperature.h>    // temperature sensor driver

const int LED_PIN = 2;
float oldTemp;

//****************************************************************************************
// OLED module definitions
//****************************************************************************************
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);



//****************************************************************************************
// OLED module definitions
//****************************************************************************************
const int oneWireBus = 16;            // GPIO where the DS18B20 is connected to
OneWire oneWire(oneWireBus);          // Setup a oneWire instance to communicate with any OneWire devices
DallasTemperature sensors(&oneWire);  // Pass our oneWire reference to Dallas Temperature sensor 



//****************************************************************************************
// program Functions
//****************************************************************************************
float readTemp(void){
  digitalWrite(LED_PIN,HIGH);
  sensors.requestTemperatures(); 
  float temperatureC = sensors.getTempCByIndex(0);
  digitalWrite(LED_PIN,LOW);
  return temperatureC;
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

  // show new data on display
  display.setCursor(0,15);
  display.setTextColor(SSD1306_WHITE);
  display.print(inData);
  display.print(" ");
  display.write(0xF8);
  display.print("C");
  display.display();
}


//****************************************************************************************
// Program begin
//****************************************************************************************
void setup() {
  delay(100);

  Serial.begin(115200);
  while(!Serial){}
  Serial.println("OLED display Test!");
  
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32 OLED module
    Serial.println(F("SSD1306 allocation failed"));
    while(true) {}  // Don't proceed, loop forever
  }
  display.cp437(true);
  display.clearDisplay();
  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(0,0);             // Start at top-left corner
  display.println(F("Temperature"));
//  display.setCursor(65,15);
//  display.setTextSize(2);
//  display.write(0xF8);
//  display.print("C");
  display.display();


  // Start the DS18B20 sensor
  sensors.begin();

  pinMode(LED_PIN,OUTPUT); digitalWrite(LED_PIN,LOW);

}



//****************************************************************************************
// Main Loop
//****************************************************************************************
void loop() {
  float newTemp = readTemp();
  
  updateDisplay(newTemp);
  Serial.print(newTemp);Serial.println("*C");

  
  oldTemp = newTemp;
  delay(5000);
}
