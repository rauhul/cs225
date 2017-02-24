/*
 * Simple C++ program that manipulates images
 */

//c++ include
#include <iostream>

//local include
#include "png.h"
#include "rgbapixel.h"

using namespace std;

/*
 * Takes an input image and rotates the image 180 degrees, O(n) run time
 */
PNG rotateImage(PNG input) {
	PNG output(input.width(), input.height());
	for(size_t x = 0; x < input.width(); x++) {
		for(size_t y = 0; y < input.height(); y++) {
			output(input.width()-1-x,input.height()-1-y)->red   = input(x,y)->red;
			output(input.width()-1-x,input.height()-1-y)->green = input(x,y)->green;
			output(input.width()-1-x,input.height()-1-y)->blue  = input(x,y)->blue;
			output(input.width()-1-x,input.height()-1-y)->alpha = input(x,y)->alpha;
		}
	}
	return output;
}

int main() {
	PNG inputImage("in.png");
	PNG outputImage = rotateImage(inputImage);
	outputImage.writeToFile("out.png");
	return 0;
}