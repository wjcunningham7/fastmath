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

#include "BenchFastNumInt.h"

/* This benchmarks all the 1D GSL integration methods in
 * FastNumInt.cpp using the integrand 'testfunc' and prints to file
 * data on precision and timings. */

int main(int argc, char **argv) {
    double *data;
    double *results;
    double *error;

    long seed = -18100L;
    double tmax = 0.8458;
    int nnum = 10000;
    int i;

    // Allocate memory for test data
    data = (double *)malloc(sizeof(double) * nnum);

    // Allocate memory for test results
    results = (double *)malloc(sizeof(double) * nnum);

    // Allocate memory for errors
    error = (double *)malloc(sizeof(double) * nnum);

    // Generate values
    for (i = 0; i < nnum; i++)
        data[i] = ran2(&seed) * tmax;

    // Prepare stopwatch
    Stopwatch watch = Stopwatch();

    // Prepare numerical integration data
    IntData idata = IntData();

    // Method times
    double qng;
    double qag0, qag1, qag2, qag3, qag4, qag5;
    double qags;
    double qaws0, qaws1, qaws2, qaws3;

    // Perform Operations
    qng = measureFastInt(data, results, error, nnum, idata, QNG, 0, "QNG",
                         "dat/QNG_Integration.dat");

    qag0 =
        measureFastInt(data, results, error, nnum, idata, QAG,
                       GSL_INTEG_GAUSS15, "QAG", "dat/QAG_15_Integration.dat");
    qag1 =
        measureFastInt(data, results, error, nnum, idata, QAG,
                       GSL_INTEG_GAUSS21, "QAG", "dat/QAG_21_Integration.dat");
    qag2 =
        measureFastInt(data, results, error, nnum, idata, QAG,
                       GSL_INTEG_GAUSS31, "QAG", "dat/QAG_31_Integration.dat");
    qag3 =
        measureFastInt(data, results, error, nnum, idata, QAG,
                       GSL_INTEG_GAUSS41, "QAG", "dat/QAG_41_Integration.dat");
    qag4 =
        measureFastInt(data, results, error, nnum, idata, QAG,
                       GSL_INTEG_GAUSS51, "QAG", "dat/QAG_51_Integration.dat");
    qag5 =
        measureFastInt(data, results, error, nnum, idata, QAG,
                       GSL_INTEG_GAUSS61, "QAG", "dat/QAG_61_Integration.dat");

    qags = measureFastInt(data, results, error, nnum, idata, QAGS, 0, "QAGS",
                          "dat/QAGS_Integration.dat");

    qaws0 = measureFastInt(data, results, error, nnum, idata, QAWS, 0, "QAWS",
                           "dat/QAWS_0_Integration.dat");
    qaws1 = measureFastInt(data, results, error, nnum, idata, QAWS, 1, "QAWS",
                           "dat/QAWS_1_Integration.dat");
    qaws2 = measureFastInt(data, results, error, nnum, idata, QAWS, 2, "QAWS",
                           "dat/QAWS_2_Integration.dat");
    qaws3 = measureFastInt(data, results, error, nnum, idata, QAWS, 3, "QAWS",
                           "dat/QAWS_3_Integration.dat");

    // Print Method Times
    std::ofstream os("dat/fast_int_method_times.dat");

    os << "QNG\t\t" << (qng / nnum) << std::endl;

    os << "QAG\t15\t" << (qag0 / nnum) << std::endl;
    os << "QAG\t21\t" << (qag1 / nnum) << std::endl;
    os << "QAG\t31\t" << (qag2 / nnum) << std::endl;
    os << "QAG\t41\t" << (qag3 / nnum) << std::endl;
    os << "QAG\t51\t" << (qag4 / nnum) << std::endl;
    os << "QAG\t61\t" << (qag5 / nnum) << std::endl;

    os << "QAGS\t\t" << (qags / nnum) << std::endl;

    os << "QAWS\t0\t" << (qaws0 / nnum) << std::endl;
    os << "QAWS\t1\t" << (qaws1 / nnum) << std::endl;
    os << "QAWS\t2\t" << (qaws2 / nnum) << std::endl;
    os << "QAWS\t3\t" << (qaws3 / nnum) << std::endl;

    os.flush();
    os.close();

    // Free Memory
    free(data);
    free(results);
    free(error);
}

double testfunc(double t, void *params) {
    return POW(SINH(1.5 * t, STL), (-2.0 / 3.0), STL);
}

double measureFastInt(double *const data, double *const results,
                      double *const error, const int &nnum,
                      struct IntData &idata, const enum FastIntMethod fim,
                      const int variation, const char *funcname,
                      const char *filename) {
    assert(data != NULL);
    assert(results != NULL);
    assert(error != NULL);

    assert(funcname != NULL);
    assert(filename != NULL);

    Stopwatch watch = Stopwatch();
    double time = 0.0;
    int i;

    printf("Measuring %s-%d Integration.....\n", funcname, variation);
    fflush(stdout);

    if (fim == QAG || fim == QAGS || fim == QAWS) {
        idata.workspace = gsl_integration_workspace_alloc(idata.nintervals);

        if (fim == QAG)
            idata.key = variation;
        else if (fim == QAWS) {
            idata.lower = 0.0;
            idata.mu = variation / 2;
            idata.nu = variation % 2;
            idata.table = gsl_integration_qaws_table_alloc(
                idata.alpha, idata.beta, idata.mu, idata.nu);
            if (idata.table == 0) {
                fprintf(stderr, "Failed to allocate qaws_table!\n");
                return 0.0;
            }
        }
    }

    stopwatchStart(&watch);
    for (i = 0; i < nnum; i++) {
        idata.upper = data[i];
        results[i] = integrate1D(&testfunc, NULL, &idata, fim);
        error[i] = idata.abserr;
    }
    stopwatchStop(&watch);

    if (fim == QAG || fim == QAGS || fim == QAWS) {
        gsl_integration_workspace_free(idata.workspace);

        if (fim == QAWS)
            gsl_integration_qaws_table_free(idata.table);
    }

    time = watch.elapsedTime;
    stopwatchReset(&watch);

    printFastInt(data, results, error, nnum, filename);

    printf("\tCompleted.\n\n");
    fflush(stdout);

    return time;
}

void printFastInt(const double *const data, const double *const results,
                  const double *const error, const int &nnum,
                  const char *filename) {
    assert(data != NULL);
    assert(results != NULL);
    assert(error != NULL);
    assert(filename != NULL);

    std::ofstream os(filename);
    int i;

    for (i = 0; i < nnum; i++)
        os << data[i] << " " << results[i] << " " << error[i] << std::endl;

    os.flush();
    os.close();
}
