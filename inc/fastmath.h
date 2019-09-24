#ifndef FASTMATH_H
#define FASTMATH_H

#ifdef AVX2_ENABLED
#include <x86intrin.h>
#endif

#include <algorithm>
#include <assert.h>
#include <cstring>
#define __STDC_FORMAT_MACROS
#include <inttypes.h>
#include <limits.h>
#include <math.h>
#include <new>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>

/////////////////////////////
//(C) Will Cunningham 2014 //
//         DK Lab          //
// Northeastern University //
/////////////////////////////

namespace fastmath {

//Used for Unit Testing
#define FM_DEBUG false

//Machine Epsilon
#define FM_TOL (1e-28)

//Useful Constants
#define HALF_PI  1.57079632679489661923
#define TWO_PI   6.28318530717958647692
#define SQRT_PI  1.77245385090551602729

//ACOS Chebyshev Series Coefficients
#define ACOS_C0  HALF_PI
#define ACOS_C1 -1.06305396909634217923
#define ACOS_C3  0.88385729242991187525
#define ACOS_C5 -4.69522239734719040073
#define ACOS_C7  7.39114112136511672686
#define ACOS_C9 -4.02406572163211910684

//ACOS Integration Series Coefficients
#define ACOS_I0   HALF_PI
#define ACOS_I1  -1.00000000000000000000
#define ACOS_I3  -0.16666666666666666666
#define ACOS_I5  -0.07500000000000000000
#define ACOS_I7  -0.04464285714285714285
#define ACOS_I9  -0.03038194444444444444
#define ACOS_I11 -0.02237215909090909090
#define ACOS_I13 -0.01735276442307692307
#define ACOS_I15 -0.01396484375000000000

//ATAN Chebyshev Series Coefficients
#define ATAN_C1  1.04538834220118418960
#define ATAN_C3 -0.39082098431982330905
#define ATAN_C5  0.17944049001227966481
#define ATAN_C7 -0.08419846479405229950
#define ATAN_C9  0.02041955547722351862

//ATAN Integration Series Coefficients (x < 0)
#define ATAN_H1   1.00000000000000000000
#define ATAN_H3  -0.33333333333333333333
#define ATAN_H5   0.20000000000000000000
#define ATAN_H7  -0.14285714285714285714
#define ATAN_H9   0.11111111111111111111
#define ATAN_H11 -0.09090909090909090909
#define ATAN_H13  0.07692307692307692307
#define ATAN_H15 -0.06666666666666666666

#define ATAN_I0   HALF_PI
#define ATAN_I1  -1.00000000000000000000
#define ATAN_I3   0.33333333333333333333
#define ATAN_I5  -0.20000000000000000000
#define ATAN_I7   0.14285714285714285714
#define ATAN_I9  -0.11111111111111111111
#define ATAN_I11  0.09090909090909090909
#define ATAN_I13 -0.07692307692307692307
#define ATAN_I15  0.06666666666666666666

//ASINH Integration Series Coefficients
#define ASINH_I1   1.00000000000000000000
#define ASINH_I3  -0.16666666666666666666
#define ASINH_I5   0.07500000000000000000
#define ASINH_I7  -0.04464285714285714285
#define ASINH_I9   0.03038194444444444444
#define ASINH_I11 -0.02237215909090909090
#define ASINH_I13  0.01735276442307692307
#define ASINH_I15 -0.01396484375000000000

//ACOSH Integration Series Coefficients
#define ACOSH_I2  -0.25000000000000000000
#define ACOSH_I4  -0.09375000000000000000
#define ACOSH_I6  -0.10416666666666666666
#define ACOSH_I8  -0.06835937500000000000
#define ACOSH_I10 -0.04921875000000000000
#define ACOSH_I12 -0.03759765625000000000
#define ACOSH_I14 -0.02992466517857142857

#define sec(x) (1.0/cos(x))
#define asec(x) acos(1.0/x)

//Used for SGN function
static const double table[] = {-1.0, 1.0};

//Method of Calculation
enum FastMethod {
	DEF = 0,
	STL = 0,	//STL from <math.h>
	FAST = 1,	//fastX from fastapprox.h
	FASTER = 2,	//fasterX from fastapprox.h
	BITWISE = 3,	//Bitwise operation
	CHEBYSHEV = 4,	//Chebyshev series approximation
	INTEGRATION = 5,//Integration of binomial approximation
	BOOST = 6,	//Boost library function
	EXACT = 7	//Exact solution
};

//Defines Number of Terms in Series for
//Selected Power Series Approximations
enum Precision {
	DEFAULT = 0,
	LOW_PRECISION = 0,
	HIGH_PRECISION = 1,
	VERY_HIGH_PRECISION = 2
};

//Type of transformation used for 2F1
struct HyperType {
	HyperType() : w(0.0), type(0) {}

	double w;
	int type;
};

//Power Functions
double POW2(const double x);
double POW2(const double x, const enum FastMethod fm);
double POW3(const double x);
double POW3(const double x, const enum FastMethod fm);
double POW(const double x, const double p, const enum FastMethod fm);
double SQRT(const double x, const enum FastMethod fm);

//Absolute Value, Natural Log, and Sign
double ABS(const double x, const enum FastMethod fm);
double LOG(const double x, const enum FastMethod fm);
double SGN(const double x);
double SGN(const double x, const enum FastMethod fm);

//Trigonometric Functions
double SIN(const double x, const enum FastMethod fm);
double COS(const double x, const enum FastMethod fm);
double TAN(const double x, const enum FastMethod fm);

//Inverse Trigonometric Functions
double ACOS(const double x, const enum FastMethod fm, const enum Precision p);
double ATAN(const double x, const enum FastMethod fm, const enum Precision p);

//Hyperbolic Functions
double SINH(const double x, const enum FastMethod fm);
double COSH(const double x, const enum FastMethod fm);

//Inverse Hyperbolic Functions
double ASINH(const double x, const enum FastMethod fm, const enum Precision p);
double ACOSH(const double x, const enum FastMethod fm, const enum Precision p);

//Statistical Functions
double GAMMA(const double x);
double GAMMA(const double x, const enum FastMethod fm);
double LOGGAMMA(const double x);
double LOGGAMMA(const double x, const enum FastMethod fm);
double GAMMA_RATIO(const double x, const double y);
double POCHHAMMER(const double x, const int j);
double POCHHAMMER_F(const double x, const int j);

//Gauss Hypergeometric Function
double _2F1_An(const double a, const double b, const double c, const int n);
double _2F1_An_F(const double a, const double b, const double c, const int n);
void _2F1(const double a, const double b, const double c, const double z, double * const sol, double * const err, int * const nterms, const bool check);
void _2F1_F(const double a, const double b, const double c, const double z, double * const sol, double * const err, int * const nterms);
int getNumTerms(const double &z, const double &err);

//Hypergeometric Transformation Type
HyperType getHyperType(const double &z);

}

#endif
