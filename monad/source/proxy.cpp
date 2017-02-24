// proxy.cpp
// NOTE: This is a generic file. Actual unit tests are located in
//       unit_tests.cpp.
// By Jack Toole for CS 225 spring 2011

// For strsignal:
#ifndef _GNU_SOURCE
	#define _GNU_SOURCE
#endif

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <numeric>

#include "monad_shared.h"
#include "pipestream.h"
#include "proxy.h"
#include "util.h"

using namespace std;
using namespace util;
using namespace monad_shared;


OUTPUT_CHECK(equals)
{
	return output == expected;
}


OUTPUT_CHECK(contains)
{
	return output.find(expected) != string::npos;
}


OUTPUT_CHECK(not_contains)
{
	return output.find(expected) == string::npos;
}


namespace proxy
{
	vector<unit_test_input> * global_tests = NULL;
	output_check_map * global_output_checks = NULL;

	double time_constant   (size_t)      { return 1.0; }
	double time_logn       (size_t size) { return std::log(size); }
	double time_linear     (size_t size) { return size; }
	double time_nlogn      (size_t size) { return size * std::log(size); }
	double time_nsquared   (size_t size) { return size * size; }
	double time_cubed      (size_t size) { return size * size * size; }
	double time_exponential(size_t size) { return std::exp(size); }
	double time_infinity   (size_t)      { return std::numeric_limits<double>::max(); }

	runtime_function_t runtime_function[TIME_COUNT] =
	{
		time_constant,
		time_logn,
		time_linear,
		time_nlogn,
		time_nsquared,
		time_cubed,
		time_exponential,
		time_infinity
	};
	const char * runtime_str[TIME_COUNT] =
	{
		"O(1)",
		"O(logn)",
		"O(n)",
		"O(nlogn)",
//		"O(nrootn)",
		"O(n^2)",
		"O(n^3)",
		"O(e^n)",
		"O(never)"
	};
	proxy_runtime_t runtimes[TIME_COUNT] =
	{
		CONSTANT_TIME,
		LOGN_TIME,
		N_TIME,
		NLOGN_TIME,
		N2_TIME,
		N3_TIME,
		EXPONENTIAL_TIME,
		INFINITY_TIME
	};
}



int main(int argc, char ** argv)
{
	using namespace proxy;
	
	// set up EXIT_IF_ERROR messages
	output::set_error_message();

	RunTimeEnvironment::Options opts;
	const char * testname = parse_options(argc, argv, opts);

	// Set up run-time environment
	RunTimeEnvironment env(global_tests, global_output_checks, opts, testname);

	// Set up the tests
	RunTests runner(env);

	// Execute
	return runner.execute();
}


