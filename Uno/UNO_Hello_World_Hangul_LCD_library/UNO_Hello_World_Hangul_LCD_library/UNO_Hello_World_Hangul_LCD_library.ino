/*
 Name:		UNO_Hello_World_Hangul_LCD_library.ino
 Created:	8/9/2024 6:11:35 PM
 Author:	Steven

 Not really Hello World but an example of using the Hangul LCD library for the Korean alphabet
*/

#include<LiquidCrystal_I2C_Hangul.h>
#include<Wire.h>

unsigned long seconds;
unsigned long minutes;
unsigned long hours;
unsigned long currentMillis;

LiquidCrystal_I2C_Hangul lcd(0x27, 16, 2);

void setup() {
	Serial.begin(115200);
	lcd.init();
	lcd.backlight();
	lcd.clear();
	delay(100);
	lcd.setCursor(0, 0); //Column starting at 0, Row 0 for top and 1 for bottom
	lcd.print("Time since start");
	delay(100);
	Serial.println("Setup complete");
}

void loop() {
	time_elapsed();
	delay(1000);
	time_elapsed_to_LCD();
}

void time_elapsed_to_LCD() {
	lcd.setCursor(0, 1); //Column starting at 0, Row 0 for top and 1 for bottom

	if (hours < 10) { // if hours is less than 10, print a leading zero
		lcd.print("0");
	}
	lcd.print(hours);
	lcd.print(":");

	if (minutes < 10) { // if minutes is less than 10, print a leading zero
		lcd.print("0");
	}
	lcd.print(minutes);
	lcd.print(":");

	if (seconds < 10) { // if seconds is less than 10, print a leading zero
		lcd.print("0");
	}
	lcd.print(seconds);
}

void time_elapsed() {
	currentMillis = millis(); // get the current number of milliseconds since sketch uploaded, which is why the count starts about 3 seconds with setup() delays of 1s but changing to 100mS the count starts at 1s
	seconds = currentMillis / 1000; // convert milliseconds to seconds
	minutes = seconds / 60; // convert seconds to minutes
	hours = minutes / 60; // convert minutes to hours
	seconds %= 60; // calculate remaining seconds after minutes are removed
	minutes %= 60; // calculate remaining minutes after hours are removed
	Serial.print("Time since start: "); // print a message
	Serial.print(hours); // print hours
	Serial.print(":");
	Serial.print(minutes); // print minutes
	Serial.print(":");
	Serial.println(seconds); // print seconds
	//delay(1000); // wait for 1 second - not needed, has no function
}