OVERVIEW: Monad Autograder (c) Jack Toole
  Monad is a system for grading students' C/C++ code with point-weighted unit
  tests.

USAGE: monad <assignment> [options]
  Runs the tests for <assignment> (mp1, lab01, ...)
  '../<assignment>/' must exist
  If '../<assignment>_tests/' or any necessary library directories do not exist,
  they will be downloaded from SVN

GENERAL OPTIONS:
  Note: Any option may be negated with --no<option> or --<option>=false
  Options which default to true are shown negated below

  --noclean               Do not re-copy test cases into sandbox. This improves
                          performance at the cost of safety
  --provided              Use <assignment>_provided/ instead of
                          <assignment>_tests/ for test cases
  --noupdate              Do not call 'svn update' on the tests and library
                          directories
  --verbose               Display detailed test output information for passing
                          tests
  --parallel              Compiles code in parallel if possible

INFORMATION OPTIONS:
  --help                  Display this dialog
  --help config           Help for writing config files
  --help tests            Help for writing test cases
  --info, --version       Display version information
  --license               Display the license

STAFF OPTIONS:
  --staff                 Use the staff SVN repository to download test cases.
                          This allows monad to be run by staff without
                          releasing the test cases to students
  --solution              Grade <assignment>_solution/ instead of
                          <assignment>/
  --newtests              Use <assignment>_newtests/ instead of
                          <assignment>_tests/ for test cases. This allows new
                          test cases to be developed without replacing the old
                          ones for release tests
