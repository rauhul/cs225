/**
 * @file quadtree.cpp
 * Quadtree class implementation.
 * @date Spring 2008
 */
#include "quadtree.h"
#include <queue>
#include <cmath>
using namespace std;

Quadtree::Quadtree() {
	root = NULL;
	resolution = 0;
}

Quadtree::Quadtree(PNG const& source, int resolution) {
	root = NULL;
	buildTree(source, resolution);
}

void Quadtree::buildTree(PNG const& source, int resolution) {
	clear();
	this->resolution = resolution;
	if (resolution == 0) {
		root = NULL;
	} else {
		root = new QuadtreeNode();
		root->element = buildNode(source, root, 0, 0, resolution);
	}
}

RGBAPixel Quadtree::buildNode(PNG const& source, QuadtreeNode* node, int offsetX, int offsetY, int size) {
	if (size == 1) {
		const RGBAPixel* p = source(offsetX, offsetY);
		node->element = RGBAPixel(p->red, p->green, p->blue, p->alpha);;
		return node->element;
	} else {
		node->nwChild = new QuadtreeNode();
		node->neChild = new QuadtreeNode();
		node->swChild = new QuadtreeNode();
		node->seChild = new QuadtreeNode();

		RGBAPixel nw  = buildNode(source, node->nwChild, offsetX,          offsetY,          size/2);
		RGBAPixel ne  = buildNode(source, node->neChild, offsetX + size/2, offsetY,          size/2);
		RGBAPixel sw  = buildNode(source, node->swChild, offsetX,          offsetY + size/2, size/2);
		RGBAPixel se  = buildNode(source, node->seChild, offsetX + size/2, offsetY + size/2, size/2);
		RGBAPixel avg = RGBAPixel((nw.red   + ne.red   + sw.red   + se.red  )/4,
								  (nw.green + ne.green + sw.green + se.green)/4,
								  (nw.blue  + ne.blue  + sw.blue  + se.blue )/4,
								  (nw.alpha + ne.alpha + sw.alpha + se.alpha)/4);
		node->element = avg;
		return avg;
	}
}

Quadtree::Quadtree(Quadtree const &other) {
	copy(other);
}

Quadtree::~Quadtree() {
	clear();
}

Quadtree const& Quadtree::operator=(Quadtree const &other) {
	if (this != &other) {
		clear();
		copy(other);
	}
	return *this;
}

void Quadtree::clear() {
	clearQuadtreeNode(root);
	resolution = 0;
}

void Quadtree::clearQuadtreeNode(QuadtreeNode*& node) {
	if (node != NULL) {
		clearQuadtreeNode(node->nwChild);
		clearQuadtreeNode(node->neChild);
		clearQuadtreeNode(node->swChild);
		clearQuadtreeNode(node->seChild);
		delete node;
		node = NULL;
	}
}

int Quadtree::leavesOnQuadtreeNode(QuadtreeNode* node) const {
	if (node != NULL) {
		return leavesOnQuadtreeNode(node->nwChild) +
			   leavesOnQuadtreeNode(node->neChild) +
			   leavesOnQuadtreeNode(node->swChild) +
			   leavesOnQuadtreeNode(node->seChild) + 1;
	} else {
		return 0;
	}
}


void Quadtree::copy(Quadtree const &other) {
	if (other.root == NULL) {
		root = NULL;
		resolution = 0;
	} else {
		resolution = other.resolution;
		root = new QuadtreeNode();
		copySubNodesToSuperNode(root, other.root);
	}
}

void Quadtree::copySubNodesToSuperNode(QuadtreeNode* superNode, QuadtreeNode* otherNode) {
	superNode->element = otherNode->element;
	if (otherNode->nwChild != NULL) {
		superNode->nwChild = new QuadtreeNode();
		superNode->neChild = new QuadtreeNode();
		superNode->swChild = new QuadtreeNode();
		superNode->seChild = new QuadtreeNode();
		copySubNodesToSuperNode(superNode->nwChild, otherNode->nwChild);
		copySubNodesToSuperNode(superNode->neChild, otherNode->neChild);
		copySubNodesToSuperNode(superNode->swChild, otherNode->swChild);
		copySubNodesToSuperNode(superNode->seChild, otherNode->seChild);
	}
}

RGBAPixel Quadtree::getPixel(int x, int y) const {
	if (root == NULL || x < 0 || y < 0 || x >= resolution || y >= resolution) {
		return RGBAPixel();
	} else {
		return getPixelFromNode(root, 0, 0, resolution, x, y);
	}
}

RGBAPixel Quadtree::getPixelFromNode(QuadtreeNode* node, int offsetX, int offsetY, int size, int x, int y) const {
	if (node == NULL) {
		return RGBAPixel();
	} else {
		if (node->nwChild != NULL) {
			int xRegion = floor((x - offsetX)/(size/2));
			int yRegion = floor((y - offsetY)/(size/2));
			if (xRegion == 0 && yRegion == 0) {
				return getPixelFromNode(node->nwChild, offsetX,          offsetY,          size/2, x, y);
			} else if (xRegion == 1 && yRegion == 0) {
				return getPixelFromNode(node->neChild, offsetX + size/2, offsetY,          size/2, x, y);
			} else if (xRegion == 0 && yRegion == 1) {
				return getPixelFromNode(node->swChild, offsetX,          offsetY + size/2, size/2, x, y);
			} else if (xRegion == 1 && yRegion == 1) {
				return getPixelFromNode(node->seChild, offsetX + size/2, offsetY + size/2, size/2, x, y);
			}
			return RGBAPixel();
		} else {
			return node->element;
		}
	}
}

