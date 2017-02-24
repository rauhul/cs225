#include "png.h"
using std::uint8_t;

#ifndef IMAGE_H
#define IMAGE_H
class Image: public PNG {
	public:
		Image();

		Image(string const & file_name);

		Image(size_t width, size_t height);



		Image(Image const &source);

		~Image();

		/**
         * Assignment operator for setting two Images equal to one another.
         * @param other Image to copy into the current image.
         * @return The current image for assignment chaining.
         */
        Image const & operator=(Image const & other);



		//X location of image in Scene
		int x;
		//Y location of image in Scene 
		int y;

		
		//Flips the image about a vertical line through its center by swapping pixels
		void flipleft();
		//Adds to the red, green, and blue parts of each pixel in the image
		void adjustbrightness(int r, int g, int b);
 		//Makes each RGB component of each pixel equal to 255 minus its original value
		void invertcolors();
	private:
		//Swaps all the channels of two pixels
		void swapPixels(RGBAPixel * pixelA, RGBAPixel * pixelB);

		void copy(Image const &source);

		void clear();

};
#endif

