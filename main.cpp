#include "ip.h"

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

int main(int argc, char const *argv[])
{
    std::vector<IP> ip_pool;

    for(std::string line; std::getline(std::cin, line);)
    {
        std::vector<std::string> v = IP::splitBy( line, '\t' );
        ip_pool.push_back( IP( v.at(0) ) );
    }

    std::sort(ip_pool.begin(), ip_pool.end(), [](IP a, IP b) {
        return !(a < b);
    });

    for ( IP ip : ip_pool ) {
        std::cout << ip.asString() << std::endl;
    }

    for ( IP ip : ip_pool ) {
        if ( ip.isFirst( 1 ) ) {
            std::cout << ip.asString() << std::endl;
        }
    }

    for ( IP ip: ip_pool ) {
        if ( ip.isTwo( 46, 70 ) ) {
            std::cout << ip.asString() << std::endl;
        }
    }

    for ( IP ip : ip_pool ) {
        if ( ip.isAny( 46 ) ) {
            std::cout << ip.asString() << std::endl;
        }
    }
}