PNG Quadtree::decompress() const {
	if (root != NULL) {
		PNG img(resolution, resolution);
		decompressNode(img, root, 0, 0, resolution);
		return img;
	} else {
		return PNG();
	}
}

void Quadtree::decompressNode(PNG& img, QuadtreeNode* node, int offsetX, int offsetY, int size) const {
	if (node != NULL) {
		if (node->nwChild == NULL) {
			for (int x = 0; x < size; x++) {
				for (int y = 0; y < size; y++) {
					*img(x + offsetX, y + offsetY) = node->element;
				}
			}
		} else {
			decompressNode(img, node->nwChild, offsetX,          offsetY,          size/2);
			decompressNode(img, node->neChild, offsetX + size/2, offsetY,          size/2);
			decompressNode(img, node->swChild, offsetX,          offsetY + size/2, size/2);
			decompressNode(img, node->seChild, offsetX + size/2, offsetY + size/2, size/2);
		}
	}
}

void Quadtree::clockwiseRotate() {
	clockwiseRotateNode(root);
}

void Quadtree::clockwiseRotateNode(QuadtreeNode* node) {
	if (node != NULL && node->nwChild != NULL) {
		QuadtreeNode* temp = node->nwChild;
		node->nwChild = node->swChild;
		node->swChild = node->seChild;
		node->seChild = node->neChild;
		node->neChild = temp;
		clockwiseRotateNode(node->nwChild);
		clockwiseRotateNode(node->neChild);
		clockwiseRotateNode(node->swChild);
		clockwiseRotateNode(node->seChild);
	}
}


void Quadtree::prune(int tolerance) {
	prune(root, tolerance);
}

void Quadtree::prune(QuadtreeNode* node, int tolerance) {
	if (node != NULL) {
		if (shouldPruneNode(node, node, tolerance)) {
			clearQuadtreeNode(node->nwChild);
			clearQuadtreeNode(node->neChild);
			clearQuadtreeNode(node->swChild);
			clearQuadtreeNode(node->seChild);
		} else {
			prune(node->nwChild, tolerance);
			prune(node->neChild, tolerance);
			prune(node->swChild, tolerance);
			prune(node->seChild, tolerance);
		}
	}
}

bool Quadtree::shouldPruneNode(QuadtreeNode* superNode, QuadtreeNode* leafNode, int tolerance) const {
	if (leafNode->nwChild != NULL) {
		bool shouldContinue = true;
		if (shouldContinue)
			shouldContinue &= shouldPruneNode(superNode, leafNode->nwChild, tolerance);
		if (shouldContinue)
			shouldContinue &= shouldPruneNode(superNode, leafNode->neChild, tolerance);
		if (shouldContinue)
			shouldContinue &= shouldPruneNode(superNode, leafNode->swChild, tolerance);
		if (shouldContinue)
			shouldContinue &= shouldPruneNode(superNode, leafNode->seChild, tolerance);
		return shouldContinue;
	} else if (superNode != leafNode) {
		return pow(superNode->element.red   - leafNode->element.red,   2) +
			   pow(superNode->element.green - leafNode->element.green, 2) +
			   pow(superNode->element.blue  - leafNode->element.blue,  2) <= tolerance;
	} else {
		return false;
	}
}

int Quadtree::pruneSize(int tolerance) const {
	return root == NULL ? 0 : pruneSize(root, tolerance);
}

int Quadtree::pruneSize(QuadtreeNode* node, int tolerance) const {
	if (node != NULL) {
		if (node->nwChild == NULL || shouldPruneNode(node, node, tolerance)) {
			return 1;
		} else {
			return pruneSize(node->nwChild, tolerance) +
			       pruneSize(node->neChild, tolerance) +
			       pruneSize(node->swChild, tolerance) +
			       pruneSize(node->seChild, tolerance);
		}
	} else {
		return 0;
	}
}


int Quadtree::idealPrune(int numLeaves) const {
	int lowerBound = 0;
	int upperBound = 100000;
	int guess;
	while (true) {
		guess = (lowerBound+upperBound)/2;
		if (pruneSize(root, guess) <= numLeaves) {
			upperBound = guess;
		} else {
			lowerBound = guess;
		}
		if (upperBound - lowerBound <= 1) {
			break;
		}
	}
	return upperBound;
}

#include <string>
void Quadtree::exportAsLayers() {
	Quadtree newTree;
	newTree = *this;
	int count = 0;
	int layer = resolution;
	while (layer != 0) {
		PNG imgOut(resolution, resolution);
		imgOut = newTree.decompress();
		imgOut.writeToFile("outLayer" + std::to_string(count) + ".png");
		deleteLowestChildren(newTree.root);
		layer = layer/2;
		count++;
	}
}

void Quadtree::deleteLowestChildren(QuadtreeNode*& node) {
	if (node != NULL) {
		if (node->nwChild == NULL) {
			clearQuadtreeNode(node);
		} else {
			deleteLowestChildren(node->nwChild);
			deleteLowestChildren(node->neChild);
			deleteLowestChildren(node->swChild);
			deleteLowestChildren(node->seChild);
		}
	}
}