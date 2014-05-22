#include <boost/math/special_functions/gamma.hpp>

#include "fastapprox.h"
#include "FastMath.h"

/////////////////////////////
//(C) Will Cunningham 2014 //
// Northeastern University //
// Krioukov Research Group //
/////////////////////////////

//Approximation of x^2
float POW2(const float x, const enum FastMethod fm)
{
	if (x == 0.0f)
		return x;

	//assert (fm == STL || fm == FAST || fm == FASTER || fm == EXACT);

	float y = 0.0f;

	if (fm == STL)
		//Defined in <math.h>
		y = powf(x, 2.0f);
	else if (fm == FAST)
		//Defined in "fastapprox.h"
		y = fastpow(x, 2.0f);
	else if (fm == FASTER)
		//Defined in "fastapprox.h"
		y = fasterpow(x, 2.0f);
	else if (fm == EXACT)
		y = x * x;

	return y;
}

//Approximation of x^3
float POW3(const float x, const enum FastMethod fm)
{
	if (x == 0.0f)
		return x;

	//assert (fm == STL || fm == FAST || fm == FASTER || fm == EXACT);

	float y = 0.0f;

	if (fm == STL)
		//Defined in <math.h>
		y = powf(x, 3.0f);
	else if (fm == FAST)
		//Defined in "fastapprox.h"
		y = fastpow(x, 3.0f);
	else if (fm == FASTER)
		//Defined in "fastapprox.h"
		y = fasterpow(x, 3.0f);
	else if (fm == EXACT)
		y = x * x * x;

	return y;
}

//Approximation of x^p
float POW(const float x, const float p, const enum FastMethod fm)
{
	//assert (fm == STL || fm == FAST || fm == FASTER);

	float y = 0.0f;

	if (fm == STL)
		//Defined in <math.h>
		y = powf(x, p);
	else if (fm == FAST)
		//Defined in "fastapprox.h"
		y = fastpow(x, p);
	else if (fm == FASTER)
		//Defined in "fastapprox.h"
		y = fasterpow(x, p);

	return y;
}

//Approximation of x^(1/2)
float SQRT(const float x, const enum FastMethod fm)
{
	if (x == 0.0f)
		return x;

	//assert (fm == STL || fm == BITWISE);
	//assert (x > 0.0f);

	float y = 0.0f;

	if (fm == STL)
		//Defined in <math.h>
		y = sqrtf(x);
	else if (fm == BITWISE) {
		//Bit shift approximation
		unsigned int i = *(unsigned int*) &x;
		i += 127 << 23;
		i >>= 1;
		y = *(float*) &i;
	}

	return y;
}

//Approximation of |x|
float ABS(const float x, const enum FastMethod fm)
{
	if (x == 0.0f)
		return x;

	//assert (fm == STL || fm == BITWISE);
	
	float y = 0.0f;

	if (fm == STL)
		//Defined in <math.h>
		y = fabs(x);
	else if (fm == BITWISE) {
		//Bitwise operation
		int i = *(int*) &x;
		i &= 0x7FFFFFFF;
		y = *(float*) &i;
	}

	return y;
}

//Approximation of ln(x)
float LOG(const float x, const enum FastMethod fm)
{
	//assert (fm == STL || fm == FAST || fm == FASTER);
	//assert (x > 0.0f);

	float y = 0.0f;

	if (fm == STL)
		//Defined int <math.h>
		y = logf(x);
	else if (fm == FAST)
		//Defined in "fastapprox.h"
		y = fastlog(x);
	else if (fm == FASTER)
		//Defined in "fastapprox.h"
		y = fasterlog(x);

	return y;
}

//Returns sign(x)
float SGN(const float x, const enum FastMethod fm)
{	
	//Cannot be zero
	//assert (ABS(round(x) - x, STL) > TOL);
	//assert (fm == DEF || fm == BITWISE);

	float y = 1.0f;

	if (fm == DEF);
		if (y < 0.0f)
			y = -1.0f;
	else if (fm == BITWISE)
		(int&)y |= ((int&)y & 0x80000000);

	return y;
}

