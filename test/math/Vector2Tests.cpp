#include <ray/math/Vector2.hpp>
#include <gtest/gtest.h>
#include <type_traits>
#include <typeinfo>
#include <ray/platform/Print.hpp>

using namespace ray;
using namespace math;
using namespace platform;

using f32   = float;
using f64   = double;
using i32   = int32_t;
using u32   = uint32_t;
using vec2  = Vector2<f32>;
using dvec2 = Vector2<f64>;
using ivec2 = Vector2<i32>;
using uvec2 = Vector2<u32>;

#define TEST_VECTOR2_COORDINATES(vec, scalar, coord1, coord2, s1, s2) \
    TEST(vec, coord_##coord1##_IsScalar) { auto v = vec{s1,s2}; EXPECT_EQ(typeid(v.coord1), typeid(scalar)); }\
    TEST(vec, coord_##coord2##_IsScalar) { auto v = vec{s1,s2}; EXPECT_EQ(typeid(v.coord2), typeid(scalar)); }\
    TEST(vec, coord_##coord1##_CanBeAccessed) { auto v = vec{s1,s2}; EXPECT_EQ(v.coord1,s1); }\
    TEST(vec, coord_##coord2##_CanBeAccessed) { auto v = vec{s1,s2}; EXPECT_EQ(v.coord2,s2); }

#define TEST_VECTOR2_STRUCTURE(vec, scalar, s1, s2)\
    TEST(vec, isPod) { EXPECT_TRUE(std::is_pod<vec>::value); }\
    TEST(vec, arePacked) { EXPECT_EQ(sizeof(vec), 2*sizeof(scalar)); }\
    TEST(vec, canBeDefaultConstructed) { vec v; }\
    TEST(vec, canBeConstructedFromOneScalar) { vec v(33); EXPECT_EQ(v.x,scalar(33)); EXPECT_EQ(v.y,scalar(33)); }\
    TEST(vec, canBeConstructedFromMultipleScalar) { vec v(33,34); EXPECT_EQ(v.x,scalar(33)); EXPECT_EQ(v.y,scalar(34)); }\
    TEST_VECTOR2_COORDINATES(vec, scalar, x, y, s1, s2)\
    TEST_VECTOR2_COORDINATES(vec, scalar, u, v, s1, s2)\
    TEST_VECTOR2_COORDINATES(vec, scalar, w, h, s1, s2)

#define TEST_VECTOR2_ONE_BIN_OP(vec, x1, y1, x2, y2, op, opName)\
    TEST(vec, canBe##opName##edTogether) {\
        auto v1 = vec{x1,y1};\
        auto v2 = vec{x2,y2};\
        auto v3 = v1;\
        auto v4 = v1 op v2;\
        v3 op ## = v2;\
        EXPECT_EQ(v3.x, v1.x op v2.x);\
        EXPECT_EQ(v3.y, v1.y op v2.y);\
        EXPECT_EQ(v4.x, v1.x op v2.x);\
        EXPECT_EQ(v4.y, v1.y op v2.y);\
        EXPECT_EQ(typeid(v1 op ivec2{1,1}), typeid(Vector2<decltype(v1.x op 1)>));\
        EXPECT_EQ(typeid(ivec2{1,1} op v1), typeid(Vector2<decltype(1 op v1.x)>));\
        EXPECT_EQ(typeid(v1 op vec2{1.0f,1.0f}), typeid(Vector2<decltype(v1.x op 1.0f)>));\
        EXPECT_EQ(typeid(vec2{1.0f,1.0f} op v1), typeid(Vector2<decltype(1.0f op v1.x)>));\
        EXPECT_EQ(typeid(v1 op uvec2{1u,1u}), typeid(Vector2<decltype(v1.x op 1u)>));\
        EXPECT_EQ(typeid(uvec2{1u,1u} op v1), typeid(Vector2<decltype(1u op v1.x)>));\
        EXPECT_EQ(typeid(v1 op dvec2{1.0,1.0}), typeid(Vector2<decltype(v1.x op 1.0)>));\
        EXPECT_EQ(typeid(dvec2{1.0,1.0} op v1), typeid(Vector2<decltype(1.0 op v1.x)>));\
    }

#define EXPECT_VECTOR2_EQUAL(v1,v2) do {\
    EXPECT_TRUE(v1 == v2);\
    EXPECT_FALSE(v1 != v2);\
    EXPECT_EQ(v1,v2);\
} while(0);

#define EXPECT_VECTOR2_NOT_EQUAL(v1,v2) do {\
    EXPECT_FALSE(v1 == v2);\
    EXPECT_TRUE(v1 != v2);\
    EXPECT_NE(v1,v2);\
} while(0)

#define TEST_VECTOR2_COMPARISON_OP(vec, s1, s2, s3, s4)\
    TEST(vec, canBeCompared)\
    {\
        auto v12=vec{s1,s2}, v12Prime=vec{s1,s2}, v12Double=v12;\
        auto v13=vec{s1,s3}, v32=vec{s3,s2};\
        auto v34=vec{s3,s4};\
        EXPECT_VECTOR2_EQUAL(v12,v12);\
        EXPECT_VECTOR2_EQUAL(v12Prime, v12);\
        EXPECT_VECTOR2_EQUAL(v12, v12Prime);\
        EXPECT_VECTOR2_EQUAL(v12Double, v12);\
        EXPECT_VECTOR2_EQUAL(v12, v12Double);\
        EXPECT_VECTOR2_NOT_EQUAL(v12,v34);\
        EXPECT_VECTOR2_NOT_EQUAL(v34,v12);\
        EXPECT_VECTOR2_NOT_EQUAL(v13,v12);\
        EXPECT_VECTOR2_NOT_EQUAL(v12,v13);\
        EXPECT_VECTOR2_NOT_EQUAL(v12,v32);\
        EXPECT_VECTOR2_NOT_EQUAL(v32,v12);\
    }

#define TEST_VECTOR2_SCALED_BY_SCALAR(vec, s1, s2, f, op, opName)\
    TEST(vec, canBe ## opName ## edByAndFromAScalar)\
    {\
        auto v1 = vec{s1,s1};\
        auto v2 = v1;\
        v2 op##= f;\
        EXPECT_EQ(v2.x,v1.x op f);\
        EXPECT_EQ(v2.y,v1.y op f);\
        auto v3 = v1  op  f;\
        EXPECT_EQ(v3.x,v1.x op f);\
        EXPECT_EQ(v3.y,v1.y op f);\
        auto v4 = f  op  v1;\
        EXPECT_EQ(v4.x,f op v1.x);\
        EXPECT_EQ(v4.y,f op v1.y);\
        EXPECT_EQ(typeid(v1  op  1), typeid(Vector2<decltype(v1.x op 1)>));\
        EXPECT_EQ(typeid(1  op  v1), typeid(Vector2<decltype(1 op v1.x)>));\
        EXPECT_EQ(typeid(v1  op  1u), typeid(Vector2<decltype(v1.x op 1u)>));\
        EXPECT_EQ(typeid(1u  op  v1), typeid(Vector2<decltype(1u op v1.x)>));\
        EXPECT_EQ(typeid(v1  op  1.0f), typeid(Vector2<decltype(v1.x op 1.0f)>));\
        EXPECT_EQ(typeid(1.0f  op  v1), typeid(Vector2<decltype(1.0f op v1.x)>));\
        EXPECT_EQ(typeid(v1  op  1.0), typeid(Vector2<decltype(v1.x op 1.0)>));\
        EXPECT_EQ(typeid(1.0  op  v1), typeid(Vector2<decltype(1.0 op v1.x)>));\
    }       

#define TEST_VECTOR2_NEGATION(vec, s1, s2)\
    TEST(vec, canBeNegated)\
    {\
        auto v1 = vec{s1,s2};\
        auto v2 = -v1;\
        EXPECT_EQ(v2.x, -v1.x);\
        EXPECT_EQ(v2.y, -v1.y);\
    }

#define TEST_VECTOR2_OPERATIONS(vec, x1, y1, x2, y2)\
    TEST_VECTOR2_COMPARISON_OP(vec, x1, y1, x2, y2)\
    TEST_VECTOR2_ONE_BIN_OP(vec, x1, y1, x2, y2, +, Add)\
    TEST_VECTOR2_ONE_BIN_OP(vec, x1, y1, x2, y2, -, Substract)\
    TEST_VECTOR2_ONE_BIN_OP(vec, x1, y1, x2, y2, *, Multiply)\
    TEST_VECTOR2_ONE_BIN_OP(vec, x1, y1, x2, y2, /, Divide)\
    TEST_VECTOR2_SCALED_BY_SCALAR(vec, x1, y1, x2, *, Multiply)\
    TEST_VECTOR2_SCALED_BY_SCALAR(vec, x1, y1, x2, /, Divide)\
    TEST_VECTOR2_NEGATION(vec, x1, y1)\
    TEST(vec, canBeUsedInDotProduct) { EXPECT_EQ(dot(vec{x1,y1},vec{x2,y2}), x1*x2 + y1*y2); }

#define TEST_VECTOR2_CAN_BE_PRINTED(vec, str, s1, s2)\
    TEST(vec, canBePrinted) { EXPECT_EQ(str, fmt("%1%", vec{s1,s2})); }

TEST_VECTOR2_STRUCTURE(vec2, f32, 5.0f, 1.0f)
TEST_VECTOR2_OPERATIONS(vec2, 1.2f, 2.5f, 5.3f, 6.8f)
TEST_VECTOR2_CAN_BE_PRINTED(vec2, "(1.2,3.5)", 1.2f, 3.5f)

TEST_VECTOR2_STRUCTURE(dvec2, double, 5.0, 1.0)
TEST_VECTOR2_OPERATIONS(dvec2, 1.2, 2.5, 5.3, 6.8)
TEST_VECTOR2_CAN_BE_PRINTED(dvec2, "(1.2,3.5)", 1.2, 3.5)

TEST_VECTOR2_STRUCTURE(ivec2, i32, 5, 1)
TEST_VECTOR2_OPERATIONS(ivec2, 10, 25, 53, 58)
TEST_VECTOR2_CAN_BE_PRINTED(ivec2, "(1,2)", 1, 2)

TEST_VECTOR2_STRUCTURE(uvec2, u32, 5u, 10u)
TEST_VECTOR2_OPERATIONS(uvec2, 10u, 25u, 53u, 58u)
TEST_VECTOR2_CAN_BE_PRINTED(uvec2, "(1,2)", 1u, 2u)


TEST(widest, worksOnVector2)
{
    EXPECT_EQ(typeid(widest<ivec2,ivec2>),typeid(ivec2));
    EXPECT_EQ(typeid(widest<ivec2,vec2>),typeid(vec2));
    EXPECT_EQ(typeid(widest<ivec2,dvec2>),typeid(dvec2));
    EXPECT_EQ(typeid(widest<ivec2,uvec2>),typeid(uvec2));

    EXPECT_EQ(typeid(widest<vec2,ivec2>),typeid(vec2));
    EXPECT_EQ(typeid(widest<vec2,vec2>),typeid(vec2));
    EXPECT_EQ(typeid(widest<vec2,dvec2>),typeid(dvec2));
    EXPECT_EQ(typeid(widest<vec2,uvec2>),typeid(vec2));

    EXPECT_EQ(typeid(widest<uvec2,ivec2>),typeid(uvec2));
    EXPECT_EQ(typeid(widest<uvec2,vec2>),typeid(vec2));
    EXPECT_EQ(typeid(widest<uvec2,dvec2>),typeid(dvec2));
    EXPECT_EQ(typeid(widest<uvec2,uvec2>),typeid(uvec2));

    EXPECT_EQ(typeid(widest<dvec2,ivec2>),typeid(dvec2));
    EXPECT_EQ(typeid(widest<dvec2,vec2>),typeid(dvec2));
    EXPECT_EQ(typeid(widest<dvec2,dvec2>),typeid(dvec2));
    EXPECT_EQ(typeid(widest<dvec2,uvec2>),typeid(dvec2));
}
