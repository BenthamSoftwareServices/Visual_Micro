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