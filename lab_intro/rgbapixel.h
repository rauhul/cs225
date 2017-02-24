#include <cstdint>
using std::uint8_t;

#ifndef RBGAPixel_H
#define RBGAPixel_H

//Represents a single pixel in an image
class RGBAPixel {
	public:
		//Bytes for the channel components of the pixel
		uint8_t red, green, blue, alpha;
		//Constructs a default RGBAPixel
		RGBAPixel();
		//Constructs an opaque RGBAPixel with the given red, green, blue color values
		RGBAPixel(uint8_t red, uint8_t green, uint8_t blue);
};

#endif