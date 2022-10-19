#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>

#include "../ip.h"

BOOST_AUTO_TEST_SUITE(TestIPValidation)

BOOST_AUTO_TEST_CASE(Valid) {
    std::string str = "10.1.3.4";
    IP ip1(str);

    BOOST_REQUIRE( ip1.isValid() );

    str = "255.255.255.255";
    IP ip2(str);

    BOOST_REQUIRE( ip2.isValid() );

    str = "0.0.0.0";
    IP ip3(str);

    BOOST_REQUIRE( ip3.isValid() );
}

BOOST_AUTO_TEST_CASE(InValid) {
    std::vector<std::string> strVec {"10..1.3.4", "255.256.256.255", "0.0.0", "-1.4.5.6", "ab.4.5.6"};

    for ( auto str : strVec ) {
        BOOST_CHECK_THROW ( IP ip( str ) , std::logic_error );
    }
}

BOOST_AUTO_TEST_SUITE_END()