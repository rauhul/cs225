#include "image.h"
using std::uint8_t;

// MARK: - Public Methods
Image::Image(): PNG() {
	x = 0;
	y = 0;
}

Image::Image(string const & file_name): PNG(file_name) {
	x = 0;
	y = 0;
}

Image::Image(size_t width, size_t height): PNG(width, height) {
	x = 0;
	y = 0;
}

Image::Image(Image const &source) {
	copy(source);
}

Image const & Image::operator=(Image const &source) {
	if (this != &source) {
		clear();
		copy(source);
	}
	return *this;
}

Image::~Image() {
	clear();
}

void Image::copy(Image const &source) {
	this->x = source.x;
	this->y = source.y;
	this->resize(source.width(),source.height());
	for(size_t x = 0; x < source.width(); x++) {
		for(size_t y = 0; y < source.height(); y++) {
			(*this)(x,y)->red   = source(x,y)->red;
			(*this)(x,y)->green = source(x,y)->green;
			(*this)(x,y)->blue  = source(x,y)->blue;
		}
	}
}

void Image::clear() {
	
}

// Flips the image about a vertical line through its center by swapping pixels
void Image::flipleft() {
	for (size_t x = 0; x < this->width()/2; x++ ) {
		for (size_t y = 0; y < this->height(); y++ ) {
			swapPixels((*this)(x,y), (*this)(this->width()-1-x,y));
		}
	}
}

// Adds to the red, green, and blue parts of each pixel in the image
void Image::adjustbrightness(int r, int g, int b) {
	for (size_t x = 0; x < this->width(); x++ ) {
		for (size_t y = 0; y < this->height(); y++ ) {
			(*this)(x,y)->red   = (*this)(x,y)->red   + r > 255 ? 255 : ((*this)(x,y)->red   + r < 0 ? 0 : (*this)(x,y)->red   + r);
			(*this)(x,y)->green = (*this)(x,y)->green + g > 255 ? 255 : ((*this)(x,y)->green + g < 0 ? 0 : (*this)(x,y)->green + g);
			(*this)(x,y)->blue  = (*this)(x,y)->blue  + b > 255 ? 255 : ((*this)(x,y)->blue  + b < 0 ? 0 : (*this)(x,y)->blue  + b);
		}
	}
}

// Makes each RGB component of each pixel iequal to 255 minus its original value
void Image::invertcolors() {
	for (size_t x = 0; x < this->width(); x++ ) {
		for (size_t y = 0; y < this->height(); y++ ) {
			(*this)(x,y)->red   = 255 - (*this)(x,y)->red;
			(*this)(x,y)->green = 255 - (*this)(x,y)->green;
			(*this)(x,y)->blue  = 255 - (*this)(x,y)->blue;
		}
	}
}

// MARK: - Private Methods
// Swaps all the channels of two pixels
void Image::swapPixels(RGBAPixel *pixelA, RGBAPixel *pixelB) {
	uint8_t temp;
	temp = pixelA->red;
	pixelA->red   = pixelB->red;
	pixelB->red   = temp;

	temp = pixelA->green;
	pixelA->green = pixelB->green;
	pixelB->green = temp;

	temp = pixelA->blue;
	pixelA->blue  = pixelB->blue;
	pixelB->blue  = temp;
}