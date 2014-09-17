#include <boost/math/special_functions/gamma.hpp>

#include "fastapprox.h"
#include "FastMath.h"

/////////////////////////////
//(C) Will Cunningham 2014 //
// Northeastern University //
// Krioukov Research Group //
/////////////////////////////

////////////////////////////////////////////////////////////////////
// Note:  At a future point, these functions will be modified to  //
// be able to use the SSE2 functions defined in 'fastapprox.h'    //
// This will allow functions to execute 4x in parallel using SIMD //
////////////////////////////////////////////////////////////////////

//Approximation of x^2
float POW2(const float x, const enum FastMethod fm)
{
	if (x == 0.0f)
		return x;

	if (FM_DEBUG)
		assert (fm == STL || fm == FAST || fm == FASTER || fm == EXACT);

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

	if (FM_DEBUG)
		assert (fm == STL || fm == FAST || fm == FASTER || fm == EXACT);

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
	if (FM_DEBUG)
		assert (fm == STL || fm == FAST || fm == FASTER);

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

	if (FM_DEBUG) {
		assert (fm == STL || fm == BITWISE);
		assert (x > 0.0f);
	}

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

	if (FM_DEBUG)
		assert (fm == STL || fm == BITWISE);
	
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
	if (FM_DEBUG) {
		assert (fm == STL || fm == FAST || fm == FASTER);
		assert (x > 0.0f);
	}

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
	if (FM_DEBUG)
		assert (fm == DEF || fm == BITWISE);

	if (ABS(round(x) - x, STL) < FM_TOL)
		return 0.0f;

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

	if (FM_DEBUG)
		assert (fm == STL || fm == FAST || fm == FASTER);

	float y = 0.0f;

	if (fm == STL)
		//Defined in <math.h>
		y = sinf(x);
	else if (fm == FAST) {
		//Defined in "fastapprox.h"
		if (FM_DEBUG)
			assert (ABS(x, STL) < M_PI);
		y = fastsin(x);
	} else if (fm == FASTER) {
		//Defined in "fastapprox.h"
		if (FM_DEBUG)
			assert (ABS(x, STL) < M_PI);
		y = fastersin(x);
	}

	return y;
}

//Approximation of cosine(x)
float COS(const float x, const enum FastMethod fm)
{
	if (x == 0.0f)
		return 1.0f;

	if (FM_DEBUG)
		assert (fm == STL || fm == FAST || fm == FASTER);

	float y = 0.0f;

	if (fm == STL)
		//Defined in <math.h>
		y = cosf(x);
	else if (fm == FAST) {
		//Defined in "fastapprox.h"
		if (FM_DEBUG)
			assert (ABS(x, STL) < M_PI);
		y = fastcos(x);
	} else if (fm == FASTER) {
		//Defined in "fastapprox.h"
		if (FM_DEBUG)
			assert (ABS(x, STL) < M_PI);
		y = fastercos(x);
	}

	return y;
}

//Approximation of tangent(x)
float TAN(const float x, const enum FastMethod fm)
{
	if (x == 0.0f)
		return x;

	if (FM_DEBUG)
		assert (fm == STL || fm == FAST || fm == FASTER);

	float y = 0.0f;

	if (fm == STL)
		//Defined in <math.h>
		y = tanf(x);
	else if (fm == FAST) {
		//Defined in "fastapprox.h"
		if (FM_DEBUG)
			assert (ABS(x, STL) < HALF_PI);
		y = fasttan(x);
	} else if (fm == FASTER) {
		//Defined in "fastapprox.h"
		if (FM_DEBUG)
			assert (ABS(x, STL) < HALF_PI);
		y = fastertan(x);
	}

	return y;
}

//Approximation of arccosine(x)
float ACOS(const float x, const enum FastMethod fm, const enum Precision p)
{
	if (x == 0.0f)
		return HALF_PI;

	if (FM_DEBUG) {
		assert (fm == STL || fm == CHEBYSHEV || fm == INTEGRATION);
		assert (ABS(x, STL) < 1.0f);
	}

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
			y = ACOS_I0 + x * (_x2 * (ACOS_I5 * _x2 + ACOS_I3) + ACOS_I1);
		else if (p == HIGH_PRECISION)
			y = ACOS_I0 + x * (_x2 * (_x2 * (_x2 * (ACOS_I9 * _x2 + ACOS_I7) + ACOS_I5) + ACOS_I3) + ACOS_I1);
		else if (p == VERY_HIGH_PRECISION)
			y = ACOS_I0 + x * (_x2 * (_x2 * (_x2 * (_x2 * (_x2 * (_x2 * (ACOS_I15 * _x2 + ACOS_I13) + ACOS_I11) + ACOS_I9) + ACOS_I7) + ACOS_I5) + ACOS_I3) + ACOS_I1);
	}

	return y;
}

