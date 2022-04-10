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

#ifndef BENCH_FAST_NUM_INT_H_
#define BENCH_FAST_NUM_INT_H_

#include <fstream>
#include <stdio.h>

#include <fastmath/fastmath.h>
#include <fastmath/integration.h>
#include <fastmath/ran2.h>
#include <fastmath/stopwatch.h>

double testfunc(double t, void *params);

double measureFastInt(double * const data, double * const results, double * const error, const int &nnum, struct IntData &idata, const enum FastIntMethod fim, const int variation, const char *funcname, const char *filename);

void printFastInt(const double * const data, const double * const results, const double * const error, const int &nnum, const char *filename);

#endif
