#include <gtest/gtest.h>
#include <ray/assets/Bitmap.hpp>

using namespace ray;
using namespace assets;
using namespace math;

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

TEST(Bitmap, canBeConstructedUsingSizeAndColor__depth1)
{
    auto depth = 1;
    auto color = pack(vec4(0.1f, 0.2f, 0.3f, 0.4f));
    auto bitmap = Bitmap(5,3,depth, color);
    EXPECT_EQ(bitmap.width(), 5);
    EXPECT_EQ(bitmap.height(), 3);
    EXPECT_EQ(bitmap.depth(), depth);
    EXPECT_EQ(bitmap.stride(), depth*5);
    EXPECT_EQ(bitmap.size(), depth*5*3);
    EXPECT_NE(bitmap.pixels(), nullptr);
    EXPECT_EQ(color.r, (int)(0.1f * 255.0f));
    EXPECT_EQ(color.g, (int)(0.2f * 255.0f));
    EXPECT_EQ(color.b, (int)(0.3f * 255.0f));
    EXPECT_EQ(color.a, (int)(0.4f * 255.0f));  

    for (auto ptr = bitmap.pixels(); ptr < bitmap.pixels()+bitmap.size(); ptr += bitmap.depth())
    {
        EXPECT_EQ(ptr[0], color.r);
    }
}


TEST(Bitmap, canBeConstructedUsingSizeAndColor__depth2)
{
    auto depth = 2;
    auto color = pack(vec4(0.1f, 0.2f, 0.3f, 0.4f));
    auto bitmap = Bitmap(5,3,depth, color);
    EXPECT_EQ(bitmap.width(), 5);
    EXPECT_EQ(bitmap.height(), 3);
    EXPECT_EQ(bitmap.depth(), depth);
    EXPECT_EQ(bitmap.stride(), depth*5);
    EXPECT_EQ(bitmap.size(), depth*5*3);
    EXPECT_NE(bitmap.pixels(), nullptr);
    EXPECT_EQ(color.r, (int)(0.1f * 255.0f));
    EXPECT_EQ(color.g, (int)(0.2f * 255.0f));
    EXPECT_EQ(color.b, (int)(0.3f * 255.0f));
    EXPECT_EQ(color.a, (int)(0.4f * 255.0f));
    
    for (auto ptr = bitmap.pixels(); ptr < bitmap.pixels()+bitmap.size(); ptr += bitmap.depth())
    {
        EXPECT_EQ(ptr[0], color.r);
        EXPECT_EQ(ptr[1], color.g);
    }
}


TEST(Bitmap, canBeConstructedUsingSizeAndColor__depth3)
{
    auto depth = 3;
    auto color = pack(vec4(0.1f, 0.2f, 0.3f, 0.4f));
    auto bitmap = Bitmap(5,3,depth, color);
    EXPECT_EQ(bitmap.width(), 5);
    EXPECT_EQ(bitmap.height(), 3);
    EXPECT_EQ(bitmap.depth(), depth);
    EXPECT_EQ(bitmap.stride(), depth*5);
    EXPECT_EQ(bitmap.size(), depth*5*3);
    EXPECT_NE(bitmap.pixels(), nullptr);
    EXPECT_EQ(color.r, (int)(0.1f * 255.0f));
    EXPECT_EQ(color.g, (int)(0.2f * 255.0f));
    EXPECT_EQ(color.b, (int)(0.3f * 255.0f));
    EXPECT_EQ(color.a, (int)(0.4f * 255.0f));
    
    for (auto ptr = bitmap.pixels(); ptr < bitmap.pixels()+bitmap.size(); ptr += bitmap.depth())
    {
        EXPECT_EQ(ptr[0], color.r);
        EXPECT_EQ(ptr[1], color.g);
        EXPECT_EQ(ptr[2], color.b);
    }
}


TEST(Bitmap, canBeConstructedUsingSizeAndColor__depth4)
{
    auto depth = 4;
    auto color = pack(vec4(0.1f, 0.2f, 0.3f, 0.4f));
    auto bitmap = Bitmap(5,3,depth, color);
    EXPECT_EQ(bitmap.width(), 5);
    EXPECT_EQ(bitmap.height(), 3);
    EXPECT_EQ(bitmap.depth(), depth);
    EXPECT_EQ(bitmap.stride(), depth*5);
    EXPECT_EQ(bitmap.size(), depth*5*3);
    EXPECT_NE(bitmap.pixels(), nullptr);
    EXPECT_EQ(color.r, (int)(0.1f * 255.0f));
    EXPECT_EQ(color.g, (int)(0.2f * 255.0f));
    EXPECT_EQ(color.b, (int)(0.3f * 255.0f));
    EXPECT_EQ(color.a, (int)(0.4f * 255.0f));
    
    for (auto ptr = bitmap.pixels(); ptr < bitmap.pixels()+bitmap.size(); ptr += bitmap.depth())
    {
        EXPECT_EQ(ptr[0], color.r);
        EXPECT_EQ(ptr[1], color.g);
        EXPECT_EQ(ptr[2], color.b);
        EXPECT_EQ(ptr[3], color.a);
    }
}