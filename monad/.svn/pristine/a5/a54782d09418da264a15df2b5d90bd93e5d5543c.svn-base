// proxy.h
// NOTE: This is a generic file. Actual unit tests are located in
//       unit_tests.cpp.
// By Jack Toole for CS 225 2011

// Randomized inclusion guards so that students can't hotswap the proxy
#ifndef PROXY_H
#define PROXY_H

#include <math.h>
#include <signal.h>
#include <string.h>

#include <iostream>
#include <functional>
#include <limits>
#include <map>
#include <string>
#include <vector>
#include <utility>

#include "pipestream.h"
#include "monad_shared.h"
#include "mp_part.h"

namespace proxy
{
	using namespace std;
    using std::vector;
	using namespace monad_shared;

	class RunTests;
	typedef bool (*output_check)(const std::string &, const std::string &);

	extern std::vector<unit_test_input> * global_tests;
	typedef std::map<std::string, output_check> output_check_map;
	extern output_check_map * global_output_checks;

	class add_unit_test
	{
		public:
		add_unit_test(const char * name, unit_test_input::function func,
		              int32_t points_in_part, int32_t points_in_total, long timeout,
		              bool is_asan);

		private:
		void lazy_init_global_tests();
		int32_t get_points(int32_t points_in_total, int32_t points_in_part);
	};

	class add_output_check
	{
		public:
		add_output_check(const char * name, output_check func);
	};

	enum mode_t
	{
		SINGLE_TEST,
		MP_PART_TESTS,
		ALL_TESTS
	};


	struct RunTimeEnvironment
	{
		public:
		// Types
		struct Options
		{
			bool verbose;
			bool redirect_test_output;
			bool asan;
            bool detailed;

			Options()
				: verbose(false),
				  redirect_test_output(true),
				  asan(false),
                  detailed(true)
			{ }
		};

		// Variables
		public:
		const size_t max_output_length;
		const char * const single_test_passed_string;
		const std::vector<unit_test_input> * const heap_tests;
		const output_check_map * const output_checks;
		const Options opts;
		const mode_t mode;
		const char * const test_arg;

		// Methods
		public:
		RunTimeEnvironment(std::vector<unit_test_input> *& init_tests,
		                   output_check_map *& init_output_checks,
		                   const Options & init_opts, const char * testname);
		
		bool is_timeout_signal(const util::Signal & s)
		{
			// The below isn't correct anymore: the proxy child has a
			// signal handler for SIGPROF and SIGALRM that sends SIGTERM to
			// all child processes in the group: so our "timeout" signal is
			// now SIGTERM.
			//return s.number() == SIGPROF || s.number() == SIGALRM;
			return s.number() == SIGTERM;
		}
		
		void cleanup_globals();

		private:
		// Non-copyable
		RunTimeEnvironment(const RunTimeEnvironment & other);
		RunTimeEnvironment & operator=(RunTimeEnvironment & other);
	};

	class Failure
	{
		private:
		const char * const myFile;
		const size_t myLine;
		const string myCause;
		
		public:
		Failure(const char * file, size_t line, const string & cause)
			: myFile(file), myLine(line), myCause(cause) { }

		string message() const
		{
			stringstream ss;
			ss << myFile << ":" << myLine << ": " << myCause;
			return ss.str();
		}
	};

	class RunTests
	{
		private:
		RunTimeEnvironment & environment;

		public:
		RunTests(RunTimeEnvironment & env);
		int execute();
		private:
		void process_args(int argc, char ** argv);

		protected:
		int32_t execute_by_mode();
		int32_t run_single_test(const char * testname);
		int32_t run_single_test(const unit_test_input & curr_test);
		void    handle_single_test_output(const std::string & output);
		void    output_single_test_passfail(const unit_test_result & curr_test);
		
		int32_t run_all_tests();
		int32_t get_sum_points();
		int32_t get_max_testname_length();
		int32_t get_max_points_length();
		void output_detailed_info_if_any_failed(const vector<unit_test_result> & results, int32_t score);
		void output_detailed_tests_info(const vector<unit_test_result> & results, int32_t score);

		unit_test_result execute_test(const unit_test_input & test, bool enable_asan);

		private:
		RunTests(const RunTests & other);
		RunTests & operator=(const RunTests & other);
	};

	template <typename F>
	int32_t fork_execute(F & executor);

	// handler for our signals: ensures that on SIGALRM or SIGPROF that all
	// child processes are killed as well by sending SIGTERM to the process
	// group
	void signal_handler(int signum);
	