//Approximation of sine(x)
float SIN(const float x, const enum FastMethod fm)
{
	if (x == 0.0f)
		return x;

	//assert (fm == STL || fm == FAST || fm == FASTER);

	float y = 0.0f;

	if (fm == STL)
		//Defined in <math.h>
		y = sinf(x);
	else if (fm == FAST) {
		//Defined in "fastapprox.h"
		//assert (ABS(x, STL) < static_cast<float>(M_PI));
		y = fastsin(x);
	} else if (fm == FASTER) {
		//Defined in "fastapprox.h"
		//assert (ABS(x, STL) < static_cast<float>(M_PI));
		y = fastersin(x);
	}

	return y;
}

//Approximation of cosine(x)
float COS(const float x, const enum FastMethod fm)
{
	if (x == 0.0f)
		return 1.0f;

	//assert (fm == STL || fm == FAST || fm == FASTER);

	float y = 0.0f;

	if (fm == STL)
		//Defined in <math.h>
		y = cosf(x);
	else if (fm == FAST) {
		//Defined in "fastapprox.h"
		//assert (ABS(x, STL) < static_cast<float>(M_PI));
		y = fastcos(x);
	} else if (fm == FASTER) {
		//Defined in "fastapprox.h"
		//assert (ABS(x, STL) < static_cast<float>(M_PI));
		y = fastercos(x);
	}

	return y;
}

//Approximation of tangent(x)
float TAN(const float x, const enum FastMethod fm)
{
	if (x == 0.0f)
		return x;

	//assert (fm == STL || fm == FAST || fm == FASTER);

	float y = 0.0f;

	if (fm == STL)
		//Defined in <math.h>
		y = tanf(x);
	else if (fm == FAST) {
		//Defined in "fastapprox.h"
		//assert (ABS(x, STL) < HALF_PI);
		y = fasttan(x);
	} else if (fm == FASTER) {
		//Defined in "fastapprox.h"
		//assert (ABS(x, STL) < HALF_PI);
		y = fastertan(x);
	}

	return y;
}

//Approximation of arccosine(x)
float ACOS(const float x, const enum FastMethod fm, const enum Precision p)
{
	if (x == 0.0f)
		return HALF_PI;

	//assert (fm == STL || fm == CHEBYSHEV || fm == INTEGRATION);
	//assert (ABS(x, STL) < 1.0f);

	float y = 0.0f;

	if (fm == STL)
		//Defined in <math.h>
		y = acosf(x);
	else if (fm == CHEBYSHEV) {
		//Chebyshev Approximation
		float _x2 = POW2(x, EXACT);
		if (p == LOW_PRECISION)
			y = ACOS_C0 + x * (ACOS_C3 * _x2 + ACOS_C1);
		else if (p == HIGH_PRECISION)
			y = ACOS_C0 + x * (_x2 * (ACOS_C5 * _x2 + ACOS_C3) + ACOS_C1);
		else if (p == VERY_HIGH_PRECISION)
			y = ACOS_C0 + x * (_x2 * (_x2 * (_x2 * (ACOS_C9 * _x2 + ACOS_C7) + ACOS_C5) + ACOS_C3) + ACOS_C1);
	} else if (fm == INTEGRATION) {
		//Series from Integration (for |x| < 1)
		float _x2 = POW2(x, EXACT);
		if (p == LOW_PRECISION)
			y = ACOS_I0 + x * (ACOS_I3 * _x2 + ACOS_I1);
		else if (p == HIGH_PRECISION)
			y = ACOS_I0 + x * (_x2 * (ACOS_I5 * _x2 + ACOS_I3) + ACOS_I1);
		else if (p == VERY_HIGH_PRECISION)
			y = ACOS_I0 + x * (_x2 * (_x2 * (_x2 * (ACOS_I9 * _x2 + ACOS_I7) + ACOS_I5) + ACOS_I3) + ACOS_I1);
	}

	return y;
}

