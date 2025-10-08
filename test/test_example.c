#include "../example_for_test/example.h"
#include "vendor/Unity/src/unity.h"

void setUp(void)
{
    // Set up whatever needs to be set up
}

void tearDown(void)
{
    // clean stuff up here
}

void test_multiply(void)
{
    float a = 1.0;
    float b = 2.5;
    float output1 = multiply_win(a, b);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 2.5001, output1);
}

void test_return_c(void)
{
    char *output2 = return_c();
    TEST_ASSERT_EQUAL_STRING("hei!", output2);
}

// not needed when using generate_test_runner.rb
int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_multiply);
    RUN_TEST(test_return_c);
    return UNITY_END();
}