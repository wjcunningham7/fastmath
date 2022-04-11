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

#ifndef BENCH_FAST_MATH_H
#define BENCH_FAST_MATH_H

#include <fstream>
#include <math.h>

#include <fastmath/fastmath.h>
#include <fastmath/ran2.h>
#include <fastmath/stopwatch.h>

double measure(double (*solve0)(const double x, const enum FastMethod fm),
               double (*solve1)(const double x, const double p,
                                const enum FastMethod fm),
               double (*solve2)(const double x, const enum FastMethod fm,
                                const enum Precision p),
               double *const data, double *const results, double *const error,
               const int &nnum, const int func, const enum FastMethod fm,
               const char *funcname, const char *filename,
               const double exponent, const enum Precision p,
               const bool calc_error);

void printErrors(const double *const data, const double *const error,
                 const int &nnum, const char *filename);

#endif
