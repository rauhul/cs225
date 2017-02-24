#include "monad.h"

namespace monad
{

void printHelp()
{
	cout
#include "README.cgo"
	;
}

void printHelpConfig()
{
	cout
#include "README_config.cgo"
	;
}

void printHelpTests()
{
	cout
#include "README_tests.cgo"
	;
}

void printLicense()
{
	cout
#include "LICENSE.cgo"
	;
}

const char * getRandomQuote()
{
	const char * quotes[] = {
#include "quotes.cga"
	};
	size_t quotes_size = (sizeof quotes) / (sizeof quotes[0]);
	return quotes[urandn(quotes_size)];
}

} // namespace monad
