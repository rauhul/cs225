TIOBJECTS = testimage.o image.o png.o rgbapixel.o

TSOBJECTS = testscene.o scene.o image.o png.o rgbapixel.o

CLANG     = clang++
CXXFLAGS  = -std=c++1y -stdlib=libc++ -c -g -O0 -Wall -Wextra -pedantic
LDFLAGS   = -std=c++1y -stdlib=libc++ -lpng -lc++abi

OSXLIBPNG = -L /usr/local/lib -I /usr/local/include


all: testimage testscene


testimage: $(TIOBJECTS)
	$(CLANG) $(TIOBJECTS) $(LDFLAGS) $(OSXLIBPNG) -o testimage

testimage.o: testimage.cpp image.h png.h rgbapixel.h
	$(CLANG) $(CXXFLAGS) $(OSXLIBPNG) testimage.cpp


testscene: $(TSOBJECTS)
	$(CLANG) $(TSOBJECTS) $(LDFLAGS) $(OSXLIBPNG) -o testscene

testscene.o: testscene.cpp image.h png.h rgbapixel.h
	$(CLANG) $(CXXFLAGS) $(OSXLIBPNG) testscene.cpp


scene.o: scene.cpp png.h image.h rgbapixel.h
	$(CLANG) $(CXXFLAGS) $(OSXLIBPNG) scene.cpp

image.o: image.cpp png.h rgbapixel.h
	$(CLANG) $(CXXFLAGS) $(OSXLIBPNG) image.cpp

png.o: png.cpp rgbapixel.h
	$(CLANG) $(CXXFLAGS) $(OSXLIBPNG) png.cpp

rgbapixel.o: rgbapixel.cpp
	$(CLANG) $(CXXFLAGS) $(OSXLIBPNG) rgbapixel.cpp

clean:
	-rm -f *.o $(EXENAME)