/*
 Name:		ESP32_basic_blink.ino
 Created:	8/10/2024 12:05:03 PM
 Author:	Steven
*/

/*
  Blink Built-in LED on Digital Pin 2
*/
const int LED = 2; // Onboard LED pin for ESP32

void setup() {
	// initialize digital pin LED_BUILTIN as an output.
	pinMode(LED, OUTPUT);
}

void loop() {
	digitalWrite(LED, HIGH);  // turn the LED on (HIGH is the voltage level)
	delay(1000);                      // wait for a second
	digitalWrite(LED, LOW);   // turn the LED off by making the voltage LOW
	delay(100);                      // wait for a second
}