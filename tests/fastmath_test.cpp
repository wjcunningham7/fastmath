#include "gtest/gtest.h"

#include "fastmath.h"

TEST (POW2, Pow2Simple)
{
	ASSERT_EQ (fastmath::POW2(0.5), 0.25);
	ASSERT_EQ (fastmath::POW2(-0.5), 0.25);
}

int main(int argc, char **argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
