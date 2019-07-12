#include <stdint.h>
#include "colorhslrgb.h"

/** Helper method that converts hue to rgb */
float hueToRgb(float p, float q, float t) {
	if (t < (float)0)
			t += (float)1;
	if (t > (float)1)
			t -= (float)1;
	if (t < (float)1/(float)6)
			return p + (q - p) * (float)6 * t;
	if (t < (float)1/(float)2)
			return q;
	if (t < (float)2/(float)3)
			return p + (q - p) * ((float)2.0/ (float)3.0 - t) * (float)6;
	return p;
}

/* Converts an HSL color value to RGB. Conversion formula
-- adapted from http://en.wikipedia.org/wiki/HSL_color_space.
-- Assumes h, s, and l are contained in the set [0, 1] and
-- returns r, g, and b in the set [0, 1].
--
-- @param	Number	h				The hue
-- @param	Number	s				The saturation
-- @param	Number	l				The lightness
-- @return	Array					The RGB representation*/
void hslToRgb(float h, float s, float l, int rgb[]) {
	float r, g, b;

	if (s == (float)0) {
			r = g = b = l; // achromatic
	} else {
			float q = l < 0.5 ? l * (1 + s) : l + s - l * s;
			float p = 2 * l - q;
			r = hueToRgb(p, q, h + (float)1/(float)3);
			g = hueToRgb(p, q, h);
			b = hueToRgb(p, q, h - (float)1/(float)3);
	}
	rgb[0] = (int) (r * 255);
	rgb[1] = (int) (g * 255);
	rgb[2] = (int) (b * 255);
}

uint32_t ConvertToStripColor(uint8_t r, uint8_t g, uint8_t b) {
	return ((uint32_t)r << 16) | ((uint32_t)g <<  8) | b;
}

uint32_t HSLtoColor(float H, float S, float L) {
	int nRGB[3] = {0,0,0};
	hslToRgb(H, S, L, nRGB);
	return ConvertToStripColor(nRGB[0], nRGB[1], nRGB[2]);
}
