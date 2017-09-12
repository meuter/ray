#include <gtest/gtest.h>
#include <ray/math/Utils.hpp>
#include <ray/math/Vector2.hpp>

using namespace ray::math;

using vec2 = Vector2<float>;

TEST(lerp, canBeUsedOnFloat)
{
    EXPECT_FLOAT_EQ(3.0f, lerp(1,5,0.5));
}

TEST(lerp, canBeUsedOnVec2)
{
    auto start = vec2(1,5), end = vec2(7,-15);
    EXPECT_EQ(vec2(4,-5), lerp(start,end,0.5));
}

TEST(nlerp, canBeUsedOnVec2)
{
    auto start =vec2(1,5), end = vec2(7,-15);
    auto normalized_lerp = normalize(lerp(start, end, 0.5f));
    auto result = nlerp(start, end, 0.5f);

    EXPECT_EQ(result, normalized_lerp);
}

