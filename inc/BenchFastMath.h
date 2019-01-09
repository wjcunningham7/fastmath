#ifndef BENCH_FAST_MATH_H
#define BENCH_FAST_MATH_H

/////////////////////////////
//(C) Will Cunningham 2017 //
//         DK Lab          //
// Northeastern University //
/////////////////////////////

#include <fstream>
#include <math.h>

#include "FastMath.h"
#include "ran2.h"
#include "stopwatch.h"

double measure(double (*solve0)(const double x, const enum FastMethod fm), double (*solve1)(const double x, const double p, const enum FastMethod fm), double (*solve2)(const double x, const enum FastMethod fm, const enum Precision p), double * const data, double * const results, double * const error, const int &nnum, const int func, const enum FastMethod fm, const char *funcname, const char *filename, const double exponent, const enum Precision p, const bool calc_error);

void printErrors(const double * const data, const double * const error, const int &nnum, const char *filename);

#endif
