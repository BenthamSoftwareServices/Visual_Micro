/************************************************************
* WHAT: Magic eye emulator FPS
* GC9A01
* library Arduino_GFX
* DETAILS:
* ESP32 with milrophone on pin 34
* rotated 180 degree
* DC ‐ 27, CS  ‐ 5, RS ‐ 33, indexed canvas
*/
/************************************************************/
#include <Arduino_GFX_Library.h>
//#include <RunningAverage.h>
#include "RunningAverage.h"
#define TFT_RST 33
Arduino_DataBus* bus
= new Arduino_ESP32SPI(27 /* DC */, 5 /* CS */, 18 /* SCK */, 23 /* MOSI */, ‐
	1 /* MISO */,
	HSPI /* spi_num */);
Arduino_G* output_display
= new Arduino_GC9A01(bus, TFT_RST,
	2 /* rotation */, true /* IPS */);
Arduino_GFX* gfx
= new Arduino_Canvas(240 /* width */, 240 /* height */, output_display);
RunningAverage myRA(20);
uint16_t r, r0;
//uint16_t d=0;
//uint16_t counter=0;
uint16_t v_inVal;
uint16_t i;
float p, p_avg;
void setup() {
	Serial.begin(115200);
	while (!Serial)
		Serial.print("Sketch: "); Serial.println(__FILE__);
	Serial.print("Uploaded: "); Serial.println(__DATE__);
	gfx‐ > begin(80000000);
	r = gfx‐ > width();
	r0 = r / 2;
	analogSetAttenuation(ADC_6db);
	myRA.clear();
}
void drawE5(uint16_t v_i) {
	gfx‐ > fillScreen(gfx‐ > color565(37, 234, 61));
	gfx‐ > fillTriangle(r0, r0 + 10, r0‐v_i, r, r0 + v_i, r, gfx‐ > color565(28, 152, 53));
	gfx‐ > fillTriangle(0, r0, r0, r0‐8, r0, r0 + 8, gfx‐ > color565(20, 140, 40));
	gfx‐ > fillTriangle(r, r0, r0, r0‐8, r0, r0 + 8, gfx‐ > color565(20, 140, 40));
	gfx‐ > fillCircle(r0 + 2, r0 + 2, 52, gfx‐ > color565(40, 40, 40));
	gfx‐ > fillCircle(r0‐3, r0‐2, 52, gfx‐ > color565(150, 162, 160));
	gfx‐ > fillCircle(r0, r0, 52, gfx‐ > color565(71, 76, 69));
	gfx‐ > flush();
}
void loop() {
	p = analogRead(34);
	myRA.addValue(p);
	p_avg = myRA.getAverage();
	i = map(p_avg, 50, 1000, 120, 20);
	// i=map(p_avg,50,1200,20,200);
	if (i < 0) i = 0;
	drawE5(i);
}