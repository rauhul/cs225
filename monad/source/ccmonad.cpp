#include "util.h"

using namespace std;
using namespace util;

void partial_compilation( const string & compiler, 
        const vector<string> & options, const string & inputFile, 
        const string & outputFile ) {

    vector<string> args1 = options;
    args1.push_back("-c");
    args1.push_back(inputFile);
    args1.push_back("-o");
    args1.push_back(outputFile);
    int8_t result1 = exec(compiler, args1);
    if (result1 != 0)
    {
        vector<size_t> splits;
        splits.push_back(0); // for compiling something always (should be empty)

        ifstream input(inputFile.c_str());
        for (size_t line_i = 1; input.good(); line_i++)
        {
            string line;
            getline(input, line);
            vector<string> tokens = tokenize(line, " \t");
            if (tokens.size() >= 2 && tokens[0] == "#if" && tokens[1] == "MONAD_SPLIT")
                splits.push_back(line_i);
        }

        vector<string> intermediate_outputs; // = options;
        //intermediate_outputs.push_back("-shared");
        intermediate_outputs.push_back("-r");
        for (size_t split_i = 0; split_i < splits.size(); split_i++)
        {
            vector<string> args = options;
            args.push_back("-c");
            args.push_back(inputFile);
            args.push_back("-DMONAD_SPLIT_LINE_NUMBER=" + lexical_cast<string>(splits[split_i]));
            args.push_back("-o");
            args.push_back(outputFile + "." + split_i);
        
#if 0
            cout << "\tmonacc: " << compiler;
            for (size_t j = 0; j < args.size(); j++)
                cout << ' ' << args[j];
            cout << endl;
#endif

            int8_t result2 = exec(compiler, args);
            if (result2 == 0)
                intermediate_outputs.push_back(outputFile + "." + split_i);
        }
        intermediate_outputs.push_back("-o");
        intermediate_outputs.push_back(outputFile);

        exec("ld", intermediate_outputs);
        //return exec("ld", intermediate_outputs);
        //return exec("mv", intermediate_outputs[2], outputFile);
    }
}

int main(int argc, const char * const * argv)
{
    bool isCompileOnly = false;

    for (int i = 2; i < argc; i++)
        if (strcmp(argv[i], "-c") == 0)
            isCompileOnly = true;

    if (!isCompileOnly) // not really using cc-monad, just linking (maybe)
    {
        
        if (argc < 3)
        {
            cerr << "cc-monad: usage error" << endl;
            exit(1);
        }
        vector<string> linkargs(argv + 2, argv + argc);
        int8_t link_result = exec(argv[1], linkargs);
        
        if( link_result != 0 ) {
            // we failed to link
            
            // determine if we had an implied object.
            // happens with weird stuff like:
            // g++ testimage.cpp image.o png.o -o testimage
            vector<string> possibleInputs;
            vector<string> linkingArgs;
            for( size_t i = 0; i < linkargs.size(); ++i ) {
                if( linkargs[i].find_last_of(".") != string::npos ) {
                    string file = linkargs[i];
                    string extension = file.substr( file.find_last_of("."), file.length() );
                    if( extension == ".cpp" ) {
                        possibleInputs.push_back( file.substr(0, file.find_last_of(".") ) );
                        continue;
                    }
                }
                linkingArgs.push_back( linkargs[i] );
            }
            if( possibleInputs.size() > 0 ) {
                for( size_t i = 0; i < possibleInputs.size(); ++i ) {
                    partial_compilation( argv[1], vector<string>(), possibleInputs[i] + ".cpp", possibleInputs[i] + ".o" );
                    linkingArgs.push_back( possibleInputs[i] + ".o" );
                }
                link_result = exec( argv[1], linkingArgs );
            }
        }
        return link_result;
    }

    if (argc < 4)
    {
        cerr << "cc-monad: usage error" << endl;
        exit(1);
    }
    string compiler = argv[1];
    vector<string> inputFiles;
    vector<string> options;
    string outputFile;
    bool isNextOutput = false;

    for (int32_t i = 2; i < argc; i++)
    {
        if (strlen(argv[i]) > 0)
        {
            if (argv[i][0] == '-')
            {
                if (strlen(argv[i]) > 1 && argv[i][1] == 'o')
                    isNextOutput = true;
                else if (strcmp(argv[i], "-c") != 0)
                    options.push_back(argv[i]);
            }
            else
            {
                if (isNextOutput) 
                {
                    outputFile = argv[i];
                    isNextOutput = false;
                }
                else
                {
                    inputFiles.push_back( argv[i] );
                }
            }
        }
    }

    if (outputFile != "" && inputFiles.size() != 1) {
        cerr << "cc-monad: error: cc-monad can only take one input file when given an explicit output file" << endl;
        exit(1);
    }

    for (size_t i = 0; i < inputFiles.size(); ++i )
    {
        string inputFile = inputFiles[i];
        if (outputFile == "")
        {
            size_t dot = inputFile.find_last_of(".");
            string extension = inputFile.substr(dot, inputFile.length());
            string name = inputFile.substr(0, dot);
            if (extension == ".h" || extension == ".hpp")
                outputFile = name + extension + ".gch";
            else
                outputFile = name + ".o";
        }
        if (inputFile == "")
        {
            cerr << "cc-monad: usage error" << endl;
            continue;
        }
#if 0
        cout << "Compiler: " << compiler << endl;
        cout << "Input: " << inputFile << endl;
        cout << "Output: " << outputFile << endl;
#endif

        if (!exists(inputFile))
        {
            cerr << "cc-monad: error: file not found: " << inputFile << endl;
            continue;
        }

        partial_compilation( compiler, options, inputFile, outputFile );

        // reset the output file if we are running multiple times: this
        // case only happens when there are multiple implied output files,
        // so resetting the output file name is both safe and correct
        outputFile = "";
    }
    return 0;
}
