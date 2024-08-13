/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.
*/

int count = 0;

void setup() {
	// initialize digital pin 13 as an output.
	pinMode(13, OUTPUT);
	Serial.begin(115200);
}

void loop() {
	digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
	delay(100);				// wait for a second
	Serial.println("The LED is off");
	digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
	delay(1000);              // wait for a second
	Serial.println("The LED is on!");
	count++;
}