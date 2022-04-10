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

#ifndef FASTMATH_INTEGRATION_H
#define FASTMATH_INTEGRATION_H

#include <assert.h>
#include <stdio.h>

#include <gsl/gsl_errno.h>
#include <gsl/gsl_integration.h>
#include <fastmath/nint.h>

namespace fastmath {

#define FNI_DEBUG false

//Integration scheme (see GSL documentation)
enum FastIntMethod {
	QNG,
	QAG,
	QAGS,
	QAWS
};

//Compact data structure to hold all relevant integration data
struct IntData {
	IntData() : workspace(NULL), table(NULL), lower(0.0), upper(1.0), tol(1e-3), abserr(0.0), alpha(-0.8), beta(-0.3), mu(0), nu(0), key(0), nintervals(100), neval(0), limit(10) {}

	gsl_integration_workspace *workspace;
	gsl_integration_qaws_table *table;

	double lower;	//Lower bound
	double upper;	//Upper bound
	double tol;	//Relative error
	double abserr;	//Estimate of absolute error

	//Used for QAWS
	double alpha;
	double beta;

	int mu;
	int nu;

	//Used for QAG
	int key;

	size_t nintervals; //Number of integration intervals
	size_t neval;	   //Number of function evaluations used
	size_t limit;	   //Max number of subintervals
};

double integrate1D(double (*f)(double x, void *params), void *params, struct IntData *idata, enum FastIntMethod fim);

double integrate2D(double (*f)(int dim, double x[], double *params), double lx, double ly, double ux, double uy, double *params, int &seed, int m);

void disableGSLErrHandler();

}

#endif