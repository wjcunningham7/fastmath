#ifndef FAST_MATH_H_
#define FAST_MATH_H_

#include <assert.h>
#include <math.h>
#include <stdio.h>

/////////////////////////////
//(C) Will Cunningham 2014 //
// Northeastern University //
// Krioukov Research Group //
/////////////////////////////

//Used for Unit Testing
#define FM_DEBUG false

//Machine Epsilon
#define FM_TOL (1e-28)

//Useful Constants
#define HALF_PI  1.57079632679489661923f
#define TWO_PI   6.28318530717958647692f

//ACOS Chebyshev Series Coefficients
#define ACOS_C0  HALF_PI
#define ACOS_C1 -1.06305396909634217923f
#define ACOS_C3  0.88385729242991187525f
#define ACOS_C5 -4.69522239734719040073f
#define ACOS_C7  7.39114112136511672686f
#define ACOS_C9 -4.02406572163211910684f

//ACOS Integration Series Coefficients
#define ACOS_I0   HALF_PI
#define ACOS_I1  -1.00000000000000000000f
#define ACOS_I3  -0.16666666666666666666f
#define ACOS_I5  -0.07500000000000000000f
#define ACOS_I7  -0.04464285714285714285f
#define ACOS_I9  -0.03038194444444444444f
#define ACOS_I11 -0.02237215909090909090f
#define ACOS_I13 -0.01735276442307692307f
#define ACOS_I15 -0.01396484375000000000f

//ATAN Chebyshev Series Coefficients
#define ATAN_C1  1.04538834220118418960f
#define ATAN_C3 -0.39082098431982330905f
#define ATAN_C5  0.17944049001227966481f
#define ATAN_C7 -0.08419846479405229950f
#define ATAN_C9  0.02041955547722351862f

//ATAN Integration Series Coefficients (x < 0)
#define ATAN_H1   1.00000000000000000000f
#define ATAN_H3  -0.33333333333333333333f
#define ATAN_H5   0.20000000000000000000f
#define ATAN_H7  -0.14285714285714285714f
#define ATAN_H9   0.11111111111111111111f
#define ATAN_H11 -0.09090909090909090909f
#define ATAN_H13  0.07692307692307692307f
#define ATAN_H15 -0.06666666666666666666f

#define ATAN_I0   HALF_PI
#define ATAN_I1  -1.00000000000000000000f
#define ATAN_I3   0.33333333333333333333f
#define ATAN_I5  -0.20000000000000000000f
#define ATAN_I7   0.14285714285714285714f
#define ATAN_I9  -0.11111111111111111111f
#define ATAN_I11  0.09090909090909090909f
#define ATAN_I13 -0.07692307692307692307f
#define ATAN_I15  0.06666666666666666666f

#define ASINH_I1   1.00000000000000000000f
#define ASINH_I3  -0.16666666666666666666f
#define ASINH_I5   0.07500000000000000000f
#define ASINH_I7  -0.04464285714285714285f
#define ASINH_I9   0.03038194444444444444f
#define ASINH_I11 -0.02237215909090909090f
#define ASINH_I13  0.01735276442307692307f
#define ASINH_I15 -0.01396484375000000000f

#define ACOSH_I2  -0.25000000000000000000f
#define ACOSH_I4  -0.09375000000000000000f
#define ACOSH_I6  -0.10416666666666666666f
#define ACOSH_I8  -0.06835937500000000000f
#define ACOSH_I10 -0.04921875000000000000f
#define ACOSH_I12 -0.03759765625000000000f
#define ACOSH_I14 -0.02992466517857142857f

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

//Power Functions
float POW2(const float x, const enum FastMethod fm);
float POW3(const float x, const enum FastMethod fm);
float POW(const float x, const float p, const enum FastMethod fm);
float SQRT(const float x, const enum FastMethod fm);

//Absolute Value, Natural Log, and Sign
float ABS(const float x, const enum FastMethod fm);
float LOG(const float x, const enum FastMethod fm);
float SGN(const float x, const enum FastMethod fm);

//Trigonometric Functions
float SIN(const float x, const enum FastMethod fm);
float COS(const float x, const enum FastMethod fm);
float TAN(const float x, const enum FastMethod fm);

//Inverse Trigonometric Functions
float ACOS(const float x, const enum FastMethod fm, const enum Precision p);
float ATAN(const float x, const enum FastMethod fm, const enum Precision p);

//Hyperbolic Functions
float SINH(const float x, const enum FastMethod fm);
float COSH(const float x, const enum FastMethod fm);

//Inverse Hyperbolic Functions
float ASINH(const float x, const enum FastMethod fm, const enum Precision p);
float ACOSH(const float x, const enum FastMethod fm, const enum Precision p);

//Statistical Functions
float GAMMA(const float x, const enum FastMethod fm);
float LOGGAMMA(const float x, const enum FastMethod fm);
float POCHHAMMER(const float x, const int j);

//Gauss Hypergeometric Function
void _2F1(float (*z)(const float &x, void * const param), const float &x, void * const param, const float a, const float b, const float c, float * const sol, float * const err, int * const nterms);

#endif
