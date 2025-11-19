// ======================================================================
// \title  LedTestMain.cpp
// \author cpaulmech
// \brief  cpp file for Led component test main function
// ======================================================================

#include "LedTester.hpp"

TEST(Nominal, testBlinking) {
    Components::LedTester tester;
    tester.testBlinking();
}

TEST(Nominal, testBlinkInterval) {
    Components::LedTester tester;
    tester.testBlinkInterval();
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
