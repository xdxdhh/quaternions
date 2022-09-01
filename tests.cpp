/* 
 * build and run using ./helpers/run_tests.sh, this also downloads googletest
 * itself if not already present, the build directory should get created
 * 
 * 
 * for VS Code highlighting and hinting add the following lines
 * 
 * "${workspaceFolder}/include/",
 * "${workspaceFolder}/tests/"
 * 
 * to "includePath": [] in c_cpp_properties.json and install the following extensions
 * - C/C++ Extension Pack
 * - CMake Language Support
 * 
 * reload VS Code, this file should no longer have any include errors in it
 */



#include "quaternion.hpp"


#include "gtest/gtest.h"

#include <vector>


TEST(MathOperators, PrecisionBoundary)
{
    std::cout << "precision_boundary<T>() of different types T:" << std::endl;
    std::cout << "float  " << precision_boundary<float>() << std::endl;
    std::cout << "double " << precision_boundary<double>() << std::endl;
}

TEST(MathNumVector, BasicOperations)
{
    // const auto iteration test 
    num_vector<int, 7> c = {5,5,5,5,5,5,5};
    for(const auto& i : c){
        EXPECT_EQ(i, 5);
    }

    // fill test:
    num_vector<int, 3> x = {0};
    for(const auto& i : x){
        EXPECT_EQ(i, 0);
    }
    x.fill(10);
    num_vector<int, 3> y = {10,10,10};
    EXPECT_TRUE(x == y);

    // +-, unary-, scalar multiply
    num_vector<double, 3> a = {8, 10.5, 10.1};
    num_vector<double, 3> minus_a = {-8, -10.5, -10.1};
    num_vector<double, 3> b = {1, 4, 8};
    num_vector<double, 3> expected_sum = {9, 14.5, 18.1};
    num_vector<double, 3> expected_diff_1 = {7, 6.5, 2.1};
    num_vector<double, 3> expected_diff_2 = {-7, -6.5, -2.1};
    num_vector<double, 3> a_times3 = {24, 31.5, 30.3};

    EXPECT_TRUE(-a == minus_a);
    EXPECT_TRUE(a+b == expected_sum);
    EXPECT_TRUE(a-b == expected_diff_1) << a-b;
    EXPECT_TRUE(b-a == expected_diff_2) << b-a;

    EXPECT_TRUE(a * 3.0 == a_times3);
    EXPECT_TRUE(3.0 * a == a_times3);


}


TEST(MathNumVector, AdvancedOperations) 
{
    //vector norm
    num_vector<int, 3> v1 = {1, 2, 2};
    EXPECT_TRUE(v1.norm() == 3) << v1.norm();
    num_vector<int, 3> v2 = {1, -2, 2};
    num_vector<int, 3> v3 = {-1, -2, -2};
    EXPECT_TRUE(v1.norm() == v2.norm() && v2.norm() == v3.norm());

    //dot product
    num_vector<int, 3> a = {1, 2, 3};
    num_vector<int, 3> b = {4, -5, 6};
    EXPECT_TRUE(dot(a, b) == 12);
    num_vector<int, 3> c= {-4, -9};
    num_vector<int, 3> d = {-1, 2};
    EXPECT_TRUE(dot(c, d) == -14);
    num_vector<int, 3> e = {6, -1, 3};
    num_vector<int, 3> f = {4, 18, -2};
    EXPECT_TRUE(dot(e, f) == 0);


    //cross product
    num_vector<int, 3> x = {-1, 2, 5};
    num_vector<int, 3> y = {4, 0, -3};
    num_vector<int, 3> x_cross_y = {-6, 17, -8 };
    EXPECT_TRUE(cross(x, y) == x_cross_y) << cross(x, y);
    EXPECT_FALSE(cross(x, y) == cross(y, x)); //non-commutative
    num_vector<int, 3> u = {0, 2, 1};
    num_vector<int, 3> v = {3, -1, 0};
    num_vector<int, 3> u_cross_v = {1, 3, -6};
    num_vector<int, 3> v_cross_u = {-1, -3, 6};
    EXPECT_TRUE(cross(u, v) == u_cross_v && cross(v, u) == v_cross_u);

}

