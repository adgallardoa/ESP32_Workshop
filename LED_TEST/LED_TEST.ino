/*
 * LED Test
 * 
 * This sketch shows how to control an LED connected to a microcontroller pin
 * You must take a warness with the current you will drain from the pin.
 * It must be < 10mA to avoid permanently damage the microcontroller.
 * 
 * You can use and modify this code under your own responsability 
 * 
 * by Adrian Gallardo, Cristian Orrego
 * CEAZA
 * March 2024
 */

const int LED_PIN = 2;        // GPIO where the LED is connected to

void setup() {
  pinMode(LED_PIN,OUTPUT);    // set pin as digital output (can be at 0V or 3.3V depending on the program)
  digitalWrite(LED_PIN,LOW);  // set pin at 0V output
}

void loop() {
  digitalWrite(LED_PIN,HIGH); // set LED pin at 3.3V (LED ON)
  delay(500);
  digitalWrite(LED_PIN,LOW);  // set LED pin at 0V (LED OFF)
  delay(500);
}
