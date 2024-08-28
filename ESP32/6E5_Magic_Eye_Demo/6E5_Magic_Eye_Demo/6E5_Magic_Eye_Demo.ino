/* 
   This project creates an emulation of a 6E5C magic eye valve using a GC9A01 1.28" 240 x 240 pixels round display.
*  The graphics library is Arduino GFX by https://github.com/moononournation more videos here: https://www.youtube.com/@moononournation
*  To avoid display refresh flicker the canvas function is used: https://github.com/moononournation/Arduino_GFX/wiki/Canvas-Class
*  The ESP32 pin definitions could be e.g. PIN_RES but as Arduino GFX uses GFX_xxx I did too.
*/

#define GFX_RES  4 //configurable = Display Reset
#define GFX_BL  14 //configurable = Backlight control
#define GFX_DC  16 //configurable = Data/Command selection
#define GFX_SCL 18 //fixed as SPI = SCLK
#define GFX_CS  22 //configurable = Display chip select
#define GFX_SDA 23 //fixed as SPI = MOSI

#include <Arduino_GFX_Library.h>

// Declare variables
uint16_t r, r0;
uint16_t v_inVal;
uint16_t i;
bool direction = true; // count direction true = up and false = down
uint16_t top = 600; //highest value for i
uint16_t bottom = 5; //lowest value for i

unsigned long previousMillis = 0;  // Stores the last time the LED was updated
const long interval = 1000;   // Interval at which to blink the LED (milliseconds)

constexpr auto LED = 2; // for info re constexpr see: https://learn.microsoft.com/en-us/cpp/cpp/constexpr-cpp?view=msvc-170
int count = 0;

// Start of Arduino_GFX setting

/* More data bus class: https://github.com/moononournation/Arduino_GFX/wiki/Data-Bus-Class */
Arduino_DataBus *bus = new Arduino_HWSPI(GFX_DC, GFX_CS); //create data bus class

/* More display class: https://github.com/moononournation/Arduino_GFX/wiki/Display-Class */
//Arduino_GFX *gfx = new Arduino_GC9A01(bus, GFX_RES, 0 /* rotation */, true /* IPS */, 240 /* width */, 240 /* height */); // use for no canvas and disable next two lines
Arduino_G *output_display = new Arduino_GC9A01(bus, GFX_RES, 0 /* rotation */, true /* IPS */);
Arduino_GFX *gfx = new Arduino_Canvas_Indexed(240 /* width */, 240 /* height */, output_display, 0 /* output_x */, 0 /* output_y */, MAXMASKLEVEL /* mask_level */);

// End of Arduino_GFX setting

void setup() {

	// Init Display
	if (!gfx->begin()) // If initialization fails, handle the error
	{
		while (1); {  // Halt the program (infinite loop) if gfx initialisation fails and flash the on-board LED quickly
			digitalWrite(LED, HIGH);  // turn the LED off (HIGH is the voltage level)
			delay(100);
			digitalWrite(LED, LOW);   // turn the LED on by making the voltage LOW
			delay(100);
		}
	}
	r = gfx->width(); // i.e. 240 pixels
	r0 = r / 2;

	// initialize digital pin LED_BUILTIN as an output.
	pinMode(LED, OUTPUT);
}

// build 6E5C emulation in the canvas function and then write (flush) to the display.
static void drawE5(uint16_t v_i) {
	gfx->fillScreen(gfx->color565(37, 234, 61));
	gfx->fillTriangle(r0, r0 + 10, r0 - v_i, r, r0 + v_i, r, gfx->color565(28, 152, 53));
	gfx->fillTriangle(0, r0, r0, r0 - 8, r0, r0 + 8, gfx->color565(20, 140, 40));
	gfx->fillTriangle(r, r0, r0, r0 - 8, r0, r0 + 8, gfx->color565(20, 140, 40));
	gfx->fillCircle(r0 + 2, r0 + 2, 52, gfx->color565(40, 40, 40));
	gfx->fillCircle(r0 - 3, r0 - 2, 52, gfx->color565(150, 162, 160));
	gfx->fillCircle(r0, r0, 52, gfx->color565(71, 76, 69));
	gfx->flush();
}

static void checks()
{
	if (direction) { // if direction is true add 10 to i
		i = i + 10;
	}
	else {
		i = i - 10; // if direction is false subtract 10 from i
	}

	if (i > top) { // when i is greater than top set direction to false and set i = top
		direction = false;
		i = top;
	}

	if (i < bottom) { // when i is less than bottom set direction to true and set i = bottom
		direction = true;
		i = bottom;
	}
}

static void flash() {

	// Get the current time in milliseconds
	unsigned long currentMillis = millis();

	// Check if the interval has passed
	if (currentMillis - previousMillis >= interval) {
		// Save the last time the LED was toggled
		previousMillis = currentMillis;

		// If the LED is off, turn it on. If it's on, turn it off.
		digitalWrite(LED, !digitalRead(LED));
		// count the number of flashes
		count++;
	}
}

void loop() {

	checks();  // do boolean and numeric checks
	drawE5(i); // draw display using mapped i value
	delay(20); // 20mS delay

	flash(); // Flash on-board LED slowly
}