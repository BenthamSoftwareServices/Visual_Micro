
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
Arduino_G *output_display = new Arduino_GC9A01(bus, GFX_RES, 0 /* rotation */, true /* IPS */);
Arduino_GFX *gfx = new Arduino_Canvas_Indexed(240 /* width */, 240 /* height */, output_display, 0 /* output_x */, 0 /* output_y */, MAXMASKLEVEL /* mask_level */);

// End of Arduino_GFX setting

RunningAverage myRA(20); // allocates dynamic memory, one float (4 bytes) per element - 20 elements

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
	if (!gfx->begin())
	{
		Serial.println("gfx->begin() failed!");
	}
	r = gfx->width();
	r0 = r / 2;
	analogSetAttenuation(ADC_6db);
	myRA.clear();

#ifdef GFX_BL
	pinMode(GFX_BL, OUTPUT); //Setting GFX_BL as an output causes the GFX to go low as this is the reset condition
	delay(10);
	digitalWrite(GFX_BL, HIGH); //GFX_BL high to enable display backlight LED
#endif
}

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
	p = analogRead(34);
	myRA.addValue(p);
	p_avg = myRA.getAverage();
	//i = map(p_avg, 50, 1000, 120, 20);
	//i=map(p_avg,50,1200,20,200);
	i = map(p_avg, 0, 4096, 0, 700);
	//i = p_avg;
	if (i < 0) i = 0;
	drawE5(i);
	delay(100);
}