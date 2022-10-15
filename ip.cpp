#include "ip.h"

#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <regex>
#include <boost/algorithm/string.hpp>

IP::IP(std::vector<std::string> &address)
{
    fromVector( address );
}

IP::IP(std::string &address) 
{
    auto vecAddress = IP::splitBy( address, '.' );
    fromVector( vecAddress );
}

std::vector<std::string> IP::splitBy(std::string string, char letter)
{
    std::vector<std::string> rv;

    std::string rvStr = "";
    for ( std::string::iterator it = string.begin(); it != string.end(); ++it ) 
    {
        if ( *it == letter ) {
            rv.push_back( rvStr );
            rvStr.clear();
            continue;
        } else {
            rvStr.push_back(*it);
        }
    }
    rv.push_back( rvStr );

    return rv;
}

bool IP::isIP(std::string address)
{
    std::regex expr("^([0-9]{1,3})\\.([0-9]{1,3})\\.([0-9]{1,3})\\.([0-9]{1,3})$");
    return std::regex_match(address, expr);
}

std::string IP::asString()
{
    if ( isValid() ) {
        return m_asString;
    } 
    
    return std::string();
}

bool IP::isValid() const
{
    return IP::isIP( m_asString );
}

bool IP::isFirst(short f) const
{
    if ( isValid() ) {
        return m_first == f;
    }
    
    return false;
}

bool IP::isTwo(short f, short s) const
{
    if ( isValid() ) {
        return m_first == f && m_second == s;
    }

    return false;
}

bool IP::isAny(short a) const
{
    if ( isValid() ) {
        return m_first == a || m_second == a || m_third == a || m_fourth == a;
    }

    return false;
}

bool IP::operator<(const IP &right)
{
    if ( isValid() && right.isValid() ) {
        if ( first() != right.first() ) {
            return first() < right.first();
        } else if ( second() != right.second() ) {
            return second() < right.second();
        } else if ( third() != right.third() ) {
            return third() < right.third();
        } else if ( fourth() != right.fourth() ) {
            return fourth() < right.fourth();
        }
    }

    return false;
}

void IP::convertToInt(std::vector<std::string> &address)
{
    m_first = std::stoi( address[0] );
    m_second = std::stoi( address[1] );
    m_third = std::stoi( address[2] );
    m_fourth = std::stoi( address[3] );
}

void IP::fromVector(std::vector<std::string> &address)
{
    for ( int index = 0; index < address.size(); ++index ) {
        m_asString.append( address[index] );
        if ( index != address.size() - 1 ) {
            m_asString += '.';
        }
    }

    if ( IP::isIP( m_asString ) ) {
        convertToInt( address );
    }
}