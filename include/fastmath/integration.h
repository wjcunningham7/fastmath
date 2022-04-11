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

#ifndef FASTMATH_INTEGRATION_H
#define FASTMATH_INTEGRATION_H

#include <assert.h>
#include <stdio.h>

#include <fastmath/nint.h>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_integration.h>

/* These functions provide a convenient wrapper for 1D and 2D
 * numerical integration using the GSL and NINTLIB integration
 * functions. Note that in some cases memory is allocated and freed,
 * so if these functions are a bottleneck in other code, consider
 * re-writing them explicitly with memory management outside any
 * loops. */

namespace fastmath {

#define FNI_DEBUG false

// Integration scheme (see GSL documentation)
enum FastIntMethod { QNG, QAG, QAGS, QAWS };

// Compact data structure to hold all relevant integration data
struct IntData {
    IntData()
        : workspace(NULL), table(NULL), lower(0.0), upper(1.0), tol(1e-3),
          abserr(0.0), alpha(-0.8), beta(-0.3), mu(0), nu(0), key(0),
          nintervals(100), neval(0), limit(10) {}

    gsl_integration_workspace *workspace;
    gsl_integration_qaws_table *table;

    double lower;  // Lower bound
    double upper;  // Upper bound
    double tol;    // Relative error
    double abserr; // Estimate of absolute error

    // Used for QAWS
    double alpha;
    double beta;

    int mu;
    int nu;

    // Used for QAG
    int key;

    size_t nintervals; // Number of integration intervals
    size_t neval;      // Number of function evaluations used
    size_t limit;      // Max number of subintervals
};

// GNU Scientific Library
// Numerical Integration
inline double integrate1D(double (*f)(double x, void *params), void *params,
                   struct IntData *idata, enum FastIntMethod fim) {
    if (FNI_DEBUG) {
        assert(f != NULL);
        assert(idata != NULL);
    }

    gsl_function F;
    F.function = *f;
    F.params = params;

    double result = 0.0;
    int code;

    if (fim == QNG) {
        code =
            gsl_integration_qng(&F, idata->lower, idata->upper, 0.0, idata->tol,
                                &result, &idata->abserr, &idata->neval);
    } else if (fim == QAG) {
        code = gsl_integration_qag(&F, idata->lower, idata->upper, 0.0,
                                   idata->tol, idata->limit, idata->key,
                                   idata->workspace, &result, &idata->abserr);
    } else if (fim == QAGS) {
        code = gsl_integration_qags(&F, idata->lower, idata->upper, 0.0,
                                    idata->tol, idata->limit, idata->workspace,
                                    &result, &idata->abserr);
    } else if (fim == QAWS) {
        code = gsl_integration_qaws(&F, idata->lower, idata->upper,
                                    idata->table, 0.0, idata->tol, idata->limit,
                                    idata->workspace, &result, &idata->abserr);
    }

    if (code != 0) {
        fprintf(stderr, "GSL Error: %s\n", gsl_strerror(code));
        result = NAN;
    }

    return result;
}


// NINTLIB C++ Library
// Multi-Dimensional Quadrature
// Uses Monte Carlo Sampling
inline double integrate2D(double (*f)(int dim, double x[], double *params), double lx,
                   double ly, double ux, double uy, double *params, int &seed,
                   int m) {
    if (FNI_DEBUG) {
        assert(f != NULL);
        assert(m == 0);
    }

    double *lower;
    double *upper;
    double result = 0.0;

    int dim = 2;
    int test = 2;
    int neval;
    int i;

    if (m == 0) {
        lower = (double *)malloc(sizeof(double) * dim);
        upper = (double *)malloc(sizeof(double) * dim);

        lower[0] = lx;
        lower[1] = ly;

        upper[0] = ux;
        upper[1] = uy;

        neval = i4_power(8, test) * 20000;
        result = monte_carlo_nd(*f, dim, lower, upper, params, neval, &seed);

        free(lower);
        lower = NULL;

        free(upper);
        upper = NULL;
    }

    return result;
}

void disableGSLErrHandler() { gsl_set_error_handler_off(); }

}

#endif
