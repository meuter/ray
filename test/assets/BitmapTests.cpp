#include <gtest/gtest.h>
#include <ray/assets/Bitmap.hpp>

using namespace ray;
using namespace assets;
using u8 = unsigned char;

TEST(Bitmap, canBeDefaultConstructed)
{
    auto bitmap = Bitmap();
    EXPECT_EQ(bitmap.width(), 0);
    EXPECT_EQ(bitmap.height(), 0);
    EXPECT_EQ(bitmap.depth(), 0);
    EXPECT_EQ(bitmap.stride(), 0);
    EXPECT_EQ(bitmap.size(), 0);
    EXPECT_EQ(bitmap.pixels(), nullptr);
}

TEST(Bitmap, canAquireMemory)
{
    auto pixels = (u8*)malloc(1024);
    auto bitmap = Bitmap(12,20,2,pixels);
    EXPECT_EQ(bitmap.width(), 12);
    EXPECT_EQ(bitmap.height(), 20);
    EXPECT_EQ(bitmap.depth(), 2);
    EXPECT_EQ(bitmap.stride(), 24);
    EXPECT_EQ(bitmap.size(), 480);
    EXPECT_EQ(bitmap.pixels(), pixels);
}

TEST(Bitmap, canAllocateItsMemory)
{
    auto bitmap = Bitmap(13,15,3);
    EXPECT_EQ(bitmap.width(), 13);
    EXPECT_EQ(bitmap.height(), 15);
    EXPECT_EQ(bitmap.depth(), 3);
    EXPECT_EQ(bitmap.stride(), 3*13);
    EXPECT_EQ(bitmap.size(), 3*13*15);
    EXPECT_NE(bitmap.pixels(), nullptr);
}

