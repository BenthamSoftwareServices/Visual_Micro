
#define GFX_RES  4 //configurable = Display Reset
#define GFX_BL  14 //configurable = Backlight control
#define GFX_DC  16 //configurable = Data/Command selection
#define GFX_SCL 18 //fixed as SPI SCLK
#define GFX_CS  22 //configurable = Display chip select
#define GFX_SDA 23 //fixed as SPI MOSI

#include <Arduino_GFX_Library.h>

// Start of Arduino_GFX setting

/* More data bus class: https://github.com/moononournation/Arduino_GFX/wiki/Data-Bus-Class */
Arduino_DataBus *bus = new Arduino_HWSPI(GFX_DC, GFX_CS); //create data bus class

/* More display class: https://github.com/moononournation/Arduino_GFX/wiki/Display-Class */
Arduino_G *output_display = new Arduino_GC9A01(bus, GFX_RES, 0 /* rotation */, true /* IPS */);
Arduino_GFX *gfx = new Arduino_Canvas_Indexed(240 /* width */, 240 /* height */, output_display, 0 /* output_x */, 0 /* output_y */, MAXMASKLEVEL /* mask_level */);

// End of Arduino_GFX setting

void setup(void)
{
	Serial.begin(115200);
	// Serial.setDebugOutput(true);
	// while(!Serial);
	Serial.println("Arduino_GFX Hello World example");

	// Init Display
	if (!gfx->begin())
	{
		Serial.println("gfx->begin() failed!");
	}
	gfx->fillScreen(BLACK);
	delay(5000);
	gfx->flush();
	gfx->fillScreen(gfx->color565(37, 234, 61));
	delay(5000); // 5 seconds
	gfx->flush();

#ifdef GFX_BL
	delay(5000);
	pinMode(GFX_BL, OUTPUT); //Setting GFX_BL as an output causes the GFX to go low as this is the reset condition
	digitalWrite(GFX_BL, LOW); //GFX_BL low to disable display backlight LED
	delay(5000);
	digitalWrite(GFX_BL, HIGH); //GFX_BL high to enable display backlight LED
#endif

	gfx->setCursor(12, 100);
	gfx->setTextSize(2, 2);
	gfx->setTextColor(RED);
	gfx->println("Hello from setup!");

	delay(5000); // 5 seconds
	gfx->flush();
}

void loop()
{
	gfx->setCursor(random(gfx->width()), random(gfx->height()));
	gfx->setTextColor(random(0xffff), random(0xffff));
	//gfx->setTextSize(random(6) /* x scale */, random(6) /* y scale */, random(2) /* pixel_margin */);
	gfx->setTextSize(2, 2);
	gfx->println("Hello from loop!");

	delay(5000); // 1 second
	gfx->flush();
}