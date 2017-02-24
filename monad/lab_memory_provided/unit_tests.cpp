// Cs 225 Lab03 Unit Tests

#include "allocator.h"
#include "fileio.h"
#include "letter.h"
#include "room.h"

#include "proxy.h"

VALGRIND_TEST(test_constructor, 5, 5, 10000)
{
	fileio::init();
	Allocator theAllocator("students.txt", "rooms.txt");
	fileio::halt();
	PASS;
}

VALGRIND_TEST(test_allocate, 5, 5, 10000)
{
	fileio::init();
	Allocator theAllocator("students.txt", "rooms.txt");
	theAllocator.allocate();
	fileio::halt();
	PASS;
}

VALGRIND_TEST(test_printrooms, 20, 20, 10000)
{
	fileio::init();
	Allocator theAllocator("students.txt", "rooms.txt");
	theAllocator.allocate();
	theAllocator.printRooms();
	fileio::halt();
	PASS;
}

UNIT_TEST(test_any_output, 30, 30, 1000)
{
	fileio::init();
	Allocator theAllocator("students.txt", "rooms.txt");
	theAllocator.allocate();
	theAllocator.printRooms();
	fileio::halt();
	ASSERT_OUTPUT(contains, "Room Allocation (237/248)");
	PASS;
}

UNIT_TEST(test_correct_output, 40, 40, 1000)
{
	fileio::init();
	Allocator theAllocator("students.txt", "rooms.txt");
	theAllocator.allocate();
	theAllocator.printRooms();
	fileio::halt();
	ASSERT_OUTPUT(contains, "MEB 153 (31/32): W F");
	ASSERT_OUTPUT(contains, "MEB 218 (30/31): L H");
	ASSERT_OUTPUT(contains, "MEB 243 (29/30): S P Q");
	ASSERT_OUTPUT(contains, "MEB 335 (28/30): K R U V");
	ASSERT_OUTPUT(contains, "Siebel 1105 (24/24): G A X");
	ASSERT_OUTPUT(contains, "Siebel 1109 (23/24): M J I");
	ASSERT_OUTPUT(contains, "Siebel 1302 (20/21): Z N O");
	ASSERT_OUTPUT(contains, "Siebel 1304 (19/21): B D E");
	ASSERT_OUTPUT(contains, "BUR 140 (33/35): C Y T");
	PASS;
}

