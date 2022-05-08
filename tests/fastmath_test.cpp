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
	double x = fastmath::POW2(0.5, fastmath::BOOST);
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
	fastmath::POW3(0, fastmath::EXACT);
	fastmath::POW(0, 0, fastmath::STL);
	fastmath::SQRT(0, fastmath::STL);
	fastmath::ABS(0, fastmath::STL);
	fastmath::LOG(1, fastmath::STL);
	fastmath::SGN(1);
	fastmath::SGN(1, fastmath::DEF);
	fastmath::SIN(0, fastmath::STL);
	fastmath::COS(0, fastmath::STL);
	fastmath::TAN(0, fastmath::STL);
	fastmath::ACOS(0, fastmath::STL, fastmath::DEFAULT);
	fastmath::ATAN(0, fastmath::STL, fastmath::DEFAULT);
	fastmath::SINH(0, fastmath::STL);
	fastmath::COSH(0, fastmath::STL);
	fastmath::ASINH(0, fastmath::STL, fastmath::DEFAULT);
	fastmath::ACOSH(1, fastmath::STL, fastmath::DEFAULT);
	fastmath::GAMMA(1);
	fastmath::GAMMA(1, fastmath::STL);
	fastmath::LOGGAMMA(1);
	fastmath::LOGGAMMA(1, fastmath::STL);
	fastmath::GAMMA_RATIO(1, 1);
	fastmath::POCHHAMMER(1, 0);
	fastmath::POCHHAMMER_F(1, 0);
	fastmath::_2F1_An_F(1, 1, 1, 1);
	fastmath::_2F1_An(1, 1, 1, 1);

	double sol, err;
	int nterms = 1;
	fastmath::_2F1(1, 1, 1, 1, &sol, &err, &nterms, false);
	fastmath::_2F1_F(1, 1, 1, 1, &sol, &err, &nterms);

	double z = 0;
	err = 0.1;
	fastmath::getNumTerms(z, err);

	fastmath::getHyperType(z);
}
