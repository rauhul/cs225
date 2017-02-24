#include "rgbapixel.h"
using std::uint8_t;

/*
 * Constructs a default RGBAPixel: white opaque
 */
RGBAPixel::RGBAPixel() {
	red   = 255;
	green = 255;
	blue  = 255;
	alpha = 255;
}

/*
 * Constructs an opaque RGBAPixel with color from the input channel values
 */
RGBAPixel::RGBAPixel(uint8_t _red, uint8_t _green, uint8_t _blue) {
	red   = _red;
	green = _green;
	blue  = _blue;
	alpha = 255;
}