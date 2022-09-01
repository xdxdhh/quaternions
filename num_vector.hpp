#ifndef MATH_NUMVECTOR
#define MATH_NUMVECTOR



#include "operators.hpp"


#include <array>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <type_traits>




/* Non-dynamic version of a vector, which can actually perform proper math vector operations.
    Available operations: 
    1. addition(+), substraction(-), equality(==), scalar multiplication(*)
    2. vector norm, dot product, cross product
    3. angle between two vectors
    Access operators for elements: [], at()
    Iterators can be used to iterate num_vector.
*/
template<typename T, std::size_t N>
requires std::is_arithmetic_v<T> //whole concept is not neccesary in our case, requires keyword is enough
struct num_vector{
    std::array<T, N> _elems;

    using value_type        = T;
    using size_type         = std::array<T, N>::size_type;
    using difference_type   = std::array<T, N>::difference_type;
    using reference         = std::array<T, N>::reference;
    using const_reference   = std::array<T, N>::const_reference;
    using pointer           = std::array<T, N>::pointer;
    using const_pointer     = std::array<T, N>::const_pointer;
    using iterator          = std::array<T, N>::iterator;
    using const_iterator    = std::array<T, N>::const_iterator;


    bool operator==(const num_vector<T,N>& rhs) const {//equal 
        for(std::size_t i = 0; i < N; i++){
            if(!is_equal(at(i), rhs.at(i)))
                return false;
        }
        return true;
    } 
    bool operator!=(const num_vector<T,N>& rhs) const {return !(*this == rhs);} //non-equal


    T& operator[](int index) {return _elems.at(index);}
    const T& operator[](int index) const {return _elems.at(index);}
    T& at(int index) {return _elems.at(index);}
    const T& at(int index) const {return _elems.at(index);}

    iterator begin() {return _elems.begin();}
    iterator end() {return _elems.end();}
    const_iterator begin() const {return _elems.begin();}
    const_iterator end() const {return _elems.end();}
    const_iterator cbegin() const {return _elems.cbegin();}
    const_iterator cend() const {return _elems.cend();}

    /*
    Fills the whole vector with given element. 
    */
    void fill(const T to_fill){for(auto& e : _elems) e = to_fill;}

    num_vector operator+(const num_vector<T, N>& other) const { 
        num_vector<T, N> sum;
        for(std::size_t i = 0; i < N; i++){
            sum[i] = at(i) + other.at(i);
        }
        return sum;
    }

    num_vector operator-(const num_vector<T, N>& other) const { 
        num_vector<T, N> diff;
        for(std::size_t i = 0; i < N; i++){
            diff[i] = at(i) - other.at(i);
        }
        return diff;
    }

    num_vector operator-() const {
        num_vector<T, N> additive_inverse;
        for(std::size_t i = 0; i < N; i++){
            additive_inverse[i] = -at(i);
        }
        return additive_inverse;
    }

    /*
    Returns an Euclidian norm of vector in N dimension space. 
    */
    T norm() const { 
        T sum = 0;
        for(const auto& e : _elems)
            sum += e*e;
        return std::sqrt(sum);
    }
};


template<typename X, std::size_t S> 
std::ostream& operator<<(std::ostream& os, const num_vector<X, S>& vec){
    std::cout << "[ " ;
    for(const auto& e : vec){
        std::cout << e << ' ';
    }
    std::cout << ']' << std::endl;
    return os;
}


template<typename X, std::size_t S>
num_vector<X, S> operator*(const num_vector<X, S>& lhs, const X& scalar){
    num_vector<X, S> res;
    std::transform(lhs.begin(), lhs.end(), res.begin(), [&](X elem){return elem * scalar;});
    return res;
}


template<typename X, std::size_t S>
num_vector<X, S> operator*(const X& scalar, const num_vector<X, S>& rhs){
    return rhs*scalar;
}


/*
Returns the Euclidian dot product(inner product) of two vectors.
*/
template<typename X, std::size_t S>
X dot(const num_vector<X, S>& lhs,const num_vector<X, S>& rhs){
    X ret = 0;
    for(std::size_t i = 0; i < S; i++){
        ret += lhs.at(i) * rhs.at(i);
    }
    return ret;
}

/*
Returns cross product of two vectors.
Note: Cross product is defined only for vectors of size three.
*/
template<typename X> 
num_vector<X, 3> cross(const num_vector<X, 3>& lhs,const num_vector<X, 3>& rhs){
    num_vector<X,3> ret;
    ret[0] = lhs[1]*rhs[2] - lhs[2]*rhs[1];
    ret[1] = lhs[2]*rhs[0] - lhs[0]*rhs[2];
    ret[2] = lhs[0]*rhs[1] - lhs[1]*rhs[0];
    return ret;
}

/* 
Returns angle in [0, pi] radians. 
*/
template<typename X, std::size_t S> 
double angle(const num_vector<X, S>& lhs,const num_vector<X, S>& rhs){
    return std::acos(dot(lhs, rhs)/ (lhs.norm() * rhs.norm()));
}




#endif