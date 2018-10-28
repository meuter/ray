#include <ray/math/Matrix.hpp>
#include <ray/platform/Print.hpp>
#include <type_traits>
#include <gtest/gtest.h>

using namespace ray::math;
using namespace ray::platform;

using mat4 = Matrix<float,4,4>;
using mat2 = Matrix<float,2,2>;
using imat2 = Matrix<int,2,2>;

TEST(Matrix, isDefaultConstructible)
{
    auto m = Matrix<int,5,2>();
    (void)m;
}

TEST(Matrix, isPOD)
{
    EXPECT_TRUE(std::is_pod<mat4>::value);
}

TEST(Matrix, hasALineAndColumnCount)
{
    Matrix<int,5,2> m;
    
    EXPECT_EQ(5, m.lines());
    EXPECT_EQ(2, m.columns());
}

TEST(Matrix, canBeConstructedFromScalars)
{
    auto m = imat2{
        1, 2,
        3, 4,
    };
    EXPECT_EQ(m.data[0], 1);
    EXPECT_EQ(m.data[1], 2);
    EXPECT_EQ(m.data[2], 3);
    EXPECT_EQ(m.data[3], 4);
}

TEST(Matrix, isPacked)
{
    Matrix<int,5,2> m;
    EXPECT_EQ(sizeof(m), sizeof(int)*5*2);
}

TEST(Matrix, canBeAccessedUsingParenOperator)
{
    Matrix<int,2,3> m {
        1,2,3,
        4,5,6
    };

    EXPECT_EQ(1, m(0,0));
    EXPECT_EQ(2, m(0,1));
    EXPECT_EQ(3, m(0,2));

    EXPECT_EQ(4, m(1,0));
    EXPECT_EQ(5, m(1,1));
    EXPECT_EQ(6, m(1,2));
}

TEST(Matrix, canBeComparedForEquality)
{
    auto a = imat2{ 1,2,3,4 };
    auto b = imat2{ 1,2,3,4 };
    auto c1 = imat2{ 5,2,3,4 };
    auto c2 = imat2{ 1,5,3,4 };
    auto c3 = imat2{ 1,2,5,4 };
    auto c4 = imat2{ 1,2,3,5 };
    auto f = mat2{1.0f,2.0f,3.0f,4.0f};
    
    EXPECT_EQ(a, a);
    EXPECT_EQ(a, b);
    EXPECT_NE(a, c1);
    EXPECT_NE(a, c2);
    EXPECT_NE(a, c3);
    EXPECT_NE(a, c4);
    EXPECT_EQ(f, a);
}


TEST(Matrix, canBeCopyConstructed)
{
    auto a = imat2{1,2,3,4};
    auto b = imat2{a};
    
    EXPECT_EQ(a,b);
    a(1,1) = 5;
    EXPECT_NE(a,b);
}

TEST(Matrix, canBeSummed)
{
    auto a = imat2{ 1,2,3,4 };
    auto b = imat2{ 10,20,30,40 };
    auto c = imat2{ 11,22,33,44 };
    auto d = a + b;
    
    a += b;
    EXPECT_EQ(a, c);
    EXPECT_EQ(d, c);

    auto f = mat2{1.0f,2.0f,3.0f,4.0f};
    auto g = f+b;
    auto h = b+f;
    auto i = f;
    i+=b;

    EXPECT_EQ(typeid(g), typeid(mat2));
    EXPECT_EQ(g, d);
    EXPECT_EQ(d, g);
    EXPECT_EQ(typeid(h), typeid(mat2));
    EXPECT_EQ(h, d);
    EXPECT_EQ(d, h);
    EXPECT_EQ(typeid(i), typeid(mat2));
    EXPECT_EQ(i, d);
    EXPECT_EQ(d, i);
}

TEST(Matrix, canBeSubttracted)
{
    auto a = imat2{ 11,22,33,44 };
    auto b = imat2{ 10,20,30,40 };
    auto c = imat2{ 1,2,3,4 };
    auto d = a - b;
    
    a -= b;
    EXPECT_EQ(a, c);
    EXPECT_EQ(d, c);

    auto f = mat2{11.0f,22.0f,33.0f,44.0f};
    auto g = f-b;
    auto i = f;
    i-=b;

    EXPECT_EQ(typeid(g), typeid(mat2));
    EXPECT_EQ(g, d);
    EXPECT_EQ(d, g);
    EXPECT_EQ(typeid(i), typeid(mat2));
    EXPECT_EQ(i, d);
    EXPECT_EQ(d, i);
}

TEST(Matrix, canBeMultipliedByAScalar)
{
    auto a = imat2{ 1,2,3,4 };
    auto b = imat2{ 10,20,30,40 };
    auto c = 10 * a;
    auto d = a * 10;
    a *= 10;
    auto f = a * 1.0f;
    a *= 1.0f;
    
    EXPECT_EQ(a, b);
    EXPECT_EQ(c, b);
    EXPECT_EQ(d, b);
    EXPECT_EQ(typeid(f), typeid(mat2));
    EXPECT_EQ(f, a);
}

TEST(Matrix, canBeDividedByAScalar)
{
    auto a = imat2{ 10,20,30,40 };
    auto b = imat2{ 1,2,3,4 };
    auto d = a / 10;
    a /= 10;

    EXPECT_EQ(a, b);
    EXPECT_EQ(d, b);
    
    auto f = 10.0f / b;
    EXPECT_EQ(typeid(f), typeid(mat2));
    EXPECT_EQ(f(0,0), 10.0f/1);
    EXPECT_EQ(f(0,1), 10.0f/2);
    EXPECT_EQ(f(1,0), 10.0f/3);
    EXPECT_EQ(f(1,1), 10.0f/4);
}

TEST(Matrix, canBeNegated)
{
    auto a = imat2{ 1,2,3,4 };
    auto b = mat2{ -1.0f,-2.0f,-3.0f,-4.0f };
    EXPECT_EQ(-a, b);
}


TEST(Matrix, canBePrinted)
{
    auto m = imat2{
        1, 2,
        3, 4,
    };
    EXPECT_EQ("1 2 \n3 4 \n", fmt("%1%", m));
}

TEST(matrices, canBeMultipliedTogether)
{
    auto a = Matrix<int, 2, 3>{
        1, 2, 3,
        4, 5, 6,
    };

    auto b = Matrix<int, 3, 2>{
        7,  8 ,
        9,  10,
        11, 12,
    };

    auto expected = Matrix<int, 2,2>{
        58, 64,
        139, 154,
    };

    auto actual = a*b;

    EXPECT_EQ(2, actual.lines());
    EXPECT_EQ(2, actual.columns());
    EXPECT_EQ(expected, actual);

    auto m = imat2{
        1,2,
        3,4
    };
    auto n = m;
    n*=m;

    auto expected2 = imat2{
        7,  10,
        15, 22,
    };

    EXPECT_EQ(expected2, m*m);
    EXPECT_EQ(expected2, n);

}

TEST(matrix, canBeTransposed)
{
    auto m = Matrix<int,2,3>{
        1,2,3,
        4,5,6
    };
    auto orig = m;

    auto expected = Matrix<int,3,2>{
        1,4,
        2,5,
        3,6,
    };

    EXPECT_EQ(expected, transpose(m));
    EXPECT_EQ(orig, m);
}

TEST(SquareMatrix, hasATrace)
{
    auto m = Matrix<int,3,3>{
        1, 2, 5,
        6, 7, 2,
        3, 7, 6
    };

    EXPECT_EQ(1+7+6, trace(m));
}
