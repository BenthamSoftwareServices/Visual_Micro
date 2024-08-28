#define FASTLED_INTERNAL //This is vital to add before FastLED include or the compiler outputs a harmless but annoying message about unused SPI pins
#include <FastLED.h>

constexpr auto LED_PIN = 48;
constexpr auto NUM_LEDS = 1;
int count = 0;

CRGB leds[NUM_LEDS];

void setup() {

	//FastLED.addLeds<WS2812, DATA_PIN, RGB>(leds, NUM_LEDS);  // GRB ordering is typical
	FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);

}

void loop() {
	//Choose colour using auto-complete, just start typing a colour
	leds[0] = CRGB(10, 0, 0); //Red
	FastLED.show();
	count++;
	delay(1000);

	leds[0] = CRGB(0, 10, 0); //Green
	FastLED.show();
	count++;
	delay(1000);

	leds[0] = CRGB(0, 0, 10); //Blue
	FastLED.show();
	count++;
	delay(1000);
}