#include "scene.h"
using std::uint8_t;
using std::cout;

//Initializes this Scene object to be able to hold "max" number of images with indices 0 through max-1
Scene::Scene(int max) {
	maxImages = max;
	_images   = new Image*[maxImages];
	for(int i = 0; i < maxImages; i++) {
		_images[i] = NULL;
	}
}

//The copy constructor makes this Scene an independent copy of the source
Scene::Scene(const Scene &source) {
	copy(source);
}

//Frees all space that was dynamically allocated by this Scene
Scene::~Scene() {
	clear();
}

//The assignment operator for the Scene class
Scene & Scene::operator=(const Scene &source) {
	if (this != &source) {
		clear();
		copy(source);
	}
	return *this;
}

void Scene::copy(const Scene &source) {
	maxImages = source.maxImages;
	_images = new Image*[maxImages];
	for(int i = 0; i < maxImages; i++) {
		_images[i] = NULL;
		if (source._images[i] != NULL) {
			 *_images[i] = *source._images[i];
		}
	}
}

void Scene::clear() {
	if (_images != NULL) {
		for(int i = 0; i < maxImages; i++) {
			if (_images[i] != NULL) {
				delete _images[i];
			}
		}
		delete [] _images;
	}
}

//Modifies the size of the array of Image pointers without changing their indices
void Scene::changemaxlayers(int newmax) {
	for(int i = newmax; i < maxImages; i++) {
		if (_images[i] != NULL) {
			std::cout << "invalid newmax" << endl;
			return;
		}
	}

	Image *(* _newImages) = new Image*[newmax];
	for(int i = 0; i < newmax; i++) {
		if (i < maxImages) {
			_newImages[i] = _images[i];
		}
	}
	clear();
	_images = _newImages;
	maxImages = newmax;
}

//This function will add a picture to the scene, by placing it in the array cell corresponding to the given index, and storing its x coordinate and y coordinate
void Scene::addpicture(const char *FileName, int index, int x, int y) {
	if (indexInBounds(index)) {
		deletepicture(index);
		_images[index]    = new Image(FileName);
		_images[index]->x = x;
		_images[index]->y = y;
	} else {
        cout << "index out of bounds" << endl;
    }
}

//Moves an Image from one layer to another
void Scene::changelayer(int index, int newindex) {
	if (index == newindex) {
		return;
	} else if (indexInBounds(index) && indexInBounds(newindex)) {
		deletepicture(newindex);
		_images[newindex] = _images[index];
		deletepicture(index);
	} else {
		cout << "invalid index" << endl;
	}
}

//Changes the x and y coordinates of the Image in the specified layer
void Scene::translate(int index, int xcoord, int ycoord) {
	if (indexInBounds(index) && _images[index] != NULL) {
		_images[index]->x = xcoord;
		_images[index]->y = ycoord;
	} else {
		cout << "invalid index" << endl;
	}
}
	
//Deletes the Image at the given index
void Scene::deletepicture(int index) {
	if (indexInBounds(index)) {
		if (_images[index] != NULL) {
			delete _images[index];
			_images[index] = NULL;
		}
	} else {
		cout << "invalid index" << endl;
	}
}

//This function will return a pointer to the Image at the specified index, not a copy of it
Image* Scene::getpicture(int index) const {
	if (indexInBounds(index)) {
		return _images[index];
	} else {
		cout << "invalid index" << endl;
		return NULL;
	}
}

//Draws the whole scene on one Image and returns that Image by value
Image Scene::drawscene() const {
	size_t width  = 0;
	size_t height = 0;
	for(int i = 0; i < maxImages; i++) {
		if (_images[i] != NULL) {
			size_t maxWidth  = _images[i]->width()  + _images[i]->x;
			size_t maxHeight = _images[i]->height() + _images[i]->y;

			if (maxWidth  > width ) { width  = maxWidth;  }
			if (maxHeight > height) { height = maxHeight; }
		}
	}

	Image img = Image(width, height);

	for(int i = 0; i < maxImages; i++) {
		if (_images[i] != NULL) {
			size_t xOff = _images[i]->x;
			size_t yOff = _images[i]->y;
			for(size_t x = 0; x < _images[i]->width(); x++) {
				for(size_t y = 0; y < _images[i]->height(); y++) {
					img(xOff+x, yOff+y)->red   = (*_images[i])(x,y)->red;
					img(xOff+x, yOff+y)->green = (*_images[i])(x,y)->green;
					img(xOff+x, yOff+y)->blue  = (*_images[i])(x,y)->blue;
				}
			}
		}
	}

	return img;
}

bool Scene::indexInBounds(int index) const {
	return index >= 0 && index < maxImages;
}

