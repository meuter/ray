#include <ray/platform/Print.hpp>
#include <gtest/gtest.h>

using namespace ray;
using namespace platform;

TEST(print, withoutStream)
{
    print("hello world\n");
    print(1,2,"world\n");
}

TEST(print, withOutputStream)
{
    print(std::cerr, "error\n");
    print(std::cerr, 5,6,"world\n");
}

TEST(print, withStringStream)
{
    std::stringstream buffer;
    print(buffer, "error\n");
    print(buffer, 5,6,"world\n");
    EXPECT_EQ(buffer.str(), "error\n56world\n");
}

TEST(println, withoutStream)
{
    println("hello world");
    println(1,2,"world");
}

TEST(println, withOutputStream)
{
    println(std::cerr, "error");
    println(std::cerr, 5,6,"world");
}

TEST(println, withStringStream)
{
    std::stringstream buffer;
    println(buffer, "error");
    println(buffer, 5,6,"world");
    EXPECT_EQ(buffer.str(), "error\n56world\n");
}

TEST(cat, works)
{
    EXPECT_EQ(cat(), "");
    EXPECT_EQ(cat(1), "1");
    EXPECT_EQ(cat(1,2,3, "Hello", 3.2f), "123Hello3.2");
}

TEST(fprint, withoutStream)
{
    fprint("Hello World\n");
    fprint("some_int=%08X\n", 10);
    fprint("(x,y) = (%d,%d)\n", 1, 2);
    fprint("(x,y,z) = (%d,%d,%d)\n", 1, 2, 3);
}

TEST(fprint, withOutputStream)
{
    fprint(std::cerr, "Hello World\n");
    fprint(std::cerr, "some_int=%08X\n", 10);
    fprint(std::cerr, "(x,y) = (%d,%d)\n", 1, 2);
    fprint(std::cerr, "(x,y,z) = (%d,%d,%d)\n", 1, 2, 3);
    fprint(std::cerr, "(x,y,z) = (%1%, %2%, %3%)\n", "hello", 2, 3.0f);
}

TEST(fprint, withStringStream)
{
    std::stringstream buffer;
    fprint(buffer, "Hello World\n");
    fprint(buffer, "some_int=%08X\n", 10);
    fprint(buffer, "(x,y) = (%d,%d)\n", 1, 2);
    fprint(buffer, "(x,y,z) = (%d,%d,%d)\n", 1, 2, 3);
    fprint(buffer, "(x,y,z) = (%1%, %2%, %3%)\n", "hello", 2, 3.0f);
    EXPECT_EQ(buffer.str(), "Hello World\nsome_int=0000000A\n(x,y) = (1,2)\n(x,y,z) = (1,2,3)\n(x,y,z) = (hello, 2, 3)\n");
}

TEST(fprintln, withoutStream)
{
    fprintln("Hello World");
    fprintln("some_int=%08X", 10);
    fprintln("(x,y) = (%d,%d)", 1, 2);
    fprintln("(x,y,z) = (%d,%d,%d)", 1, 2, 3);
}

TEST(fprintln, withOutputStream)
{
    fprintln(std::cerr, "Hello World");
    fprintln(std::cerr, "some_int=%08X", 10);
    fprintln(std::cerr, "(x,y) = (%d,%d)", 1, 2);
    fprintln(std::cerr, "(x,y,z) = (%d,%d,%d)", 1, 2, 3);
}

TEST(fprintln, withStringStream)
{
    std::stringstream buffer;    
    fprintln(buffer, "Hello World");
    fprintln(buffer, "some_int=%08X", 10);
    fprintln(buffer, "(x,y) = (%d,%d)", 1, 2);
    fprintln(buffer, "(x,y,z) = (%d,%d,%d)", 1, 2, 3);
    fprintln(buffer, "(x,y,z) = (%1%, %2%, %3%)", "hello", 2, 3.0f);
    EXPECT_EQ(buffer.str(), "Hello World\nsome_int=0000000A\n(x,y) = (1,2)\n(x,y,z) = (1,2,3)\n(x,y,z) = (hello, 2, 3)\n");    
}

TEST(fmt, works)
{    
    EXPECT_EQ(fmt("Hello World"), "Hello World");
    EXPECT_EQ(fmt("(%1%, %2%)", 123, 456), "(123, 456)");
    EXPECT_EQ(fmt("(x,y) = (%d,%d)", 1, 2), "(x,y) = (1,2)");
    EXPECT_EQ(fmt("%07.4f", 0.5f), "00.5000");
}

TEST(fprint, worksWitHex)
{
    std::stringstream buffer;
    fprint(buffer, "%02x\n", 0xAB);
    EXPECT_EQ(buffer.str(), "ab\n");    
}