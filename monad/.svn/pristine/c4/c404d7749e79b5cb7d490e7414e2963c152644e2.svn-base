/**
 * @file monad.cpp
 * Main entry point for the monad executable program.
 * @note For illinois.edu CS 225 spring 2011
 * @author Jack Toole
 */

#include "monad.h"
#include "svn.h"

namespace monad
{
void find_base_dir(const char* argv0);
void chdirToMonad();
void chdirToCallingDir();
void create_mp_part_file();
void processArgs(int argc, const char* const* argv);
void copyRequiredFiles();
void getLibs(const vector<string>& libs);
string updateFolder(const string& folder, bool link);
string tryGetFolder(string dirAndFolder, const string& target, bool link);
string getFolder(const string& folder, const string& searchDir = "",
                 bool link = false);
void importFiles(const string& preservedFolder, const string& theSourceFolder,
                 const string& destFolder, const vector<string>& files,
                 bool exit_if_error = true);
void exec_command(const string& command);

string assignment_base;
int8_t mp_part;
const int8_t no_mp_part = -1;

int callingDirFD;
int monadDirFD;

string monadSourceFolder;
string testsFolder;
string sourceFolder;
string gradeFolder;
string tempFolder;

FileMap config;

namespace opts
{
bool solution = false;
bool clean = true;
bool update = true;
bool staff = false;
bool newtests = false;
bool provided = true;

bool svn = true;

bool verbose = false;
bool detailed = true;
bool buffer = true;
bool asan = false;
bool parallel = false;

bool help = false;
bool info = false;
bool license = false;
#if OPTIMIZE
bool optimize = true;
#else
bool optimize = false;
#endif
}
}

// http://stackoverflow.com/questions/997946/c-get-current-time-and-date
string getTime()
{
    time_t now = time(0);
    struct tm tstruct;
    char buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);
    return buf;
}

int main(int argc, const char* const* argv)
{
    using namespace monad;

    output::set_error_message();

    // Find monad/ directory
    find_base_dir(argv[0]);

    // Read in local config settings.
    // Necessary to do this early for [SVN Root] url
    config.readConfig(monadSourceFolder);
    processArgs(argc, argv);

    cout << versioninfo::official_name << endl;
    cout << "Testing " << assignment_base;
    if (mp_part != no_mp_part)
        cout << '.' << (int)mp_part;
    cout << " at " << getTime() << endl;
    cout << endl << getRandomQuote() << endl << endl;
    cout << "Setting up test environment..." << endl;

    // Read in test-specific config settings
    config.readConfig(testsFolder);
    if (mp_part != no_mp_part)
        config.delim(to_string((int)mp_part));

    copyRequiredFiles();

    // Sleep for a second to avoid clock skew warnings
    // This cummulatively adds about 5 minutes to grade each mp,
    // but with the benefit of avoiding newsgroup posts
    // CHANGED: Judging by previous emails, the time needed for
    // this would vary significantly. not sure about solution
    //	sleep(1);

    output::header("Compiling");
    chdir(gradeFolder);

    // #define MP_PART_NUMBER in proxy
    create_mp_part_file();
    // Change inclusion guards in proxy.h
    exec("sed", "-i", "-e",
         "s/PROXY_H/PROXY_" + lexical_cast<string>(urand()) + "_H/g",
         "./proxy.h");

    // run [Pre-Make Commands] config header
    const vector<string>& processing_commands = config["Pre-Make Commands"];
    for (size_t i = 0; i < processing_commands.size(); i++)
        exec_command(processing_commands[i]);

    string makestr = "/usr/bin/make --warn-undefined-variables -Wfatal-errors";
    if (opts::parallel)
        makestr += " --jobs=4";
    if (!opts::verbose)
        makestr += " --quiet";
    if (opts::optimize)
        makestr += " OPTIMIZE=on";
    if (!config["Make Options"].empty())
        makestr += " " + config["Make Options"][0];
    if (opts::verbose)
    {
        cout << "in " << getcwdstr() << endl;
        cout << makestr << endl;
    }

    // Compile with make
    system(makestr.c_str()); // yes, system is insecure if the user has control
                             // over config.ini. But students don't.
                             // TODO (toole1): Yeah but this leaves us open to
                             // aliasing issues, or forces us to specify make's
                             // path. Ugly either way.
    // TODO exec("make", "--quiet", "--warn-undefined-variables",

    cout << endl << endl;

    // run [Post-Make commands] config header
    const vector<string>& postprocessing_commands =
        config["Post-Make Commands"];
    for (size_t i = 0; i < postprocessing_commands.size(); ++i)
        exec_command(postprocessing_commands[i]);

    // invoke proxy
    vector<string> args;
    if (opts::verbose)
        args.push_back("--verbose");
    if (opts::asan)
        args.push_back("--asan");
    if (!opts::buffer)
        args.push_back("--noredirect");
    if (!opts::detailed)
        args.push_back("--nodetailed");

    int score = 0;
    // TODO (toole1): this causes weird output when scores are like 200
    if (!std::ifstream("./proxy") || (score = exec("./proxy", args)) < 0)
    {
        output::header("Running tests");
        cout << "Could not execute test cases" << endl << endl;
        score = 0;

        cout << endl;
        output::total_score(score, -1);
    }

    return score;
}

