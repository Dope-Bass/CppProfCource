#include <iostream>
#include <boost/program_options.hpp>
#include <vector>

#include "runner.h"

namespace po = boost::program_options;

void checkParams(po::variables_map &vm, std::string &hash)
{
    for ( auto it = vm.begin(); it != vm.end(); ++it ) {
        auto key = (*it).first;
        if ( key != EXCLUDE_DIR && key != MASK ) {
            if ( !vm.count( key ) ) {
                throw po::required_option( key );
            }

            if ( key == HASH ) {
                if ( vm.count(HASH) && (!((hash == crc32) || (hash == fnv))) ) {
                    throw po::validation_error(po::validation_error::invalid_option_value, HASH);
                }
            }
        }
    }
}

int main(int argc, char* argv[]) 
{
    try {
        bool level;
        unsigned int size;
        unsigned int block_size;
        std::string hash;

        po::options_description desc("Options");
        desc.add_options()
            (boost::str( boost::format("%1%,%2%") % HELP % HELP[0] ).c_str(),              
                "Show options list")
            (boost::str( boost::format("%1%,%2%") % INCLUDE_DIR % INCLUDE_DIR[0] ).c_str(),       
                po::value< std::vector<std::string> >(),
                "Set directory for scan. Current dir by default")
            (boost::str( boost::format("%1%,%2%") % EXCLUDE_DIR % EXCLUDE_DIR[0] ).c_str(),
                po::value< std::vector<std::string> >(),
                "Exclude directory from scan")
            (boost::str( boost::format("%1%,%2%") % LEVEL % LEVEL[0] ).c_str(),
                po::value<bool>(&level)->default_value(0),
                "Level of scanning:\
                \n1 - with nested dirs\
                \n0 - without nested dirs")
            (boost::str( boost::format("%1%,%2%") % SIZE % SIZE[0] ).c_str(),
                po::value<unsigned int>(&size)->default_value(1),
                "Minimal size of file to scan - default > 1 byte")
            (boost::str( boost::format("%1%,%2%") % MASK % MASK[0] ).c_str(),
                po::value< std::vector<std::string> >(),
                "File masks to scan - ex: \"*.txt\"")
            (boost::str( boost::format("%1%,%2%") % BLOCK_SIZE % BLOCK_SIZE[0] ).c_str(),
                po::value<unsigned int>(&block_size)->default_value(5),
                "Size of block using for file reading")
            (boost::str( boost::format("%1%,%2%") % HASH % "H" ).c_str(),
                po::value<std::string>(&hash)->default_value(crc32),
                "Hash algorithm - crc32 or FNV");

        po::variables_map vm;
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);

        if ( vm.count(HELP) ) {
            std::cout << desc << "\n";
        } else {
            checkParams(vm, hash);

            Runner runner(block_size, level, size, hash);

            if ( vm.count(INCLUDE_DIR) ) {
                runner.setIncludeDirs( vm[INCLUDE_DIR].as< std::vector<std::string> >() );
            }
            if ( vm.count(EXCLUDE_DIR) ) {
                runner.setExcludeDirs( vm[EXCLUDE_DIR].as< std::vector<std::string> >() );
            }
            if ( vm.count(MASK) ) {
                runner.setMasks( vm[MASK].as< std::vector<std::string> >() );
            }
            
            runner.run();
        }
    } catch(std::exception &e) {
        std::cerr << e.what() << std::endl;
    }

    return 1;
}