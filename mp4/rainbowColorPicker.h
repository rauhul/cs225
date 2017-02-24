#ifndef RAINBOWCOLORPICKER_H
#define RAINBOWCOLORPICKER_H

#include <stdint.h>
#include <map>
#include <utility>
#include "colorPicker.h"

/**
 * rainbowColorPicker: a functor that determines the color that should be used
 * given an x and a y coordinate using a rainbow pattern.
 *
 * @author CS 225 Staff
 * @date Fall 2010
 */
class rainbowColorPicker : public colorPicker
{
  public:
    /**
     * Constructs a new rainbowColorPicker.
     *
     * @param freq_ Frequency at which to cycle colors
     */
    rainbowColorPicker(long double freq_);

    /**
     * Picks the color for pixel (x, y).
     *
     * @param x The x coordinate to pick a color for.
     * @param y The y coordinat to pick a color for.
     * @return The color chosen for (x, y).
     */
    virtual RGBAPixel operator()(int x, int y);

  private:
    RGBAPixel nextColor();

    typedef std::map<std::pair<int, int>, RGBAPixel> PNGmap;
    size_t iter;
    PNGmap prev;
    long double freq;
};

#endif
