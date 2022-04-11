/* Copyright 2014-2022 Will Cunningham
 * 
 * This file is part of FastMath.
 *
 * Licensed under the GNU General Public License 3.0 (the "License").
 * A copy of the License may be obtained with this software package or at
 *
 *      https://www.gnu.org/licenses/gpl-3.0.en.html
 *
 * Use of this file is prohibited except in compliance with the License. Any
 * modifications or derivative works of this file must retain this copyright
 * notice, and modified files must contain a notice indicating that they have
 * been altered from the originals.
 *
 * FastMath is distributed in the hope that it will be useful, but WITHOUT 
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or 
 * FITNESS FOR A PARTICULAR PURPOSE. See the License for more details. */

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
#include <sstream>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include <boost/math/special_functions/gamma.hpp>

#include "fastapprox.h"

namespace fastmath {

// Used for Unit Testing
#define FM_DEBUG false

// Machine Epsilon
#define FM_TOL (1e-28)

// Useful Constants
#define HALF_PI 1.57079632679489661923
#define TWO_PI 6.28318530717958647692
#define SQRT_PI 1.77245385090551602729

// ACOS Chebyshev Series Coefficients
#define ACOS_C0 HALF_PI
#define ACOS_C1 -1.06305396909634217923
#define ACOS_C3 0.88385729242991187525
#define ACOS_C5 -4.69522239734719040073
#define ACOS_C7 7.39114112136511672686
#define ACOS_C9 -4.02406572163211910684

// ACOS Integration Series Coefficients
#define ACOS_I0 HALF_PI
#define ACOS_I1 -1.00000000000000000000
#define ACOS_I3 -0.16666666666666666666
#define ACOS_I5 -0.07500000000000000000
#define ACOS_I7 -0.04464285714285714285
#define ACOS_I9 -0.03038194444444444444
#define ACOS_I11 -0.02237215909090909090
#define ACOS_I13 -0.01735276442307692307
#define ACOS_I15 -0.01396484375000000000

// ATAN Chebyshev Series Coefficients
#define ATAN_C1 1.04538834220118418960
#define ATAN_C3 -0.39082098431982330905
#define ATAN_C5 0.17944049001227966481
#define ATAN_C7 -0.08419846479405229950
#define ATAN_C9 0.02041955547722351862

// ATAN Integration Series Coefficients (x < 0)
#define ATAN_H1 1.00000000000000000000
#define ATAN_H3 -0.33333333333333333333
#define ATAN_H5 0.20000000000000000000
#define ATAN_H7 -0.14285714285714285714
#define ATAN_H9 0.11111111111111111111
#define ATAN_H11 -0.09090909090909090909
#define ATAN_H13 0.07692307692307692307
#define ATAN_H15 -0.06666666666666666666

#define ATAN_I0 HALF_PI
#define ATAN_I1 -1.00000000000000000000
#define ATAN_I3 0.33333333333333333333
#define ATAN_I5 -0.20000000000000000000
#define ATAN_I7 0.14285714285714285714
#define ATAN_I9 -0.11111111111111111111
#define ATAN_I11 0.09090909090909090909
#define ATAN_I13 -0.07692307692307692307
#define ATAN_I15 0.06666666666666666666

// ASINH Integration Series Coefficients
#define ASINH_I1 1.00000000000000000000
#define ASINH_I3 -0.16666666666666666666
#define ASINH_I5 0.07500000000000000000
#define ASINH_I7 -0.04464285714285714285
#define ASINH_I9 0.03038194444444444444
#define ASINH_I11 -0.02237215909090909090
#define ASINH_I13 0.01735276442307692307
#define ASINH_I15 -0.01396484375000000000

// ACOSH Integration Series Coefficients
#define ACOSH_I2 -0.25000000000000000000
#define ACOSH_I4 -0.09375000000000000000
#define ACOSH_I6 -0.10416666666666666666
#define ACOSH_I8 -0.06835937500000000000
#define ACOSH_I10 -0.04921875000000000000
#define ACOSH_I12 -0.03759765625000000000
#define ACOSH_I14 -0.02992466517857142857

#define sec(x) (1.0 / cos(x))
#define asec(x) acos(1.0 / x)

// Used for SGN function
static const double table[] = {-1.0, 1.0};

// Method of Calculation
enum FastMethod {
    DEF = 0,
    STL = 0,         // STL from <math.h>
    FAST = 1,        // fastX from fastapprox.h
    FASTER = 2,      // fasterX from fastapprox.h
    BITWISE = 3,     // Bitwise approximation
    CHEBYSHEV = 4,   // Chebyshev series approximation
    INTEGRATION = 5, // Integration of binomial approximation
    BOOST = 6,       // Boost library function
    EXACT = 7        // Exact solution
};

// Defines Number of Terms in Series for
// Selected Power Series Approximations
enum Precision {
    DEFAULT = 0,
    LOW_PRECISION = 0,
    HIGH_PRECISION = 1,
    VERY_HIGH_PRECISION = 2
};

// Type of transformation used for 2F1
struct HyperType {
    HyperType() : w(0.0), type(0) {}

