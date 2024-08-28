
//	This project flashes the UNOs onboard LED on D13 using the non-blocking Millis method

unsigned long previousMillis = 0;  // Stores the last time the LED was updated
const long interval = 1000;   // Interval at which to blink the LED (milliseconds)

constexpr auto LED = 13; // for info re constexpr see: https://learn.microsoft.com/en-us/cpp/cpp/constexpr-cpp?view=msvc-170

void setup() {
	pinMode(LED, OUTPUT);
}

void loop() {
	//delay for interval
	// Get the current time in milliseconds
	unsigned long currentMillis = millis();
	if (currentMillis - previousMillis >= interval) {
		// Save the last time the LED was toggled
		previousMillis = currentMillis;
		// Toggle the LED
		digitalWrite(LED, !digitalRead(LED));
	}
}