//Approximation of arctangent(x)
float ATAN(const float x, const enum FastMethod fm, const enum Precision p)
{
	if (x == 0.0f)
		return x;
	else if (ABS(x, STL) == 1.0f)	//Integration series not valid for |x| = 1.0
		return HALF_PI / 2.0f;

	//assert (fm == STL || fm == CHEBYSHEV || fm == INTEGRATION);
	
	float y = 0.0f;

	if (fm == STL)
		//Defined in <math.h>
		y = atanf(x);
	else if (fm == CHEBYSHEV) {
		//Chebyshev Approximation
		float _x2 = POW2(x, EXACT);
		if (p == LOW_PRECISION)
			y = x * (ATAN_C3 * _x2 + ATAN_C1);
		else if (p == HIGH_PRECISION)
			y = x * (_x2 * (ATAN_C5 * _x2 + ATAN_C3) + ATAN_C1);
		else if (p == VERY_HIGH_PRECISION)
			y = x * (_x2 * (_x2 * (_x2 * (ATAN_C9 * _x2 + ATAN_C7) + ATAN_C5) + ATAN_C3) + ATAN_C1);
	} else if (fm == INTEGRATION) {
		//Series from Integration (for x != 1.0)
		if (ABS(x, STL) < 1.0f) {
			float _x2 = POW2(x, EXACT);
			if (p == LOW_PRECISION)
				y = x * (ATAN_H3 * _x2 + ATAN_H1);
			else if (p == HIGH_PRECISION)
				y = x * (_x2 * (ATAN_H5 * _x2 + ATAN_H3) + ATAN_H1);
			else if (p == VERY_HIGH_PRECISION)
				y = x * (_x2 * (_x2 * (_x2 * (ATAN_H9 * _x2 + ATAN_H7) + ATAN_H5) + ATAN_H3) + ATAN_H1);
		} else {
			float _x2minus = 1.0 / POW2(x, EXACT);
			if (p == LOW_PRECISION)
				y = ATAN_I0 * SGN(x, DEF) + (ATAN_I3 * _x2minus + ATAN_I1) / x;
			else if (p == HIGH_PRECISION)
				y = ATAN_I0 * SGN(x, DEF) + (_x2minus * (ATAN_I5 * _x2minus + ATAN_I3) + ATAN_I1) / x;
			else if (p == VERY_HIGH_PRECISION)
				y = ATAN_I0 * SGN(x, DEF) + (_x2minus * (_x2minus * (_x2minus * (ATAN_I9 * _x2minus + ATAN_I7) + ATAN_I5) + ATAN_I3) + ATAN_I1) / x;
		}
	}

	return y;
}

//Approximation of sinh(x)
float SINH(const float x, const enum FastMethod fm)
{
	if (x == 0.0f)
		return x;

	//assert (fm == STL || fm == FAST || fm == FASTER);

	float y = 0.0f;

	if (fm == STL)
		//Defined in <math.h>
		y = sinhf(x);
	else if (fm == FAST)
		//Defined in "fastapprox.h"
		y = fastsinh(x);
	else if (fm == FASTER)
		//Defined in "fastapprox.h"
		y = fastersinh(x);

	return y;
}

//Approximation of cosh(x)
float COSH(const float x, const enum FastMethod fm)
{
	if (x == 0.0f)
		return 1.0f;

	//assert (fm == STL || fm == FAST || fm == FASTER);

	float y = 0.0f;

	if (fm == STL)
		//Defined in <math.h>
		y = coshf(x);
	else if (fm == FAST)
		//Defined in "fastapprox.h"
		y = fastcosh(x);
	else if (fm == FASTER)
		//Defined in "fastapprox.h"
		y = fastercosh(x);

	return y;
}

//Approximation of arcsinh(x)
float ASINH(const float x, const enum FastMethod fm, const enum Precision p)
{
	if (x == 0.0f)
		return x;

	//assert (fm == STL || fm == INTEGRATION);

	float y = 0.0f;

	if (fm == STL)
		//Defined in <math.h>
		y = asinhf(x);
	else if (fm == CHEBYSHEV)
		//Chebyshev Approximation
		//Not implemented yet
		y = x;
	else if (fm == INTEGRATION) {
		//Series from Integration (for |x| < 1)
		//assert (ABS(x, STL) < 1.0f);
		float _x2 = POW2(x, EXACT);
		if (p == LOW_PRECISION)
			y = x * (ASINH_I3 * _x2 + ASINH_I1);
		else if (p == HIGH_PRECISION)
			y = x * (_x2 * (ASINH_I5 * _x2 + ASINH_I3) + ASINH_I1);
		else if (p == VERY_HIGH_PRECISION)
			y = x * (_x2 * (_x2 * (_x2 * (ASINH_I9 * _x2 + ASINH_I7) + ASINH_I5) + ASINH_I3) + ASINH_I1);
	}

	return y;
}

