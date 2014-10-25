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
double POW2(const double x, const enum FastMethod fm)
{
	if (x == 0.0)
		return x;

	if (FM_DEBUG)
		assert (fm == STL || fm == FAST || fm == FASTER || fm == EXACT);

	double y = 0.0;

	if (fm == STL)
		//Defined in <math.h>
		y = pow(x, 2.0);
	else if (fm == FAST)
		//Defined in "fastapprox.h"
		y = static_cast<double>(fastpow(static_cast<float>(x), 2.0f));
	else if (fm == FASTER)
		//Defined in "fastapprox.h"
		y = static_cast<double>(fasterpow(static_cast<float>(x), 2.0f));
	else if (fm == EXACT)
		y = x * x;

	return y;
}

//Approximation of x^3
double POW3(const double x, const enum FastMethod fm)
{
	if (x == 0.0)
		return x;

	if (FM_DEBUG)
		assert (fm == STL || fm == FAST || fm == FASTER || fm == EXACT);

	double y = 0.0;

	if (fm == STL)
		//Defined in <math.h>
		y = pow(x, 3.0);
	else if (fm == FAST)
		//Defined in "fastapprox.h"
		y = static_cast<double>(fastpow(static_cast<float>(x), 3.0f));
	else if (fm == FASTER)
		//Defined in "fastapprox.h"
		y = static_cast<double>(fasterpow(static_cast<float>(x), 3.0f));
	else if (fm == EXACT)
		y = x * x * x;

	return y;
}

//Approximation of x^p
double POW(const double x, const double p, const enum FastMethod fm)
{
	if (FM_DEBUG)
		assert (fm == STL || fm == FAST || fm == FASTER);

	double y = 0.0;

	if (fm == STL)
		//Defined in <math.h>
		y = pow(x, p);
	else if (fm == FAST)
		//Defined in "fastapprox.h"
		y = static_cast<double>(fastpow(static_cast<float>(x), static_cast<float>(p)));
	else if (fm == FASTER)
		//Defined in "fastapprox.h"
		y = static_cast<double>(fasterpow(static_cast<float>(x), static_cast<float>(p)));

	return y;
}

//Approximation of x^(1/2)
double SQRT(const double x, const enum FastMethod fm)
{
	if (x == 0.0)
		return x;

	if (FM_DEBUG) {
		assert (fm == STL || fm == BITWISE);
		assert (x > 0.0);
	}

	double y = 0.0;

	if (fm == STL)
		//Defined in <math.h>
		y = sqrt(x);
	else if (fm == BITWISE) {
		//Bit shift approximation
		float z = static_cast<float>(x);
		unsigned int i = *(unsigned int*) &z;
		i += 127 << 23;
		i >>= 1;
		y = static_cast<double>(*(float*) &i);
	}

	return y;
}

//Approximation of |x|
double ABS(const double x, const enum FastMethod fm)
{
	if (x == 0.0)
		return x;

	if (FM_DEBUG)
		assert (fm == STL || fm == BITWISE);
	
	double y = 0.0;

	if (fm == STL)
		//Defined in <math.h>
		y = fabs(x);
	else if (fm == BITWISE) {
		//Bitwise operation
		float z = static_cast<float>(x);
		int i = *(int*) &z;
		i &= 0x7FFFFFFF;
		y = static_cast<double>(*(float*) &i);
	}

	return y;
}

//Approximation of ln(x)
double LOG(const double x, const enum FastMethod fm)
{
	if (FM_DEBUG) {
		assert (fm == STL || fm == FAST || fm == FASTER);
		assert (x > 0.0);
	}

	double y = 0.0;

	if (fm == STL)
		//Defined int <math.h>
		y = log(x);
	else if (fm == FAST)
		//Defined in "fastapprox.h"
		y = static_cast<double>(fastlog(static_cast<float>(x)));
	else if (fm == FASTER)
		//Defined in "fastapprox.h"
		y = static_cast<double>(fasterlog(static_cast<float>(x)));

	return y;
}

