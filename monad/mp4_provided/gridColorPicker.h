#ifndef GRIDCOLORPICKER_H
#define GRIDCOLORPICKER_H

#include"colorPicker.h"

class gridColorPicker : public colorPicker{
  public:
    gridColorPicker(RGBAPixel fillColor, int gridSpacing);
    virtual RGBAPixel operator()(int x, int y);
  private:
    RGBAPixel color;
    int spacing;
};

#endif
