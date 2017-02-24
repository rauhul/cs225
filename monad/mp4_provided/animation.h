#ifndef ANIMATION_H
#define ANIMATION_H

#include <string>
#include <vector>
#include "png.h"

using namespace std;

// NOTE This is the grading "animation"  as such it doesn't actually make animations, but rather just stores the frames so the grading is faster/easier.
class animation {
  public:
    void addFrame(PNG const & img);
    int size();
    const PNG & frame(int frameNum);

  private:
    std::vector<PNG> frames;
};

#endif