	class test_execution
	{
		private:
		util::pipestream fmsg_pipe; // For error messages
		util::pipestream cout_pipe; // For stdout/stderr
		util::pipestream nums_pipe; // for numbers: time
		const unit_test_input & test;
		RunTimeEnvironment & environment;
		bool do_asan;
		util::pipestream checkstream;
		string output;

		public:
		test_execution(const unit_test_input & _test, RunTimeEnvironment & env, bool enable_asan);
		void parent();
		int child();
		unit_test_result result(int32_t return_code);
		unit_test_result result(const util::Signal & s);

		private:
		int child_test();
		int child_asan();
		unit_test_result after_test_success();
		unit_test_result after_asan_success();
		void start_timeout();
		long end_timeout();
		static bool prof_timeout_enabled();
		
		private:
		test_execution(const test_execution & other);
		test_execution & operator=(const test_execution & other);
	};


	const char * parse_options(int argc, const char * const * const argv, RunTimeEnvironment::Options & opts);

} // namespace proxy

using std::cout;
using std::cerr;
using std::endl;

// Define the test namespace
// This is used for functions that have been rename_main'd
namespace proxy_test { }

#define NO_OUTPUT_CHECK (void) ___monad_checkstream___

#define UNIT_TEST_OUTPUT(func,pointsInPart,pointsInTotal,timeout)               \
	namespace proxy_test {                                               \
	void func(util::pipestream & ___monad_checkstream___);               \
	proxy::add_unit_test                                                 \
		func##_adder(#func, func, pointsInPart,                          \
		             pointsInTotal, timeout, false);                     \
	}                                                                    \
	void proxy_test::func(util::pipestream & ___monad_checkstream___)

#define UNIT_TEST(func,pointsInPart,pointsInTotal,timeout)               \
	namespace proxy_test {                                               \
	void func(util::pipestream & ___monad_checkstream___);               \
	proxy::add_unit_test                                                 \
		func##_adder(#func, func, pointsInPart,                          \
		             pointsInTotal, timeout, false);                     \
	}                                                                    \
	void proxy_test::func(__attribute__((unused)) util::pipestream & ___monad_checkstream___)

#define VALGRIND_TEST_OUTPUT(func,pointsInPart,pointsInTotal,timeout)           \
	namespace proxy_test {                                               \
	void func(util::pipestream & ___monad_checkstream___);               \
	proxy::add_unit_test                                                 \
		func##_adder(#func, func, pointsInPart,                          \
		             pointsInTotal, timeout, true);                      \
	}                                                                    \
	void proxy_test::func(util::pipestream & ___monad_checkstream___)

#define VALGRIND_TEST(func,pointsInPart,pointsInTotal,timeout)           \
	namespace proxy_test {                                               \
	void func(util::pipestream & ___monad_checkstream___);               \
	proxy::add_unit_test                                                 \
		func##_adder(#func, func, pointsInPart,                          \
		             pointsInTotal, timeout, true);                      \
	}                                                                    \
	void proxy_test::func(__attribute__((unused)) util::pipestream & ___monad_checkstream___)

#define HELPER_TEST_OUTPUT(func, ...)                                           \
	namespace proxy_test {                                               \
	void func(util::pipestream & ___monad_checkstream___, __VA_ARGS__);  \
	}                                                                    \
	void proxy_test::func(util::pipestream & ___monad_checkstream___, __VA_ARGS__)

#define HELPER_TEST(func, ...)                                           \
	namespace proxy_test {                                               \
	void func(util::pipestream & ___monad_checkstream___, __VA_ARGS__);  \
	}                                                                    \
	void proxy_test::func(util::pipestream & ___monad_checkstream___, __VA_ARGS__)

#define CALL_HELPER(func, ...)                                           \
	do {                                                                 \
		func(___monad_checkstream___, __VA_ARGS__);                      \
	} while (0)

#define DECLARE_OUTPUT_CHECK(func)  \
	bool ___monad_output_check_##func(const std::string & output, const std::string & expected);

#define OUTPUT_CHECK(func)                                                                       \
	bool ___monad_output_check_##func(const std::string & output, const std::string & expected); \
	proxy::add_output_check                                                                      \
		___monad_output_check_##func##_adder(#func, ___monad_output_check_##func);               \
	bool ___monad_output_check_##func(const std::string & output, const std::string & expected)

#define STRINGIFY1(p)   #p
#define STR(p)          STRINGIFY1(p)

#define FAIL(error)     throw proxy::Failure(__FILE__, __LINE__, (error))

#define PASS            return;

