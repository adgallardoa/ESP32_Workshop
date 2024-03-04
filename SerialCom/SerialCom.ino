/*
 * Serial communication test!
 * --------------------------
 * 
 * This sketch show you how to enable and transmit text
 * by serial port to your computer.
 * 
 * you can use and modify this code under your own responsability
 * 
 * by Adrian Gallardo, Cristian Orrego, CEAZA
 * March 2024.
 */

void setup() {
  // this code runs only one time when the program starts
  
  delay(100);   // avoid random cpu crash at beginning of the program.

  Serial.begin(115200);   // enable serial communication with computer at 115200 Baud
  Serial.println("Board serial COM Test!");

}

void loop() {
  // put your main code here, to run repeatedly:

}