namespace proxy {

// class add_unit_test
add_unit_test::add_unit_test(const char * name, unit_test_input::function func,
                             int32_t points_in_part, int32_t points_in_total, long timeout,
                             bool is_asan)
{
	lazy_init_global_tests();
	int32_t points = get_points(points_in_total, points_in_part);
	// Add to global tests vector
	global_tests->push_back(unit_test_input(name, func, points, timeout, is_asan));
}


// Check to make global tests vector
void add_unit_test::lazy_init_global_tests()
{
	if (global_tests == NULL)
		global_tests = new std::vector<unit_test_input>;
}

// Discriminate which points value to add
int32_t add_unit_test::get_points(int32_t points_in_total, int32_t points_in_part)
{
	// silence unused parameter warnings
	(void) points_in_total;
	(void) points_in_part;
	#if MP_PART(NO_MP_PART)
		return points_in_total;
	#else
		return points_in_part;
	#endif
}


const char * parse_options(int argc, const char * const * const argv, RunTimeEnvironment::Options & opts)
{
	string getTestName = "all";

	OptionsParser parseopts;
	parseopts.addOption("verbose",  opts.verbose);
	parseopts.addOption("redirect", opts.redirect_test_output);
	parseopts.addOption("asan", opts.asan);
	parseopts.addOption("detailed", opts.detailed);
	parseopts.addArg(getTestName);
	parseopts.parse(argc, argv);

	const char * testname = "all";
	for (int32_t i = 0; i < argc; i++)
		if (getTestName == argv[i])
			testname = argv[i];
	return testname;
}


// class add_output_check
add_output_check::add_output_check(const char * name, output_check func)
{
	if (global_output_checks == NULL)
		global_output_checks = new output_check_map;
	(*global_output_checks)[name] = func;
}



// class Run_Time_Environment
RunTimeEnvironment::RunTimeEnvironment(vector<unit_test_input> *& init_tests,
                                       output_check_map *& init_output_checks,
                                       const Options & init_opts, const char * testname)
	: max_output_length(8*1024), //arbitrary
	  single_test_passed_string("Result: passed"),
	  heap_tests(init_tests),
	  output_checks(init_output_checks != NULL ? init_output_checks : new output_check_map),
	  opts(init_opts),
	  mode(strcasecmp(testname, "all") == 0 ? ALL_TESTS : SINGLE_TEST),
	  test_arg(testname)
{
	// Copy globals to the RunTimeEnvironment space
	// And remove them from the global scope
	static int8_t singleton = 0;
	EXIT_IF_ERROR(singleton++ != 0, "There may only be one runtime environment");
	EXIT_IF_ERROR(heap_tests == NULL, "No test cases found");
	
	init_tests = NULL;
	init_output_checks = NULL;
}


void RunTimeEnvironment::cleanup_globals()
{
	static bool called_already = false;
	if (!called_already)
	{
		if (heap_tests    != NULL) delete heap_tests;
		if (output_checks != NULL) delete output_checks;
		called_already = true;
	}
}



// class RunTests
RunTests::RunTests(RunTimeEnvironment & env)
	: environment(env)
{ }


int32_t RunTests::execute()
{
	int32_t return_code = execute_by_mode();
	environment.cleanup_globals();
	return return_code;
}


int32_t RunTests::execute_by_mode()
{
	if (environment.mode == SINGLE_TEST)
		return run_single_test(environment.test_arg);
	else if (environment.mode == ALL_TESTS)
		return run_all_tests();
	
	EXIT_IF_ERROR(true, string("Failed to implement case for mode ") + environment.mode);
	exit(-1);
}


int32_t RunTests::run_single_test(const char * testname)
{
	const vector<unit_test_input> & tests = *environment.heap_tests;

	for (size_t test_i = 0; test_i < tests.size(); test_i++)
		if (strcmp(tests[test_i].name(), testname) == 0)
			return run_single_test(tests[test_i]);

	cout << "Test '" << testname << "' not found" << endl;
	exit(-1);
}


int32_t RunTests::run_single_test(const unit_test_input & curr_test)
{
	cout << "Running " << curr_test.name() << " [worth "
		 << curr_test.points() << " points, output below]" << endl;

	test_execution executor(curr_test, environment, false);
	auto pid = fork();
	if (pid == 0)
		return executor.child();

	EXIT_IF_ERROR(pid == -1);

	int child_status;
	executor.parent();
	auto ws = waitpid(pid, &child_status, 0);
	EXIT_IF_ERROR(ws == -1);

	std::unique_ptr<unit_test_result> result;
	if (WIFEXITED(child_status))
		result = std::make_unique<unit_test_result>(executor.result(WEXITSTATUS(child_status)));
	else if (WIFSIGNALED(child_status))
		result = std::make_unique<unit_test_result>(executor.result(Signal(WTERMSIG(child_status))));
	else
		result = std::make_unique<unit_test_result>(executor.result(Signal(SIGSTOP)));

	const std::string& error = result->errormsg();

	handle_single_test_output(result->output());

	if (result->passed())
		cout << environment.single_test_passed_string << endl;
	else
		cout << "Result: FAILED:" << endl << error << endl;
	return 0;

	/*
	unit_test_result result = execute_test(curr_test, false);

	const string & error  = result.errormsg();

	handle_single_test_output(result.output());

	if (result.passed())
		cout << environment.single_test_passed_string << endl;
	else
		cout << "Result: FAILED:" << endl << error << endl;
	return 0;
	*/
}


void RunTests::handle_single_test_output(const string & output)
{
	if (output != "")
	{
		cout << output;
		if (output[output.size()-1] != '\n')
			cout << endl;
	}
}

int32_t foldTestScore (int score,  const unit_test_result & test) { return score  + test.points_scored(); }
int32_t foldTestPoints(int points, const unit_test_input  & test) { return points + test.points(); }

int RunTests::run_all_tests()
{
	const vector<unit_test_input> & tests = *environment.heap_tests;

	output::header("Running tests");

	int32_t points_sum = get_sum_points();
	int32_t max_testname_len = get_max_testname_length();
	int32_t max_points_len   = get_max_points_length();

#if 0 // removed due to partial compilation lowering this number artificially
	if (points_sum != 0 && points_sum < 100)
		output::warning("Unit test scores sum to " + to_string(points_sum) +
		                ", should be at least 100");
#endif
	if (points_sum > 125)
		output::warning("Unit test scores sum to " + to_string(points_sum) +
		                ", this will overflow the return value. Should be <= 125");

	vector<unit_test_result> results;
	for (size_t test_i = 0; test_i < tests.size(); test_i++)
	{
		output::testname(tests[test_i], max_testname_len, max_points_len);
		results.push_back(execute_test(tests[test_i], true));
		output_single_test_passfail(results.back());
	}

	const int32_t score = std::accumulate(results.begin(), results.end(), 0, foldTestScore);

	cout << endl << endl;
    if (environment.opts.detailed)
        output_detailed_info_if_any_failed(results, score);
	output::total_score(score, -1);
	
	return score;
}

int32_t RunTests::get_sum_points()
{
	static int32_t cached_sum = INT_MIN;
	if (cached_sum == INT_MIN)
		cached_sum = std::accumulate(environment.heap_tests->begin(), environment.heap_tests->end(), 0, foldTestPoints);
	return cached_sum;
}

int32_t RunTests::get_max_testname_length()
{
	const vector<unit_test_input> & tests = *environment.heap_tests;
	int32_t max_testname_len = 0;
	for (size_t test_i = 0; test_i < tests.size(); test_i++)
	{
		// TODO (toole1): This is horrible style!
		int32_t currlen = strlen(tests[test_i].name()) + (int)tests[test_i].is_asan() * 7; // strlen(" (asan)");

		if (currlen > max_testname_len)
			max_testname_len = currlen;
	}
	return max_testname_len;
}

int32_t RunTests::get_max_points_length()
{
	const vector<unit_test_input> & tests = *environment.heap_tests;
	uint32_t max_points_len = 0;
	for (size_t test_i = 0; test_i < tests.size(); test_i++)
		max_points_len = max(max_points_len, intlen(tests[test_i].points()));
	return max_points_len;
}

void RunTests::output_detailed_info_if_any_failed(const vector<unit_test_result> & results, int32_t score)
{
	if (count_if(results.begin(), results.end(), unit_test_result::_failed) ||
			environment.opts.verbose)
		output_detailed_tests_info(results, score);
}
	
	
void RunTests::output_detailed_tests_info(const vector<unit_test_result> & results, int32_t score)
{
	output::total_score(score, -1);
	cout << endl << endl;
	
	output::header("Detailed test output");
	
	for (size_t test_i = 0; test_i < results.size(); test_i++)
		if (results[test_i].failed() || environment.opts.verbose)
			output::detailed_info(results[test_i]);
	
	cout << endl << string(64, '-') << endl; // TODO (toole1): poor style, should be refactored to monad_shared::output
}


void RunTests::output_single_test_passfail(const unit_test_result & curr_test)
{
	if (curr_test.passed())
		std::cout << output::passed_string() << endl;
	else
    {
		std::cout << output::failed_string();
        if (environment.opts.detailed)
            std::cout << ": " << curr_test.errormsg();
        std::cout << endl;
    }
}

test_execution::test_execution(const unit_test_input & _test, RunTimeEnvironment & env, bool enable_asan)
	: test(_test), environment(env)
{
	do_asan = enable_asan && (test.is_asan() || env.opts.asan);
}

// Signal handler for the child fork of proxy: handles SIGALRM and SIGPROF
// by killing all processes in the current process group
void signal_handler(int)
{
	// kill our entire process group
	kill(-1 * getpid(), SIGTERM);
}

int test_execution::child()
{
	// close ends of pipes we don't need
	fmsg_pipe.close_read();
	cout_pipe.close_read();
	nums_pipe.close_read();

	// install our signal handlers to ensure that child processes created
	// by proxy inside of the test cases are properly cleaned up
	//
	// first, we create a new process group, using the current process id
	// as the group id
	setpgid(0, 0);
	// next, we install our signal handlers on SIGALRM and SIGPROF, our two
	// timers
	struct sigaction new_action;
	new_action.sa_handler = proxy::signal_handler;
	sigemptyset(&new_action.sa_mask);
	new_action.sa_flags = 0;
	sigaction(SIGPROF, &new_action, NULL);
	sigaction(SIGALRM, &new_action, NULL);

	// redirect stdout/stderr to pipe
	if (environment.opts.redirect_test_output)
	{
		cout_pipe.steal_output(STDOUT_FILENO);
		cout_pipe.steal_output(STDERR_FILENO);
	}

	if (do_asan)
	{
		return child_asan();
	}
	else // if (!test.is_asan)
	{
		return child_test();
	}
}

void test_execution::parent()
{
	fmsg_pipe.close_write();
	cout_pipe.close_write();
	nums_pipe.close_write();
//!!	if (test.checkstream != NULL)
	checkstream.close_write();

	// Read stdout/stderr pipe while process is running
	if (environment.opts.redirect_test_output)
		cout_pipe >> setmax(environment.max_output_length) >> output;
	else
		output = "Test output was displayed above instead of being buffered\n";
	
	cout_pipe.close_read();
}

unit_test_result test_execution::result(int32_t return_code)
{
	if (do_asan)
		return after_asan_success();
	else if (return_code == 23) // code used by ASAN
		return {test, "Memory Leaks Detected by ASAN", output, -1};
	else if (return_code == 0)
		return after_test_success();
	else
		return {test, "Memory Errors Detected by ASAN", output, -1};
}

unit_test_result test_execution::result(const Signal & s)
{
	string errormsg;
	long time = 0;

	fmsg_pipe.close();
	nums_pipe.close();
	if (environment.is_timeout_signal(s))
	{
		errormsg = string("Timed out") + " (" + to_string(test.timeout()) + "ms)";
		time = test.timeout();
	}
	else
		errormsg = s.what();
	
	return unit_test_result(test, errormsg, output, time);
}


unit_test_result RunTests::execute_test(const unit_test_input & test, bool enable_asan)
{
	cout << std::flush;
	test_execution executor(test, environment, enable_asan);
	try
	{
		int32_t return_code = fork_execute(executor);
		return executor.result(return_code);
	}
	catch (Signal & s)
	{
		return executor.result(s);
	}
}

template <typename F>
int32_t fork_execute(F & executor)
{
	// Fork
	pid_t process_id;
	process_id = fork();
	EXIT_IF_ERROR(process_id < 0, "Could not fork application");

	if (process_id == 0)
	{
		int return_code = executor.child();
		exit(return_code);
	}
	else // if (process_id > 0)
	{
		executor.parent();

		int child_status;
		pid_t ws = waitpid(process_id, &child_status, 0); //should return immediately because it's after parent()
		EXIT_IF_ERROR(ws == -1);

		if (WIFEXITED(child_status))
			return WEXITSTATUS(child_status);
		else if (WIFSIGNALED(child_status))
			throw Signal(WTERMSIG(child_status));
		else
			throw Signal(SIGSTOP);
	}
}

int test_execution::child_asan()
{
	// We're giving up control to the asan-compiled proxy, so we can't
	// use anything but the cout pipe now
	fmsg_pipe.close_write();
	nums_pipe.close_write();
	
	start_timeout();
	return exec("./proxy-asan", test.name());
}


int test_execution::child_test()
{
	checkstream.close_read();
	// Execute test
	bool test_failed = false;
	long test_time = -1;
	start_timeout();
	try
	{
		test.execute(checkstream); // execute function
		test_time = end_timeout();
		fmsg_pipe << unit_test_result::pass_string;
	}
	catch (Failure & failure)
	{
		test_time = end_timeout();
		test_failed = true;
		// Write failure message to pipe
		fmsg_pipe << failure.message();
	}
	fmsg_pipe.close();

	// write time to pipe
//!!	delete test.checkstream;
	checkstream.close();
	environment.cleanup_globals();
	nums_pipe << test_time;
	nums_pipe.close();
	return 0;
}


unit_test_result test_execution::after_asan_success()
{
	fmsg_pipe.close_read();
	nums_pipe.close_read();

	string errormsg;

	size_t last_endl = findNthLast(output, '\n', 2);

	if (last_endl == string::npos)
	{
		if (environment.opts.redirect_test_output)
			errormsg = "ASAN test did not complete";
		else
			errormsg = "ASAN test output was not redirected to pipe because --redirect was set.";
	}
	else
	{
		errormsg = output.substr(last_endl + 1,
				output.length() - last_endl - 2);

		if (errormsg == environment.single_test_passed_string)
			errormsg = unit_test_result::pass_string;
	}

	if (errormsg.find("Memory") != errormsg.npos)
		errormsg = "Memory errors detected by ASAN";

	return unit_test_result(test, errormsg, output, -1);
}


unit_test_result test_execution::after_test_success()
{
	string errormsg;
	long time;

	fmsg_pipe >> errormsg;
	fmsg_pipe.close();
	nums_pipe >> time;
	nums_pipe.close();
	
	// Check for output's correctness, if that was a condition of passing
	if (errormsg == unit_test_result::pass_string)
	{
		while (!checkstream.eof())
		{
			string checkname;
			string checkstr;
			checkstream >> checkname;
			if (checkstream.eof()) break;
			checkstream >> checkstr;
			if (checkstream.eof()) break;

			output_check check_function = (*environment.output_checks).find(checkname)->second;
			if (check_function == NULL)
			{
				cerr << "Internal Error: in test " << test.name() << ": "
				     << checkname << " is not a registered OUTPUT_CHECK function" << endl;
				exit(-2);
			}

			if (!check_function(output, checkstr))
				errormsg = "Incorrect Terminal Output";
		}
	}
	

//!!	delete test.checkstream;
	checkstream.close();
	
	return unit_test_result(test, errormsg, output, time);
}

bool test_execution::prof_timeout_enabled()
{
	struct itimerval temp;
	errno = 0;
	if (getitimer(ITIMER_PROF, &temp) == 0)
		return true;
	if (errno == EINVAL)
		return false;
	cerr << __FILE__ << ":" << __LINE__ << ": ERROR: getitimer failed" << endl;
	exit(-1);
}

void test_execution::start_timeout()
{
	static const bool prof_enabled = prof_timeout_enabled();

	struct itimerval timeout;
	timeout.it_interval.tv_sec  = 0;
	timeout.it_interval.tv_usec = 0;
	timeout.it_value.tv_sec  = test.timeout()/1000;
	timeout.it_value.tv_usec = (test.timeout()%1000) * 1000;

	if (prof_enabled)
	{
		EXIT_IF_ERROR(setitimer(ITIMER_PROF, &timeout, NULL));
		// second real time signal in case the student calls a blocking call
		timeout.it_value.tv_sec *= 10;
		EXIT_IF_ERROR(setitimer(ITIMER_REAL, &timeout, NULL));
	}
	else
	{
		EXIT_IF_ERROR(setitimer(ITIMER_REAL, &timeout, NULL));
	}
}


long test_execution::end_timeout()
{
	static const bool prof_enabled = prof_timeout_enabled();

	struct itimerval timeout;
	timeout.it_interval.tv_sec  = 0;
	timeout.it_interval.tv_usec = 0;
	timeout.it_value.tv_sec  = 0;
	timeout.it_value.tv_usec = 0;
	struct itimerval remaining;

	if (prof_enabled)
	{
		EXIT_IF_ERROR(setitimer(ITIMER_PROF, &timeout, &remaining));
		EXIT_IF_ERROR(setitimer(ITIMER_REAL, &timeout, NULL));
	}
	else
	{
		EXIT_IF_ERROR(setitimer(ITIMER_REAL, &timeout, &remaining));
	}

	// There seems to be a strange -1 error here. I may just be tired,
	// but I can't figure out why right now
	long time = test.timeout() - remaining.it_value.tv_sec*1000 - remaining.it_value.tv_usec/1000;
	return (time < 0) ? 0 : time;
}



} // namespace proxy