void monad::create_mp_part_file()
{
    ofstream mp_part_file("mp_part.h");
    mp_part_file << "#ifndef MONAD_SPLIT_LINE_NUMBER" << endl;
    mp_part_file << "#define MONAD_SPLIT_LINE_NUMBER -1" << endl;
    mp_part_file << "#endif" << endl;
    mp_part_file << "#define MONAD_SPLIT (MONAD_SPLIT_LINE_NUMBER == __LINE__ "
                    "|| MONAD_SPLIT_LINE_NUMBER == -1)" << endl;
    mp_part_file << "#define NO_MP_PART " << static_cast<int>(no_mp_part)
                 << endl;
    mp_part_file << "#define MP_PART_NUMBER ";
    if (mp_part == no_mp_part)
        mp_part_file << "NO_MP_PART";
    else
        mp_part_file << static_cast<int>(mp_part);
    mp_part_file << endl;
    mp_part_file << "#define MP_PART(x) (MP_PART_NUMBER == (x) || "
                    "MP_PART_NUMBER == NO_MP_PART)" << endl;
    mp_part_file.close();
}

void monad::processArgs(int argc, const char* const* argv)
{
    // Create OptionsMap for options and vector for positional arguments:
    OptionsParser options;

    // Add our possible options to our map
    options.addOption("solution", opts::solution);
    options.addOption("newtests", opts::newtests);
    options.addOption("provided", opts::provided);
    options.addOption("clean", opts::clean);
    options.addOption("update", opts::update);
    options.addOption("staff", opts::staff);
    options.addOption("svn", opts::svn);

    options.addOption("optimize", opts::optimize);
    options.addOption("detailed", opts::detailed);
    options.addOption("verbose", opts::verbose);
    options.addOption("buffer", opts::buffer);
    options.addOption("asan", opts::asan);
    options.addOption("parallel", opts::parallel);

    options.addOption("help", opts::help);
    options.addOption("h", opts::help);
    options.addOption("info", opts::info);
    options.addOption("version", opts::info);
    options.addOption("v", opts::info);
    options.addOption("license", opts::license);

    // Add arguments
    string assignment = "";
    options.addArg(assignment);

    // Read in options and arguments
    vector<string> posargs = options.parse(argc, argv);

    // Help
    if (opts::help || toLower(assignment) == "help")
    {
        if (toLower(assignment) == "config")
            printHelpConfig();
        else if (toLower(assignment) == "tests")
            printHelpTests();
        else
            printHelp();
        exit(0);
    }

    // Info
    if (opts::info)
    {
        printInfo();
        exit(0);
    }

    // License
    if (opts::license)
    {
        printLicense();
        exit(0);
    }

    // Clean
    if (toLower(assignment) == "clean")
    {
        system("/bin/rm -rf *_grade/ *_tests/ *_newtests/ *_provided/ "
               "*_solution/");
        exit(0);
    }

    // Check argument list length
    if (assignment == "")
    {
        cout << "Usage: " << argv[0] << " mp1" << endl;
        cout << "Run \'" << argv[0] << " --help\' for more information" << endl;
        exit(0);
    }

    int svnstatus = svn::init(opts::svn);
    if (svnstatus != 0) {
        cerr << "Could not initialize SVN" << endl;
        exit(-1);
    }

    // Find mp/lab name and tests folder
    if (assignment[assignment.length() - 1] == '/')
        assignment = assignment.substr(0, assignment.length() - 1);
    size_t slash_i = assignment.find_last_of('/');
    string assignment_name =
        (slash_i == string::npos) ? assignment : assignment.substr(slash_i + 1);
    string assignment_dir =
        (slash_i == string::npos) ? "" : assignment.substr(0, slash_i);

    // TODO (toole1): All this stuff is ugly!
    vector<string> splitname = tokenize(assignment_name, '.');
    assignment_base = splitname[0];
    if (splitname.size() == 1)
        mp_part = no_mp_part;
    else
        mp_part = lexical_cast<int>(splitname[1].c_str());

    gradeFolder = "./" + assignment_base + "_grade/";
    if (!exists(gradeFolder))
        opts::clean = true;

    if (opts::clean)
        tempFolder = "";
    else
        tempFolder = "./" + assignment_base + "_temp/";

    // Find source folder (i.e. ../mp1)
    // TODO (toole1): I'm not sure if we should really use updateFolder
    // vs getFolder here, and I've forgotten if the original reasoning
    // was deeper than I have the _solution in ../
    if (opts::solution)
        sourceFolder = updateFolder(assignment_base + "_solution/", false);
    else
        sourceFolder = getFolder(assignment_base + '/', assignment_dir, false);

    // tests folder
    if (opts::provided)
        testsFolder = updateFolder(assignment_base + "_provided/", false);
    else if (opts::newtests)
        testsFolder = updateFolder(assignment_base + "_newtests/", false);
    else
        testsFolder = updateFolder(assignment_base + "_tests/", false);
}

