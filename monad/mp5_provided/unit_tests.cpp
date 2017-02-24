/**
 * MP5 Provided Test Cases.
 * Ported by Jack Toole
 */

#include "png.h"
#include "proxy.h"
#include "quadtree.h"

using namespace std;
using namespace util;

#if MP_PART(1)

// test constructor, decompress
UNIT_TEST(test_outHalf, 5, 2, 1000)
{
	PNG imgIn("in.png");

	Quadtree halfTree(imgIn, 128);
	PNG imgOut = halfTree.decompress();

	PNG imgSoln("soln_outHalf.png");
	ASSERT(imgOut == imgSoln);
	PASS;
}

#endif

#if MP_PART(2)

UNIT_TEST(test_pruneSize, 0, 1, 1000)
{
	PNG imgIn("in.png");
	Quadtree fullTree;
	fullTree.buildTree(imgIn, 256);

	ASSERT_EQUALS(65494, fullTree.pruneSize(0));
	ASSERT_EQUALS(39256, fullTree.pruneSize(100));
	ASSERT_EQUALS(18283, fullTree.pruneSize(1000));
	ASSERT_EQUALS(1,     fullTree.pruneSize(100000));

	PASS;
}

UNIT_TEST(test_idealPrune, 0, 2, 10000)
{
	PNG imgIn("in.png");
	Quadtree fullTree;
	fullTree.buildTree(imgIn, 256);

	ASSERT_EQUALS(28594, fullTree.idealPrune(1000));
	ASSERT_EQUALS(3521,  fullTree.idealPrune(10000));

	PASS;
}


UNIT_TEST(test_clockwiseRotate, 0, 2, 1000)
{
	PNG imgIn("in.png");
	Quadtree fullTree;
	fullTree.buildTree(imgIn, 256);

	fullTree.clockwiseRotate();
	PNG imgOut = fullTree.decompress();
	
	PNG imgSoln("soln_outRotated.png");
	ASSERT(imgOut == imgSoln);
	PASS;
}

UNIT_TEST(test_prune, 0, 1, 1000)
{
	PNG imgIn("in.png");
	Quadtree fullTree;
	fullTree.buildTree(imgIn, 256);

	fullTree.prune(1000);
	PNG imgOut = fullTree.decompress();
	
	PNG imgSoln("soln_outPruned.png");
	ASSERT(imgOut == imgSoln);
	PASS;
}

// test several functions in succession
UNIT_TEST(test_outEtc, 0, 1, 1000)
{
	PNG imgIn("in.png");
	Quadtree fullTree;
	fullTree.buildTree(imgIn, 256);

	Quadtree fullTree3(fullTree);
	fullTree3.clockwiseRotate();
	fullTree3.prune(10000);
	fullTree3.clockwiseRotate();
	fullTree3.clockwiseRotate();
	fullTree3.clockwiseRotate();
	PNG imgOut = fullTree3.decompress();
	
	PNG imgSoln("soln_outEtc.png");
	ASSERT(imgOut == imgSoln);
	PASS;
}

// ensure that printTree still works
UNIT_TEST(test_printTinyTree, 0, 1, 1000)
{
	PNG imgIn("in.png");
	Quadtree tinyTree(imgIn, 32);
	cout << "Printing tinyTree:\n";
	tinyTree.prune(100);
	tinyTree.printTree();
	ASSERT_OUTPUT(contains, readFile("soln_tinyTree.out"));
	PASS;
}

#endif
