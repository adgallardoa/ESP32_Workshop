/*
 * DS18B20 / 1-Wire digital temperature sensor Test!
 * 
 * This scketch show how to get temperature data from a DS18B20 sensor connected to the microcontroller.
 * 
 * You can use and modify this program under your own responsability
 * 
 * by Adrian Gallardo, Cristian Orrego
 * CEAZA
 * 
 * May 2024
 */
#include <OneWire.h>              // Communication protocol (1-Wire) used by the sensor
#include <DallasTemperature.h>    // Temperature sensor controller library

const int oneWireBus = 16;     // GPIO where the DS18B20 is connected to
const int DS18B20_VCC_PIN = 17;
const int DS18B20_GND_PIN = 5;

OneWire oneWire(oneWireBus);  // Setup a oneWire instance to communicate with any OneWire devices
DallasTemperature sensors(&oneWire);  // Pass our oneWire reference to Dallas Temperature sensor 



void setup() {
  delay(100);   // to avoid initial random crash on ESP32 microontroller

  // powering temerature sensor from IO pins (not ideal)
  pinMode(DS18B20_VCC_PIN,OUTPUT);
  digitalWrite(DS18B20_VCC_PIN,HIGH);
  pinMode(DS18B20_GND_PIN,OUTPUT);
  digitalWrite(DS18B20_GND_PIN,LOW);
  delay(100);
  
  Serial.begin(115200);
  while(!Serial){}
  Serial.println("\nDS-18B20 Temperature Sensor Test!");
  Serial.println("----------------------------------\n");

  // Start the DS18B20 sensor
  sensors.begin();
}



void loop() {
  sensors.requestTemperatures();  // Get data from sensor
  
  float temperatureC = sensors.getTempCByIndex(0);    // put celsius temperature data on temperatureC variable
  
  Serial.print(temperatureC);     // Show temperature value by serial
  Serial.println("*C");
  
  delay(5000);                    // wait 5 seg. 

  // and start again the loop...
}