void monad::chdirToMonad()
{
    fchdir(monadDirFD);
}

void monad::chdirToCallingDir()
{
    fchdir(callingDirFD);
}

void monad::find_base_dir(const char* argv0)
{
    // First save calling directory for relative paths later
    callingDirFD = open(".", 0);

    EXIT_IF_ERROR(argv0 == NULL);
    size_t argv0len = strlen(argv0);
    char* dir = new char[argv0len + 1];
    strncpy(dir, argv0, argv0len);
    dir[argv0len] = '\0';

    size_t i = argv0len + 1;
    do
    {
        i--;
        if (argv0[i] == '/')
            break;
    } while (i != 0);

    // Change directory
    if (i != 0)
    {
        dir[i] = '\0';
        EXIT_IF_ERROR(chdir(dir));
    }
    delete[] dir;

    if (exists("../.monadid"))
    {
        chdir("../");
        monadSourceFolder = "./source/";
    }
    else
        monadSourceFolder = "./";

    monadDirFD = open(".", 0);

    // Ensure the dir is correct
    if (!exists("./.monadid"))
    {
        cerr << "Could not find monad directory. "
                "Please run ./monad from the directory it is located in."
             << endl;
        exit(-1);
    }
}

void monad::copyRequiredFiles()
{
    // Clear out the temp testing folder
    if (opts::clean)
        forceRemoveDir(gradeFolder);
    else
    {
        forceRemoveDir(tempFolder);
        EXIT_IF_ERROR(rename(gradeFolder.c_str(), tempFolder.c_str()));
    }

    exec("/bin/mkdir", gradeFolder);

    // Copy and link appropriate files - parsed from config.ini
    importFiles(tempFolder, "", gradeFolder, config["Preserved Files"]);
    // We *MUST* copy required files for this part
    importFiles("", sourceFolder, gradeFolder, config["Required Files"]);
    // We *MAY* copy required files for all other parts, but won't error if
    // they do not exist
    importFiles("", sourceFolder, gradeFolder, config("Required Files", ""),
                false);
    importFiles(tempFolder, monadSourceFolder, gradeFolder,
                config["Monad Files"]);
    importFiles(tempFolder, testsFolder, gradeFolder, config["Testing Files"]);

    forceRemoveDir(tempFolder);

    getLibs(config["Libraries"]);
}

void monad::importFiles(const string& preservedFolder,
                        const string& theSourceFolder, const string& destFolder,
                        const vector<string>& files,
                        bool exit_if_err /* = true */)
{
    // 0 for student errors for missing Required Files dir
    int student_error_code = ((preservedFolder == "") ? 0 : -1);

    assertExists(destFolder);
    if (preservedFolder != "")
        assertExists(preservedFolder);
    if (theSourceFolder != "")
        assertExists(theSourceFolder, student_error_code);

    for (size_t i = 0; i < files.size(); i++)
    {
        string preservedFile = preservedFolder + files[i];
        string destFile = destFolder + files[i];

        // Move the file from it's preservation instance
        if (preservedFolder != "" && exists(preservedFile) &&
            (theSourceFolder == "" ||
             (permissions(preservedFile) & S_IWUSR) == 0))
        {
            EXIT_IF_ERROR(rename(preservedFile.c_str(), destFile.c_str()));
        }
        else if (theSourceFolder != "")
        {
            // copy the file from it's source
            string sourceFile = theSourceFolder + files[i];
            if (exit_if_err)
                assertExists(sourceFile, student_error_code);

            // Remove hacky call to exec here...
            string baseCopyDir = getBaseDir(stripTrailingSlash(destFile));
            if (exit_if_err)
            {
                // file *must* be copied
                EXIT_IF_ERROR(exec("mkdir", "-p", baseCopyDir) != 0);
                EXIT_IF_ERROR(exec("cp", "-RL", sourceFile, destFile) != 0,
                              "cp " + sourceFile + " " + destFile + " failed");
            }
            else
            {
                // file *may* be copied
                exec("mkdir", "-p", baseCopyDir);
                // -1 to ignore output
                exec(-1, "cp", "-RL", sourceFile, destFile);
            }
        }
        else
            continue;

        // TODO (toole1) Protect files. This is hacky and should be in util
        // TODO (toole1) this should also be AFTER Pre-Make Commands
        // EXIT_IF_ERROR(chmod(destFile.c_str(),
        //	S_IRUSR | (permissions(destFile) & S_IXUSR)) != 0);
    }
}