//Approximation of arctangent(x)
float ATAN(const float x, const enum FastMethod fm, const enum Precision p)
{
	if (x == 0.0f)
		return x;
	else if (ABS(x, STL) == 1.0f)
		//Integration series not valid for |x| = 1.0
		return HALF_PI / 2.0f;

	if (FM_DEBUG)
		assert (fm == STL || fm == CHEBYSHEV || fm == INTEGRATION);
	
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
				y = x * (_x2 * (ATAN_H5 * _x2 + ATAN_H3) + ATAN_H1);
			else if (p == HIGH_PRECISION)
				y = x * (_x2 * (_x2 * (_x2 * (ATAN_H9 * _x2 + ATAN_H7) + ATAN_H5) + ATAN_H3) + ATAN_H1);
			else if (p == VERY_HIGH_PRECISION)
				y = x * (_x2 * (_x2 * (_x2 * (_x2 * (_x2 * (_x2 * (ATAN_H15 * _x2 + ATAN_H13) + ATAN_H11) + ATAN_H9) + ATAN_H7) + ATAN_H5) + ATAN_H3) + ATAN_H1);
		} else {
			float _x2minus = 1.0 / POW2(x, EXACT);
			if (p == LOW_PRECISION)
				y = ATAN_I0 * SGN(x, DEF) + (_x2minus * (ATAN_I5 * _x2minus + ATAN_I3) + ATAN_I1) / x;
			else if (p == HIGH_PRECISION)
				y = ATAN_I0 * SGN(x, DEF) + (_x2minus * (_x2minus * (_x2minus * (ATAN_I9 * _x2minus + ATAN_I7) + ATAN_I5) + ATAN_I3) + ATAN_I1) / x;
			else if (p == VERY_HIGH_PRECISION)
				y = ATAN_I0 * SGN(x, DEF) + (_x2minus * (_x2minus * (_x2minus * (_x2minus * (_x2minus * (_x2minus * (ATAN_I15 * _x2minus + ATAN_I13) + ATAN_I11) + ATAN_I9) + ATAN_I7) + ATAN_I5) + ATAN_I3) + ATAN_I1) / x;
		}
	}

	return y;
}

//Approximation of sinh(x)
float SINH(const float x, const enum FastMethod fm)
{
	if (x == 0.0f)
		return x;

	if (FM_DEBUG)
		assert (fm == STL || fm == FAST || fm == FASTER);

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

	if (FM_DEBUG)
		assert (fm == STL || fm == FAST || fm == FASTER);

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

	if (FM_DEBUG)
		assert (fm == STL || fm == INTEGRATION);

	float y = 0.0f;

	if (fm == STL)
		//Defined in <math.h>
		y = asinhf(x);
	else if (fm == INTEGRATION) {
		//Series from Integration (for |x| < 1)
		if (FM_DEBUG)
			assert (ABS(x, STL) < 1.0f);
		float _x2 = POW2(x, EXACT);
		if (p == LOW_PRECISION)
			y = x * (_x2 * (ASINH_I5 * _x2 + ASINH_I3) + ASINH_I1);
		else if (p == HIGH_PRECISION)
			y = x * (_x2 * (_x2 * (_x2 * (ASINH_I9 * _x2 + ASINH_I7) + ASINH_I5) + ASINH_I3) + ASINH_I1);
		else if (p == VERY_HIGH_PRECISION)
			y = x * (_x2 * (_x2 * (_x2 * (_x2 * (_x2 * (_x2 * (ASINH_I15 * _x2 + ASINH_I13) + ASINH_I11) + ASINH_I9) + ASINH_I7) + ASINH_I5) + ASINH_I3) + ASINH_I1);
	}

	return y;
}

//Approximation of arccosh(x)
float ACOSH(const float x, const enum FastMethod fm, const enum Precision p)
{
	if (x == 1.0f)
		return 0.0f;

	if (FM_DEBUG)
		assert (fm == STL || fm == INTEGRATION);

	float y = 0.0f;

	if (fm == STL)
		//Defined in <math.h>
		y = acoshf(x);
	else if (fm == INTEGRATION) {
		//Series from Integration (for x > 1)
		if (FM_DEBUG)
			assert (ABS(x, STL) > 1.0f);
		float _x2minus = 1.0f / POW2(x, EXACT);
		y += LOG(2.0f * x, STL);
		if (p == LOW_PRECISION)
			y += _x2minus * (_x2minus * (ACOSH_I6 * _x2minus + ACOSH_I4) + ACOSH_I2);
		else if (p == HIGH_PRECISION)
			y += _x2minus * (_x2minus * (_x2minus * (_x2minus * (ACOSH_I10 * _x2minus + ACOSH_I8) + ACOSH_I6) + ACOSH_I4) + ACOSH_I2);
		else if (p == VERY_HIGH_PRECISION)
			y += _x2minus * (_x2minus * (_x2minus * (_x2minus * (_x2minus * (_x2minus * (ACOSH_I14 * _x2minus + ACOSH_I12) + ACOSH_I10) + ACOSH_I8) + ACOSH_I6) + ACOSH_I4) + ACOSH_I2);
	}

	return y;
}

