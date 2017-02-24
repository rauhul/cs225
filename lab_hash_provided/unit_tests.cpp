// UNIT_TEST(name, #1, #2, 1000){
// code goes here
// ends with PASS, ASSERT(bool condition), or FAIL("message")
// }
#include <algorithm>
#include <map>
#include <vector>

#include "textfile.h"

#include "lphashtable.h"
#include "schashtable.h"
#include "word_counter.h"
#include "anagram_finder.h"

#include "util.h"
#include "proxy.h"

using namespace std;
using namespace util;

#if MONAD_SPLIT
UNIT_TEST(testRemoveEASY, 6, 6, 5000){
	TextFile infile( "textEasy.txt" );
	SCHashTable<string, int> hashTable(32);
	while( infile.good() ) {
		string word = infile.getNextWord();
		hashTable.insert(word, 0);
	}
	hashTable.remove("all");
	if( hashTable.keyExists("all") )
		FAIL("Did not remove key: 'all'");
	hashTable.remove("base");
	if( hashTable.keyExists("base") )
		FAIL("Did not remove key: 'base'");
}
#endif

#if MONAD_SPLIT
UNIT_TEST(testRemoveHard, 10, 10, 5000){
	TextFile infile( "textHard.txt" );
	SCHashTable<string, int> hashTable(32);
	while( infile.good() ) {
		string word = infile.getNextWord();
		if( hashTable.keyExists(word) ){
			hashTable.remove(word);
			if( hashTable.keyExists(word) )
				FAIL("Did not remove key on update");
			hashTable.insert(word, 1);
		}
		else
			hashTable.insert(word, 0);
	}
	hashTable.remove("to");
	if( hashTable.keyExists("to") )
		FAIL("Did not remove key: 'to'");
	hashTable.remove("decided");
	if( hashTable.keyExists("decided") )
		FAIL("Did not remove key: 'decided'");
}
#endif
#if MONAD_SPLIT
UNIT_TEST(testResizeOnce, 7, 7, 5000){
	TextFile infile( "textLong.txt" );
	SCHashTable<string, int> hashTable(16);
	int val = 0;
	while( infile.good() ) {
		string word = infile.getNextWord();
		if( !hashTable.keyExists(word) ) {
            val++;	
			hashTable.insert(word, val);
        }
	}
	ASSERT(hashTable.tableSize() >= val / 0.7);
	if( hashTable.find("got") != 2 )
		FAIL("Didn't copy over existing lists properly on resize");
	if( hashTable.find("to") != 3 )
		FAIL("Didn't re-hash keys properly");
}
#endif

#if MONAD_SPLIT
UNIT_TEST(testResizeAll, 11, 11, 5000){
	vector<string> strings;
	for(int i = 'a'; i <= 'z'; i++) {
		string word = "";
		word += ((char)i);
		strings.push_back(word);
	}
    cout << strings.size() << endl;
	SCHashTable<string, int> hashTable(16);
	int val = 0;
	for(unsigned int i = 0; i < strings.size(); i++) {
		val++;
		hashTable.insert(strings[i], val);
	}
	ASSERT(hashTable.tableSize() >= strings.size() / 0.7);
	for(int i = 'a'; i <= 'z'; i++) {
		string word = "";
		word += ((char)i);
		if( !hashTable.keyExists(word) )
			FAIL("Missed a key during resize");
	}
}
#endif


#if MONAD_SPLIT
UNIT_TEST(testInsertEasy, 6, 6, 5000){
	TextFile infile( "textEasy.txt" );
	LPHashTable<string, int> hashTable(32);
	int val = 0;
	while( infile.good() ) {
		val++;
		string word = infile.getNextWord();
		hashTable.insert(word, val);
	}
	if( !hashTable.keyExists("all") )
		FAIL("Did not insert key: 'all'");
	if( !hashTable.keyExists("base") )
		FAIL("Did not insert key: 'base'");
	if( hashTable.find("are") != 4 )
		FAIL("Did not insert key-value pair properly");
}
#endif

#if MONAD_SPLIT
UNIT_TEST(testInsertHard, 10, 10, 5000){
	TextFile infile( "textHard.txt" );
	LPHashTable<string, int> hashTable(32);
    int val = 0;
	while( infile.good() ) {
		string word = infile.getNextWord();
        if( hashTable.keyExists(word) )
            hashTable.remove(word);
        hashTable.insert(word, ++val);
	}
	if( !hashTable.keyExists("has") )
		FAIL("Did not insert properly key: 'has'");
	if( !hashTable.keyExists("far") )
		FAIL("Did not insert properly key: 'far'");
	if( hashTable.find("want") != 13 )
		FAIL("Did not insert key-value pair properly");
}
#endif

