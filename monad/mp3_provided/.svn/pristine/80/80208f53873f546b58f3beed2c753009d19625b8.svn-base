#include "proxy.h"
#include <algorithm>
#include <map>

#include "list.h"
#include "png.h"
#include "blockpng.h"

using namespace std;

#define SAVE_MORE_POINTERS(list)                                        \
	for (List<int>::iterator it = list.begin(); it != list.end(); ++it)  \
		SAVE_POINTERS__map[&(*it)] = *it;

#define SAVE_POINTERS(list)                  \
	map<const int*,int> SAVE_POINTERS__map;  \
	SAVE_MORE_POINTERS(list);

#define CHECK_POINTERS(list)                                              \
	for (List<int>::iterator it = list.begin(); it != list.end(); ++it)    \
	{                                                                     \
		if (SAVE_POINTERS__map.find(&(*it)) == SAVE_POINTERS__map.end())  \
			FAIL("Function allocated new ListNodes");                     \
		if (SAVE_POINTERS__map[&(*it)] != *it)                            \
			FAIL("Modified ListNode data");                               \
	}

#define SAVE_MORE_POINTERS_RGBAPixel(list)                                    \
	for (List<RGBAPixel>::iterator it = list.begin(); it != list.end(); ++it)  \
		SAVE_POINTERS__map[&(*it)] = *it;

#define SAVE_POINTERS_RGBAPixel(list)                      \
	map<const RGBAPixel *, RGBAPixel> SAVE_POINTERS__map;  \
	SAVE_MORE_POINTERS_RGBAPixel(list);

#define CHECK_POINTERS_RGBAPixel(list)                                        \
	for (List<RGBAPixel>::iterator it = list.begin(); it != list.end(); ++it)  \
	{                                                                         \
		if (SAVE_POINTERS__map.find(&(*it)) == SAVE_POINTERS__map.end())      \
			FAIL("Function allocated new ListNodes");                         \
		if (SAVE_POINTERS__map[&(*it)] != *it)                                \
			FAIL("Modified ListNode data");                                   \
	}

List<RGBAPixel> imageToList(PNG const & img, bool reverse = false)
{
	List<RGBAPixel> list;
	for (int i = 0; i < img.width(); i++)
	{
		for (int j = 0; j < img.height(); j++)
		{
			if (reverse)
				list.insertFront(*img(i,j));
			else
				list.insertBack(*img(i,j));
		}
	}
	return list;
}

BlockPNG listToImage(List<RGBAPixel> const & list, int width, int height)
{
	BlockPNG ret;
	ret.resize(width, height);
	vector<RGBAPixel> v(list.begin(), list.end());
	int x = 0;
	int y = 0;
	for (unsigned int i = 0; i < v.size(); i++)
	{
		*ret(x,y) = v[i];
		y++;
		if (y == height)
		{
			y = 0;
			x++;
		}
	}
	return ret;
}

vector<int> buildVector(BlockPNG const & b, int d)
{
	vector<int> v;
	for (int i = 1; i <= (b.width() * b.height()) / (d * d); i++)
		v.push_back(i);

	return v;
}

#if MP_PART(1)

UNIT_TEST(testInsertFront, 1, 1, 1000)
{
	cout << "[main]: " << __func__ << "()" << endl;
	List<int> list;

	// test insertFront
	for (int i = 1; i <= 10; i++)
		list.insertFront(i);

	cout << "[testInserts]: " << list << endl;
	cout << "[testInserts]: size: " << list.size() << endl;

	ASSERT_OUTPUT(contains, "< 10 9 8 7 6 5 4 3 2 1 >");
	ASSERT_OUTPUT(contains, "size: 10");
}

UNIT_TEST(testInsertBack, 1, 1, 1000)
{
	cout << "[main]: " << __func__ << "()" << endl;
	List<int> list2;

	// test insertBack
	for (int i = 1; i <= 10; i++)
		list2.insertBack(i);

	cout << "[testInserts]: " << list2 << endl;
	cout << "[testInserts]: size: " << list2.size() << endl;

	ASSERT_OUTPUT(contains, "< 1 2 3 4 5 6 7 8 9 10 >");
	ASSERT_OUTPUT(contains, "size: 10");
}

UNIT_TEST(testReverse, 1, 1, 2000)
{
	cout << "[main]: " << __func__ << "()" << endl;
	PNG in("in_02.png");

	List<RGBAPixel> list = imageToList(in);
	SAVE_POINTERS_RGBAPixel(list);

	list.reverse();
	CHECK_POINTERS_RGBAPixel(list);

	PNG out = listToImage(list, in.width(), in.height());
	PNG soln("soln_reverse.png");
	ASSERT(out == soln);
}

UNIT_TEST(testReverseNth, 1, 1, 5000)
{
	cout << "[main]: " << __func__ << "()" << endl;
	
	// testReverseNth_02
	PNG in("in_03.png");

	List<RGBAPixel> list = imageToList(in);
	SAVE_POINTERS_RGBAPixel(list);

	list.reverseNth(in.height() * 20);
	CHECK_POINTERS_RGBAPixel(list);

	PNG out1 = listToImage(list, in.width(), in.height());
	PNG soln_reverseNth_01("soln_reverseNth_01.png");
	ASSERT(out1 == soln_reverseNth_01);

	// testReverseNth_02
	in.readFromFile("in_04.png");

	list = imageToList(in);
	SAVE_MORE_POINTERS_RGBAPixel(list);

	list.reverseNth(in.height() * 61);
	CHECK_POINTERS_RGBAPixel(list);

	PNG out2 = listToImage(list, in.width(), in.height());
	PNG soln_reverseNth_02("soln_reverseNth_02.png");
	ASSERT(out2 == soln_reverseNth_02);
}

