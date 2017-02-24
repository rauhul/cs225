EXENAME = mp1
OBJECTS = main.o png.o rgbapixel.o

CLANG = clang++
CXXFLAGS = -std=c++1y -stdlib=libc++ -c -g -O0 -Wall -Wextra -pedantic
LDFLAGS = -std=c++1y -stdlib=libc++ -lpng -lc++abi

OSXLIBPNG = -L /usr/local/lib -I /usr/local/include

all : $(EXENAME)

$(EXENAME) : $(OBJECTS)
	$(CLANG) $(OBJECTS) $(LDFLAGS) $(OSXLIBPNG) -o $(EXENAME)

main.o : main.cpp png.h rgbapixel.h
	$(CLANG) $(CXXFLAGS) $(OSXLIBPNG) main.cpp

png.o : png.cpp png.h rgbapixel.h
	$(CLANG) $(CXXFLAGS) $(OSXLIBPNG) png.cpp

rgbapixel.o : rgbapixel.cpp rgbapixel.h
	$(CLANG) $(CXXFLAGS) $(OSXLIBPNG) rgbapixel.cpp

clean :
	-rm -f *.o $(EXENAME)
