#include <cstdlib>
#include <iostream>
#include "png.h"

// #include <typeinfo> - for debuging

using namespace std;

void sketchify() {
    // Load in.png
    PNG* original = new PNG("in.png");

    // Create out.png
    PNG* output = new PNG(original->width(), original->height());

    // Load our favorite color to color the outline
    RGBAPixel* myPixel = new RGBAPixel(0, 96, 192);

    // Go over the whole image, and if a pixel differs from that to its upper
    // left, color it my favorite color in the output
    for (int y = 1; y < original->height(); y++) {
        for (int x = 1; x < original->width(); x++) {
            // Calculate the pixel difference
            RGBAPixel* prev = (*original)(x - 1, y - 1);
            RGBAPixel* curr = (*original)(x, y);

            // If the pixel is an edge pixel,
            // color the output pixel with my favorite color
            if (abs(curr->red - prev->red) + abs(curr->green - prev->green) + abs(curr->blue - prev->blue) > 100) {
                (*output)(x,y)->red   = myPixel->red;
                (*output)(x,y)->green = myPixel->green;
                (*output)(x,y)->blue  = myPixel->blue;
            }
        }
    }

    // Save the output file
    output->writeToFile("out.png");

    // Clean up memory
    delete myPixel;
    delete output;
    delete original;
}