UNIT_TEST(testWaterfall, 1, 1, 2000)
{
	cout << "[main]: " << __func__ << "()" << endl;
	
	// testWaterfall_01
	PNG in("in_05.png");

	List<RGBAPixel> list = imageToList(in);
	SAVE_POINTERS_RGBAPixel(list);

	list.waterfall();
	CHECK_POINTERS_RGBAPixel(list);

	PNG out1 = listToImage(list, in.width(), in.height());
	PNG soln_waterfall_01("soln_waterfall_01.png");
	ASSERT(out1 == soln_waterfall_01);

	// testWaterfall_02
	in.readFromFile("in_06.png");

	list = imageToList(in);
	SAVE_MORE_POINTERS_RGBAPixel(list);

	list.waterfall();
	CHECK_POINTERS_RGBAPixel(list);

	PNG out2 = listToImage(list, in.width(), in.height());
	PNG soln_waterfall_02("soln_waterfall_02.png");
	ASSERT(out2 == soln_waterfall_02);
}


#endif // MP_PART(1)

#if MP_PART(2)

UNIT_TEST(testSplit, 0, 1, 2000)
{
	cout << "[main]: " << __func__ << "()" << endl;

	PNG in("in_07.png");
	List<RGBAPixel> list1;
	for (int i = 0; i < in.width(); i++)
		for (int j = 0; j < in.height(); j++)
			list1.insertBack(*in(i,j));
	List<RGBAPixel> list2 = list1.split(400*240);
	List<RGBAPixel> list3 = list2.split(400*240);

	vector<RGBAPixel> im1vect(list1.begin(), list1.end());
	vector<RGBAPixel> im2vect(list2.begin(), list2.end());
	vector<RGBAPixel> im3vect(list3.begin(), list3.end());

	PNG out1(400,240);
	int x = 0;
	for (int i = 0; i < im1vect.size(); i++)
	{
		int y = i % 240;
		*out1(x,y) = im1vect[i];
		if (y == 239)
			x++;
	}
	out1.writeToFile("split_01.png");
	PNG soln_split_01("soln_split_01.png");
	ASSERT(out1 == soln_split_01);

	PNG out2(400,240);
	x = 0;
	for (int i = 0; i < im2vect.size(); i++)
	{
		int y = i % 240;
		*out2(x,y) = im2vect[i];
		if (y == 239)
			x++;
	}
	PNG soln_split_02("soln_split_02.png");
	ASSERT(out2 == soln_split_02);

	PNG out3(400,240);
	x = 0;
	for (int i = 0; i < im3vect.size(); i++)
	{
		int y = i % 240;
		*out3(x,y) = im3vect[i];
		if (y == 239)
			x++;
	}
	PNG soln_split_03("soln_split_03.png");
	ASSERT(out3 == soln_split_03);
}

UNIT_TEST(testMerge, 0, 1, 2000)
{
	cout << "[main]: " << __func__ << "()" << endl;

	PNG im1("in_08.png");
	PNG im2("in_09.png");
	PNG out(600, 400);

	vector<RGBAPixel> v1;
	for (int i = 0; i < im1.width(); i++)
		for (int j = 0; j < im1.height(); j++)
			v1.push_back(*im1(i,j));
	vector<RGBAPixel> v2;
	for (int i = 0; i < im2.width(); i++)
		for (int j = 0; j < im2.height(); j++)
			v2.push_back(*im2(i,j));
	List<RGBAPixel> l1(v1.begin(), v1.end());
	List<RGBAPixel> l2(v2.begin(), v2.end());
	l1.mergeWith(l2);
	vector<RGBAPixel> merged(l1.begin(), l1.end());
	int x = 0;
	for (int i = 0; i < merged.size(); i++)
	{
		int y = i % 400;
		*out(x,y) = merged[i];
		if (y == 399)
			x++;
	}

	PNG soln_merge("soln_merge.png");
	ASSERT(out == soln_merge);
}

UNIT_TEST(testSort, 0, 3, 5000)
{
	cout << "[main]: " << __func__ << "()" << endl;

	srand(225);

	// read in image to be shuffled
	BlockPNG b;
	b.readFromFile("in_01.png");

	int d = 60;
	vector<int> v = buildVector(b, d);
	random_shuffle(v.begin(), v.end());

	// generate shuffled image (done for you already)
	/*
	   PNG b2 = b.genImg(v, d);
	   b2.writeToFile("in_shuffled_64.png");
	 */

	// make list and sort it
	List<int> img_srt(v.begin(), v.end());
	SAVE_POINTERS(img_srt);

	img_srt.sort();

	// vectorize and rebuild image
	vector<int> v2(img_srt.begin(), img_srt.end());

	PNG unshuffled_60 = b.genImg(v2, d);
	PNG in_01("in_01.png");
	ASSERT(unshuffled_60 == in_01);
	CHECK_POINTERS(img_srt);

	d = 1;
	v = buildVector(b, d);
	random_shuffle(v.begin(), v.end());

	/*
	   PNG q = b.genImg(v, d);
	   q.writeToFile("in_shuffled_1.png");
	 */

	List<int> q_srt(v.begin(), v.end());
	SAVE_MORE_POINTERS(q_srt);
	q_srt.sort();

	v2 = vector<int>(q_srt.begin(), q_srt.end());

	PNG unshuffled_1 = b.genImg(v2, d);
	ASSERT(unshuffled_1 == in_01);
	CHECK_POINTERS(q_srt);
}

#endif // MP_PART(2)


