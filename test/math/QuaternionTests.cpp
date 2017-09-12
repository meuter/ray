#include <gtest/gtest.h>
#include <ray/math/Quaternion.hpp>

using namespace ray::math;

using quat = Quaternion<float>;
using vec3 = Vector3<float>;
using vec4 = Vector4<float>;

TEST(quat, canBeDefaultConstructed)
{
    auto q = quat();
}

TEST(quat, canBeConstructedFromScalars)
{
    auto q = quat{1.0f,2.0f,3.0f,4.0f};
    EXPECT_EQ(q.x, 1.0f);
    EXPECT_EQ(q.y, 2.0f);
    EXPECT_EQ(q.z, 3.0f);
    EXPECT_EQ(q.w, 4.0f);
}

TEST(quat, canBeCopyConstructed)
{
    auto q1 = quat{1.0f,2.0f,3.0f,4.0f}, q2 = q1;
    EXPECT_EQ(q1.x, 1.0f);
    EXPECT_EQ(q1.y, 2.0f);
    EXPECT_EQ(q1.z, 3.0f);
    EXPECT_EQ(q1.w, 4.0f);
}

TEST(quat, canBeConstructedFromAxisAngle)
{
    auto a = 90_deg;
    auto s = (float)sin(a/2);
    auto c = (float)cos(a/2);
    auto v = vec3{1,2,3};
    
    auto q = quat(v, a);
    auto e = normalize(vec4(v*s, c));

    EXPECT_EQ(q.x, e.x);
    EXPECT_EQ(q.y, e.y);
    EXPECT_EQ(q.z, e.z);
    EXPECT_EQ(q.w, e.w);
}

TEST(quat, arePOD) 
{ 
    EXPECT_TRUE(std::is_pod<quat>::value); 
}

TEST(quat, arePacked)
{
    EXPECT_EQ(sizeof(quat), 4*sizeof(float));
}

TEST(quat, canBeConjugates)
{
    auto q1 = quat(1,2,3,4);
    auto q2 = conjugate(q1);
    EXPECT_EQ(q1.x, -q2.x);
    EXPECT_EQ(q1.y, -q2.y);
    EXPECT_EQ(q1.z, -q2.z);
    EXPECT_EQ(q1.w,  q2.w);
}

TEST(quat, canBeUsedToRotateVector3)
{
    auto xAxis = vec3(1,0,0);
    auto yAxis = vec3(0,1,0);
    auto zAxis = vec3(0,0,1);

    EXPECT_EQ(-xAxis,   rotate(xAxis,   quat(yAxis, 180_deg)));
	EXPECT_EQ(-3*xAxis, rotate(3*xAxis, quat(yAxis, 180_deg)));
	EXPECT_EQ(yAxis,    rotate(xAxis,   quat(zAxis, 90_deg)));
    EXPECT_EQ(-xAxis,   rotate(zAxis,   quat(yAxis, -90_deg)));
    
    EXPECT_EQ(-xAxis,   rotate(xAxis,   yAxis, 180_deg));
	EXPECT_EQ(-3*xAxis, rotate(3*xAxis, yAxis, 180_deg));
	EXPECT_EQ(yAxis,    rotate(xAxis,   zAxis, 90_deg));
	EXPECT_EQ(-xAxis,   rotate(zAxis,   yAxis, -90_deg));
}

