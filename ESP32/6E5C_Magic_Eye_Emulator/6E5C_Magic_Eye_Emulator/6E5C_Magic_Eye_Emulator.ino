/* This project creates an emulation of a 6E5C magic eye valve using a GC9A01 1.28" 240 x 240 pixels round display.
*  The graphics library is Arduino GFX by https://github.com/moononournation more videos here: https://www.youtube.com/@moononournation
*  To avoid display refresh flicker the canvas function is used: https://github.com/moononournation/Arduino_GFX/wiki/Canvas-Class
*  The ESP32 pin definitions could be e.g. PIN_RES but as Arduino GFX uses GFX_xxx I did too.
*  To smooth out input and ADC fluctuations a FIFO ring buffer data averaging library is used: https://github.com/RobTillaart/RunningAverage
*/
#define GFX_RES  4 //configurable = Display Reset
#define GFX_BL  14 //configurable = Backlight control
#define GFX_DC  16 //configurable = Data/Command selection
#define GFX_SCL 18 //fixed as SPI SCLK
#define GFX_CS  22 //configurable = Display chip select
#define GFX_SDA 23 //fixed as SPI MOSI

#include <Arduino_GFX_Library.h>
#include <RunningAverage.h>

// Start of Arduino_GFX setting

/* More data bus class: https://github.com/moononournation/Arduino_GFX/wiki/Data-Bus-Class */
Arduino_DataBus *bus = new Arduino_HWSPI(GFX_DC, GFX_CS); //create data bus class

/* More display class: https://github.com/moononournation/Arduino_GFX/wiki/Display-Class */
//Arduino_GFX *gfx = new Arduino_GC9A01(bus, GFX_RES, 0 /* rotation */, true /* IPS */, 240 /* width */, 240 /* height */); // use for no canvas and disable next two lines
Arduino_G *output_display = new Arduino_GC9A01(bus, GFX_RES, 0 /* rotation */, true /* IPS */);
Arduino_GFX *gfx = new Arduino_Canvas_Indexed(240 /* width */, 240 /* height */, output_display, 0 /* output_x */, 0 /* output_y */, MAXMASKLEVEL /* mask_level */);

// End of Arduino_GFX setting

RunningAverage myRA(20); // allocates dynamic memory, one float (4 bytes) per element - 20 elements. Enables use of myRA to call functions like myRA.addValue(p)

// Declare variables
uint16_t r, r0;
uint16_t v_inVal;
uint16_t i;
float p, p_avg;

void setup(void)
{
	Serial.begin(115200); // initiate serial communication via USART

	while(!Serial); // keeps the program stuck in this loop until the serial connection is established
	Serial.println(); // send new line to USART
	Serial.println("6E5C Emulator Starting"); // send text to USART

	// Init Display
	if (!gfx->begin()) // If initialization fails, handle the error
	{
		Serial.println("gfx->begin() failed!");
		while (1);  // Halt the program (infinite loop)
	}
	r = gfx->width(); // i.e. 240 pixels
	r0 = r / 2;
	analogSetAttenuation(ADC_6db); // Input voltages can be attenuated before being input to the ADC. There are 4 available attenuation options, the higher the attenuation is, the higher the measurable input voltage could be.
	myRA.clear(); // clear the FIFO ring buffer

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

void loop()
{
	p = analogRead(34); // GPIO34 is an input only pin on an ESP32
	myRA.addValue(p); // add value of p to ring buffer
	p_avg = myRA.getAverage(); // get average value of p
	//i = map(p_avg, 50, 1000, 120, 20);
	//i=map(p_avg,50,1200,20,200);
	i = map(p_avg, 0, 4096, 0, 700); // ESP32 ADC is 12 bits i.e. 4096 values so this maps 0 to 4096 on the input pin to i = 0 (eye closed) to 700 (eye open).  Range pairs can be reversed if needed.
	if (i < 0) i = 0; // don't allow i to be negative
	drawE5(i); // draw display using mapped i value
	delay(100);
}