#include "png.h"
#include "image.h"
using std::uint8_t;

#ifndef SCENE_H
#define SCENE_H
class Scene: public PNG {
	public:
		// Initializes this Scene object to be able to hold "max" number of images with indices 0 through max-1
		Scene(int max);
		// The copy constructor makes this Scene an independent copy of the source
		Scene(const Scene &source);
		// Frees all space that was dynamically allocated by this Scene
		~Scene();
		// The assignment operator for the Scene class
		Scene & operator=(const Scene &source);
 		// Modifies the size of the array of Image pointers without changing their indices
		void changemaxlayers(int newmax);
 		// This function will add a picture to the scene, by placing it in the array cell corresponding to the given index, and storing its x coordinate and y coordinate
		void addpicture(const char *FileName, int index, int x, int y);
  		// Moves an Image from one layer to another
		void changelayer(int index, int newindex);
  		// Changes the x and y coordinates of the Image in the specified layer
		void translate(int index, int xcoord, int ycoord);
  		// Deletes the Image at the given index
		void deletepicture(int index);
  		// This function will return a pointer to the Image at the specified index, not a copy of it
		Image* getpicture(int index) const;
  		// Draws the whole scene on one Image and returns that Image by value
		Image drawscene() const;
	private:
		// Holds the images in the scene
		Image *(* _images);
		// Holds the maximum number of images in the scene
		int maxImages;
		// checks if an index is within the _images bounds
		bool indexInBounds(int index) const;
		// Copy operation
		void copy(const Scene &source);
		// Clear operation
		void clear();
};
#endif