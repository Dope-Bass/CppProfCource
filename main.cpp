#include <iostream>
#include <memory>
#include "reader.cpp"

int main(int argc, char* argv[]) 
{
    int count;

    switch(argc) {
        case 1:
            std::cout << "Not enough arguments" << std::endl;
            return 1;
        case 2: {
            try { count = std::stoi(std::string(argv[1])); }
            catch(std::invalid_argument) 
            {
                std::cout << "Invalid argument" << std::endl;
                return 1;
            };
            break;
        }
        default:
            std::cout << "Too many arguments" << std::endl;
            return 1;
    }

    Reader reader(count);
    CmdLogger lg( &reader );
    CmdPrinter pr( &reader );

    std::string com_name;
    while ( std::cin >> com_name ) {
        reader << CommandPtr( new PrintItselfCommand( com_name ) );
    }

    if ( reader.dynamic == true ) {
        reader.executePool();
    }
}