void monad::getLibs(const vector<string>& libs)
{
    for (size_t lib_i = 0; lib_i < libs.size(); lib_i++)
    {
        string folder = updateFolder(libs[lib_i], false);
        protectDir(folder);
        chdir(gradeFolder.c_str());
        system(("/bin/cp -r ../" + folder + "* ./").c_str());
        chdir("..");
    }
}

string monad::updateFolder(const string& folder, bool link)
{
    string get = getFolder(folder, "", link);
    if (opts::update)
        svn::update(get);
    return get;
}

string monad::tryGetFolder(string dirAndFolder, const string& target, bool link)
{
    if (dirAndFolder[dirAndFolder.length() - 1] == '/')
        dirAndFolder = dirAndFolder.substr(0, dirAndFolder.size() - 1);

    if (exists(dirAndFolder + "/"))
    {
        if (!link)
            return dirAndFolder + "/";
        EXIT_IF_ERROR(symlink(dirAndFolder.c_str(), target.c_str()) != 0);
        return target + "/";
    }

    return "";
}

string monad::getFolder(const string& folder,
                        const string& searchDir /* = "" */,
                        bool link /* = false */)
{
    // TODO (toole1): All this stuff is ugly!
    string target = "./" + folder;
    string result;

    // Look in the searchDir folder
    if (searchDir != "")
    {
        chdirToCallingDir();
        result = tryGetFolder(searchDir + "/" + folder, target, link);
        if (result != "")
        {
            chdir(result.c_str());
            result = getcwdstr();
        }
        chdirToMonad();
        if (result != "")
            return result;

        // Try SVN
        // Clear out our cache first
        if (exists(target + "/"))
            exec("rm", "-rf", target + "/");

        EXIT_IF_ERROR(config["SVN Root"].size() != 1);
        string svndir = config["SVN Root"][0] + "/" + searchDir + "/" + folder;

        int svnstatus = svn::checkout(svndir);
        if (svnstatus == 0)
            return target + "/";

        cerr << "Error: " << folder << " not found." << endl;
        exit(-1);
    }

    // Look in the parent folder
    result = tryGetFolder("../" + folder, target, link);
    if (result != "")
        return result;

    // Look in the current folder
    if (exists(target + "/"))
        return target + "/";

    // Look two directories up and over - why not? If the parent folder is
    // the target source folder for the mp/lab, then the tests or libs
    // may be two up and over
    // This also covers the case where it is our parent folder
    result = tryGetFolder("../../" + folder, target, link);
    if (result != "")
        return result;

    // Check Subversion

    string svn_config_name;
    string svn_config_subdir;
    if (!opts::staff)
    {
        svn_config_name = "SVN Root";
        svn_config_subdir = "SVN Public Dir";
    }
    else
    {
        svn_config_name = "Staff SVN";
        svn_config_subdir = "Staff SVN Dir";
    }
    if (!config[svn_config_name].empty())
    {
        EXIT_IF_ERROR(config[svn_config_name].size() != 1);
        EXIT_IF_ERROR(config[svn_config_subdir].size() != 1);
        string svndir = config[svn_config_name][0] + "/" +
                        config[svn_config_subdir][0] + "/" + folder;

        // TODO (toole1): Won't work if user needs to type password
        int svnstatus = svn::checkout(svndir);
        if (svnstatus == 0)
            return target + "/";
    }

    cerr << "Error: " << folder << " not found." << endl;
    exit(-1);
}

// Execute a monad or command line command
void monad::exec_command(const string& command)
{
    vector<string> args = tokenize(command, ' ');

    // Allow processing of special internals
    if (args[0] == "rename_main")
    {
        EXIT_IF_ERROR(
            args.size() != 3,
            "rename_main must take 2 arguments: a file and a new name");
        rename_main(args[1], args[2]);
        return;
    }

    if (opts::verbose)
        cout << "Pre-Make Command: " << command << endl;
    system(command.c_str());
}

