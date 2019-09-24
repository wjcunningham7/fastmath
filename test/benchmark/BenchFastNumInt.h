#ifndef BENCH_FAST_NUM_INT_H_
#define BENCH_FAST_NUM_INT_H_

/////////////////////////////
//(C) Will Cunningham 2017 //
//         DK Lab          //
// Northeastern University //
/////////////////////////////

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
