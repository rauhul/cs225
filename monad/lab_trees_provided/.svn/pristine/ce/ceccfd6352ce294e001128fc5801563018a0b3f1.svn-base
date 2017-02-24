// Cs 225 Lab07 Unit Tests

#include <algorithm>
#include <vector>

#include "proxy.h"
#include "util.h"

#include "binarytree.h"

using namespace std;
using namespace util;


// sum

UNIT_TEST(test_isOrdered, 25, 25, 1000)
{	
	vector<int> ordering;
	ordering.resize(10);
	for (size_t i = 0; i < ordering.size(); i++)
		ordering[i] = i;
	srand( 1234 );
	random_shuffle(ordering.begin(), ordering.end());

	BinaryTree<int> tree;
	for (size_t i = 0; i < ordering.size(); i++)
		tree.insert(ordering[i]);
	
	tree.print();
	
	ASSERT(tree.isOrdered() == false);

	// second test
	ordering.resize(12);
	for (size_t i = 0; i < ordering.size(); i++)
		ordering[i] = i;
	srand( 123 );
	random_shuffle(ordering.begin(), ordering.end());

	BinaryTree<int> tree2;
	for (size_t i = 0; i < ordering.size(); i++)
		tree2.insert(ordering[i], true);
	tree2.print();
	
	ASSERT(tree2.isOrdered() == true);
}

UNIT_TEST(test_mirror, 25, 25, 1000){
	vector<int> nodes;
	nodes.push_back(58); //         58
	nodes.push_back(35); //
	nodes.push_back(72); //    35        72
	nodes.push_back(10); //
	nodes.push_back(40); //  10  40    61  80
	nodes.push_back(61); //
	nodes.push_back(80); // 0 19            93
	nodes.push_back(93); //
	nodes.push_back(0); //
	nodes.push_back(19); //

	BinaryTree<int> tree;
	for(size_t i = 0; i < nodes.size(); i++)
		tree.insert(nodes[i], true);
	tree.mirror();
	tree.print();
	
	const char * expectedTree =
        "            __ 58 _              \n"
        "         __/       \\__           \n"
        "       72              35        \n"
        "     /   \\           /   \\       \n"
        "   80      61      40      10    \n"
        "  /                       / \\    \n"
        " 93                      19  0   \n";
	ASSERT_OUTPUT(contains, expectedTree);
}

OUTPUT_CHECK(contains_or)
{
	vector<string> alternatives = util::tokenize(expected, '|');
	for (size_t i = 0; i < alternatives.size(); i++)
	{
		if (output.find(alternatives[i]) != string::npos && alternatives[i] != "")
			return true;
	}
	return false;
}

UNIT_TEST(test_printPaths, 25, 25, 1000){
	vector<int> nodes;
	nodes.push_back(52); //         52
	nodes.push_back(39); //
	nodes.push_back(71); //    39         71
	nodes.push_back(17); //
	nodes.push_back(47); //  17   47     69   80
	nodes.push_back(69); //
	nodes.push_back(80); // 0 24             90
	nodes.push_back(90); //
	nodes.push_back(0); //
	nodes.push_back(24); //

	BinaryTree<int> tree;
	for(size_t i = 0; i < nodes.size(); i++)
		tree.insert(nodes[i], true);
	tree.printPaths();
	ASSERT_OUTPUT(contains_or,
		"Path: 52 39 17 0\n"
		"Path: 52 39 17 24\n"
		"Path: 52 39 47\n"
		"Path: 52 71 69\n"
		"Path: 52 71 80 90\n|"

		"Path: 52 39 17 0 \n"
		"Path: 52 39 17 24 \n"
		"Path: 52 39 47 \n"
		"Path: 52 71 69 \n"
		"Path: 52 71 80 90 \n"
	);
}

UNIT_TEST(test_sumDistances, 25, 25, 1000){
	vector<int> nodes;
	nodes.push_back(50); //         50
	nodes.push_back(32); //
	nodes.push_back(70); //    32        70
	nodes.push_back(10); //
	nodes.push_back(45); //  10  45    69  88
	nodes.push_back(69);
	nodes.push_back(88);

	BinaryTree<int> tree;
	for(size_t i = 0; i < nodes.size(); i++)
		tree.insert(nodes[i], true);
	tree.print();
	ASSERT_EQUALS(10, tree.sumDistances());
}
