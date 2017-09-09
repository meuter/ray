#include <ray/math/Vector4.hpp>
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
using vec4  = Vector4<f32>;
using dvec4 = Vector4<f64>;
using ivec4 = Vector4<i32>;
using uvec4 = Vector4<u32>;

#define TEST_VECTOR4_COORDINATES_SWIZZLE2(vec, coord1, coord2, s1, s2, s3, s4, e1, e2)\
    TEST(vec, coord_##coord1##coord2##_CanBeAccessedWithSwizzlingByGroupsOf2) {\
        auto v = vec{s1,s2,s3,s4};\
        EXPECT_EQ(v.coord1##coord2.x,e1);\
        EXPECT_EQ(v.coord1##coord2.y,e2);\
    }

#define TEST_VECTOR4_COORDINATES_SWIZZLE3(vec, coord1, coord2, coord3, s1, s2, s3, s4, e1, e2, e3)\
    TEST(vec, coord_##coord1##coord2##_CanBeAccessedWithSwizzlingByGroupsOf3) {\
        auto v = vec{s1,s2,s3,s4};\
        EXPECT_EQ(v.coord1##coord2##coord3.x,e1);\
        EXPECT_EQ(v.coord1##coord2##coord3.y,e2);\
        EXPECT_EQ(v.coord1##coord2##coord3.z,e3);\
    }

#define TEST_VECTOR4_COORDINATES(vec, scalar, coord1, coord2, coord3, coord4, s1, s2, s3, s4) \
    TEST(vec, coord_##coord1##_IsScalar) { EXPECT_EQ(typeid(vec::coord1), typeid(scalar)); }\
    TEST(vec, coord_##coord2##_IsScalar) { EXPECT_EQ(typeid(vec::coord2), typeid(scalar)); }\
    TEST(vec, coord_##coord3##_IsScalar) { EXPECT_EQ(typeid(vec::coord3), typeid(scalar)); }\
    TEST(vec, coord_##coord4##_IsScalar) { EXPECT_EQ(typeid(vec::coord4), typeid(scalar)); }\
    TEST(vec, coord_##coord1##_CanBeAccessed) { auto v = vec{s1,s2,s3,s4}; EXPECT_EQ(v.coord1,s1); }\
    TEST(vec, coord_##coord2##_CanBeAccessed) { auto v = vec{s1,s2,s3,s4}; EXPECT_EQ(v.coord2,s2); }\
    TEST(vec, coord_##coord3##_CanBeAccessed) { auto v = vec{s1,s2,s3,s4}; EXPECT_EQ(v.coord3,s3); }\
    TEST(vec, coord_##coord4##_CanBeAccessed) { auto v = vec{s1,s2,s3,s4}; EXPECT_EQ(v.coord4,s4); }\
    TEST_VECTOR4_COORDINATES_SWIZZLE2(vec, coord1, coord2, s1, s2, s3, s4, s1, s2)\
    TEST_VECTOR4_COORDINATES_SWIZZLE2(vec, coord2, coord3, s1, s2, s3, s4, s2, s3)\
    TEST_VECTOR4_COORDINATES_SWIZZLE2(vec, coord3, coord4, s1, s2, s3, s4, s3, s4)\
    TEST_VECTOR4_COORDINATES_SWIZZLE3(vec, coord1, coord2, coord3, s1, s2, s3, s4, s1, s2, s3)\
    TEST_VECTOR4_COORDINATES_SWIZZLE3(vec, coord2, coord3, coord4, s1, s2, s3, s4, s2, s3, s4)\
    
#define TEST_VECTOR4_STRUCTURE(vec, scalar, s1, s2, s3,s4)\
     TEST(vec, isPod) { EXPECT_TRUE(std::is_pod<vec>::value); }\
     TEST(vec, arePacked) { EXPECT_EQ(sizeof(vec), 4*sizeof(scalar)); }\
     TEST(vec, canBeDefaultConstructed) { vec v; }\
     TEST(vec, canBeConstructedFromOneScalar) {\
        vec v(33); EXPECT_EQ(v.x,scalar(33)); EXPECT_EQ(v.y,scalar(33)); EXPECT_EQ(v.y,scalar(33));EXPECT_EQ(v.w,scalar(33)); }\
     TEST(vec, canBeConstructedFromMultipleScalar) {\
        vec v(33,34,35,36); EXPECT_EQ(v.x,scalar(33)); EXPECT_EQ(v.y,scalar(34)); EXPECT_EQ(v.z,scalar(35));EXPECT_EQ(v.w,scalar(36));}\
     TEST(vec, canBeConstructedFrom1xVector2And2xScalar) {\
        vec v(Vector2<scalar>{33,34},35,36); EXPECT_EQ(v.x,scalar(33)); EXPECT_EQ(v.y,scalar(34)); EXPECT_EQ(v.z,scalar(35));EXPECT_EQ(v.w,scalar(36));\
        vec u(33,Vector2<scalar>{34,35},36); EXPECT_EQ(u.x,scalar(33)); EXPECT_EQ(u.y,scalar(34)); EXPECT_EQ(u.z,scalar(35));EXPECT_EQ(u.w,scalar(36));\
        vec w(33,34,Vector2<scalar>{35,36}); EXPECT_EQ(w.x,scalar(33)); EXPECT_EQ(w.y,scalar(34)); EXPECT_EQ(w.z,scalar(35));EXPECT_EQ(w.w,scalar(36));\
     }\
     TEST(vec, canBeConstructedFrom2xVector2) {\
        vec v(Vector2<scalar>{33,34},Vector2<scalar>{35,36}); EXPECT_EQ(v.x,scalar(33)); EXPECT_EQ(v.y,scalar(34)); EXPECT_EQ(v.z,scalar(35));EXPECT_EQ(v.w,scalar(36));\
    }\
     TEST(vec, canBeConstructedFrom1xVector3And1xScalar) {\
        vec v(Vector3<scalar>{33,34,35},36); EXPECT_EQ(v.x,scalar(33)); EXPECT_EQ(v.y,scalar(34)); EXPECT_EQ(v.z,scalar(35));EXPECT_EQ(v.w,scalar(36));\
        vec u(33,Vector3<scalar>{34,35,36}); EXPECT_EQ(u.x,scalar(33)); EXPECT_EQ(u.y,scalar(34)); EXPECT_EQ(u.z,scalar(35));EXPECT_EQ(u.w,scalar(36));\
     }\
     TEST(vec, canBeConstructedFrom1xVector4And) {\
        vec v(Vector4<scalar>{33,34,35,36}); EXPECT_EQ(v.x,scalar(33)); EXPECT_EQ(v.y,scalar(34)); EXPECT_EQ(v.z,scalar(35));EXPECT_EQ(v.w,scalar(36));\
     }\
     TEST_VECTOR4_COORDINATES(vec, scalar, x, y, z, w, s1, s2, s3, s4)\
     TEST_VECTOR4_COORDINATES(vec, scalar, r, g, b, a, s1, s2, s3, s4)

#define EXPECT_VECTOR4_EQUAL(v1,v2) do {\
    EXPECT_TRUE(v1 == v2);\
    EXPECT_FALSE(v1 != v2);\
    EXPECT_EQ(v1,v2);\
} while(0);

#define EXPECT_VECTOR4_NOT_EQUAL(v1,v2) do {\
    EXPECT_FALSE(v1 == v2);\
    EXPECT_TRUE(v1 != v2);\
    EXPECT_NE(v1,v2);\
} while(0)

#define TEST_VECTOR4_COMPARISON_OP(vec, s1, s2, s3, s4, s5, s6)\
    TEST(vec, canBeCompared)\
    {\
        auto v1234=vec{s1,s2,s3,s4}, v1234Prime=vec{s1,s2,s3,s4}, v1234Double=v1234;\
        auto v5234=vec{s5,s2,s3,s4}, v1534=vec{s1,s5,s3,s4}, v1235=vec{s1,s2,s3,s5}, v1254=vec{s1,s2,s5,s4};\
        auto v3456=vec{s3,s4,s5,s6};\
        EXPECT_VECTOR4_EQUAL(v1234,v1234);\
        EXPECT_VECTOR4_EQUAL(v1234Prime, v1234);\
        EXPECT_VECTOR4_EQUAL(v1234, v1234Prime);\
        EXPECT_VECTOR4_EQUAL(v1234Double, v1234);\
        EXPECT_VECTOR4_EQUAL(v1234, v1234Double);\
        EXPECT_VECTOR4_NOT_EQUAL(v1234,v3456);\
        EXPECT_VECTOR4_NOT_EQUAL(v3456,v1234);\
        EXPECT_VECTOR4_NOT_EQUAL(v5234,v1234);\
        EXPECT_VECTOR4_NOT_EQUAL(v1234,v5234);\
        EXPECT_VECTOR4_NOT_EQUAL(v1534,v1234);\
        EXPECT_VECTOR4_NOT_EQUAL(v1234,v1534);\
        EXPECT_VECTOR4_NOT_EQUAL(v1254,v1234);\
        EXPECT_VECTOR4_NOT_EQUAL(v1234,v1254);\
        EXPECT_VECTOR4_NOT_EQUAL(v1235,v1234);\
        EXPECT_VECTOR4_NOT_EQUAL(v1234,v1235);\
    }

#define TEST_VECTOR4_ONE_BIN_OP(vec, x1, y1, z1, w1, x2, y2, z2, w2, op, opName)\
    TEST(vec, canBe##opName##edTogether) {\
        auto v1 = vec{x1,y1,z1,w1};\
        auto v2 = vec{x2,y2,z2,w2};\
        auto v3 = v1;\
        v3 op ## = v2;\
        EXPECT_EQ(v3.x, v1.x op v2.x);\
        EXPECT_EQ(v3.y, v1.y op v2.y);\
        EXPECT_EQ(v3.z, v1.z op v2.z);\
        EXPECT_EQ(v3.w, v1.w op v2.w);\
        auto v4 = v1 op v2;\
        EXPECT_EQ(v4.x, v1.x op v2.x);\
        EXPECT_EQ(v4.y, v1.y op v2.y);\
        EXPECT_EQ(v4.z, v1.z op v2.z);\
        EXPECT_EQ(v4.w, v1.w op v2.w);\
        EXPECT_EQ(typeid(v1 op ivec4{1,1,1,1}), typeid(Vector4<decltype(v1.x op 1)>));\
        EXPECT_EQ(typeid(ivec4{1,1,1,1} op v1), typeid(Vector4<decltype(1 op v1.x)>));\
        EXPECT_EQ(typeid(v1 op vec4{1.0f,1.0f,1.0f,1.0f}), typeid(Vector4<decltype(v1.x op 1.0f)>));\
        EXPECT_EQ(typeid(vec4{1.0f,1.0f,1.0f,1.0f} op v1), typeid(Vector4<decltype(1.0f op v1.x)>));\
        EXPECT_EQ(typeid(v1 op uvec4{1u,1u,1u,1u}), typeid(Vector4<decltype(v1.x op 1u)>));\
        EXPECT_EQ(typeid(uvec4{1u,1u,1u,1u} op v1), typeid(Vector4<decltype(1u op v1.x)>));\
        EXPECT_EQ(typeid(v1 op dvec4{1.0,1.0,1.0,1.0}), typeid(Vector4<decltype(v1.x op 1.0)>));\
        EXPECT_EQ(typeid(dvec4{1.0,1.0,1.0,1.0} op v1), typeid(Vector4<decltype(1.0 op v1.x)>));\
    }
    
#define TEST_VECTOR4_SCALED_BY_SCALAR(vec, s1, s2, s3, s4, f, op, opName)\
    TEST(vec, canBe ## opName ## edByAndFromAScalar)\
    {\
        auto v1 = vec{s1,s1,s3,s4};\
        auto v2 = v1;\
        v2 op##= f;\
        EXPECT_EQ(v2.x,v1.x op f);\
        EXPECT_EQ(v2.y,v1.y op f);\
        EXPECT_EQ(v2.z,v1.z op f);\
        EXPECT_EQ(v2.w,v1.w op f);\
        auto v3 = v1  op  f;\
        EXPECT_EQ(v3.x,v1.x op f);\
        EXPECT_EQ(v3.y,v1.y op f);\
        EXPECT_EQ(v3.z,v1.z op f);\
        EXPECT_EQ(v3.w,v1.w op f);\
        auto v4 = f  op  v1;\
        EXPECT_EQ(v4.x,f op v1.x);\
        EXPECT_EQ(v4.y,f op v1.y);\
        EXPECT_EQ(v4.z,f op v1.z);\
        EXPECT_EQ(v4.w,f op v1.w);\
        EXPECT_EQ(typeid(v1  op  1), typeid(Vector4<decltype(v1.x op 1)>));\
        EXPECT_EQ(typeid(1  op  v1), typeid(Vector4<decltype(1 op v1.x)>));\
        EXPECT_EQ(typeid(v1  op  1u), typeid(Vector4<decltype(v1.x op 1u)>));\
        EXPECT_EQ(typeid(1u  op  v1), typeid(Vector4<decltype(1u op v1.x)>));\
        EXPECT_EQ(typeid(v1  op  1.0f), typeid(Vector4<decltype(v1.x op 1.0f)>));\
        EXPECT_EQ(typeid(1.0f  op  v1), typeid(Vector4<decltype(1.0f op v1.x)>));\
        EXPECT_EQ(typeid(v1  op  1.0), typeid(Vector4<decltype(v1.x op 1.0)>));\
        EXPECT_EQ(typeid(1.0  op  v1), typeid(Vector4<decltype(1.0 op v1.x)>));\
    }    

#define TEST_VECTOR4_NEGATION(vec, s1, s2, s3, s4)\
    TEST(vec, canBeNegated)\
    {\
        auto v1 = vec{s1,s2,s3,s4};\
        auto v2 = -v1;\
        EXPECT_EQ(v2.x, -v1.x);\
        EXPECT_EQ(v2.y, -v1.y);\
        EXPECT_EQ(v2.z, -v1.z);\
        EXPECT_EQ(v2.w, -v1.w);\
    }

#define TEST_VECTOR4_OPERATORS(vec, x1, y1, z1, w1, x2, y2, z2, w2)\
     TEST_VECTOR4_COMPARISON_OP(vec, x1, y1, z1, x2, y2, z2)\
     TEST_VECTOR4_ONE_BIN_OP(vec, x1, y1, z1, w1, x2, y2, z2, w2, +, Add)\
     TEST_VECTOR4_ONE_BIN_OP(vec, x1, y1, z1, w1, x2, y2, z2, w2, -, Substract)\
     TEST_VECTOR4_ONE_BIN_OP(vec, x1, y1, z1, w1, x2, y2, z2, w2, *, Multiply)\
     TEST_VECTOR4_ONE_BIN_OP(vec, x1, y1, z1, w1, x2, y2, z2, w2, /, Divide)\
     TEST_VECTOR4_SCALED_BY_SCALAR(vec, x1, y1, z1, w1, x2, *, Multiply)\
     TEST_VECTOR4_SCALED_BY_SCALAR(vec, x1, y1, z1, w1, x2, /, Divide)\
     TEST_VECTOR4_NEGATION(vec, x1, y1, z1, w1)

#define TEST_VECTOR4_CAN_BE_PRINTED(vec, str, s1, s2, s3,s4)\
    TEST(vec, canBePrinted) { EXPECT_EQ(str, fmt("%1%", vec{s1,s2,s3,s4})); }
            
TEST_VECTOR4_STRUCTURE(vec4, f32, 5.0f, 1.0f, 7.0f, 2.3f)
TEST_VECTOR4_OPERATORS(vec4, 1.2f, 2.5f, 0.2f, 5.3f, 6.8f, 1.9f, 20.5f, 10.3f)
TEST_VECTOR4_CAN_BE_PRINTED(vec4, "(1.2,3.5,5.5,4.4)", 1.2f, 3.5f, 5.5f, 4.4f)

TEST_VECTOR4_STRUCTURE(dvec4, double, 5.0, 1.0, 7.0, 2.3)
TEST_VECTOR4_OPERATORS(dvec4, 1.2, 2.5, 0.2, 5.3, 6.8, 1.9, 20.5, 10.3)
TEST_VECTOR4_CAN_BE_PRINTED(dvec4, "(1.2,3.5,5.5,4.4)", 1.2, 3.5,5.5,4.4)

TEST_VECTOR4_STRUCTURE(ivec4, i32, 5, 1, 7, 4)
TEST_VECTOR4_OPERATORS(ivec4, 10, 25, 5, 53, 58, 3, 20, 10)
TEST_VECTOR4_CAN_BE_PRINTED(ivec4, "(1,2,3,4)", 1, 2, 3,4)

TEST_VECTOR4_STRUCTURE(uvec4, u32, 5u, 10u, 70u, 1u)
TEST_VECTOR4_OPERATORS(uvec4, 10u, 25u, 5u, 53u, 58u, 3u, 20u, 10u)
TEST_VECTOR4_CAN_BE_PRINTED(uvec4, "(1,2,3,4)", 1u, 2u, 3u,4u)

TEST(widest, worksOnVector4)
{
    EXPECT_EQ(typeid(widest<ivec4,ivec4>),typeid(ivec4));
    EXPECT_EQ(typeid(widest<ivec4,vec4>),typeid(vec4));
    EXPECT_EQ(typeid(widest<ivec4,dvec4>),typeid(dvec4));
    EXPECT_EQ(typeid(widest<ivec4,uvec4>),typeid(uvec4));

    EXPECT_EQ(typeid(widest<vec4,ivec4>),typeid(vec4));
    EXPECT_EQ(typeid(widest<vec4,vec4>),typeid(vec4));
    EXPECT_EQ(typeid(widest<vec4,dvec4>),typeid(dvec4));
    EXPECT_EQ(typeid(widest<vec4,uvec4>),typeid(vec4));

    EXPECT_EQ(typeid(widest<uvec4,ivec4>),typeid(uvec4));
    EXPECT_EQ(typeid(widest<uvec4,vec4>),typeid(vec4));
    EXPECT_EQ(typeid(widest<uvec4,dvec4>),typeid(dvec4));
    EXPECT_EQ(typeid(widest<uvec4,uvec4>),typeid(uvec4));

    EXPECT_EQ(typeid(widest<dvec4,ivec4>),typeid(dvec4));
    EXPECT_EQ(typeid(widest<dvec4,vec4>),typeid(dvec4));
    EXPECT_EQ(typeid(widest<dvec4,dvec4>),typeid(dvec4));
    EXPECT_EQ(typeid(widest<dvec4,uvec4>),typeid(dvec4));
}

TEST(Vector4, constexpr_ness)
{
    constexpr auto v = ivec4(1,0,0,0) + ivec4(0,1,0,0);
    static_assert(v == ivec4(1,1,0,0), "wrong");
}
