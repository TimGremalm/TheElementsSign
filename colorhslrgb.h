#ifndef COLORHSLRGB_H_
#define COLORHSLRGB_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

float hueToRgb(float p, float q, float t);
void hslToRgb(float h, float s, float l, int rgb[]);
uint32_t HSLtoColor(float H, float S, float L);

#ifdef __cplusplus
}
#endif

#endif /* COLORHSLRGB_H_ */
