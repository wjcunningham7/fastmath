#ifndef BENCH_FAST_MATH_H
#define BENCH_FAST_MATH_H

#include <fstream>
#include <math.h>

#include "FastMath.h"
#include "ran2.h"
#include "stopwatch.h"

double measure(float (*solve0)(const float x, const enum FastMethod fm), float (*solve1)(const float x, const float p, const enum FastMethod fm), float (*solve2)(const float x, const enum FastMethod fm, const enum Precision p), float * const data, float * const results, float * const error, const int &nnum, const int func, const enum FastMethod fm, const char *funcname, const char *filename, const float exponent, const enum Precision p, const bool calc_error);

void printErrors(const float * const data, const float * const error, const int &nnum, const char *filename);

#endif
