
//	This project flashes the UNOs onboard LED on D13 using the non-blocking vTaskDelay method


constexpr auto LED = 2; // for info re constexpr see: https://learn.microsoft.com/en-us/cpp/cpp/constexpr-cpp?view=msvc-170

void setup() {
	pinMode(LED, OUTPUT);
	Serial.begin(115200); 	// Start serial communication at a baud rate of 115200
	Serial.println();  // Adds an extra newline
	Serial.println("Delay using vTaskDelay is starting"); 	// Print a welcome message to the Serial Monitor
	xTaskCreate(
		toggleLED,           // Task function name, the RTOS task handler in an ESP32 will look for a function with this name and start running it.
		"Toggle LED Task",   // Name of the task (for debugging)
		1000,                // Stack size (in words)
		NULL,                // Parameter to pass to the task
		1,                   // Task priority (higher number = higher priority), max is 25
		NULL                 // A task handle. This can be used to control the task later (e.g., suspend, resume, or delete the task)
	);
}

void loop() {
	// Insert a thousand NOP instructions using a loop for a 4.17mS delay
	for (int i = 0; i < 1000; ++i) {
		__asm__ __volatile__("nop"); // An assembler NOP is a single CPU clock cycle and at 240MHz is equal to 4.17 nanoseconds
	}
}

static void toggleLED(void *parameter) {
	while (true) {
		digitalWrite(LED, !digitalRead(LED));
		vTaskDelay(1000 / portTICK_PERIOD_MS);  // Delay for 1 second
		UBaseType_t highWaterMark = uxTaskGetStackHighWaterMark(NULL); // get the size of the task stack in use
		Serial.print("The size of the task stack is : ");
		Serial.println(highWaterMark); // print the size of the task stack to serial
		}
}