//Returns sign(x)
double SGN(const double x, const enum FastMethod fm)
{	
	if (FM_DEBUG)
		assert (fm == DEF || fm == BITWISE);

	if (ABS(round(x) - x, STL) < FM_TOL)
		return 0.0;

	double y = 1.0;

	if (fm == DEF);
		if (y < 0.0)
			y = -1.0;
	else if (fm == BITWISE) {
		float z = static_cast<float>(y);
		(int&)z |= ((int&)z & 0x80000000);
		y = static_cast<double>(z);
	}

	return y;
}

//Approximation of sine(x)
double SIN(const double x, const enum FastMethod fm)
{
	if (x == 0.0)
		return x;

	if (FM_DEBUG)
		assert (fm == STL || fm == FAST || fm == FASTER);

	double y = 0.0;

	if (fm == STL)
		//Defined in <math.h>
		y = sin(x);
	else if (fm == FAST) {
		//Defined in "fastapprox.h"
		if (FM_DEBUG)
			assert (ABS(x, STL) < M_PI);
		y = static_cast<double>(fastsin(static_cast<float>(x)));
	} else if (fm == FASTER) {
		//Defined in "fastapprox.h"
		if (FM_DEBUG)
			assert (ABS(x, STL) < M_PI);
		y = static_cast<double>(fastersin(static_cast<float>(x)));
	}

	return y;
}

//Approximation of cosine(x)
double COS(const double x, const enum FastMethod fm)
{
	if (x == 0.0)
		return 1.0;

	if (FM_DEBUG)
		assert (fm == STL || fm == FAST || fm == FASTER);

	double y = 0.0;

	if (fm == STL)
		//Defined in <math.h>
		y = cos(x);
	else if (fm == FAST) {
		//Defined in "fastapprox.h"
		if (FM_DEBUG)
			assert (ABS(x, STL) < M_PI);
		y = static_cast<double>(fastcos(static_cast<float>(x)));
	} else if (fm == FASTER) {
		//Defined in "fastapprox.h"
		if (FM_DEBUG)
			assert (ABS(x, STL) < M_PI);
		y = static_cast<double>(fastercos(static_cast<float>(x)));
	}

	return y;
}

//Approximation of tangent(x)
double TAN(const double x, const enum FastMethod fm)
{
	if (x == 0.0)
		return x;

	if (FM_DEBUG)
		assert (fm == STL || fm == FAST || fm == FASTER);

	double y = 0.0;

	if (fm == STL)
		//Defined in <math.h>
		y = tan(x);
	else if (fm == FAST) {
		//Defined in "fastapprox.h"
		if (FM_DEBUG)
			assert (ABS(x, STL) < HALF_PI);
		y = static_cast<double>(fasttan(static_cast<float>(x)));
	} else if (fm == FASTER) {
		//Defined in "fastapprox.h"
		if (FM_DEBUG)
			assert (ABS(x, STL) < HALF_PI);
		y = static_cast<double>(fastertan(static_cast<float>(x)));
	}

	return y;
}

