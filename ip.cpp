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

std::vector<std::string> IP::splitBy(std::string &string, char letter)
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

bool IP::isIP(std::string &address)
{
    std::regex expr("^((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$");
    return std::regex_match(address, expr);
}

std::string IP::asString()
{
    if ( isValid() ) {
        return convertToStr();
    } 
    
    return std::string();
}

bool IP::isValid() const
{
    auto rv = convertToStr();
    return IP::isIP( rv );
}

bool IP::isFirst(short f) const
{
    if ( isValid() ) {
        return first() == f;
    }
    
    return false;
}

bool IP::isTwo(short f, short s) const
{
    if ( isValid() ) {
        return first() == f && second() == s;
    }

    return false;
}

bool IP::isAny(short a) const
{
    if ( isValid() ) {
        return first() == a || second() == a || third() == a || fourth() == a;
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
    for( int index = 0; index < address.size(); ++index ) {
        m_ip[index] = std::stoi( address[index] );
    }
}

std::string IP::convertToStr() const
{
    std::string strAddress = "";
    for ( int index = 0; index < m_ip.size(); ++index ) {
        strAddress.append( std::to_string(m_ip[index]) );
        if ( index != m_ip.size() - 1 ) {
            strAddress += '.';
        }
    }

    return strAddress;
}

void IP::fromVector(std::vector<std::string> &address)
{
    std::string strAddress = "";
    for ( int index = 0; index < address.size(); ++index ) {
        strAddress.append( address[index] );
        if ( index != address.size() - 1 ) {
            strAddress += '.';
        }
    }

    if ( IP::isIP( strAddress ) ) {
        convertToInt( address );
    } else {
        throw std::logic_error( strAddress );
    }
}