// void cast for a crazy hack around unused parameter warnings
#define ASSERT(expr)                               \
	do {                                           \
		if (!(expr))                               \
			FAIL("Assertion (" #expr ") failed");  \
	} while (0)

namespace proxy {
template <typename T, typename K>
inline std::string assert_equals_help(const T & expected, const K & actual, const char * expstr, const char * actstr)
{
	std::stringstream ss;
	if (actual != expected)
	{
		ss << "[" << actstr << " => " << actual << "] != [" << expstr << " => " << expected << "]";
		return ss.str();
	}
	return monad_shared::unit_test_result::pass_string;
}
}

// void cast for a crazy hack around unused parameter warnings
#define ASSERT_EQUALS(expected, actual)                                                     \
	do {                                                                                    \
		std::string errormsg = proxy::assert_equals_help(expected, actual, #expected, #actual);  \
		if (errormsg != monad_shared::unit_test_result::pass_string)                        \
			FAIL(errormsg);                                                                 \
	} while (0)

namespace proxy {
template <typename S>
inline void assert_output_impl(util::pipestream & checkstream, output_check checkFunc, const char * checkFuncName, const S & str)
{
	if (checkFuncName == NULL) checkFunc("", ""); // Should never occur. Used to avoid unused variable warnings
	checkstream << checkFuncName << str;
}
}

#define ASSERT_OUTPUT(checkFunc, str)  \
	proxy::assert_output_impl(___monad_checkstream___, ___monad_output_check_##checkFunc, #checkFunc, str)

enum proxy_runtime_t
{
	CONSTANT_TIME = 0,
	LOGN_TIME,
	N_TIME,
	NLOGN_TIME,
//	NROOTN_TIME,
	N2_TIME,
	N3_TIME,
	EXPONENTIAL_TIME,
	INFINITY_TIME,
	TIME_COUNT
};

namespace proxy
{
	typedef double (*runtime_function_t)(size_t);
	extern runtime_function_t runtime_function[TIME_COUNT];
	extern const char * runtime_str[TIME_COUNT];
	extern proxy_runtime_t runtimes[TIME_COUNT];

	struct TimeIterationsData
	{
		double timePerCall;
		size_t iterations;
		uint64_t totalTime;
	};

	template <typename Generator, typename Timer, typename Cleaner>                  TimeIterationsData timeIterationsImpl(Generator gen,            Timer timeFunctor, Cleaner cleanupFunc, size_t input_size);
	template <typename Generator, typename Timer, typename Cleaner>                  TimeIterationsData timeIterations    (Generator gen,            Timer timeFunctor, Cleaner cleanupFunc, size_t input_size);
	template <typename GenResult, typename GenArg, typename Timer, typename Cleaner> TimeIterationsData timeIterations    (GenResult (*gen)(GenArg), Timer timeFunctor, Cleaner cleanupFunc, size_t input_size);
	
	template <typename Generator, typename Timer, typename Cleaner>
	bool assert_time_impl(Generator gen, Timer functor, Cleaner cleanupFunc, proxy_runtime_t expectedTime, size_t size1 = 100, size_t size2 = 400);
	
	struct nop
	{
		template <typename T>
		void operator()(const T &) { }
	};
}

#define ASSERT_TIME3(gen, functor, expectedTime)                                             \
	do {                                                                                     \
		if (proxy::assert_time_impl(gen, functor, proxy::nop(), expectedTime))                 \
			FAIL(std::string("Runtime was larger than ") + proxy::runtime_str[expectedTime]);     \
	} while(0)

#define ASSERT_TIME4(gen, functor, cleanupFunc, expectedTime)                                \
	do {                                                                                     \
		if (proxy::assert_time_impl(gen, functor, cleanupFunc, expectedTime))                \
			FAIL(std::string("Runtime was larger than ") + proxy::runtime_str[expectedTime]);     \
	} while(0)

#define ASSERT_TIME5(gen, functor, expectedTime, size1, size2)                               \
	do {                                                                                     \
		if (proxy::assert_time_impl(gen, functor, proxy::nop(), expectedTime, size1, size2))   \
			FAIL(std::string("Runtime was larger than ") + proxy::runtime_str[expectedTime]);     \
	} while(0)

#define ASSERT_TIME6(gen, functor, cleanupFunc, expectedTime, size1, size2)                  \
	do {                                                                                     \
		if (proxy::assert_time_impl(gen, functor, cleanupFunc, expectedTime, size1, size2))  \
			FAIL(std::string("Runtime was larger than ") + proxy::runtime_str[expectedTime]);     \
	} while(0)

#define ASSERT_TIME_CURVE4(gen, functor, expectedTime, sizes) \
	do { \
		if (!proxy::assert_time_curve(gen, functor, proxy::nop(), expectedTime, sizes)) \
			FAIL(std::string("Runtime was larger than ") + proxy::runtime_str[expectedTime]); \
	} while(0)

#define ASSERT_TIME_CURVE5(gen, functor, cleanupFunc, expectedTime, sizes) \
	do { \
		if (!proxy::assert_time_curve(gen, functor, cleanupFunc, expectedTime, sizes)) \
		FAIL(std::string("Runtime was larger than ") + proxy::runtime_str[expectedTime]); \
	} while(0)

// Crazy hack for overloading!
// Arg counting from:
// http://cplusplus.co.il/2010/07/17/variadic-macro-to-count-number-of-arguments/
// Overloading tips:
// http://stackoverflow.com/questions/3046889/optional-parameters-with-c-macros
#define ASSERT_TIME_SEVENTH_ARG(a, b, c, d, e, f, g, ...) g

#define ASSERT_TIME(...)  \
	ASSERT_TIME_SEVENTH_ARG(__VA_ARGS__, ASSERT_TIME6, ASSERT_TIME5, ASSERT_TIME4, ASSERT_TIME3, 0, 0) (__VA_ARGS__)

#define ASSERT_TIME_CURVE_SIXTH_ARG(a, b, c, d, e, f, ...) f

#define ASSERT_TIME_CURVE(...) \
	ASSERT_TIME_CURVE_SIXTH_ARG(__VA_ARGS__, ASSERT_TIME_CURVE5, ASSERT_TIME_CURVE4, 0, 0, 0) (__VA_ARGS__)

namespace proxy {

template <typename Generator, typename Timer, typename Cleaner>
TimeIterationsData timeIterations(Generator gen, Timer timeFunctor, Cleaner cleanupFunc, size_t input_size)
{
	return timeIterationsImpl(
			bind1st(mem_fun(&Generator::operator()), &gen),
			timeFunctor,
			cleanupFunc,
			input_size);
}

template <typename GenResult, typename GenArg, typename Timer, typename Cleaner>
TimeIterationsData timeIterations(GenResult (*gen)(GenArg), Timer timeFunctor, Cleaner cleanupFunc, size_t input_size)
{
	return timeIterationsImpl(ptr_fun(gen), timeFunctor, cleanupFunc, input_size);
}

template <typename Generator, typename Timer, typename Cleaner>
TimeIterationsData timeIterationsImpl(Generator gen, Timer timeFunctor, Cleaner cleanupFunc, size_t input_size)
{
	const uint64_t min_time = 1000000; // in microseconds
	const size_t max_gen_iterations = 1000000;
	std::vector<typename Generator::result_type *> inputs;
	inputs.reserve(2000); // arbitrary, guess at how big it will be

	// Using pointers here allows us to avoid copying if the compiler supports copy elision
	// Since we're intentionally using large inputs, this could potentially have a significant effect on speed
	// We're also going to do something else weird here. Instead of generating a fixed number of inputs, we're
	// going to generate inputs for a fixed time.
	size_t max_iterations = 0;
	for (uint64_t genstart = util::process_clock(); max_iterations < max_gen_iterations && util::process_clock() - genstart < min_time; max_iterations++)
		inputs.push_back(new typename Generator::result_type(gen(input_size)));

	// Make a temporary from the generator to be used to warm up the timeFunctor (in case it needs to initialize static variables, etc.)
	typename Generator::result_type warmup_temp = gen(1);
	timeFunctor(warmup_temp); // Warm up time functor (i.e. initialize statics)

	size_t succeeded_iterations;
	uint64_t starttime = util::process_clock();
	for (succeeded_iterations = 0; succeeded_iterations < max_iterations && util::process_clock() - starttime < min_time;)
		for (uint32_t i = 0; i < 10 && succeeded_iterations < max_iterations; i++, succeeded_iterations++)
			timeFunctor(*inputs[succeeded_iterations]);
	uint64_t endtime = util::process_clock();

	for (size_t i = 0; i < max_iterations; i++)
	{
		cleanupFunc(*inputs[i]);
		delete inputs[i];
	}
	cleanupFunc(warmup_temp);

	TimeIterationsData result;
	result.timePerCall = static_cast<double>(endtime - starttime) / succeeded_iterations;
	result.iterations  = succeeded_iterations;
	result.totalTime   = endtime - starttime;
	return result;
}

inline void timeIterationsOutput(size_t size, const TimeIterationsData & data)
{
	std::cout << "Input size " << size << ": "
	          << data.iterations << " iterations in " << data.totalTime/1000 << " ms "
			  << "for an average of " << data.timePerCall << " us per call" << endl;
}

/**
 * Performs a linear least squares fit of \f$b + a*f(X) = T\f$, where
 * \f$X\f$ is the set of input sizes, \f$T\f$ is the set of running times, and
 * \f$f\f$ is the curve specified.
 *
 * @param sizes the vector of input sizes used (\f$X\f$)
 * @param times the vector of running times (\f$T\f$)
 * @param curve the enum value that specifies \f$f\f$
 * @return The root mean squared residual of the fit to the data.
 */
double fit_curve(const std::vector<size_t> & sizes,
	const std::vector<TimeIterationsData> & times,
	proxy_runtime_t curve);

inline std::ostream & operator<<(std::ostream & stream, proxy_runtime_t time)
{
	return stream << runtime_str[time];
}

template <class Generator, class Timed, class Cleaner>
bool assert_time_curve(Generator gen, Timed functor, Cleaner cleanup, proxy_runtime_t expectedTime, const std::vector<size_t> & sizes)
{
	// O(1), O(log n), O(n), O(n log n), O(n^2)
	std::vector<TimeIterationsData> times;
	times.reserve(sizes.size());
	for (size_t i = 0; i < sizes.size(); ++i) {
	    times.push_back(timeIterations(gen, functor, cleanup, sizes[i]));
	    timeIterationsOutput(sizes[i], times.back());
	}
	double min_residual = std::numeric_limits<double>::max();
	proxy_runtime_t best_time_fit;
	for (size_t i = 0; i < TIME_COUNT; ++i) {
		std::cout << "Fitting time: " << runtimes[i] << std::endl;
		double residual = fit_curve(sizes, times, runtimes[i]);
		std::cout << "RMS Residual: " << residual << std::endl;
		if (residual < min_residual) {
			best_time_fit = runtimes[i];
			min_residual = residual;
		}
	}
	std::cout << "Detected best time: " << best_time_fit << std::endl;
	std::cout << "Best RMS Residual: " << min_residual << std::endl;
	return best_time_fit <= expectedTime;
}

template <typename Generator, typename Timer, typename Cleaner>
bool assert_time_impl(Generator gen, Timer functor, Cleaner cleanupMem, proxy_runtime_t expectedTime, size_t size1, size_t size2)
{
	TimeIterationsData diff0 = timeIterations(gen, functor, cleanupMem, 1);
	TimeIterationsData diff1 = timeIterations(gen, functor, cleanupMem, size1);
	TimeIterationsData diff2 = timeIterations(gen, functor, cleanupMem, size2);
	timeIterationsOutput(    1, diff0);
	timeIterationsOutput(size1, diff1);
	timeIterationsOutput(size2, diff2);
	double ratio = (diff2.timePerCall - diff0.timePerCall) / (diff1.timePerCall - diff0.timePerCall);
	
	double expected_ratio = runtime_function[expectedTime](size1)
					/ runtime_function[expectedTime](size2);
	double toohigh_ratio  = runtime_function[expectedTime + 1](size1)
					/ runtime_function[expectedTime + 1](size2);
	double diffFromExpected = fabs(ratio - expected_ratio);
	double diffFromWrong    = fabs(ratio - toohigh_ratio);
	
	std::cout << "Actual ratio:     " << ratio << std::endl;
	std::cout << "Expected ratio:   " << expected_ratio << std::endl;
	std::cout << "Wrong/high ratio: " << toohigh_ratio  << std::endl;
	std::cout << "Diff from expected: " << diffFromExpected  << std::endl;
	std::cout << "Diff from wrong:    " << diffFromWrong  << std::endl;

#if 0 // This does not seem to be important. A sample of two iterations seems to work.
	const size_t min_iters = 100;
	if (diff0.iterations < min_iters || diff1.iterations < min_iters || diff2.iterations < min_iters)
	{
		std::cout << "Too few iterations: Code was too slow to be able to judge runtime accurately" << std::endl;
		return true;
	}
#endif

	return (diffFromWrong < diffFromExpected);
}

inline int32_t bitflags(unsigned long a, unsigned long b, unsigned long c,
                        unsigned long d, unsigned long e)
{
	return ((int)(a != 0))        | (((int)(b != 0)) << 1) |
           (((int)(c != 0)) << 2) | (((int)(d != 0)) << 3) |
           (((int)(e != 0)) << 4) ;
}

inline bool bitflag(int32_t flags, int32_t num)
{
	return (flags & (1 << num)) != 0;
}

}

DECLARE_OUTPUT_CHECK(equals);
DECLARE_OUTPUT_CHECK(contains);
DECLARE_OUTPUT_CHECK(not_contains);

#endif