//Approximation of arccosine(x)
double ACOS(const double x, const enum FastMethod fm, const enum Precision p)
{
	if (x == 0.0)
		return HALF_PI;

	if (FM_DEBUG) {
		assert (fm == STL || fm == CHEBYSHEV || fm == INTEGRATION);
		assert (ABS(x, STL) < 1.0);
	}

	double y = 0.0;

	if (fm == STL)
		//Defined in <math.h>
		y = acos(x);
	else if (fm == CHEBYSHEV) {
		//Chebyshev Approximation
		double _x2 = POW2(x, EXACT);
		if (p == LOW_PRECISION)
			y = ACOS_C0 + x * (ACOS_C3 * _x2 + ACOS_C1);
		else if (p == HIGH_PRECISION)
			y = ACOS_C0 + x * (_x2 * (ACOS_C5 * _x2 + ACOS_C3) + ACOS_C1);
		else if (p == VERY_HIGH_PRECISION)
			y = ACOS_C0 + x * (_x2 * (_x2 * (_x2 * (ACOS_C9 * _x2 + ACOS_C7) + ACOS_C5) + ACOS_C3) + ACOS_C1);
	} else if (fm == INTEGRATION) {
		//Series from Integration (for |x| < 1)
		double _x2 = POW2(x, EXACT);
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
double ATAN(const double x, const enum FastMethod fm, const enum Precision p)
{
	if (x == 0.0)
		return x;
	else if (ABS(x, STL) == 1.0)
		//Integration series not valid for |x| = 1.0
		return HALF_PI / 2.0;

	if (FM_DEBUG)
		assert (fm == STL || fm == CHEBYSHEV || fm == INTEGRATION);
	
	double y = 0.0;

	if (fm == STL)
		//Defined in <math.h>
		y = atan(x);
	else if (fm == CHEBYSHEV) {
		//Chebyshev Approximation
		double _x2 = POW2(x, EXACT);
		if (p == LOW_PRECISION)
			y = x * (ATAN_C3 * _x2 + ATAN_C1);
		else if (p == HIGH_PRECISION)
			y = x * (_x2 * (ATAN_C5 * _x2 + ATAN_C3) + ATAN_C1);
		else if (p == VERY_HIGH_PRECISION)
			y = x * (_x2 * (_x2 * (_x2 * (ATAN_C9 * _x2 + ATAN_C7) + ATAN_C5) + ATAN_C3) + ATAN_C1);
	} else if (fm == INTEGRATION) {
		//Series from Integration (for x != 1.0)
		if (ABS(x, STL) < 1.0) {
			double _x2 = POW2(x, EXACT);
			if (p == LOW_PRECISION)
				y = x * (_x2 * (ATAN_H5 * _x2 + ATAN_H3) + ATAN_H1);
			else if (p == HIGH_PRECISION)
				y = x * (_x2 * (_x2 * (_x2 * (ATAN_H9 * _x2 + ATAN_H7) + ATAN_H5) + ATAN_H3) + ATAN_H1);
			else if (p == VERY_HIGH_PRECISION)
				y = x * (_x2 * (_x2 * (_x2 * (_x2 * (_x2 * (_x2 * (ATAN_H15 * _x2 + ATAN_H13) + ATAN_H11) + ATAN_H9) + ATAN_H7) + ATAN_H5) + ATAN_H3) + ATAN_H1);
		} else {
			double _x2minus = 1.0 / POW2(x, EXACT);
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
double SINH(const double x, const enum FastMethod fm)
{
	if (x == 0.0)
		return x;

	if (FM_DEBUG)
		assert (fm == STL || fm == FAST || fm == FASTER);

	double y = 0.0;

	if (fm == STL)
		//Defined in <math.h>
		y = sinh(x);
	else if (fm == FAST)
		//Defined in "fastapprox.h"
		y = static_cast<double>(fastsinh(static_cast<float>(x)));
	else if (fm == FASTER)
		//Defined in "fastapprox.h"
		y = static_cast<double>(fastersinh(static_cast<float>(x)));

	return y;
}

//Approximation of cosh(x)
double COSH(const double x, const enum FastMethod fm)
{
	if (x == 0.0)
		return 1.0;

	if (FM_DEBUG)
		assert (fm == STL || fm == FAST || fm == FASTER);

	double y = 0.0;

	if (fm == STL)
		//Defined in <math.h>
		y = cosh(x);
	else if (fm == FAST)
		//Defined in "fastapprox.h"
		y = static_cast<double>(fastcosh(static_cast<float>(x)));
	else if (fm == FASTER)
		//Defined in "fastapprox.h"
		y = static_cast<double>(fastercosh(static_cast<float>(x)));

	return y;
}

//Approximation of arcsinh(x)
double ASINH(const double x, const enum FastMethod fm, const enum Precision p)
{
	if (x == 0.0)
		return x;

	if (FM_DEBUG)
		assert (fm == STL || fm == INTEGRATION);

	double y = 0.0;

	if (fm == STL)
		//Defined in <math.h>
		y = asinh(x);
	else if (fm == INTEGRATION) {
		//Series from Integration (for |x| < 1)
		if (FM_DEBUG)
			assert (ABS(x, STL) < 1.0);
		double _x2 = POW2(x, EXACT);
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
double ACOSH(const double x, const enum FastMethod fm, const enum Precision p)
{
	if (x == 1.0)
		return 0.0;

	if (FM_DEBUG)
		assert (fm == STL || fm == INTEGRATION);

	double y = 0.0;

	if (fm == STL)
		//Defined in <math.h>
		y = acosh(x);
	else if (fm == INTEGRATION) {
		//Series from Integration (for x > 1)
		if (FM_DEBUG)
			assert (ABS(x, STL) > 1.0);
		double _x2minus = 1.0 / POW2(x, EXACT);
		y += LOG(2.0 * x, STL);
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
double GAMMA(const double x, const enum FastMethod fm)
{
	if (x == 1.0 || x == 2.0)
		return 1.0;

	if (FM_DEBUG) {
		assert (fm == STL || fm == BOOST);
		//Gamma(0) is undefined
		assert (x != 0.0);
		assert (!(x > 0.0 && ABS(floor(x) - x, STL) < FM_TOL));
		assert (!(x < 0.0 && ABS(ceil(x) - x, STL) < FM_TOL));
		//Gamma not defined for negative integers
		assert (!(x < 0.0 && ABS(x - round(x), STL) < FM_TOL));
	}

	double y = 0.0;

	if (x > 10.0) {
		//Use Stirling's Approximation
		y = 1.0 + 1.0 / (12.0 * x) + 1.0 / (288.0 * POW2(x, EXACT));
		y *= SQRT(2.0 * M_PI * x, STL) * POW(x / M_E, x, STL);
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
double LOGGAMMA(const double x, const enum FastMethod fm)
{
	if (x == 1.0 || x == 2.0)
		return 0.0;

	if (FM_DEBUG) {
		assert (fm == STL || fm == BOOST);
		//Gamma(0) is undefined
		assert (x != 0.0);
		assert (!(x > 0.0 && ABS(floor(x) - x, STL) < FM_TOL));
		assert (!(x < 0.0 && ABS(ceil(x) - x, STL) < FM_TOL));
		//Gamma not defined for negative integers
		assert (!(x < 0.0 && ABS(x - round(x), STL) < FM_TOL));
		//Log not defined for negative numbers
		assert (!(x < 0.0 && static_cast<int>(-1 * floor(x)) % 2 == 1));
	}

	double y = 0.0;

	if (x > 10.0) {
		//Use Stirling's Approximation
		y = x * LOG(x, STL) - x + 0.5 * LOG(2.0 * M_PI * x, STL);
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
double POCHHAMMER(const double x, const int j)
{
	if (j == 0)
		return 1.0;

	if (FM_DEBUG)
		assert (j >= 0);

	double y = 0.0;

	if (x > 10.0 || j > 10)
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
void _2F1(double (*z)(const double &x, void * const param), const double &x, void * const param, const double a, const double b, const double c, double * const sol, double * const err, int * const nterms)
{
	if (FM_DEBUG) {
		//No null pointers
		assert (z != NULL);
		assert (sol != NULL);
		assert (err != NULL);
	}
	
	//Series will not converge in this region
	if (ABS((*z)(x, param), STL) >= 1.0) {
		*sol = NAN;
		*err = NAN;
	}

	if (*nterms == -1) {
		*nterms = static_cast<int>(LOG(*err, STL) / LOG(ABS((*z)(x, param), STL), STL));
	}

	int i;
	*sol = 0.0;
	for (i = 0; i < *nterms; i++) {
		if (i < 15)
			*sol += POCHHAMMER(a, i) * POCHHAMMER(b, i) * POW((*z)(x, param), static_cast<double>(i), STL) / (POCHHAMMER(c, i) * GAMMA(static_cast<double>(i + 1), STL));
		else
			*sol += POW((*z)(x, param), static_cast<double>(i), STL) * exp(LOGGAMMA(a + i, STL) - LOGGAMMA(a, STL) + LOGGAMMA(b + i, STL) - LOGGAMMA(b, STL) - LOGGAMMA(c + i, STL) + LOGGAMMA(c, STL) - LOGGAMMA(i + 1, STL));
	}

	//Estimate the magnitude of the error
	*err = ABS(POW((*z)(x, param), static_cast<double>(*nterms), FAST), STL);
}
