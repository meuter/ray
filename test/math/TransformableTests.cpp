#include <gtest/gtest.h>
#include <ray/math/Transformable.hpp>
#include <ray/math/Vector3.hpp>

using namespace ray::math;

using vec3 = Vector3<float>;
using mat4 = Matrix<float,4,4>;

constexpr auto X_AXIS = vec3(1,0,0);
constexpr auto Y_AXIS = vec3(0,1,0);
constexpr auto Z_AXIS = vec3(0,0,1);

TEST(Transformable, areOrientedWithTheirFrontInThePositiveDirectionAndTheTopInThePositiveYDirection)
{
    auto t = Transformable();
    
    EXPECT_EQ(t.left(),   X_AXIS);
    EXPECT_EQ(t.right(), -X_AXIS);

    EXPECT_EQ(t.front(),  Z_AXIS);
    EXPECT_EQ(t.back(),  -Z_AXIS);

    EXPECT_EQ(t.up(),    Y_AXIS);
    EXPECT_EQ(t.down(), -Y_AXIS);
}

TEST(Transformable, canBeMovedToTheLeft)
{
    auto t = Transformable();

    t.move(t.left(), 1.0f);

    auto expected = vec3{1.0f,0.0f,0.0f};
    EXPECT_EQ(t.position(), expected);

    auto m = t.modelMatrix();
    auto expectedM = mat4{
        1.0f, 0.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };

    EXPECT_EQ(m, expectedM);
}

TEST(Transformable, canBeRotatedAroundTheYAxis)
{
    auto t = Transformable();

    t.rotate(Y_AXIS, 90_deg);

    EXPECT_EQ(t.front(),  X_AXIS);
    EXPECT_EQ(t.back(),  -X_AXIS);

    EXPECT_EQ(t.up(),   Y_AXIS);
    EXPECT_EQ(t.down(), -Y_AXIS);

    EXPECT_EQ(t.left(),  -Z_AXIS);
    EXPECT_EQ(t.right(), Z_AXIS);
}

TEST(Transformable, canBeRotatedAroundTheXAxis)
{
    auto t = Transformable();

    t.rotate(X_AXIS, 90_deg);

    EXPECT_EQ(t.front(), -Y_AXIS);
    EXPECT_EQ(t.back(),  Y_AXIS);

    EXPECT_EQ(t.up(),   Z_AXIS);
    EXPECT_EQ(t.down(), -Z_AXIS);

    EXPECT_EQ(t.left(),  X_AXIS);
    EXPECT_EQ(t.right(), -X_AXIS);
}

