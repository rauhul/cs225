#include <iostream>
#include <sstream>
#include "png.h"
#include "stack.h"
#include "queue.h"
#include "fills.h"
#include "solnGradientColorPicker.h"
#include <vector>

#define TESTIMAGE1 "simple.png"
#define TESTIMAGE2 "simple2.png"

#define IMAGETOLERANCE 5

using std::cout;
using std::endl;
using std::cin;
using std::string;
using std::vector;

int abs(int a) {
  return a < 0? -1 * a : a;
}

bool PNGdiffWithTolerance(string filename, PNG img, int tolerance) {
  PNG soln_img;
  soln_img.ReadFromFile(filename.c_str());

  if(soln_img.TellWidth() != img.TellWidth() ||
     soln_img.TellHeight() != img.TellHeight()) {
      cout << "Image dimensions differ!" << endl;
      cout << "soln: " << soln_img.TellWidth() << "x" << soln_img.TellHeight() << endl;
      cout << "yours: " << img.TellWidth() << "x" << img.TellHeight() << endl;
      return false;
     }

  for(int x = 0; x < img.TellWidth(); x++)
    for(int y = 0; y < img.TellHeight(); y++) {
      if(abs(img(x,y)->red-soln_img(x,y)->red) > tolerance ||
         abs(img(x,y)->green-soln_img(x,y)->green) > tolerance ||
         abs(img(x,y)->blue-soln_img(x,y)->blue) > tolerance) {
        cout << "Images differ too much at point (" << x << ", " << y << ")." << endl;
        cout << "soln: R=" << (int)soln_img(x,y)->red << " G=" << (int)soln_img(x,y)->green << " B=" << (int)soln_img(x,y)->blue << endl;
        cout << "yours: R=" << (int)img(x,y)->red << " G=" << (int)img(x,y)->green << " B=" << (int)img(x,y)->blue << endl;
        return false;
      }
    }

  return true;
}


string itos(int x) {
  std::stringstream s;
  s << x;
  return s.str();
}

void testColorPicker(colorPicker * picker, int width, int height) {
  RGBAPixel px;

  for(int x = 1; x < width; x = x + x)
    for(int y = 1; y < height; y = y + y) {
      px = (*picker)(x, y);
      cout << "operator()(" << x << ", " << y << ") = {" << (int)px.red << ", ";
      cout << (int)px.green << ", " << (int)px.blue << "}" << endl;
    }
}

// Have to do this separatly to handle rounding errors
void testGradientColorPicker(colorPicker * picker, solnGradientColorPicker solPicker, int width, int height) {
  RGBAPixel px, solpx;

  for(int x = 1; x < width; x = x + x)
    for(int y = 1; y < height; y = y + y) {
      px = (*picker)(x, y);
      solpx = solPicker(x, y);
      if(abs((int)px.red-(int)solpx.red) <= IMAGETOLERANCE &&
         abs((int)px.blue-(int)solpx.blue) <= IMAGETOLERANCE &&
         abs((int)px.green-(int)solpx.green) <= IMAGETOLERANCE){
        cout << "operator()(" << x << ", " << y << ") is within a tolerance of {" << (int)solpx.red << ", ";
        cout << (int)solpx.green << ", " << (int)solpx.blue << "}" << endl;
      }else {
        cout << "operator()(" << x << ", " << y << ") is NOT within a tolerance of {" << (int)solpx.red << ", ";
        cout << (int)solpx.green << ", " << (int)solpx.blue << "}" << endl;
        cout << "Yours returned {" << (int)px.red << ", ";
        cout << (int)px.green << ", " << (int)px.blue << "}" << endl;
      }
    }
}

