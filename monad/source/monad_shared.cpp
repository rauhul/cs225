#include <iostream>
#include "monad_shared.h"
#include "util.h"

using namespace monad_shared;
using namespace util;
using std::cout;
using std::cerr;
using std::endl;

namespace monad_shared
{

namespace versioninfo
{
const char * official_name = "Monad Autograder";
const char * version_name  = "Leap Into the Void";
const Version version_num  = Version(3, 1, 0);
const char * date          = "27 Jun 2012";
}

const char * unit_test_result::pass_string = "~~PASSED~~";
const size_t header_length = 64;

void printInfo()
{
	cout << endl
	     << versioninfo::official_name << endl
	     << "Version " << versioninfo::version_num << ": " << versioninfo::version_name << endl
		 << "Released " << versioninfo::date << endl
	     << "Developed by Jack Toole 2011-2012" << endl
	     << "Copyright 2011-2012 Jack Toole" << endl
	     << "Run monad --license to see licensing information" << endl
		 << endl;
}

namespace output
{

// Set EXIT_IF_ERROR message
void set_error_message()
{
	SET_ERROR_MESSAGE("Oops! Something went wrong inside of me.\n"
		"Please contact course staff with the following error details, and they'll figure it out:\n");
}

void header(const string & title)
{
	cout << title << "..." << endl
	     << string(header_length, '=') << endl;
}

void warning(const string & message)
{
	cerr << endl
	     << string(header_length, '*') << endl
		 << "WARNING!" << endl
		 << message << endl
	     << string(header_length, '*') << endl << endl;
}


void total_score(int32_t score, int32_t outof)
{
	if (outof < 0)
		output::header("Total score");
	else
		output::header("Total score (out of " + to_string(outof) + ")");
	
	cout << "TOTAL SCORE: " << score << endl << endl;
}

void testname(const unit_test_input & curr_test, int32_t max_testname_len, int32_t max_points_len)
{
	// Output test name
	int32_t pos = 0; // keep track of cursor position
	std::cout << curr_test.name() << ' ';
	pos += strlen(curr_test.name()) + 1;

	if (curr_test.is_asan())
	{
		cout << "(asan) ";
		pos += 7;
	}

	if (pos % 2 == max_testname_len % 2)
	{
		cout << ' ';
		pos++;
	}

	while (pos < max_testname_len + 1)
	{
		cout << ". ";
		pos += 2;
	}
	pos = 0; // reset column

	std::cout << "[" << curr_test.points() << " pts] ";
	pos += intlen(curr_test.points()) + 7;

	while (pos < max_points_len + 7)
	{
		cout << ' ';
		pos++;
	}

	cout << "- ";
}


void detailed_info(const unit_test_result & curr_test)
{
	std::cout << string(header_length, '-') << endl
			  << curr_test.name();
	if (curr_test.is_asan()) std::cout << " (run under asan)";
	std::cout << " [" << curr_test.points() << " points]" << endl;

	const string & error = curr_test.errormsg();
	const string & output = curr_test.output();

	if (curr_test.passed())
		std::cout << "Result: " << passed_string() << endl;
	else
		std::cout << "Result: " << failed_string() << ": " << error << endl;

	if (curr_test.time() < 0)
		cout << "Took unknown time (";
	else
		cout << "Took " << curr_test.time() << "ms (";
	cout << curr_test.timeout() << "ms timeout)" << endl;

	std::cout << "Output:" << endl
			  << string(header_length, '-') << endl;

	// Tab the output over to distinguish it from the test case
	if (output != "")
	{
		//std::cout << "    ";
		//replaceAllInternal(output,"\n","\n    ");
		std::cout << output;
		if (output[output.length() - 1] != '\n') std::cout << endl;
	}

	cout << endl;
}

string passed_string()
{
	return colorize::make_color(colorize::GREEN, "passed");
}

string failed_string()
{
	return colorize::make_color(colorize::RED  , "FAILED");
}

} // namespace output
} // namespace monad_shared

