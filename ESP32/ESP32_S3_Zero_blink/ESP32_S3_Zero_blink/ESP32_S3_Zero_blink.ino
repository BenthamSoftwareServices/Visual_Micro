#define FASTLED_INTERNAL //This is vital to add before FastLED include or the compiler outputs a harmless but annoying message about unused SPI pins
#include <FastLED.h>

constexpr auto LED_PIN = 21;
constexpr auto NUM_LEDS = 1;

CRGB leds[NUM_LEDS];

void setup() {

	//FastLED.addLeds<WS2812, DATA_PIN, RGB>(leds, NUM_LEDS);  // GRB ordering is typical
	FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
}

void loop() {
	// Choose colour using auto-complete, just start typing a colour
	// Brightness values should be adjusted to compensate for the differing efficiencies of the various LED colours
	leds[0] = CRGB(3, 0, 0); //Green
	FastLED.show();
	delay(200);

	leds[0] = CRGB(0, 5, 0); //Red
	FastLED.show();
	delay(1000);

	leds[0] = CRGB(0, 0, 5); //Blue
	FastLED.show();
	delay(1000);
}