int main(int argc, char ** argv) {
  if(argv) {} // Just to get rid of a warning so people won't ask about it

  int testCase;
  cin >> testCase;

    RGBAPixel px, px2;
    px.red = 10;
    px.blue = 50;
    px.green = 250;
    px2.red = 110;
    px2.blue = 250;
    px2.green = 32;



    // MP4.1 tests begin here---------------------------------------------------

    // Stack tests
  if(testCase == 1) { 
      Stack<int> testStack;
      for(int i = 0; i < 10; i++) testStack.push(i);
      cout << testStack.peek() << ", " << testStack.peek() << endl;
  }else if(testCase == 2) {
      Stack<int> testStack;
      for(int i = 0; i < 10; i++) testStack.push(i);
      testStack.pop();
      cout << testStack.peek() << ", ";
      testStack.pop();
      cout << testStack.peek() << endl;
  }else if(testCase == 3) {
      Stack<int> testStack;
      for(int i = 0; i < 10; i++) testStack.push(i);
      for(int i = 0; i < 5; i++) testStack.pop();
      testStack.push(999);
      cout << testStack.pop() << ", " << testStack.pop() << endl;
  }else if(testCase == 4) {
      Stack<int> testStack;
      for(int i = 4; i < 16; i++) testStack.push(i);
      if(testStack.isEmpty()) cout << "Stack Empty!" << endl;
      else cout << "Stack NOT Empty!" << endl;
  }else if(testCase == 5) {
      Stack<int> testStack;
      if(testStack.isEmpty()) cout << "Stack Empty!" << endl;
      else cout << "Stack NOT Empty!" << endl;
  }else if(testCase == 6) {
      Stack<int> testStack;
      for(int i = 0; i < 16; i++) testStack.push(i);
      for(int i = 0; i < 16; i++) testStack.pop();
      if(testStack.isEmpty()) cout << "Stack Empty!" << endl;
      else cout << "Stack NOT Empty!" << endl;
  }else if(testCase == 7) {
      Stack<int> testStack;
      for(int i = 0; i < 16; i++) testStack.push(i);
      for(int i = 0; i < 16; i++) cout << testStack.pop() << " ";
      cout << endl;
  }


  // The same tests, but for the queue class
  else if(testCase == 8) { 
      Queue<int> testQueue;
      for(int i = 0; i < 10; i++) testQueue.enqueue(i);
      cout << testQueue.peek() << ", " << testQueue.peek() << endl;
  }else if(testCase == 9) {
      Queue<int> testQueue;
      for(int i = 0; i < 10; i++) testQueue.enqueue(i);
      testQueue.dequeue();
      cout << testQueue.peek() << ", ";
      testQueue.dequeue();
      cout << testQueue.peek() << endl;
  }else if(testCase == 10) {
      Queue<int> testQueue;
      for(int i = 0; i < 10; i++) testQueue.enqueue(i);
      for(int i = 0; i < 5; i++) testQueue.dequeue();
      testQueue.enqueue(999);
      cout << testQueue.dequeue() << ", " << testQueue.dequeue() << endl;
  }else if(testCase == 11) {
      Queue<int> testQueue;
      for(int i = 4; i < 16; i++) testQueue.enqueue(i);
      if(testQueue.isEmpty()) cout << "Queue Empty!" << endl;
      else cout << "Queue NOT Empty!" << endl;
  }else if(testCase == 12) {
      Queue<int> testQueue;
      if(testQueue.isEmpty()) cout << "Queue Empty!" << endl;
      else cout << "Queue NOT Empty!" << endl;
  }else if(testCase == 13) {
      Queue<int> testQueue;
      for(int i = 0; i < 16; i++) testQueue.enqueue(i);
      for(int i = 0; i < 16; i++) testQueue.dequeue();
      if(testQueue.isEmpty()) cout << "Queue Empty!" << endl;
      else cout << "Queue NOT Empty!" << endl;
  }else if(testCase == 14) {
      Queue<int> testQueue;
      for(int i = 0; i < 16; i++) testQueue.enqueue(i);
      for(int i = 0; i < 16; i++) cout << testQueue.dequeue() << " ";
      cout << endl;
  }




    //MP4.2 tests begin here---------------------------------------------------

  // Color Picker tests
  else if(testCase == 15) {
      solidColorPicker picker(px);
      testColorPicker(&picker, 17, 50);
  }else if(testCase == 16) {
      gradientColorPicker picker(px, px2, 30, 2, 40);
      solnGradientColorPicker solPicker(px, px2, 30, 2, 40);
      testGradientColorPicker(&picker, solPicker, 50, 50);
  }else if(testCase == 17) {
      gradientColorPicker picker(px2, px, 130, 50, 50);
      solnGradientColorPicker solPicker(px2, px, 130, 50, 50);
      testGradientColorPicker(&picker, solPicker, 100, 100);
  }

  // The tests that use image2
  else{
    PNG img;

    if(testCase >= 18 && testCase <= 23)
      img.ReadFromFile(TESTIMAGE1);
    else
      img.ReadFromFile(TESTIMAGE2);

    animation anim;
    string filename;

    //DFS Tests
    if(testCase == 18) {
      anim = DFSfillSolid(img, 6, 5, px, 0, 10);
      filename = "test18";
    }else if(testCase == 19) {
      anim = DFSfillSolid(img, 0, 5, px2, 0, 10);
      filename = "test19";
    }else if(testCase == 20) {
      anim = DFSfillGrid(img, 8, 6, px, 2, 0, 10);
      filename = "test20";
    }else if(testCase == 21) {
      anim = DFSfillGrid(img, 0, 6, px2, 2, 0, 15);
      filename = "test21";
    }else if(testCase == 22) {
      anim = DFSfillGradient(img, 8, 8, px, px2, 20, 0, 10);
      filename = "test22";
    }else if(testCase == 23) {
      anim = DFSfillGradient(img, 1, 1, px2, px, 5, 0, 11);
      filename = "test23";
    }

    else if(testCase == 24) {
      anim = BFSfillSolid(img, 6, 5, px, 0, 10);
      filename = "test24";
    }else if(testCase == 25) {
      anim = BFSfillSolid(img, 0, 5, px2, 0, 10);
      filename = "test25";
    }else if(testCase == 26) {
      anim = BFSfillGrid(img, 8, 6, px, 2, 0, 10);
      filename = "test26";
    }else if(testCase == 27) {
      anim = BFSfillGrid(img, 0, 6, px2, 2, 0, 15);
      filename = "test27";
    }else if(testCase == 28) {
      anim = BFSfillGradient(img, 8, 8, px, px2, 20, 0, 10);
      filename = "test28";
    }else if(testCase == 29) {
      anim = BFSfillGradient(img, 1, 1, px2, px, 5, 0, 11);
      filename = "test29";
    }

    if(anim.size() > 0) {
      for(int i = 0; i < anim.size(); i++) {
        if(argc > 1) // If you are generating the solns
          anim.frame(i).WriteToFile(("soln_" + filename + "_frame" + itos(i) + ".png").c_str());

        if(PNGdiffWithTolerance("soln_" + filename + "_frame" + itos(i) + ".png", anim.frame(i), IMAGETOLERANCE))
          cout << "Frame " << i << ": OKAY" << endl;
        else
          cout << "Frame " << i << ": NOT OKAY" << endl;

      }
    }else {
      cout << "No animation created!  Zero frames in returned object!" << endl;
    }
  }

  cout << "Testing complete." << endl;
  return 0;
}