//Approximation of arccosh(x)
float ACOSH(const float x, const enum FastMethod fm, const enum Precision p)
{
	if (x == 1.0f)
		return 0.0f;

	//assert (fm == STL || fm == INTEGRATION);

	float y = 0.0f;

	if (fm == STL)
		//Defined in <math.h>
		y = acoshf(x);
	else if (fm == CHEBYSHEV)
		//Chebyshev Approximation
		//Not implemented yet
		y = x;
	else if (fm == INTEGRATION) {
		//Series from Integration (for x > 1)
		//assert (ABS(x, STL) > 1.0f);
		float _x2minus = 1.0f / POW2(x, EXACT);
		y += LOG(2.0f * x, STL);
		if (p == LOW_PRECISION)
			y += _x2minus * (ACOSH_I4 * _x2minus + ACOSH_I2);
		else if (p == HIGH_PRECISION)
			y += _x2minus * (_x2minus * (ACOSH_I6 * _x2minus + ACOSH_I4) + ACOSH_I2);
		else if (p == VERY_HIGH_PRECISION)
			y += _x2minus * (_x2minus * (_x2minus * (_x2minus * (ACOSH_I10 * _x2minus + ACOSH_I8) + ACOSH_I6) + ACOSH_I4) + ACOSH_I2);
	}

	return y;
}

//Approximation of the Gamma Function
float GAMMA(const float x, const enum FastMethod fm)
{
	if (x == 1.0f || x == 2.0f)
		return 1.0f;

	//assert (fm == STL || fm == BOOST);
	//Gamma(0) is undefined
	//assert (x != 0.0f);
	//assert (!(x > 0.0f && ABS(floor(x) - x, STL) < TOL));
	//assert (!(x < 0.0f && ABS(ceil(x) - x, STL) < TOL));
	//Gamma not defined for negative integers
	//assert (!(x < 0.0f && ABS(x - round(x), STL) < TOL));

	float y = 0.0f;

	if (fm == STL)
		//Defined in <math.h>
		y = tgamma(x);
	else if (fm == BOOST)
		//Lanczos Approximation
		//Defined in Boost library
		y = boost::math::tgamma1pm1(x);

	return y;
}

//Approximation of the Pochhammer symbol (x)_j
//The coefficient j must be a non-negative integer
float POCHHAMMER(const float x, const int j)
{
	assert (j >= 0);

	float y = 0.0f;

	y = GAMMA(x + j, STL) / GAMMA(x, STL);

	return y;
}

//Approximates the Gauss Hypergeometric Function sol=2F1(a,b,c,z(x))
//The tolerance describes convergence of the series
//The precision 'p' describes how many terms are used in the series
bool _2F1(float (*z)(const float &x), float &sol, const float &x, const float a, const float b, const float c, const float tol, const enum Precision p)
{
	//No null pointers
	assert (z != NULL);
	
	bool success = false;

	//The value c - a - b must be greater than zero and not an integer
	if (c - a - b <= 0.0f || ABS(round(c - a - b) - (c - a - b), STL) < TOL)
		return success;

	//Valid region for power series approximation
	if (z(x) <= 1.0f) {
		int nterms;
		int i;

		if (p == LOW_PRECISION)
			nterms = 3;
		else if (p == HIGH_PRECISION)
			nterms = 5;
		else if (p == VERY_HIGH_PRECISION)
			nterms = 7;

		for (i = 0; i < nterms; i++)
			sol += POCHHAMMER(a, i) * POCHHAMMER(b, i) * POW(z(x), static_cast<float>(i), STL) / (POCHHAMMER(c, i) * GAMMA(i + 1, STL));
	} else if (z(x) > 1.0f) {
		//Use transformation described in notes
	}

	return success;
}
