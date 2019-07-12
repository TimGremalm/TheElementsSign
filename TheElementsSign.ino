//#include <Adafruit_NeoPixel.h>  // Library Manager > Adafruit NeoPixel Version >= 1.2.2
#include <WS2812FX.h>             // Library Manager > WS2812FX >= 1.1.9

//Led strip WS2812
#define WS2812PIN 3
#define WS2812PIXELS 70

//Button for changing mode
#define BUTTON_1_PIN 13
int buttonPins[] = {12, 11, 10, 9, 8, 7, 6, 5};

//Lighting Mode
int mode = 0;
int modeLast = -1;
int speed = -10;

int potLightLevelLast = -10;
int potSpeedLast = -10;

//Light configuration
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
	Serial.println("Start Elements light");
	for (int i=0; i < 8; i++) {
		pinMode(buttonPins[i], INPUT);
	}
	ws2812fx.init();
	ws2812fx.setBrightness(1);
	ws2812fx.start();
}

void checkLightLevelPot() {
	int potLightLevel = analogRead(A1);
	potLightLevel = map(potLightLevel, 0, 1023, 1, 255);
	if ((potLightLevelLast-potLightLevel > 10) || (potLightLevelLast-potLightLevel < -10)) {
		Serial.print("Light level changed to ");
		Serial.println(potLightLevel);
		ws2812fx.setBrightness(potLightLevel);
		potLightLevelLast = potLightLevel;
	}
}

void checkModeButton() {
	int buttonState = 0;
	for (int i=0; i < 8; i++) {
		buttonState = digitalRead(buttonPins[i]);
		if (!buttonState) {
			mode = i;
		}
	}
	if (mode != modeLast) {
		changeMode();
		modeLast = mode;
	}
}

void changeMode() {
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
		case 4: //Animated elements - sparkle text
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
			if (mode == 4) {
				ws2812fx.setSegment(5,  ELEMENTS_START,   ELEMENTS_START+ELEMENTS_LENGTH-1,             FX_MODE_SPARKLE,  0xFFFF99, 1000, false);
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