template <template <class K, class V> class HT>
void testAnagrams() {
	vector<string> strings;
	strings.push_back("herp");
	strings.push_back("reph");
	strings.push_back("igloo");
	strings.push_back("dog");
	strings.push_back("per");
	strings.push_back("herrp");
	strings.push_back("hrep");

	AnagramFinder<HT> finder(strings);
	vector<string> result = finder.getAnagrams("herp");
	if( result.size() > 3 )
		FAIL("Found too many anagrams");
	if( result.size() < 3 )
		FAIL("Did not find enough anagrams");
	vector<string> expected;
	expected.push_back("herp");
	expected.push_back("reph");
	expected.push_back("hrep");
	
	sort(expected.begin(), expected.end());
	sort(result.begin(), result.end());
	for(unsigned int i = 0; i < expected.size(); i++)
		if( expected[i].compare(result[i]) != 0 )
			FAIL("anagrams did not match");
}

#if MONAD_SPLIT
UNIT_TEST(testAnagramsSC, 8, 8, 5000){
    testAnagrams<SCHashTable>();
}
#endif

#if MONAD_SPLIT
UNIT_TEST(testAnagramsLP, 8, 8, 5000){
    testAnagrams<LPHashTable>();
}
#endif

template <template <class K, class V> class HT>
void testWordCount() {
    WordFreq<HT> wf("wordCount.txt");
    vector< pair<string, int> > result = wf.getWords(11);
    if( result.size() > 7 )
        FAIL("Found too many words");
    if( result.size() < 7 )
        FAIL("Did not find all words");

    map<string, int> expected;
    expected["more"] = 12;
    expected["like"] = 12;
    expected["to"] = 13;
    expected["far"] = 21;
    expected["as"] = 21;
    expected["use"] = 12;
    expected["even"] = 22;
    expected["has"] = 6;
    expected["anyone"] = 10;

    for (unsigned int i = 0; i < result.size(); i++)
        if (expected[result[i].first] != result[i].second)
            FAIL("'" + result[i].first + "' gave " + result[i].second + ", expected " + expected[result[i].first]);
}

#if MONAD_SPLIT
UNIT_TEST(testWordCountSC, 8, 8, 5000){
    testWordCount<SCHashTable>();
}
#endif

#if MONAD_SPLIT
UNIT_TEST(testWordCountLP, 8, 8, 5000){
    testWordCount<LPHashTable>();
}
#endif

#if MONAD_SPLIT

#include "logfile_parser.h"
// The following is a hack, but useful so that all the extra credit code falls
// within a MONAD_SPLIT block to resolve compilation issues
#include "logfile_parser.cpp"

UNIT_TEST(testLogfileParse, 18, 18, 5000){
	LogfileParser lfp("log.txt");
	vector<string> urls = lfp.uniquePages();
	if( urls.size() > 4 )
		FAIL("Found too many URLs");
	if( urls.size() < 4 )
		FAIL("Did not find enough URLs");

	vector<string> expected;
	expected.push_back("/product/3/");
	expected.push_back("/product/5/");
	expected.push_back("/product/2/");
	expected.push_back("/product/4/");

	sort(expected.begin(), expected.end());
	sort(urls.begin(), urls.end());
	for(unsigned int i = 0; i < expected.size(); i++)
		if( expected[i].compare(urls[i]) != 0 )
			FAIL("URLs did not match");

	bool v1 = lfp.hasVisited("chase", "/product/1/");
    if( v1 )
        FAIL( "Found visit that did not exist" );
	bool v2 = lfp.hasVisited("alex", "/product/3/");
	bool v3 = lfp.hasVisited("dave", "/product/2/");
	if( !v2 || !v3 )
		FAIL("Failed to find visit");

	time_t result1, result2, result3;
	result1 = lfp.dateVisited("chase", "/product/5/");
	result2 = lfp.dateVisited("alex", "/product/3/");
	result3 = lfp.dateVisited("dave", "/product/2/");

	time_t res1 = time(NULL);
	time_t res2 = time(NULL);
	time_t res3 = time(NULL);

	tm * tme = localtime( &res1 );
	strptime( "Sun Apr 10 09:39:34 2011", "%c", tme );
	tme->tm_isdst = 1;
	res1 = mktime( tme );
	strptime( "Sat Apr  9 07:56:24 2011", "%c", tme );
	tme->tm_isdst = 1;
	res2 = mktime( tme );
	strptime( "Sat Apr  9 08:19:33 2011", "%c", tme );
	tme->tm_isdst = 1;
	res3 = mktime( tme );

	ASSERT_EQUALS(result1, res1);
	ASSERT_EQUALS(result2, res2);
	ASSERT_EQUALS(result3, res3);
}
#endif
