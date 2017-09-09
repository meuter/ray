#include <ray/math/Vector3.hpp>
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
using vec3  = Vector3<f32>;
using dvec3 = Vector3<f64>;
using ivec3 = Vector3<i32>;
using uvec3 = Vector3<u32>;

#define TEST_VECTOR3_COORDINATES(vec, scalar, coord1, coord2, coord3, s1, s2, s3) \
    TEST(vec, coord_##coord1##_IsScalar) { EXPECT_EQ(typeid(vec::coord1), typeid(scalar)); }\
    TEST(vec, coord_##coord2##_IsScalar) { EXPECT_EQ(typeid(vec::coord2), typeid(scalar)); }\
    TEST(vec, coord_##coord3##_IsScalar) { EXPECT_EQ(typeid(vec::coord3), typeid(scalar)); }\
    TEST(vec, coord_##coord1##_CanBeAccessed) { auto v = vec{s1,s2,s3}; EXPECT_EQ(v.coord1,s1); }\
    TEST(vec, coord_##coord2##_CanBeAccessed) { auto v = vec{s1,s2,s3}; EXPECT_EQ(v.coord2,s2); }\
    TEST(vec, coord_##coord3##_CanBeAccessed) { auto v = vec{s1,s2,s3}; EXPECT_EQ(v.coord3,s3); }\
    TEST(vec, coord_##coord1##coord2##_CanBeAccessedWithSwizzling) {\
        auto v = vec{s1,s2,s3}; EXPECT_EQ(v.coord1##coord2.x,s1); EXPECT_EQ(v.coord1##coord2.y,s2);\
    }
    

TEST(vec, constructionFromSmallerVector)
{
    auto xy = ivec2{1,2};
    auto u = ivec3{xy,2};
}

#define TEST_VECTOR3_STRUCTURE(vec, scalar, s1, s2, s3)\
    TEST(vec, isPod) { EXPECT_TRUE(std::is_pod<vec>::value); }\
    TEST(vec, arePacked) { EXPECT_EQ(sizeof(vec), 3*sizeof(scalar)); }\
    TEST(vec, canBeDefaultConstructed) { vec v; }\
    TEST(vec, canBeConstructedFromOneScalar) { vec v(33); EXPECT_EQ(v.x,scalar(33)); EXPECT_EQ(v.y,scalar(33)); EXPECT_EQ(v.y,scalar(33)); }\
    TEST(vec, canBeConstructedFromMultipleScalar) { vec v(33,34,35); EXPECT_EQ(v.x,scalar(33)); EXPECT_EQ(v.y,scalar(34)); EXPECT_EQ(v.z,scalar(35));}\
    TEST(vec, canBeConstructedFromVector2AndScalar) { vec v(Vector2<scalar>(33,34),35); EXPECT_EQ(v.x,scalar(33)); EXPECT_EQ(v.y,scalar(34)); EXPECT_EQ(v.z,scalar(35));}\
    TEST(vec, canBeConstructedFromScalarAndVector2) { vec v(33,Vector2<scalar>(34,35)); EXPECT_EQ(v.x,scalar(33)); EXPECT_EQ(v.y,scalar(34)); EXPECT_EQ(v.z,scalar(35));}\
    TEST_VECTOR3_COORDINATES(vec, scalar, x, y, z, s1, s2, s3)\
    TEST_VECTOR3_COORDINATES(vec, scalar, r, g, b, s1, s2, s3)\
    TEST_VECTOR3_COORDINATES(vec, scalar, w, h, d, s1, s2, s3)

#define EXPECT_VECTOR3_EQUAL(v1,v2) do {\
    EXPECT_TRUE(v1 == v2);\
    EXPECT_FALSE(v1 != v2);\
    EXPECT_EQ(v1,v2);\
} while(0);

#define EXPECT_VECTOR3_NOT_EQUAL(v1,v2) do {\
    EXPECT_FALSE(v1 == v2);\
    EXPECT_TRUE(v1 != v2);\
    EXPECT_NE(v1,v2);\
} while(0)

#define TEST_VECTOR3_COMPARISON_OP(vec, s1, s2, s3, s4, s5, s6)\
    TEST(vec, canBeCompared)\
    {\
        auto v123=vec{s1,s2,s3}, v123Prime=vec{s1,s2,s3}, v123Double=v123;\
        auto v423=vec{s4,s2,s3}, v143=vec{s1,s4,s3}, v124=vec{s1,s2,s4};\
        auto v456=vec{s4,s5,s6};\
        EXPECT_VECTOR3_EQUAL(v123,v123);\
        EXPECT_VECTOR3_EQUAL(v123Prime, v123);\
        EXPECT_VECTOR3_EQUAL(v123, v123Prime);\
        EXPECT_VECTOR3_EQUAL(v123Double, v123);\
        EXPECT_VECTOR3_EQUAL(v123, v123Double);\
        EXPECT_VECTOR3_NOT_EQUAL(v123,v456);\
        EXPECT_VECTOR3_NOT_EQUAL(v456,v123);\
        EXPECT_VECTOR3_NOT_EQUAL(v423,v123);\
        EXPECT_VECTOR3_NOT_EQUAL(v123,v423);\
        EXPECT_VECTOR3_NOT_EQUAL(v143,v123);\
        EXPECT_VECTOR3_NOT_EQUAL(v123,v143);\
        EXPECT_VECTOR3_NOT_EQUAL(v124,v123);\
        EXPECT_VECTOR3_NOT_EQUAL(v123,v124);\
    }

#define TEST_VECTOR3_ONE_BIN_OP(vec, x1, y1, z1, x2, y2, z2, op, opName)\
    TEST(vec, canBe##opName##edTogether) {\
        auto v1 = vec{x1,y1,z1};\
        auto v2 = vec{x2,y2,z2};\
        auto v3 = v1;\
        v3 op ## = v2;\
        EXPECT_EQ(v3.x, v1.x op v2.x);\
        EXPECT_EQ(v3.y, v1.y op v2.y);\
        EXPECT_EQ(v3.z, v1.z op v2.z);\
        auto v4 = v1 op v2;\
        EXPECT_EQ(v4.x, v1.x op v2.x);\
        EXPECT_EQ(v4.y, v1.y op v2.y);\
        EXPECT_EQ(v4.z, v1.z op v2.z);\
        EXPECT_EQ(typeid(v1 op ivec3{1,1,1}), typeid(Vector3<decltype(v1.x op 1)>));\
        EXPECT_EQ(typeid(ivec3{1,1,1} op v1), typeid(Vector3<decltype(1 op v1.x)>));\
        EXPECT_EQ(typeid(v1 op vec3{1.0f,1.0f,1.0f}), typeid(Vector3<decltype(v1.x op 1.0f)>));\
        EXPECT_EQ(typeid(vec3{1.0f,1.0f,1.0f} op v1), typeid(Vector3<decltype(1.0f op v1.x)>));\
        EXPECT_EQ(typeid(v1 op uvec3{1u,1u,1u}), typeid(Vector3<decltype(v1.x op 1u)>));\
        EXPECT_EQ(typeid(uvec3{1u,1u,1u} op v1), typeid(Vector3<decltype(1u op v1.x)>));\
        EXPECT_EQ(typeid(v1 op dvec3{1.0,1.0,1.0}), typeid(Vector3<decltype(v1.x op 1.0)>));\
        EXPECT_EQ(typeid(dvec3{1.0,1.0,1.0} op v1), typeid(Vector3<decltype(1.0 op v1.x)>));\
    }
    
#define TEST_VECTOR3_SCALED_BY_SCALAR(vec, s1, s2, s3, f, op, opName)\
    TEST(vec, canBe ## opName ## edByAndFromAScalar)\
    {\
        auto v1 = vec{s1,s1,s3};\
        auto v2 = v1;\
        v2 op##= f;\
        EXPECT_EQ(v2.x,v1.x op f);\
        EXPECT_EQ(v2.y,v1.y op f);\
        EXPECT_EQ(v2.z,v1.z op f);\
        auto v3 = v1  op  f;\
        EXPECT_EQ(v3.x,v1.x op f);\
        EXPECT_EQ(v3.y,v1.y op f);\
        EXPECT_EQ(v3.z,v1.z op f);\
        auto v4 = f  op  v1;\
        EXPECT_EQ(v4.x,f op v1.x);\
        EXPECT_EQ(v4.y,f op v1.y);\
        EXPECT_EQ(v4.z,f op v1.z);\
        EXPECT_EQ(typeid(v1  op  1), typeid(Vector3<decltype(v1.x op 1)>));\
        EXPECT_EQ(typeid(1  op  v1), typeid(Vector3<decltype(1 op v1.x)>));\
        EXPECT_EQ(typeid(v1  op  1u), typeid(Vector3<decltype(v1.x op 1u)>));\
        EXPECT_EQ(typeid(1u  op  v1), typeid(Vector3<decltype(1u op v1.x)>));\
        EXPECT_EQ(typeid(v1  op  1.0f), typeid(Vector3<decltype(v1.x op 1.0f)>));\
        EXPECT_EQ(typeid(1.0f  op  v1), typeid(Vector3<decltype(1.0f op v1.x)>));\
        EXPECT_EQ(typeid(v1  op  1.0), typeid(Vector3<decltype(v1.x op 1.0)>));\
        EXPECT_EQ(typeid(1.0  op  v1), typeid(Vector3<decltype(1.0 op v1.x)>));\
    }    

#define TEST_VECTOR3_NEGATION(vec, s1, s2, s3)\
    TEST(vec, canBeNegated)\
    {\
        auto v1 = vec{s1,s2,s3};\
        auto v2 = -v1;\
        EXPECT_EQ(v2.x, -v1.x);\
        EXPECT_EQ(v2.y, -v1.y);\
        EXPECT_EQ(v2.z, -v1.z);\
    }

#define TEST_VECTOR3_CROSS_PRODUCT(vec)\
    TEST(vec, canBeUsedInCrossProduct)\
    { \
        const auto X=vec(1,0,0), Y=vec(0,1,0), Z=vec(0,0,1);\
        EXPECT_EQ(cross(X,Y),Z);\
    }\

#define TEST_VECTOR3_OPERATIONS(vec, x1, y1, z1, x2, y2, z2)\
    TEST_VECTOR3_COMPARISON_OP(vec, x1, y1, z1, x2, y2, z2)\
    TEST_VECTOR3_ONE_BIN_OP(vec, x1, y1, z1, x2, y2, z2, +, Add)\
    TEST_VECTOR3_ONE_BIN_OP(vec, x1, y1, z1, x2, y2, z2, -, Substract)\
    TEST_VECTOR3_ONE_BIN_OP(vec, x1, y1, z1, x2, y2, z2, *, Multiply)\
    TEST_VECTOR3_ONE_BIN_OP(vec, x1, y1, z1, x2, y2, z2, /, Divide)\
    TEST_VECTOR3_SCALED_BY_SCALAR(vec, x1, y1, z1, x2, *, Multiply)\
    TEST_VECTOR3_SCALED_BY_SCALAR(vec, x1, y1, z1, x2, /, Divide)\
    TEST_VECTOR3_NEGATION(vec, x1, y1, z1)\
    TEST(vec, canBeUsedInDotProduct) { EXPECT_EQ(dot(vec{x1,y1,z1},vec{x2,y2,z2}), x1*x2 + y1*y2 + z1*z2); }\
    TEST_VECTOR3_CROSS_PRODUCT(vec)

#define TEST_VECTOR3_CAN_BE_PRINTED(vec, str, s1, s2, s3)\
    TEST(vec, canBePrinted) { EXPECT_EQ(str, fmt("%1%", vec{s1,s2,s3})); }


TEST_VECTOR3_STRUCTURE(vec3, f32, 5.0f, 1.0f, 7.0f)
TEST_VECTOR3_OPERATIONS(vec3, 1.2f, 2.5f, 0.2f, 5.3f, 6.8f, 1.9f)
TEST_VECTOR3_CAN_BE_PRINTED(vec3, "(1.2,3.5,5.5)", 1.2f, 3.5f, 5.5f)

TEST_VECTOR3_STRUCTURE(dvec3, double, 5.0, 1.0, 7.0)
TEST_VECTOR3_OPERATIONS(dvec3, 1.2, 2.5, 0.2, 5.3, 6.8, 1.9)
TEST_VECTOR3_CAN_BE_PRINTED(dvec3, "(1.2,3.5,5.5)", 1.2, 3.5,5.5)

TEST_VECTOR3_STRUCTURE(ivec3, i32, 5, 1, 7)
TEST_VECTOR3_OPERATIONS(ivec3, 10, 25, 5, 53, 58, 3)
TEST_VECTOR3_CAN_BE_PRINTED(ivec3, "(1,2,3)", 1, 2, 3)

TEST_VECTOR3_STRUCTURE(uvec3, u32, 5u, 10u, 70u)
TEST_VECTOR3_OPERATIONS(uvec3, 10u, 25u, 5u, 53u, 58u, 3u)
TEST_VECTOR3_CAN_BE_PRINTED(uvec3, "(1,2,3)", 1u, 2u, 3u)

TEST(widest, worksOnVector3)
{
    EXPECT_EQ(typeid(widest<ivec3,ivec3>),typeid(ivec3));
    EXPECT_EQ(typeid(widest<ivec3,vec3>),typeid(vec3));
    EXPECT_EQ(typeid(widest<ivec3,dvec3>),typeid(dvec3));
    EXPECT_EQ(typeid(widest<ivec3,uvec3>),typeid(uvec3));

    EXPECT_EQ(typeid(widest<vec3,ivec3>),typeid(vec3));
    EXPECT_EQ(typeid(widest<vec3,vec3>),typeid(vec3));
    EXPECT_EQ(typeid(widest<vec3,dvec3>),typeid(dvec3));
    EXPECT_EQ(typeid(widest<vec3,uvec3>),typeid(vec3));

    EXPECT_EQ(typeid(widest<uvec3,ivec3>),typeid(uvec3));
    EXPECT_EQ(typeid(widest<uvec3,vec3>),typeid(vec3));
    EXPECT_EQ(typeid(widest<uvec3,dvec3>),typeid(dvec3));
    EXPECT_EQ(typeid(widest<uvec3,uvec3>),typeid(uvec3));

    EXPECT_EQ(typeid(widest<dvec3,ivec3>),typeid(dvec3));
    EXPECT_EQ(typeid(widest<dvec3,vec3>),typeid(dvec3));
    EXPECT_EQ(typeid(widest<dvec3,dvec3>),typeid(dvec3));
    EXPECT_EQ(typeid(widest<dvec3,uvec3>),typeid(dvec3));
}
