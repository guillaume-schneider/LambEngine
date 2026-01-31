// Minimal test with custom main to verify GTest works
#include <iostream>

#include <gtest/gtest.h>

TEST(MinimalTest, BasicAssertion)
{
    std::cout << "Running BasicAssertion test" << std::endl;
    EXPECT_EQ(1, 1);
}

TEST(MinimalTest, AnotherTest)
{
    std::cout << "Running AnotherTest" << std::endl;
    EXPECT_TRUE(true);
}

int main(int argc, char** argv)
{
    std::cout << "Custom main starting..." << std::endl;
    ::testing::InitGoogleTest(&argc, argv);
    std::cout << "GoogleTest initialized, running tests..." << std::endl;
    return RUN_ALL_TESTS();
}
