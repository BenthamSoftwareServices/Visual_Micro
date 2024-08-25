/*
  Blink

  Turns an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the UNO, MEGA and ZERO
  it is attached to digital pin 13, on MKR1000 on pin 6. LED_BUILTIN is set to
  the correct LED pin independent of which board is used.
  If you want to know what pin the on-board LED is connected to on your Arduino
  model, check the Technical Specs of your board at:
  https://www.arduino.cc/en/Main/Products
/*
  Blink Built-in LED on Digital Pin 2
*/
constexpr auto LED = 2; // for info re constexpr see: https://learn.microsoft.com/en-us/cpp/cpp/constexpr-cpp?view=msvc-170
int count = 0;

void setup() {
	// initialize digital pin LED_BUILTIN as an output.
	pinMode(LED, OUTPUT);
}

void loop() {
	digitalWrite(LED, HIGH);  // turn the LED off (HIGH is the voltage level)
	delay(1000);              // wait for a second
	count++;
	digitalWrite(LED, LOW);   // turn the LED on by making the voltage LOW
	delay(1000);              // wait for a second
	count++;
}