/* This project creates an emulation of a 6E5C magic eye valve using a GC9A01 1.28" 240 x 240 pixels round display.
*  The graphics library is Arduino GFX by https://github.com/moononournation more videos here: https://www.youtube.com/@moononournation
*  To avoid display refresh flicker the canvas function is used: https://github.com/moononournation/Arduino_GFX/wiki/Canvas-Class
*  The ESP32 pin definitions could be e.g. PIN_RES but as Arduino GFX uses GFX_xxx I did too.
*  The checks function ramps up and down to open and close the magic eye
*/
#define GFX_RES  4 //configurable = Display Reset
#define GFX_BL  14 //configurable = Backlight control
#define GFX_DC  16 //configurable = Data/Command selection
#define GFX_SCL 18 //fixed as SPI = SCLK
#define GFX_CS  22 //configurable = Display chip select
#define GFX_SDA 23 //fixed as SPI = MOSI

#include <Arduino_GFX_Library.h>

// Start of Arduino_GFX setting

/* More data bus class: https://github.com/moononournation/Arduino_GFX/wiki/Data-Bus-Class */
Arduino_DataBus *bus = new Arduino_HWSPI(GFX_DC, GFX_CS); //create data bus class

/* More display class: https://github.com/moononournation/Arduino_GFX/wiki/Display-Class */
//Arduino_GFX *gfx = new Arduino_GC9A01(bus, GFX_RES, 0 /* rotation */, true /* IPS */, 240 /* width */, 240 /* height */); // use for no canvas and disable next two lines
Arduino_G *output_display = new Arduino_GC9A01(bus, GFX_RES, 0 /* rotation */, true /* IPS */);
Arduino_GFX *gfx = new Arduino_Canvas_Indexed(240 /* width */, 240 /* height */, output_display, 0 /* output_x */, 0 /* output_y */, MAXMASKLEVEL /* mask_level */);

// End of Arduino_GFX setting

// Declare variables
uint16_t r, r0;
uint16_t v_inVal;
uint16_t i;
bool direction = true; // count direction true = up and false = down
uint16_t top = 600; //highest value for i
uint16_t bottom = 5; //lowest value for i

void setup(void)
{
	Serial.begin(115200); // initiate serial communication via USART

	while (!Serial); // keeps the program stuck in this loop until the serial connection is established
	Serial.println(); // send new line to USART
	Serial.println("6E5C Moving Eye Starting"); // send text to USART

	// Init Display
	if (!gfx->begin()) // If initialization fails, handle the error
	{
		Serial.println("gfx->begin() failed!");
		while (1);  // Halt the program (infinite loop)
	}
	r = gfx->width(); // i.e. 240 pixels
	r0 = r / 2;

#ifdef GFX_BL
	pinMode(GFX_BL, OUTPUT); //Setting GFX_BL as an output causes the GFX to go low as this is the reset condition
	delay(10);
	digitalWrite(GFX_BL, HIGH); //GFX_BL high to enable display backlight LED, or leave BL pin from display unconnected as it floats high.
#endif
}

// build 6E5C emulation in the canvas function and then write (flush) to the display.
void drawE5(uint16_t v_i) {
	gfx->fillScreen(gfx->color565(37, 234, 61));
	gfx->fillTriangle(r0, r0 + 10, r0 - v_i, r, r0 + v_i, r, gfx->color565(28, 152, 53));
	gfx->fillTriangle(0, r0, r0, r0 - 8, r0, r0 + 8, gfx->color565(20, 140, 40));
	gfx->fillTriangle(r, r0, r0, r0 - 8, r0, r0 + 8, gfx->color565(20, 140, 40));
	gfx->fillCircle(r0 + 2, r0 + 2, 52, gfx->color565(40, 40, 40));
	gfx->fillCircle(r0 - 3, r0 - 2, 52, gfx->color565(150, 162, 160));
	gfx->fillCircle(r0, r0, 52, gfx->color565(71, 76, 69));
	gfx->flush();
}

void checks()
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

void loop()
{
	checks(); // do boolean and numeric checks
	drawE5(i); // draw display using mapped i value
	delay(20); // 20mS delay
}