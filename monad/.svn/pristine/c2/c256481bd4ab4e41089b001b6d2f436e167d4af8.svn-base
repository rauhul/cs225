/**
 * @file filemap.cpp
 * Implementation of a class that wraps parsing of configuration files on
 * disk.
 *
 * @author Jack Toole
 * @author Chase Geigle
 */

#include "filemap.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>

namespace util {
    void FileMap::readConfig( const std::string & folder_or_file ) {
        std::string filename;
        if( folder_or_file == "" || folder_or_file[folder_or_file.length()-1] == '/' )
            filename = folder_or_file + "config.ini";
        else
            filename = folder_or_file;

        std::ifstream infile;
        std::istream * fileptr;
        if( filename == "/dev/stdin" )
            fileptr = &std::cin;
        else {
            fileptr = &infile;
            infile.open( filename.c_str(), std::fstream::in );
        }
        std::istream & file = *fileptr;

        InnerMap * section = &_map[""];
        InnerMap * ex_section = &_exclusives[""];

        strip_rn( file );

        while( file.good() ) {

            // read next line
            std::stringbuf linebuf;
            file.get( linebuf );

            strip_rn( file );

            std::string line = linebuf.str();
            int len = line.length();

            if( len == 0 )
                continue;

            // TODO: why is this needed?
            if( line[len - 1] == '\r' )
                line.replace( --len, 1, "" );

            if( len == 0 || line[0] == ';' ) 
                continue; // skip comments

            // Update section
            if( line[0] == '[' && line[ len - 1 ] == ']' ) {
                std::string label = line.substr(1, len - 2);
                section = &_map[ label ];
                ex_section = &_exclusives[ label ];
                continue;
            } else if( line[0] == '[' || line[ len - 1 ] == ']' ) {
                std::cout << "config.ini: Format error: " << line << std::endl;
                exit(-1);
            }

            size_t delim_pos = line.find_first_of( "?:" );
            if( delim_pos == std::string::npos ) {
                (*section)[""].push_back( line );
            } else {
                std::string part = line.substr( 0, delim_pos );
                char delim_type = line[ delim_pos ];
                line = line.substr( delim_pos + 1, line.size() - delim_pos - 1 );
                if( delim_type == '?' )
                    (*ex_section)[part].push_back( line );
                else
                    (*section)[part].push_back( line );
            }
        }

        if( filename != "/dev/stdin" )
            infile.close();
    }

    void FileMap::strip_rn( std::istream & file ) {
        while( file.good() && ( file.peek() == '\n' || file.peek() == '\r' ) )
            file.get(); // strip out all newlines and carriage returns
    }

    void FileMap::delim( const std::string & marker ) {
        _delim = marker;
    }

    std::vector<std::string> FileMap::operator[]( const std::string & section ) const {
        return (*this)( section, _delim );
    }

    std::vector<std::string> FileMap::operator()( const std::string & section, const std::string & delimeter ) const {
        std::vector<std::string> result;

        // Find the inner map for the correct section
        OuterMap::const_iterator outer_it = _map.find( section );
        if( outer_it == _map.end() )
            return result;

        const InnerMap & inner = outer_it->second;

        if( delimeter == "" ) {
            // If we are the empty delimeter, copy all other values in the
            // inner map, regardless of their delimeter
            for( InnerMap::const_iterator it = inner.begin(); it != inner.end(); ++it ) {
                std::copy( it->second.begin(), it->second.end(), std::back_inserter( result ) );
            }
        } else {
            // Otherwise, append all values that reside in the no-delimiter map...
            InnerMap::const_iterator it = inner.find( "" );
            if( it != inner.end() ) {
                std::copy( it->second.begin(), it->second.end(), std::back_inserter( result ) );
            }

            // ...and append all values that reside in the delimiter map,
            // if any
            it = inner.find( delimeter );
            if( it != inner.end() ) {
                std::copy( it->second.begin(), it->second.end(), std::back_inserter( result ) );
            }

            // ...and finally append any exclusive options for this
            // delimeter
            OuterMap::const_iterator outer_ex = _exclusives.find( section );
            if( outer_ex != _exclusives.end() ) {
                InnerMap::const_iterator ex_it = outer_ex->second.find( delimeter );
                if( ex_it != outer_ex->second.end() ) {
                    std::copy( ex_it->second.begin(), ex_it->second.end(), std::back_inserter( result ) );
                }
            }
        }

        return result;
    }
}
