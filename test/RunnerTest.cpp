#include "gtest/gtest.h"

#include "Runner.h"

TEST(RunnerTest, BasicAssertions) {
    EXPECT_EQ(cpplox::Runner::run("1 + 2"), 0);
}