//Approximation of the Gamma Function
float GAMMA(const float x, const enum FastMethod fm)
{
	if (x == 1.0f || x == 2.0f)
		return 1.0f;

	if (FM_DEBUG) {
		assert (fm == STL || fm == BOOST);
		//Gamma(0) is undefined
		assert (x != 0.0f);
		assert (!(x > 0.0f && ABS(floor(x) - x, STL) < FM_TOL));
		assert (!(x < 0.0f && ABS(ceil(x) - x, STL) < FM_TOL));
		//Gamma not defined for negative integers
		assert (!(x < 0.0f && ABS(x - round(x), STL) < FM_TOL));
	}

	float y = 0.0f;

	if (x > 10.0f) {
		//Use Stirling's Approximation
		y = 1.0f + 1.0f / (12.0f * x) + 1.0f / (288.0f * POW2(x, EXACT));
		y *= SQRT(2.0f * M_PI * x, STL) * POW(x / M_E, x, STL);
	} else if (fm == STL)
		//Defined in <math.h>
		y = tgamma(x);
	else if (fm == BOOST)
		//Lanczos Approximation
		//Defined in Boost library
		y = boost::math::tgamma1pm1(x);

	return y;
}

//Approximation of ln(gamma(x))
float LOGGAMMA(const float x, const enum FastMethod fm)
{
	if (x == 1.0f || x == 2.0f)
		return 0.0f;

	if (FM_DEBUG) {
		assert (fm == STL || fm == BOOST);
		//Gamma(0) is undefined
		assert (x != 0.0f);
		assert (!(x > 0.0f && ABS(floor(x) - x, STL) < FM_TOL));
		assert (!(x < 0.0f && ABS(ceil(x) - x, STL) < FM_TOL));
		//Gamma not defined for negative integers
		assert (!(x < 0.0f && ABS(x - round(x), STL) < FM_TOL));
		//Log not defined for negative numbers
		assert (!(x < 0.0f && static_cast<int>(-1 * floor(x)) % 2 == 1));
	}

	float y = 0.0f;

	if (x > 10.0f) {
		//Use Stirling's Approximation
		y = x * LOG(x, STL) - x + 0.5f * LOG(2.0f * M_PI * x, STL);
	} else if (fm == STL)
		//Defined in <math.h>
		y = LOG(tgamma(x), STL);
	else if (fm == BOOST)
		//Lanczos Approximation
		//Defined in Boost library
		y = LOG(boost::math::tgamma1pm1(x), STL);

	return y;
}

//Approximation of the Pochhammer symbol (x)_j = gamma(x+j)/gamma(x)
//The coefficient j must be a non-negative integer by definition
float POCHHAMMER(const float x, const int j)
{
	if (j == 0)
		return 1.0f;

	if (FM_DEBUG)
		assert (j >= 0);

	float y = 0.0f;

	if (x > 10.0f || j > 10)
		y = exp(LOGGAMMA(x + j, STL) - LOGGAMMA(x, STL));
	else
		y = GAMMA(x + j, STL) / GAMMA(x, STL);

	return y;
}

//Approximates the Gauss Hypergeometric Function sol=2F1(a,b,c,z(x))
//The function z(x) and its argument x must both be real-valued
//The solution is stored in the memory location at 'sol'
//The actual error is stored in the memory location at 'err'
//The number of terms used in the power series is given by 'nterms'
void _2F1(float (*z)(const float &x, void * const param), const float &x, void * const param, const float a, const float b, const float c, float * const sol, float * const err, int * const nterms)
{
	if (FM_DEBUG) {
		//No null pointers
		assert (z != NULL);
		assert (sol != NULL);
		assert (err != NULL);
	}
	
	//Series will not converge in this region
	if (ABS((*z)(x, param), STL) >= 1.0f) {
		*sol = NAN;
		*err = NAN;
	}

	if (*nterms == -1) {
		*nterms = static_cast<int>(LOG(*err, STL) / LOG(ABS((*z)(x, param), STL), STL));
	}

	int i;
	*sol = 0.0f;
	for (i = 0; i < *nterms; i++) {
		if (i < 15)
			*sol += POCHHAMMER(a, i) * POCHHAMMER(b, i) * POW((*z)(x, param), static_cast<float>(i), STL) / (POCHHAMMER(c, i) * GAMMA(i + 1, STL));
		else
			*sol += POW((*z)(x, param), static_cast<float>(i), STL) * exp(LOGGAMMA(a + i, STL) - LOGGAMMA(a, STL) + LOGGAMMA(b + i, STL) - LOGGAMMA(b, STL) - LOGGAMMA(c + i, STL) + LOGGAMMA(c, STL) - LOGGAMMA(i + 1, STL));
	}

	//Estimate the magnitude of the error
	*err = ABS(POW((*z)(x, param), static_cast<float>(*nterms), FAST), STL);
}