TEST(MathNumVector, AngleOperation){
    //angle 2d
    num_vector<double, 2> a = {1, -2};
    num_vector<double, 2> b = {-2, 1};
    EXPECT_TRUE(round(angle(a, b), 2)  == 2.50); 
    EXPECT_EQ(round(angle(a, b), 2), 2.50);
    EXPECT_EQ(std::floor(to_degrees(angle(a, b))), 143); //should be ~143 degrees
    
} 




TEST(MathQuaternion, BasicOperations)
{
    std::vector<double> out;

    //constructor test
    quaternion<double> t(5,10,15.2,55);
    EXPECT_EQ(5, t.real());

    out = {10,15.2,55};
    EXPECT_EQ(t.imag().i, out.at(0));
    EXPECT_EQ(t.imag().j, out.at(1));
    EXPECT_EQ(t.imag().k, out.at(2));

    //comparison test
    quaternion<double> s(5,10,15,55);
    EXPECT_FALSE(s == t);

    //constructor tests
    quaternion<int> u(10, 2, 3, 4);
    num_vector<int, 3> vec = {2, 3, 4};
    EXPECT_EQ(u, quaternion(10, vec)); //value + vector constructor
    num_vector<int, 4> vec2 = {10, 2, 3, 4};
    EXPECT_EQ(u, quaternion(vec2)); //4-length vector constructor
    quaternion<int> w(0, 2, 3, 4);
    EXPECT_EQ(w, quaternion(vec)); //value should be 0 if not given

    //sum test
    auto sum = s + t;
    quaternion<double> expected(10,20,30.2,110);
    EXPECT_EQ(sum,expected); 

    //diff test
    auto diff = s-t;
    quaternion<double> expected_diff(0,0,-0.2,0);
    EXPECT_FLOAT_EQ(diff.q0(), expected_diff.q0());
    EXPECT_FLOAT_EQ(diff.qi(), expected_diff.qi());
    EXPECT_FLOAT_EQ(diff.qj(), expected_diff.qj());
    EXPECT_FLOAT_EQ(diff.qk(), expected_diff.qk());
    EXPECT_TRUE(diff.as_vector() == expected_diff.as_vector());
    EXPECT_TRUE(diff == expected_diff); 

    //scalar multiplication test
    auto x = 3.0 * s;
    auto y = s * 3.0;
    quaternion<double> expected_multiplied(15,30,45,165);
    EXPECT_TRUE(x == expected_multiplied);
    EXPECT_TRUE(y == expected_multiplied); 

}

TEST(MathQuaternion, AdvancedOperations){//norm, normalize, hamiltonian product, inverse, conjugate
    //Hamiltonian products tests:
    quaternion<int> r(1,0,0,0);
    quaternion<int> i(0,1,0,0);
    quaternion<int> j(0,0,1,0);
    quaternion<int> k(0,0,0,1);
    //identities:
    EXPECT_TRUE(r == r*r);
    EXPECT_TRUE(-r == i*i); //i*i = -1
    EXPECT_TRUE(j*j == i*i); //should be -1 aswell
    EXPECT_TRUE(j*j == k*k);

    EXPECT_TRUE(i*j == k);
    EXPECT_TRUE(j*i == -k);
    EXPECT_TRUE(i*j*k == -r);

    //random equations:
    quaternion<int> ones(1,1,1,1);
    EXPECT_TRUE(ones*j == quaternion<int>(-1,-1,1,1));

    quaternion<int> random_q1(1,2,8,-2);
    quaternion<int> random_q2(0,3,2,11);
    quaternion<int> result1(0,95,-26,-9);
    quaternion<int> result2(0,-89,30,31);

    EXPECT_TRUE(random_q1 * random_q2 == result1);
    EXPECT_TRUE(random_q2 * random_q1 == result2);

    //conjugate tests:
    EXPECT_TRUE(i.conjugate() == -i);
    EXPECT_TRUE(ones.conjugate() == quaternion<int>(1,-1,-1,-1));

    //normalize test:
    quaternion<double> non_normalized(20, -4, 2, 5);
    EXPECT_TRUE(non_normalized.norm() != 1);
    auto normalized_q = non_normalized.normalized();
    EXPECT_TRUE(is_equal(normalized_q.norm(),1.0)); 

    //inverse
    auto inversed = non_normalized.inverse();
    quaternion<double> one_double(1.0,0,0,0);
    EXPECT_TRUE(non_normalized * inversed == one_double);

}