/**
 * @file maptiles.cpp
 * Code for the maptiles function.
 */

#include <iostream>
#include <map>
#include "maptiles.h"

using namespace std;


MosaicCanvas* mapTiles(SourceImage const& theSource,
                       vector<TileImage> const& theTiles) {
	//Set up KDTree of theTiles
	map<Point<3>, TileImage> m;
	vector<Point<3>> inputVector;

	for (TileImage image: theTiles) {
		RGBAPixel color = image.getAverageColor();
		Point<3> pixelAsPoint(color.red, color.green, color.blue);
		m[pixelAsPoint] = image;
		inputVector.push_back(pixelAsPoint);
	}

	KDTree<3> tree(inputVector);

	int rows = theSource.getRows();
	int cols = theSource.getColumns();
	MosaicCanvas* canvas = new MosaicCanvas(rows, cols);
	for (int x = 0; x < rows; x++) {
		for (int y = 0; y < cols; y++) {
			RGBAPixel color = theSource.getRegionColor(x,y);
			Point<3> target(color.red, color.green, color.blue);
			Point<3> result = tree.findNearestNeighbor(target);
			canvas->setTile(x,y,m[result]);
		}
	}
 	
    return canvas;
}

