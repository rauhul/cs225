/**
 * @file quadtree_given.h
 * Contains functions of the Quadtree class used for grading.
 */

public:
/**
 * Given: prints the leaves of the Quadtree using a preorder traversal.
 */
void printTree(std::ostream& out = std::cout) const;

/**
 * Given: compares the current Quadtree with the parameter Quadtree, and
 *  determines whether or not the two are the same.
 *
 * @param other Reference to a const Quadtree object, against which the
 *  current Quadtree will be compared
 * @return True if the Quadtrees are deemed "equal", and false otherwise
 * @note This method relies on the private helper method compareTrees()
 */
bool operator==(Quadtree const& other) const;

private:
/**
 * Given: prints the contents of the Quadtree using a preorder traversal.
 *
 * @param current Pointer to the root of the subQuadtree which we wish
 *   to print
 * @param level Current recursion depth; used for determining when to
 *  terminate recursion (see note below)
 */
void printTree(std::ostream& out, QuadtreeNode const* current, int level) const;

/**
 * Given: compares the subQuadtree rooted at firstPtr with the subQuadtree
 *  rooted at secondPtr, and determines whether the two are the same.
 *
 * @param firstPtr Pointer to the root of a subtree of the "first"
 *  Quadtree under consideration
 * @param secondPtr Pointer to the root of a subtree of the "second"
 *  Quadtree under consideration
 * @return True if the subQuadtrees are deemed "equal", and false
 *  otherwise
 */
bool compareTrees(QuadtreeNode const* firstPtr,
                  QuadtreeNode const* secondPtr) const;
