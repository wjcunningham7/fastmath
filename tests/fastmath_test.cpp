#include "gtest/gtest.h"

#include "fastmath.h"

TEST (POW2, Pow2Simple)
{
	ASSERT_EQ (fastmath::POW2(0.5), 0.25);
	ASSERT_EQ (fastmath::POW2(-0.5), 0.25);
}

TEST (POW2, Pow2)
{
	ASSERT_EQ (fastmath::POW2(0, fastmath::EXACT), 0);
	ASSERT_EQ (fastmath::POW2(0.5, fastmath::EXACT), 0.25);
	ASSERT_EQ (fastmath::POW2(0.5, fastmath::STL), 0.25);
	ASSERT_NEAR (fastmath::POW2(0.5, fastmath::FAST), 0.25, 0.00001);
	ASSERT_NEAR (fastmath::POW2(0.5, fastmath::FASTER), 0.25, 0.015);
	x = fastmath::POW2(0.5, fastmath::DEF);
	ASSERT_NE (x, x);
}

int main(int argc, char **argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

void instantiate()
{	
	fastmath::POW2(0);
	fastmath::POW2(0, fastmath::EXACT);
	fastmath::POW3(0);
}
