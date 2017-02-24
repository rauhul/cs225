/**
 * @file filemap.h
 * Definition of a class that wraps parsing of configuration files on disk.
 *
 * @author Jack Toole
 * @author Chase Geigle
 */

#ifndef _FILEMAP_H_
#define _FILEMAP_H_

#include <map>
#include <string>
#include <vector>
#include <istream>

#include "util.h"

namespace util {

    /**
     * Contains options parsed from configuration files.
     */
    class FileMap {
        public:
            typedef std::map< std::string, std::vector<std::string> > InnerMap;
            // section name -> part number -> vector
            typedef std::map< std::string, std::map<std::string,
                    std::vector<std::string> >, ci_less > OuterMap;

            FileMap() : _delim("") { /* nothing */ }

            /**
             * Reads in configuration information from the configuration
             * file present in the given folder or the given file. The info
             * will be merged into the given map, and values present in the
             * map and in the file will take on the most recent values read
             * from the file.
             *
             * @param folder_or_file The folder in which to look for the
             *  configuration file, or the configuration file itself.
             * @return A new FileMap generated from reading a configuration
             *  file.
             */
            void readConfig( const std::string & folder_or_file );

            /**
             * Sets the default delimeter for option extraction.
             * @param marker The new delimeter to set.
             */
            void delim( const std::string & marker );

            /**
             * Extracts all values contained in the given section from the
             * map. The results are restricted to only contain those that
             * match the **CURRENT** delimeter (and those with no
             * delimeter).
             *
             * @param section The section to extract.
             * @return A vector of the strings in that section of the
             *  configuration file.
             */
            std::vector<std::string> operator[]( const std::string & section ) const;

            /**
             * Extracts all values contained in the given section from the
             * map. The results are restricted to only contain those that
             * match the given delimeter (and those with no delimeter).
             *
             * @param section The section to extract.
             * @param delimeter The delimeter to restrict results to.
             * @return A vector of the strings in that section of the
             *  configuration file that have no delimeter or match the given
             *  delimeter.
             */
            std::vector<std::string> operator()( 
                    const std::string & section, 
                    const std::string & delimeter ) const;
        private:
            /**
             * Strips consecutive newlines and carriage returns from a
             * stream.
             *
             * @param file The stream to strip from.
             */
            static void strip_rn( std::istream & file );

            /**
             * The internal map of options.
             */
            OuterMap _map;
            /**
             * The internal map of exclusive options (options that only
             * apply to a specific part, not the general case).
             */
            OuterMap _exclusives;

            /**
             * The default delimiter.
             */
            std::string _delim;
    };
}
#endif
