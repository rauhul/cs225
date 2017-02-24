/**
 * @file filler.cpp
 * Implementation of functions in the filler namespace. Heavily based on
 * old MP4 by CS225 Staff, Fall 2010.
 *
 * @author Chase Geigle
 * @date Fall 2012
 */
#include "filler.h"

struct point {
    int x, y;
    point(int _x, int _y): x(_x), y(_y) { }
    void print() {
        cout << "(" << x << "," << y << ")" << endl;
    }
};

animation filler::dfs::fillSolid(PNG& img, int x, int y, RGBAPixel fillColor,
                                 int tolerance, int frameFreq) {
    solidColorPicker picker(fillColor);
    return filler::fill<Stack>(img, x, y, picker, tolerance, frameFreq);
}

animation filler::dfs::fillGrid(PNG& img, int x, int y, RGBAPixel gridColor,
                                int gridSpacing, int tolerance, int frameFreq){
    gridColorPicker picker(gridColor, gridSpacing);
    return filler::fill<Stack>(img, x, y, picker, tolerance, frameFreq);
}

animation filler::dfs::fillGradient(PNG& img, int x, int y,
                                    RGBAPixel fadeColor1, RGBAPixel fadeColor2,
                                    int radius, int tolerance, int frameFreq) {
    gradientColorPicker picker(fadeColor1, fadeColor2, radius, x, y);
    return filler::fill<Stack>(img, x, y, picker, tolerance, frameFreq);
}

animation filler::dfs::fill(PNG& img, int x, int y, colorPicker& fillColor,
                            int tolerance, int frameFreq) {
    return filler::fill<Stack>(img, x, y, fillColor, tolerance, frameFreq);
}

animation filler::bfs::fillSolid(PNG& img, int x, int y, RGBAPixel fillColor,
                                 int tolerance, int frameFreq) {
    solidColorPicker picker(fillColor);
    return filler::fill<Queue>(img, x, y, picker, tolerance, frameFreq);
}

animation filler::bfs::fillGrid(PNG& img, int x, int y, RGBAPixel gridColor,
                                int gridSpacing, int tolerance, int frameFreq) {
    gridColorPicker picker(gridColor, gridSpacing);
    return filler::fill<Queue>(img, x, y, picker, tolerance, frameFreq);
}

animation filler::bfs::fillGradient(PNG& img, int x, int y,
                                    RGBAPixel fadeColor1, RGBAPixel fadeColor2,
                                    int radius, int tolerance, int frameFreq) {
    gradientColorPicker picker(fadeColor1, fadeColor2, radius, x, y);
    return filler::fill<Queue>(img, x, y, picker, tolerance, frameFreq);
}

animation filler::bfs::fill(PNG& img, int x, int y, colorPicker& fillColor,
                            int tolerance, int frameFreq) {
    return filler::fill<Queue>(img, x, y, fillColor, tolerance, frameFreq);
}

template <template <class T> class OrderingStructure>
animation filler::fill(PNG& img, int x, int y, colorPicker& fillColor,
                       int tolerance, int frameFreq) {
    animation anim;
    const RGBAPixel origin = *img(x,y);
    int counter = frameFreq;

    int height = img.height();
    int width  = img.width();

    OrderingStructure<point> list;
    list.add(point(x,y));

    bool ** arr = new bool*[width]();
    for (int i = 0; i<width; i++) {
        arr[i] = new bool[height]();
        for (int j = 0; j<height; j++) {
            arr[i][j] = false;
        }
    }

    auto inBounds    = [&img, &width, &height](point p) -> bool { 
            return p.x >= 0 && p.x<width  && 
                   p.y >= 0 && p.y<height;
        };

    auto inTolerance = [&img, &origin, &tolerance](point p) -> bool { 
            const RGBAPixel compare = *img(p.x,p.y);
            return pow(compare.red   - origin.red,   2) + 
                   pow(compare.green - origin.green, 2) + 
                   pow(compare.blue  - origin.blue , 2) <= tolerance;
        };

    auto unProcessed = [&arr](point p) -> bool {
            return !arr[p.x][p.y];
        };

    auto process     = [&img, &arr, &fillColor](point p) {
            *img(p.x,p.y) = fillColor(p.x,p.y);
            arr[p.x][p.y] = true;
        };

    auto addNeighboors = [&list, &inBounds](point p) {
            list.add(point(p.x + 1, p.y    )); //RIGHT (+x)
            list.add(point(p.x    , p.y + 1)); //DOWN  (+y)
            list.add(point(p.x - 1, p.y    )); //LEFT  (-x)
            list.add(point(p.x    , p.y - 1)); //UP    (-y)
        };

    auto renderFramesIfNeeded = [&anim, &frameFreq, &counter, &img]() {
            counter--;
            if (counter == 0) {
                counter = frameFreq;
                anim.addFrame(img);
            }
        };

    auto destruct = [&arr]() {
            if (arr != NULL) {
                for (int i = 0; i<width; i++) {
                    if (arr[i] != NULL) {
                        delete [] arr[i];
                    }
                }
                delete [] arr;
            }
        };

    while(!list.isEmpty()) {
        point p = list.peek();
        list.remove();
        if (inBounds(p) && inTolerance(p) && unProcessed(p)) {
            process(p);
            addNeighboors(p);
            renderFramesIfNeeded();
        }
    }
    destruct();

    return anim;
}