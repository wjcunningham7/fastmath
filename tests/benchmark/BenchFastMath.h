/* Copyright 2014-2022 Will Cunningham
 *
 * This file is part of FastMath.
 *
 * Licensed under the MIT License (the "License"). A copy of the
 * License may be obtained with this software package or at
 *
 *     https://opensource.org/licenses/MIT
 *
 * FastMath is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef BENCH_FAST_MATH_H
#define BENCH_FAST_MATH_H

#include <fstream>
#include <math.h>

#include <fastmath/fastmath.h>
#include <fastmath/ran2.h>
#include <fastmath/stopwatch.h>

double measure(double (*solve0)(const double x, const enum FastMethod fm), double (*solve1)(const double x, const double p, const enum FastMethod fm), double (*solve2)(const double x, const enum FastMethod fm, const enum Precision p), double * const data, double * const results, double * const error, const int &nnum, const int func, const enum FastMethod fm, const char *funcname, const char *filename, const double exponent, const enum Precision p, const bool calc_error);

void printErrors(const double * const data, const double * const error, const int &nnum, const char *filename);

#endif
