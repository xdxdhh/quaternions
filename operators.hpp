#ifndef MATH_OPERATORS
#define MATH_OPERATORS


#include <limits>
#include <cmath>
#include <iostream>



template<typename T>
inline const T precision_boundary()
{
    return std::numeric_limits<T>::epsilon() * 100.0;
}

template<typename T>
requires std::is_trivial_v<T> 
inline bool is_equal(const T lhs, const T rhs, 
        T epsilon = precision_boundary<T>())
{
    if constexpr(std::numeric_limits<T>::is_exact)
        return lhs == rhs;
    else
        return lhs <= rhs + epsilon && rhs <= lhs + epsilon;
}

template<typename T>
inline const T round(const T num, const unsigned int number_of_decimals){
    auto powered = std::pow(10, number_of_decimals); 
    return std::ceil(num * powered) / (powered); 
}

#endif