    double w;
    int type;
};

/////////////////////
// Power Functions //
/////////////////////

inline double POW2(const double x) { return x * x; }

inline double POW2(const double x, const enum FastMethod fm) {
#if FM_DEBUG
    assert(fm == STL || fm == FAST || fm == FASTER || fm == EXACT);
#endif

    double y;
    if (__builtin_expect(!x, 0L))
        y = 0.0;
    else {
        switch (fm) {
        case EXACT:
            y = x * x;
            break;
        case FAST:
            y = static_cast<double>(fastpow(static_cast<float>(x), 2.0f));
            break;
        case STL:
            y = pow(x, 2.0);
            break;
        case FASTER:
            y = static_cast<double>(fasterpow(static_cast<float>(x), 2.0f));
            break;
        default:
            y = NAN;
            break;
        }
    }

    return y;
}

inline double POW3(const double x) { return x * x * x; }

inline double POW3(const double x, const enum FastMethod fm) {
#if FM_DEBUG
    assert(fm == STL || fm == FAST || fm == FASTER || fm == EXACT);
#endif

    double y;
    if (__builtin_expect(!x, 0L))
        y = 0.0;
    else {

        switch (fm) {
        case EXACT:
            y = x * x * x;
            break;
        case FAST:
            y = static_cast<double>(fastpow(static_cast<float>(x), 3.0f));
            break;
        case STL:
            y = pow(x, 3.0);
            break;
        case FASTER:
            y = static_cast<double>(fasterpow(static_cast<float>(x), 3.0f));
            break;
        default:
            y = NAN;
            break;
        }
    }

    return y;
}

inline double POW(const double x, const double p, const enum FastMethod fm) {
#if FM_DEBUG
    assert(fm == STL || fm == FAST || fm == FASTER);
#endif

    double y;
    switch (fm) {
    case STL:
        y = pow(x, p);
        break;
    case FAST:
        y = static_cast<double>(
            fastpow(static_cast<float>(x), static_cast<float>(p)));
        break;
    case FASTER:
        break;
        y = static_cast<double>(
            fasterpow(static_cast<float>(x), static_cast<float>(p)));
    default:
        y = NAN;
        break;
    }

    return y;
}

inline double SQRT(const double x, const enum FastMethod fm) {
#if FM_DEBUG
    assert(fm == STL || fm == BITWISE);
    assert(x >= 0.0);
#endif

    double y;
    float z;
    unsigned int i;

    if (__builtin_expect(!x, 0L))
        y = 0.0;
    else {
        switch (fm) {
        case STL:
            y = sqrt(x);
            break;
        case BITWISE:
            // NOTE: Not rigorously tested
            z = static_cast<float>(x);
            i = *(unsigned int *)&z;
            i += 127 << 23;
            i >>= 1;
            y = static_cast<double>(*(float *)&i);
            break;
        default:
            y = NAN;
            break;
        }
    }

    return y;
}

///////////////////////////////////////////
// Absolute Value, Natural Log, and Sign //
///////////////////////////////////////////

inline double ABS(const double x, const enum FastMethod fm) {
#if FM_DEBUG
    assert(fm == STL || fm == BITWISE);
#endif

    double y;
    float z;
    int i;

    if (__builtin_expect(!x, 0L))
        y = 0.0;
    else {
        switch (fm) {
        case STL:
            y = fabs(x);
            break;
        case BITWISE:
            // NOTE: Not rigorously tested
            z = static_cast<float>(x);
            i = *(int *)&z;
            i &= 0x7FFFFFFF;
            y = static_cast<double>(*(float *)&i);
            break;
        default:
            y = NAN;
            break;
        }
    }

    return y;
}

inline double LOG(const double x, const enum FastMethod fm) {
#if FM_DEBUG
    assert(fm == STL || fm == FAST || fm == FASTER);
    assert(x > 0.0);
#endif

    double y;
    switch (fm) {
    case STL:
        y = log(x);
        break;
    case FAST:
        y = static_cast<double>(fastlog(static_cast<float>(x)));
        break;
    case FASTER:
        y = static_cast<double>(fasterlog(static_cast<float>(x)));
        break;
    default:
        y = NAN;
        break;
    }

    return y;
}

inline double SGN(const double x) { return table[x > 0.0]; }

inline double SGN(const double x, const enum FastMethod fm) {
#if FM_DEBUG
    assert(fm == DEF || fm == BITWISE);
#endif

    double y;
    float z;

    if (__builtin_expect(!x, 0L))
        y = 0.0;
    else {
        switch (fm) {
        case DEF:
            y = table[x > 0.0];
            break;
        case BITWISE:
            // NOTE: Not rigorously tested
            z = static_cast<float>(x);
            (int &)z |= ((int &)z & 0x80000000);
            y = static_cast<double>(z);
            break;
        default:
            y = NAN;
            break;
        }
    }

    return y;
}

// Trigonometric Functions
inline double SIN(const double x, const enum FastMethod fm) {
#if FM_DEBUG
    assert(fm == STL || fm == FAST || fm == FASTER);
#endif

    double y;
    if (__builtin_expect(!x, 0L))
        y = 0.0;
    else {
        switch (fm) {
        case STL:
            y = sin(x);
            break;
        case FAST:
#if FM_DEBUG
            assert(fabs(x) < M_PI);
#endif
            y = static_cast<double>(fastsin(static_cast<float>(x)));
            break;
        case FASTER:
#if FM_DEBUG
            assert(fabs(x) < M_PI);
#endif
            y = static_cast<double>(fastersin(static_cast<float>(x)));
            break;
        default:
            y = NAN;
            break;
        }
    }

    return y;
}

inline double COS(const double x, const enum FastMethod fm) {
#if FM_DEBUG
    assert(fm == STL || fm == FAST || fm == FASTER);
#endif

    double y;
    if (__builtin_expect(!x, 0L))
        y = 1.0;
    else {
        switch (fm) {
        case STL:
            y = cos(x);
            break;
        case FAST:
#if FM_DEBUG
            assert(fabs(x) < M_PI);
#endif
            y = static_cast<double>(fastcos(static_cast<float>(x)));
            break;
        case FASTER:
#if FM_DEBUG
            assert(fabs(x) < M_PI);
#endif
            y = static_cast<double>(fastercos(static_cast<float>(x)));
            break;
        default:
            y = NAN;
            break;
        }
    }

    return y;
}

inline double TAN(const double x, const enum FastMethod fm) {
#if FM_DEBUG
    assert(fm == STL || fm == FAST || fm == FASTER);
#endif

    double y;
    if (__builtin_expect(!x, 0L))
        y = x;
    else {
        switch (fm) {
        case STL:
            y = tan(x);
            break;
        case FAST:
#if FM_DEBUG
            assert(fabs(x) < HALF_PI);
#endif
            y = static_cast<double>(fasttan(static_cast<float>(x)));
            break;
        case FASTER:
#if FM_DEBUG
            assert(fabs(x) < HALF_PI);
#endif
            y = static_cast<double>(fastertan(static_cast<float>(x)));
            break;
        default:
            y = NAN;
            break;
        }
    }

    return y;
}

/////////////////////////////////////
// Inverse Trigonometric Functions //
/////////////////////////////////////

inline double ACOS(const double x, const enum FastMethod fm, const enum Precision p) {
#if FM_DEBUG
    assert(fm == STL || fm == CHEBYSHEV || fm == INTEGRATION);
    assert(ABS(x, STL) < 1.0);
#endif

    double y;
    double _x2;
    if (__builtin_expect(!x, 0L))
        y = HALF_PI;
    else {
        switch (fm) {
        case STL:
            y = acos(x);
            break;
        case INTEGRATION:
            // Series from Integration (for |x| < 1)
            _x2 = POW2(x);
            if (p == VERY_HIGH_PRECISION)
                y = ACOS_I0 +
                    x * (_x2 *
                             (_x2 *
                                  (_x2 * (_x2 * (_x2 * (_x2 * (ACOS_I15 * _x2 +
                                                               ACOS_I13) +
                                                        ACOS_I11) +
                                                 ACOS_I9) +
                                          ACOS_I7) +
                                   ACOS_I5) +
                              ACOS_I3) +
                         ACOS_I1);
            else if (p == HIGH_PRECISION)
                y = ACOS_I0 +
                    x * (_x2 * (_x2 * (_x2 * (ACOS_I9 * _x2 + ACOS_I7) +
                                       ACOS_I5) +
                                ACOS_I3) +
                         ACOS_I1);
            else if (p == LOW_PRECISION)
                y = ACOS_I0 + x * (_x2 * (ACOS_I5 * _x2 + ACOS_I3) + ACOS_I1);
            else
                y = NAN;
            break;
        case CHEBYSHEV:
            // Chebyshev Approximation
            _x2 = POW2(x);
            if (p == VERY_HIGH_PRECISION)
                y = ACOS_C0 +
                    x * (_x2 * (_x2 * (_x2 * (ACOS_C9 * _x2 + ACOS_C7) +
                                       ACOS_C5) +
                                ACOS_C3) +
                         ACOS_C1);
            else if (p == HIGH_PRECISION)
                y = ACOS_C0 + x * (_x2 * (ACOS_C5 * _x2 + ACOS_C3) + ACOS_C1);
            else if (p == LOW_PRECISION)
                y = ACOS_C0 + x * (ACOS_C3 * _x2 + ACOS_C1);
            else
                y = NAN;
            break;
        default:
            y = NAN;
            break;
        }
    }

    return y;
}

inline double ATAN(const double x, const enum FastMethod fm, const enum Precision p) {
#if FM_DEBUG
    assert(fm == STL || fm == CHEBYSHEV || fm == INTEGRATION);
#endif

    double y;
    double _x2, _x2minus;
    if (__builtin_expect(!x || fabs(x) == 1.0, 0L)) {
        if (!x)
            y = x;
        else
            // Integration series not valid for |x| = 1.0
            y = HALF_PI / 2.0;
    } else {
        switch (fm) {
        case STL:
            y = atan(x);
            break;
        case INTEGRATION:
            // Series from Integration (for x != 1.0)
            if (fabs(x) < 1.0) {
                _x2 = POW2(x);
                if (p == VERY_HIGH_PRECISION)
                    y = x *
                        (_x2 *
                             (_x2 *
                                  (_x2 * (_x2 * (_x2 * (_x2 * (ATAN_H15 * _x2 +
                                                               ATAN_H13) +
                                                        ATAN_H11) +
                                                 ATAN_H9) +
                                          ATAN_H7) +
                                   ATAN_H5) +
                              ATAN_H3) +
                         ATAN_H1);
                else if (p == HIGH_PRECISION)
                    y = x * (_x2 * (_x2 * (_x2 * (ATAN_H9 * _x2 + ATAN_H7) +
                                           ATAN_H5) +
                                    ATAN_H3) +
                             ATAN_H1);
                else if (p == LOW_PRECISION)
                    y = x * (_x2 * (ATAN_H5 * _x2 + ATAN_H3) + ATAN_H1);
                else
                    y = NAN;
            } else if (fabs(x) > 1.0) {
                _x2minus = 1.0 / POW2(x);
                if (p == VERY_HIGH_PRECISION)
                    y = ATAN_I0 * SGN(x, DEF) +
                        (_x2minus *
                             (_x2minus *
                                  (_x2minus *
                                       (_x2minus *
                                            (_x2minus *
                                                 (_x2minus *
                                                      (ATAN_I15 * _x2minus +
                                                       ATAN_I13) +
                                                  ATAN_I11) +
                                             ATAN_I9) +
                                        ATAN_I7) +
                                   ATAN_I5) +
                              ATAN_I3) +
                         ATAN_I1) /
                            x;
                else if (p == HIGH_PRECISION)
                    y = ATAN_I0 * SGN(x, DEF) +
                        (_x2minus *
                             (_x2minus *
                                  (_x2minus * (ATAN_I9 * _x2minus + ATAN_I7) +
                                   ATAN_I5) +
                              ATAN_I3) +
                         ATAN_I1) /
                            x;
                else if (p == LOW_PRECISION)
                    y = ATAN_I0 * SGN(x, DEF) +
                        (_x2minus * (ATAN_I5 * _x2minus + ATAN_I3) + ATAN_I1) /
                            x;
                else
                    y = NAN;
            } else {
                y = NAN;
            }
            break;
        case CHEBYSHEV:
            // Chebyshev Approximation
            _x2 = POW2(x);
            if (p == VERY_HIGH_PRECISION)
                y = x *
                    (_x2 * (_x2 * (_x2 * (ATAN_C9 * _x2 + ATAN_C7) + ATAN_C5) +
                            ATAN_C3) +
                     ATAN_C1);
            else if (p == HIGH_PRECISION)
                y = x * (_x2 * (ATAN_C5 * _x2 + ATAN_C3) + ATAN_C1);
            else if (p == LOW_PRECISION)
                y = x * (ATAN_C3 * _x2 + ATAN_C1);
            else
                y = NAN;
            break;
        default:
            y = NAN;
            break;
        }
    }

    return y;
}

//////////////////////////
// Hyperbolic Functions //
//////////////////////////

inline double SINH(const double x, const enum FastMethod fm) {
#if FM_DEBUG
    assert(fm == STL || fm == FAST || fm == FASTER);
#endif

    double y;
    if (__builtin_expect(!x, 0L))
        y = x;
    else {
        switch (fm) {
        case STL:
            y = sinh(x);
            break;
        case FAST:
            y = static_cast<double>(fastsinh(static_cast<float>(x)));
            break;
        case FASTER:
            y = static_cast<double>(fastersinh(static_cast<float>(x)));
            break;
        default:
            y = NAN;
            break;
        }
    }

    return y;
}

inline double COSH(const double x, const enum FastMethod fm) {
#if FM_DEBUG
    assert(fm == STL || fm == FAST || fm == FASTER);
#endif

    double y;
    if (__builtin_expect(!x, 0L))
        y = 1.0;
    else {
        switch (fm) {
        case STL:
            y = cosh(x);
            break;
        case FAST:
            y = static_cast<double>(fastcosh(static_cast<float>(x)));
            break;
        case FASTER:
            y = static_cast<double>(fastercosh(static_cast<float>(x)));
            break;
        default:
            y = NAN;
            break;
        }
    }

    return y;
}

//////////////////////////////////
// Inverse Hyperbolic Functions //
//////////////////////////////////

inline double ASINH(const double x, const enum FastMethod fm, const enum Precision p) {
#if FM_DEBUG
    assert(fm == STL || fm == INTEGRATION);
#endif

    double y;
    double _x2;
    if (__builtin_expect(!x, 0L))
        y = x;
    else {
        switch (fm) {
        case STL:
            y = asinh(x);
            break;
        case INTEGRATION:
// Series from Integration (for |x| < 1)
#if FM_DEBUG
            assert(fabs(x) < 1.0);
#endif
            _x2 = POW2(x);
            if (p == VERY_HIGH_PRECISION)
                y = x *
                    (_x2 * (_x2 * (_x2 * (_x2 * (_x2 * (_x2 * (ASINH_I15 * _x2 +
                                                               ASINH_I13) +
                                                        ASINH_I11) +
                                                 ASINH_I9) +
                                          ASINH_I7) +
                                   ASINH_I5) +
                            ASINH_I3) +
                     ASINH_I1);
            else if (p == HIGH_PRECISION)
                y = x * (_x2 * (_x2 * (_x2 * (ASINH_I9 * _x2 + ASINH_I7) +
                                       ASINH_I5) +
                                ASINH_I3) +
                         ASINH_I1);
            else if (p == LOW_PRECISION)
                y = x * (_x2 * (ASINH_I5 * _x2 + ASINH_I3) + ASINH_I1);
            else
                y = NAN;
            break;
        default:
            y = NAN;
            break;
        }
    }

    return y;
}

inline double ACOSH(const double x, const enum FastMethod fm, const enum Precision p) {
#if FM_DEBUG
    assert(fm == STL || fm == INTEGRATION);
#endif

    double y;
    double _x2minus;
    if (__builtin_expect(x == 1.0, 0L))
        y = 0.0;
    else {
        switch (fm) {
        case STL:
            y = acosh(x);
            break;
        case INTEGRATION:
// Series from Integration (for x > 1)
#if FM_DEBUG
            assert(fabs(x) > 1.0);
#endif
            y = log(2.0 * x);
            _x2minus = 1.0 / POW2(x);
            if (p == VERY_HIGH_PRECISION)
                y += _x2minus *
                     (_x2minus *
                          (_x2minus *
                               (_x2minus *
                                    (_x2minus *
                                         (_x2minus * (ACOSH_I14 * _x2minus +
                                                      ACOSH_I12) +
                                          ACOSH_I10) +
                                     ACOSH_I8) +
                                ACOSH_I6) +
                           ACOSH_I4) +
                      ACOSH_I2);
            else if (p == HIGH_PRECISION)
                y += _x2minus *
                     (_x2minus * (_x2minus * (_x2minus * (ACOSH_I10 * _x2minus +
                                                          ACOSH_I8) +
                                              ACOSH_I6) +
                                  ACOSH_I4) +
                      ACOSH_I2);
            else if (p == LOW_PRECISION)
                y += _x2minus *
                     (_x2minus * (ACOSH_I6 * _x2minus + ACOSH_I4) + ACOSH_I2);
            else
                y = NAN;
            break;
        default:
            y = NAN;
            break;
        }
    }

    return y;
}

///////////////////////////
// Statistical Functions //
///////////////////////////

inline double GAMMA(const double x) { return boost::math::tgamma<double>(x); }

inline double GAMMA(const double x, const enum FastMethod fm) {
#if FM_DEBUG
    assert(fm == STL || fm == BOOST);
    // Gamma(0) is undefined
    assert(x != 0.0);
    // Gamma not defined for negative integers
    assert(!(x < 0.0 && round(x) == x));
#endif

    double y = 0.0;
    if (ABS(x, STL) <= 10.0) {
        switch (fm) {
        case STL:
            y = tgamma(x);
            break;
        case BOOST:
            // Lanczos Approximation
            y = boost::math::tgamma<double>(x);
            break;
        default:
            y = NAN;
            break;
        }
    } else {
        if (x > 10.0)
            // Use Stirling's Approximation in the Gergo Nemes form
            y = sqrt(TWO_PI) * exp(-1.0 * x) * pow(x, x - 0.5);
        else
            // Use Reflection Formula, then Stirling's Approximation in the
            // Gergo Nemes form
            y = sqrt(HALF_PI) * exp(1.0 - x) * pow(1.0 - x, x - 0.5) /
                sin(M_PI * x);
    }

    return y;
}

inline double LOGGAMMA(const double x) { return boost::math::lgamma(x); }

inline double LOGGAMMA(const double x, const enum FastMethod fm) {
#if FM_DEBUG
    assert(fm == STL || fm == BOOST);
    // Gamma(0) is undefined
    assert(x != 0.0);
    // Gamma not defined for negative integers
    assert(!(x < 0.0 && round(x) == x));
    // Log not defined for negative numbers
    assert(!(x < 0.0 && static_cast<int>(-1 * floor(x)) % 2 == 1));
#endif

    double y;
    if (x > 0.0) {
        if (x <= 10.0) {
            switch (fm) {
            case STL:
                y = lgamma(x);
                break;
            case BOOST:
                // Lanczos Approximation
                y = boost::math::lgamma(x);
                break;
            default:
                y = NAN;
                break;
            }
        } else
            // Use Stirling's Approximation
            y = (x - 0.5) * log(x) - x + 0.5 * log(TWO_PI);
    } else
        y = NAN;

    return y;
}

// Returns gamma(x)/gamma(y)
// Logarithms are used when gamma(x) and gamma(y)
// are each large but the ratio is small
inline double GAMMA_RATIO(const double x, const double y) {
#if FM_DEBUG
    assert(x != 0.0);
    assert(y != 0.0);
    assert(!(x < 0.0 && round(x) == x));
    assert(!(y < 0.0 && round(y) == y));
#endif

    double z;
    double sx, sy, tx, ty;
    double c = 1.0;
    if (fabs(x) <= 10.0 && fabs(y) <= 10.0)
        z = GAMMA(x) / GAMMA(y);
    else {
        if (x > 0.0)
            tx = LOGGAMMA(x);
        else {
            sx = sin(M_PI * x);
            tx = log(GAMMA(x) * sx);
            c /= sx;
        }

        if (y > 0.0)
            ty = LOGGAMMA(y);
        else {
            sy = sin(M_PI * y);
            ty = log(GAMMA(y) * sy);
            c *= sy;
        }

        z = exp(tx - ty) * c;
    }

    return z;
}

// Approximation of the Pochhammer symbol (x)_j = gamma(x+j)/gamma(x)
// The coefficient j must be a non-negative integer by definition

inline double POCHHAMMER(const double x, const int j) {
#if FM_DEBUG
    assert(j >= 0);
#endif

    double y;
    if (__builtin_expect(!j, 0L))
        y = 1.0;
    else {
        if (!(x <= 0.0 && round(x) == x))
            y = GAMMA_RATIO(x + j, x);
        else {
            if (1.0 - x - j > 0.0)
                y = pow(-1.0, static_cast<double>(j)) *
                    GAMMA_RATIO(1.0 - x, 1.0 - x - j);
            else
                y = 0.0;
        }
    }

    return y;
}

// This is a faster version, assuming both arguments are non-negative
inline double POCHHAMMER_F(const double x, const int j) {
#if FM_DEBUG
    assert(x > 0.0);
    assert(j >= 0);
#endif

    double y;
    if (x + j <= 10)
        y = tgamma(x + j) / tgamma(x);
    else
        y = exp(lgamma(x + j) - lgamma(x));

    return y;
}

///////////////////////////////////
// Gauss Hypergeometric Function //
///////////////////////////////////

// Coefficient for the Gauss Hypergeometric function 2F1:
//	A_n = (a)_n * (b)_n / ((c)_n * n!)

// This is the fastest version
// It assumes a, b, c, and n are positive
inline double _2F1_An_F(const double a, const double b, const double c, const int n) {
#if FM_DEBUG
    assert(a > 0.0);
    assert(b > 0.0);
    assert(c > 0.0);
    assert(n >= 0);
#endif

    double A_n;

    if (a + n <= 10.0 && b + n <= 10.0 && c + n <= 10.0 && n <= 9.0)
        A_n = POCHHAMMER_F(a, n) * POCHHAMMER_F(b, n) /
              (POCHHAMMER_F(c, n) * tgamma(n + 1));
    else
        A_n = exp(lgamma(a + n) + lgamma(b + n) + lgamma(c) - lgamma(a) -
                  lgamma(b) - lgamma(c + n) - lgamma(n + 1.0));

    return A_n;
}

// This version permits negative values but takes longer to evaluate
inline double _2F1_An(const double a, const double b, const double c, const int n) {
#if FM_DEBUG
    assert(n >= 0);
#endif

    double A_n;
    double s_an, s_bn, s_cn, sa, sb, sc;
    double t_an, t_bn, t_cn, ta, tb, tc, tn;
    double m = 1.0;

    if (fabs(a + n) <= 10.0 && fabs(b + n) <= 10.0 && fabs(c + n) <= 10.0 &&
        fabs(a) <= 10.0 && fabs(b) <= 10.0 && fabs(c) <= 10.0 && n <= 9)
        A_n = POCHHAMMER(a, n) * POCHHAMMER(b, n) /
              (POCHHAMMER(c, n) * GAMMA(n + 1));
    else {
        if (a + n > 0.0) {
            if (a <= 0.0)
                return 0.0;
            t_an = LOGGAMMA(a + n);
        } else if (a + n != round(a + n)) {
            s_an = sin(M_PI * (a + n));
            t_an = log(GAMMA(a + n) * s_an);
            m /= s_an;
        } else {
            t_an = LOGGAMMA(1.0 - a);
            m *= pow(-1.0, static_cast<double>(n));
        }

        if (b + n > 0.0) {
            if (b <= 0.0)
                return 0.0;
            t_bn = LOGGAMMA(b + n);
        } else if (b + n != round(b + n)) {
            s_bn = sin(M_PI * (b + n));
            t_bn = log(GAMMA(b + n) * s_bn);
            m /= s_bn;
        } else {
            t_bn = LOGGAMMA(1.0 - b);
            m *= pow(-1.0, static_cast<double>(n));
        }

        if (c + n > 0.0) {
            if (c <= 0.0)
                return 0.0;
            t_cn = LOGGAMMA(c + n);
        } else if (c + n != round(c + n)) {
            s_cn = sin(M_PI * (c + n));
            t_cn = log(GAMMA(c + n) * s_cn);
            m *= s_cn;
        } else {
            t_cn = LOGGAMMA(1.0 - c);
            m *= pow(-1.0, static_cast<double>(n));
        }

        if (a > 0.0)
            ta = LOGGAMMA(a);
        else if (a != round(a)) {
            sa = sin(M_PI * a);
            ta = log(GAMMA(a) * sa);
            m *= sa;
        } else
            ta = LOGGAMMA(1.0 - a - n);

        if (b > 0.0)
            tb = LOGGAMMA(b);
        else if (b != round(b)) {
            sb = sin(M_PI * b);
            tb = log(GAMMA(b) * sb);
            m *= sb;
        } else
            tb = LOGGAMMA(1.0 - b - n);

        if (c > 0.0)
            tc = LOGGAMMA(c);
        else if (c != round(c)) {
            sc = sin(M_PI * c);
            tc = log(GAMMA(c) * sc);
            m /= sc;
        } else
            tc = LOGGAMMA(1.0 - c - n);

        tn = LOGGAMMA(n + 1.0);
        A_n = exp(t_an + t_bn + tc - ta - tb - t_cn - tn) * m;
    }

    return A_n;
}

// Approximates the Gauss Hypergeometric Function sol=2F1(a,b,c,z)
// The solution is stored in the memory location at 'sol'
// The actual error is stored in the memory location at 'err'
// The number of terms used in the power series is given by 'nterms'
// Specify a desired error instead by setting nterms = -1 and passing a non-zero
// err
inline void _2F1(const double a, const double b, const double c, const double z,
          double *const sol, double *const err, int *const nterms,
          const bool check) {
#if FM_DEBUG
    // No null pointers
    assert(sol != NULL);
    assert(err != NULL);
    assert(nterms != NULL);
#endif

    if (__builtin_expect(check, 0L)) {
        // Check parameters (used for debugging)
        if (a == 0.0 || b == 0.0 || z == 0.0) {
            *sol = 1.0;
            *err = 0.0;
            *nterms = 0;
        } else if (c <= 0.0 && round(c) == c) {
            // Solution will be complex infinity
            *sol = NAN;
        } else if (fabs(z) >= 1.0) {
            // Series will not converge in this region
            *sol = NAN;
        } else if (*nterms == -1 && *err <= 0.0) {
            // Invalid combination of parameters
            *sol = NAN;
        } else
            _2F1(a, b, c, z, sol, err, nterms, false);
    } else {
        // Specify desired error, calculate number of terms needed
        // Should give at least 1 term
        if (*nterms == -1)
            *nterms =
                static_cast<int>(LOG(*err, FAST) / LOG(fabs(z), FAST)) + 1;

        *sol = 0.0;
        for (int i = 0; i < *nterms; i++) {
            double An = _2F1_An(a, b, c, i);
            if (!!An)
                *sol += An * pow(z, static_cast<double>(i));
            else {
                *nterms = i;
                break;
            }
        }
        *err = fabs(pow(z, static_cast<double>(*nterms)));
    }
}

// Uses a recursion relation for Hypergeometric coefficients
// Faster than the generic _2F1 function defined above
inline void _2F1_F(const double a, const double b, const double c, const double z,
            double *const sol, double *const err, int *const nterms) {
#if FM_DEBUG
    assert(sol != NULL);
    assert(err != NULL);
    assert(nterms != NULL);
    assert(*nterms >= 0);
#endif

    if (a == 0.0 || b == 0.0 || c == 0.0) {
        *sol = 1.0;
        *err = 0.0;
        *nterms = 0;
        return;
    }

    double Ai = 1.0;
    double zi = 1.0;
    for (int i = 0; i < *nterms; i++) {
        *sol += Ai * zi;

        // Update variables
        Ai *= (a + i) * (b + i) / ((c + i) * (i + 1.0));
        zi *= z;
    }
    *err = fabs(pow(z, *nterms));
}

// Approximates the number of terms needed in 2F1 to
// achieve an error of 'err'
inline int getNumTerms(const double &z, const double &err) {
#if FM_DEBUG
    assert(err > 0.0);
#endif

    return static_cast<int>(log(err) / log(fabs(z))) + 1;
}

// Hypergeometric Transformation Type
// Used to find the proper transformation
// so that the series approximation to 2F1
// is valid for any |z| instead of just
//|z|<1 like in GSL.
// See the paper 'Computing the Hypergeometric
// Function' by R. Forrey for details
inline HyperType getHyperType(const double &z) {
    HyperType ht = HyperType();

    if (z >= 0.0 && z <= 0.5) {
        ht.w = z;
        ht.type = 0;
    } else if (z > 0.5 && z <= 1.00000000000001) {
        if (z >= 1.0 || 1.0 - z < 1.0E-14)
            ht.w = 0.0;
        else
            ht.w = 1.0 - z;
        ht.type = 1;
    } else if (z >= -1.0 && z < 0.0) {
        ht.w = z / (z - 1.0);
        ht.type = 2;
    } else if (z < -1.0) {
        ht.w = 1.0 / (1.0 - z);
        ht.type = 3;
    } else {
        ht.w = NAN;
        ht.type = -1;
    }

    return ht;
}

}

#endif
