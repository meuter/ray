#include <ray/math/Scalar.hpp>
#include <gtest/gtest.h>

using namespace ray;
using namespace math;

TEST(widest, works)
{
    static_assert(std::is_same<widest<int,int>, int>::value, "oops");
    static_assert(std::is_same<widest<int,float>, float>::value, "oops");
    static_assert(std::is_same<widest<int,unsigned int>, unsigned int>::value, "oops");
    static_assert(std::is_same<widest<int,double>, double>::value, "oops");

    static_assert(std::is_same<widest<unsigned int,int>, unsigned int>::value, "oops");
    static_assert(std::is_same<widest<unsigned int,float>, float>::value, "oops");
    static_assert(std::is_same<widest<unsigned int,unsigned int>, unsigned int>::value, "oops");
    static_assert(std::is_same<widest<unsigned int,double>, double>::value, "oops");

    static_assert(std::is_same<widest<float,int>, float>::value, "oops");
    static_assert(std::is_same<widest<float,float>, float>::value, "oops");
    static_assert(std::is_same<widest<float,unsigned int>, float>::value, "oops");
    static_assert(std::is_same<widest<float,double>, double>::value, "oops");

    static_assert(std::is_same<widest<double,int>, double>::value, "oops");
    static_assert(std::is_same<widest<double,float>, double>::value, "oops");
    static_assert(std::is_same<widest<double,unsigned int>, double>::value, "oops");
    static_assert(std::is_same<widest<double,double>, double>::value, "oops");
}

TEST(Scalar, isAValidType)
{
    static_assert(std::is_same<Scalar<double>, double>::value, "oops");    
    auto s = Scalar<double>{ 10.0 };
    auto w = Scalar<int>{10};
    auto result = w+s;
    EXPECT_EQ(typeid(result), typeid(widest<decltype(s),decltype(w)>));
}

template<class T, floating_only<T> = 0> int someFunction(T t) { (void)t; return 1; }
template<class T, integral_only<T> = 0> int someFunction(T t) { (void)t; return 2; }                          

TEST(floating_only_and_integral_only, Works)
{
    EXPECT_EQ(1, someFunction(5.0));
    EXPECT_EQ(1, someFunction(5.0f));
    EXPECT_EQ(2, someFunction(10));
    EXPECT_EQ(2, someFunction(10u));
}

TEST(scalar_equal, canBeUsedToCompareTwoIntegralType)
{
    EXPECT_TRUE(scalar_equal(1,1));
    EXPECT_TRUE(scalar_equal(1u,1));
    EXPECT_TRUE(scalar_equal(1u,1l));
}

TEST(scalar_equal, canBeUsedToCompareTwoFloatingPointType)
{
    EXPECT_TRUE(scalar_equal(1.0f,1.0f));
    EXPECT_TRUE(scalar_equal(1.0,1.0));
    EXPECT_TRUE(scalar_equal(1.0f,1.0));    
}


TEST(scalar_equal, canBeUsedToCompareIntegralAndFloatingPointType)
{
    EXPECT_TRUE(scalar_equal(1.0,1l));
    EXPECT_TRUE(scalar_equal(1u,1.0f));
}