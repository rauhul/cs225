/**
 * @file quadtree.h
 * Quadtree class definition.
 * @date Spring 2008
 */

#ifndef QUADTREE_H
#define QUADTREE_H

#include "png.h"

/**
 * A tree structure that is used to compress PNG images.
 */
class Quadtree {
  public:
    Quadtree();
    Quadtree(PNG const &source, int resolution);
    Quadtree(Quadtree const &other);
    ~Quadtree();
    Quadtree const& operator=(Quadtree const &other);
    void buildTree(PNG const& source, int resolution);
    PNG decompress() const;
    RGBAPixel getPixel(int x, int y) const;
    void clockwiseRotate();
    void prune(int tolerance);
    int pruneSize(int tolerance) const;
    int idealPrune(int numLeaves) const;


    //Testing Functions
    void exportAsLayers();

  private:
    /**
     * A simple class representing a single node of a Quadtree.
     * You may want to add to this class; in particular, it could
     * probably use a constructor or two...
     */
    class QuadtreeNode {
      public:
        QuadtreeNode(): nwChild(NULL), neChild(NULL), swChild(NULL), seChild(NULL), element(RGBAPixel()) { };
        QuadtreeNode* nwChild; /**< pointer to northwest child */
        QuadtreeNode* neChild; /**< pointer to northeast child */
        QuadtreeNode* swChild; /**< pointer to southwest child */
        QuadtreeNode* seChild; /**< pointers to southeast child */
        RGBAPixel element;     /**< the pixel stored as this node's "data" */
    };

    void clear();
    void clearQuadtreeNode(QuadtreeNode*& node);
    void copy(Quadtree const &other);
    void copySubNodesToSuperNode(QuadtreeNode* superNode, QuadtreeNode* otherNode);
    RGBAPixel buildNode(PNG const& source, QuadtreeNode* node, int offsetX, int offsetY, int size);
    void decompressNode(PNG& img, QuadtreeNode* node, int offsetX, int offsetY, int size) const;
    void clockwiseRotateNode(QuadtreeNode* node);
    void prune(QuadtreeNode* node, int tolerance);
    bool shouldPruneNode(QuadtreeNode* superNode, QuadtreeNode* leafNode, int tolerance) const;
    RGBAPixel getPixelFromNode(QuadtreeNode* node, int offsetX, int offsetY, int size, int x, int y) const;
    int pruneSize(QuadtreeNode* node, int tolerance) const;
    int leavesOnQuadtreeNode(QuadtreeNode* node) const;

    //Testing Functions
    void deleteLowestChildren(QuadtreeNode*& node);

    QuadtreeNode* root; /**< pointer to root of quadtree */
    int resolution;

/**** Functions for testing/grading                      ****/
/**** Do not remove this line or copy its contents here! ****/
#include "quadtree_given.h"
};

#endif
