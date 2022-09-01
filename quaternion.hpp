#ifndef MATH_QUATERNIONS
#define MATH_QUATERNIONS


#include "operators.hpp"
#include "num_vector.hpp"



/* 
Class meant to represent quaternions.
Constructors:
    1. quaternion(T a = 0, T i = 0, T j = 0, T k = 0)
    2. quaternion(T a, num_vector<T, 3> imag)
    3. quaternion(num_vector<T, 3> imag)
    4. quaternion(num_vector<T, 4> vec)
Available operations: 
    1. addition(+), substraction(-), equality(==), scalar multiplication(*)
    2. dot product, Hamiltonian product/quaternion multiplication(*)
    3. norm(to get norm) + normalized(to get normalized quaternion)
    4. conjugate, inverse
    5. getting real and imaginary parts, getting single coefficients(q0, qi, qj, qk)
To transform quaternion into num_vector<T, 4> use as_vector() function.
*/
template<typename T = double>
class quaternion{
    
private:
    T _a, _i, _j, _k;

public:
    
    /* 
    Struct containing imaginary part of quaternion in order (i,j,k). 
    Can be transformed into num_vector by using as_vector() function.
    */
    struct imag_part{
        T i, j, k;

        num_vector<T, 3> as_vector() const
        {
            return {i, j, k};
        }
    };

    quaternion(T a = 0, T i = 0, T j = 0, T k = 0) : _a(a), _i(i), _j(j), _k(k){}; //konstruktor s 4 hodnotama + defaultni nulovy kvaternion
    quaternion(T a, num_vector<T, 3> imag): _a(a), _i(imag[0]), _j(imag[1]), _k(imag[2]){}; //konstruktor s 1 hodnotou a vektorem delky 3
    quaternion(num_vector<T, 3> imag): _a(0), _i(imag[0]), _j(imag[1]), _k(imag[2]){};//konstruktor s vektorem delky tri -> realna cast assumed 0
    quaternion(num_vector<T, 4> vec): _a(vec[0]), _i(vec[1]), _j(vec[2]), _k(vec[3]){}; //konstruktor s vektorem o delce 4

    //Returns the quaternion as vector in order (real, i, j, k)
    num_vector<T, 4> as_vector() const {return {_a, _i, _j, _k};};

    //Returns the real part of quaternion.
    T real() const {return _a;};

    /*
    Returns the imaginary part of quaternion in form of imag_part struct containing i,j,k attributes.
    Can be returned as num_vector by using .as_vector() function.
    */

    imag_part imag() const {return {qi(),qj(),qk()};};

    /* For quaternion q = q0 + (qi) * i + (qj)* j + (qk)* k return coefficient q0.*/
    const T q0() const {return _a;}
    /* For quaternion q = q0 + (qi) * i + (qj)* j + (qk)* k return coefficient qi.*/
    const T qi() const {return _i;}
    /* For quaternion q = q0 + (qi) * i + (qj)* j + (qk)* k return coefficient qj.*/
    const T qj() const {return _j;}
    /* For quaternion q = q0 + (qi) * i + (qj)* j + (qk)* k return coefficient qk.*/
    const T qk() const {return _k;}

    quaternion<T> (const quaternion<T>& other) = default; 

    quaternion<T> operator+(const quaternion<T>& other){ 
        return quaternion<T>(as_vector() + other.as_vector());
    }

    quaternion<T> operator-(const quaternion<T>& other){ 
        return quaternion<T>(as_vector() - other.as_vector());
    }

    //unary minus
    quaternion<T> operator-(){
        return quaternion<T>(-_a,-_i,-_j,-_k);
    }

    /* Returns a norm of quaternion(An Euclidian norm in 4D space). */
    T norm() const {
        return as_vector().norm();
    }

    /*Returns normalized quaternion(with norm equal to one) called versor. */
    quaternion<T> normalized(){
        auto norm_counted = norm();
        return (*this)*(1/norm_counted);
    }

    /*
    For quaternion (q0, qi, qj, qk) return its conjugate quaternion by reversing the sign of the imaginary part (q0, -qi, -qj, -qk).
    */
    quaternion<T> conjugate() const{
        return quaternion<T>(q0(), -qi(), -qj(), -qk());
    }

    /*
    Returns the inverse of quaternion q.
    */
    quaternion<T> inverse() const {
        T squared_sum = q0()*q0() + qi()*qi() + qj()*qj() + qk()*qk();
        //squared sum of quaternion elements can be written as q times q conjugated: q0()*q0() + qi()*qi() + qj()*qj() + qk()*qk()
        //T squared_sum = (*this)*(this->conjugate());
        return conjugate()*(1/squared_sum);
    }


};

template<typename X> 
std::ostream& operator<<(std::ostream& os, const quaternion<X>& q){
    os << "["<< q.q0() << " , " << q.qi() << "i, " << q.qj() << "j, " << q.qk() << "k] " << std::endl;
    return os;
}

template<typename X>
bool operator==(const quaternion<X>& lhs, const quaternion<X>& rhs) {
    return (is_equal(lhs.q0(),rhs.q0()) && is_equal(lhs.qi(), rhs.qi()) && is_equal(lhs.qj(), rhs.qj()) && is_equal(lhs.qk(), rhs.qk()));
}


template<typename X>
quaternion<X> operator*(const quaternion<X>& lhs, const X& scalar){
    return quaternion<X>(lhs.as_vector() * scalar);
}

template<typename X>
quaternion<X> operator*(const X& scalar, const quaternion<X>& rhs){
    return rhs * scalar;
}

/*
Returns the Euclidian dot product(inner product) of two quaternions.
*/
template<typename X>
quaternion<X> dot(const quaternion<X>& lhs, const quaternion<X>& rhs){
    return quaternion<X>(dot(lhs.as_vector(), rhs.as_vector()));
}


/*
Returns Hamilton product of two quaternions (q0_1, qi_1, qj_1, qk_1)*(q0_2, qi_2, qj_2, qk_2).
The product of two rotation quaternions q1 and q2 (q1*q2) will be equivalent to rotation by q2 followed by rotation by q1.
*/
template<typename X>
quaternion<X> operator*(const quaternion<X>& lhs, const quaternion<X>& rhs){
    //use quaternion contructor with one number and one vector 
    X real_part = lhs.real()*rhs.real()- dot(lhs.imag().as_vector(), rhs.imag().as_vector());
    num_vector<X,3> imag_part = lhs.real()*rhs.imag().as_vector() + rhs.real()*lhs.imag().as_vector() + cross(lhs.imag().as_vector(), rhs.imag().as_vector());
    return quaternion<X>(real_part, imag_part);
}


#endif