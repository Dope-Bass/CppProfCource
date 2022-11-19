#include <iostream>
#include <vector>
#include <list>
#include <tuple>
#include <type_traits>

namespace ns_integral {

    // Template specification for integers
    template <typename T>
    using IsIntegral = std::enable_if_t<std::is_integral<T>::value, bool>;

    // Conversion of integer into array of bytes
    template <typename T, IsIntegral< T > = true>
    std::vector<unsigned char> intToBytes(T param)
    {
        std::vector<unsigned char> byteArray( sizeof(T) );
        for (int i = 0; i < sizeof(T); i++)
            byteArray[(sizeof(T) - 1)  - i] = (param >> (i * 8));
        return byteArray;
    }

}

namespace ns_string {

    // Template specification for strings
    template <typename T>
    using IsString = std::enable_if_t<std::is_same<T, std::string>::value, bool>;

}

namespace ns_container {

    template <typename T>           struct is_stl_container                         : std::false_type{};
    template <typename... Args>     struct is_stl_container<std::vector  <Args...>> : std::true_type{};
    template <typename... Args>     struct is_stl_container<std::list    <Args...>> : std::true_type{};

    // Template specification for vector and list containers
    template <typename T>
    using IsContainerSpec = std::enable_if_t<is_stl_container<std::decay_t<T>>::value, bool>;

    // Print container separating elements by dots
    template <typename T, IsContainerSpec< T > = true>
    void printDots(T &&container) 
    {
        for ( auto it = container.begin(); it != container.end(); ++it ) {
            if ( it != ( std::next(container.begin(), container.size() - 1) ) ) {
                std::cout << int(*it) << ".";
                continue;
            }

            std::cout << int(*it) << std::endl;
        }
    }

}

namespace ns_tuple {

    template <class T, class... Ts> struct are_same : std::conjunction<std::is_same<T, Ts>...> {};

    // Template specification for tuple containing elements of same type
    template <typename... Args>
    using IsSameType = std::enable_if_t<are_same<Args...>::value, bool>;

    template<std::size_t Index, typename... Ts>
    using Default = typename std::enable_if<Index < sizeof...(Ts), void>::type;

    template<std::size_t Index, typename... Ts>
    using Tail = typename std::enable_if<Index == sizeof...(Ts), void>::type;

    template<std::size_t I = 0, typename... Ts>
    Tail<I, Ts...> print_tuple(std::tuple<Ts...>& t) { std::cout << std::endl; }

    // Printing tuple
    template<std::size_t I = 0, typename... Ts>
    Default<I, Ts...> print_tuple(std::tuple<Ts...>& t)
    {
        std::cout << std::get<I>(t);

        if ( I < sizeof...(Ts) - 1 ) {
            std::cout << ".";
        }
        
        print_tuple<I + 1, Ts...>(t);
    }

}

template <typename T, ns_integral::IsIntegral< T > = true>
void print_ip(T ip)
{
    ns_container::printDots( ns_integral::intToBytes(ip) );
}

template <typename T, ns_string::IsString< T > = true>
void print_ip(T ip)
{
    std::cout << ip << std::endl;
}

template <typename T, ns_container::IsContainerSpec< T > = true>
void print_ip(T ip)
{
    ns_container::printDots( ip );
}

template <typename... Args, ns_tuple::IsSameType< Args... > = true>
void print_ip(std::tuple<Args...> ip)
{
    ns_tuple::print_tuple( ip );
}