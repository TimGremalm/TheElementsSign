//#include <Adafruit_NeoPixel.h>  // Library Manager > Adafruit NeoPixel Version >= 1.2.2
#include <WS2812FX.h>             // Library Manager > WS2812FX >= 1.1.9

//Led strip WS2812
#define WS2812PIN 3
#define WS2812PIXELS 70

//Button for changing mode
#define BUTTONPIN 13
bool buttonHasBeenRelesed = true;
unsigned long buttonLastReleasedTime;

//Lighting Mode
int mode = 0;
#define MODEMAX 4

//Light configuration
#define LIGHTLEVEL 20
#define ELEMENT_FIRE_START 0
#define ELEMENT_FIRE_LENGTH 8
#define ELEMENT_AIR_START 10
#define ELEMENT_AIR_LENGTH 8
#define ELEMENT_WATER_START 20
#define ELEMENT_WATER_LENGTH 8
#define ELEMENT_EARTH_START 30
#define ELEMENT_EARTH_LENGTH 8
#define ELEMENT_EATHER_START 40
#define ELEMENT_EATHER_LENGTH 8
#define ELEMENTS_START 50
#define ELEMENTS_LENGTH 18

WS2812FX ws2812fx = WS2812FX(WS2812PIXELS, WS2812PIN, NEO_GRB + NEO_KHZ800);

void setup() {
	Serial.begin(115200);
	Serial.print("Start Elements light");
	pinMode(BUTTONPIN, INPUT);
	ws2812fx.init();
	ws2812fx.setBrightness(LIGHTLEVEL);
	ws2812fx.start();
	changeMode();
}

void checkLightLevelPot() {
	int potLightLevel = analogRead(A0);
	potLightLevel = map(potLightLevelValue, 0, 700, 0, 255);
	Serial.println(potLightLevel);
}

void checkModeButton() {
	int buttonState = digitalRead(BUTTONPIN);
	if (buttonState) {
		// Reset buton release to now
		buttonLastReleasedTime = millis();
		buttonHasBeenRelesed = true;
	} else {
		// Make sure button is pushed down for at least some milliseconds
		if (millis()-buttonLastReleasedTime > 50) {
			if (buttonHasBeenRelesed) {
				buttonHasBeenRelesed = false;
				changeMode();
			}
		}
	}
}

void changeMode() {
	mode += 1;
	if (mode >= MODEMAX) {
		mode = 0;
	}
	Serial.print("Changed mode to ");
	Serial.println(mode);
	ws2812fx.resetSegments();
	switch (mode) {
		case 0: //Black scene
			for(int i = 0; i < WS2812PIXELS; i += 1) { 
				ws2812fx.setPixelColor(i, 0, 0 ,0);
			}
			ws2812fx.setColor(0, 0, 0);
			ws2812fx.mode_static();
			break;

		case 1: //Static lights
			// parameters:   index, start,                stop,                                         mode,                         color,    speed, reverse
			ws2812fx.setSegment(0,  ELEMENT_FIRE_START,   ELEMENT_FIRE_START+ELEMENT_FIRE_LENGTH-1,     FX_MODE_STATIC,               0xFF1100, 1000, false);
			ws2812fx.setSegment(1,  ELEMENT_AIR_START,    ELEMENT_AIR_START+ELEMENT_AIR_LENGTH-1,       FX_MODE_STATIC,               0xFF7700, 1000, false);
			ws2812fx.setSegment(2,  ELEMENT_WATER_START,  ELEMENT_WATER_START+ELEMENT_WATER_LENGTH-1,   FX_MODE_STATIC,               0x0011FF, 1000, false);
			ws2812fx.setSegment(3,  ELEMENT_EARTH_START,  ELEMENT_EARTH_START+ELEMENT_EARTH_LENGTH-1,   FX_MODE_STATIC,               0x00FF00, 1000, false);
			ws2812fx.setSegment(4,  ELEMENT_EATHER_START, ELEMENT_EATHER_START+ELEMENT_EATHER_LENGTH-1, FX_MODE_STATIC,               0x9900FF, 800, false);
			ws2812fx.setSegment(5,  ELEMENTS_START,       ELEMENTS_START+ELEMENTS_LENGTH-1,             FX_MODE_STATIC,               0xFFFF99, 800, false);
			break;

		case 2: //Animated elements - static text
		case 3: //Animated elements - rainbow text
			uint32_t waterColors[] = {0x0011FF, 0x0055FF, 0x0000FF};
			uint32_t earthColors[] = {0x00FF00, 0x00FF00, 0x44FF44};
			// parameters:   index, start,                stop,                                         mode,                         color,    speed, reverse
			ws2812fx.setSegment(0,  ELEMENT_FIRE_START,   ELEMENT_FIRE_START+ELEMENT_FIRE_LENGTH-1,     FX_MODE_FIRE_FLICKER_INTENSE, 0xFF6600, 500, false);
			ws2812fx.setSegment(1,  ELEMENT_AIR_START,    ELEMENT_AIR_START+ELEMENT_AIR_LENGTH-1,       FX_MODE_BREATH,               0xFF7700, 8000, false);
			ws2812fx.setSegment(2,  ELEMENT_WATER_START,  ELEMENT_WATER_START+ELEMENT_WATER_LENGTH-1,   FX_MODE_TRICOLOR_CHASE,       waterColors, 1200, false);
			ws2812fx.setSegment(3,  ELEMENT_EARTH_START,  ELEMENT_EARTH_START+ELEMENT_EARTH_LENGTH-1,   FX_MODE_TRICOLOR_CHASE,       earthColors, 2000, true);
			ws2812fx.setSegment(4,  ELEMENT_EATHER_START, ELEMENT_EATHER_START+ELEMENT_EATHER_LENGTH-1, FX_MODE_COMET,                0x9900FF, 800, false);
			if (mode == 2) {
				ws2812fx.setSegment(5,  ELEMENTS_START,   ELEMENTS_START+ELEMENTS_LENGTH-1,             FX_MODE_STATIC,               0xFFFF99, 1000, false);
			}
			if (mode == 3) {
				ws2812fx.setSegment(5,  ELEMENTS_START,   ELEMENTS_START+ELEMENTS_LENGTH-1,             FX_MODE_RAINBOW_CYCLE,        0xFFFF99, 1000, false);
			}
			break;
	}
}

void tick() {
	ws2812fx.service();
}

void loop() {
	checkModeButton();
	checkLightLevelPot();
	tick();
	delay(20);
}
