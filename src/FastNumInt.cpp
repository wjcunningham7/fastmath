#include "FastNumInt.h"

//GNU Scientific Library
//Numerical Integration
double integrate1D(double (*f)(double x, void *params), void *params, struct IntData *idata, enum FastIntMethod fim)
{
	if (FNI_DEBUG) {
		assert (f != NULL);
		assert (idata != NULL);
	}

	gsl_function F;
	F.function = *f;
	F.params = params;

	double result = 0.0;
	int code;

	if (fim == QNG) {
		code = gsl_integration_qng(&F, idata->lower, idata->upper, 0.0, idata->tol, &result, &idata->abserr, &idata->neval);
	} else if (fim == QAG) {
		code = gsl_integration_qag(&F, idata->lower, idata->upper, 0.0, idata->tol, idata->limit, idata->key, idata->workspace, &result, &idata->abserr);
	} else if (fim == QAGS) {
		code = gsl_integration_qags(&F, idata->lower, idata->upper, 0.0, idata->tol, idata->limit, idata->workspace, &result, &idata->abserr);
	} else if (fim == QAWS) {
		code = gsl_integration_qaws(&F, idata->lower, idata->upper, idata->table, 0.0, idata->tol, idata->limit, idata->workspace, &result, &idata->abserr);
	} /*else if (fim == CQUAD) {
		code = gsl_integration_cquad(&F, idata->lower, idata->upper, 0.0, idata->tol, idata->cworkspace, &result, &idata->abserr, &idata->neval);
	}*/

	if (code != 0) {
		fprintf(stderr, "GSL Error: %s\n", gsl_strerror(code));
		result = 0.0;
	}

	return result;
}

//NINTLIB C++ Library
//Multi-Dimensional Quadrature
double integrate2D(double (*f)(int dim, double x[]), double lx, double ly, double ux, double uy, long &seed, int m)
{
	if (FNI_DEBUG) {
		assert (f != NULL);
		assert (m == 0);
	}

	double *lower;
	double *upper;
	double result = 0.0;

	int dim = 2;
	int test = 2;
	int neval;
	int i;

	if (m == 0) {
		lower = (double*)malloc(sizeof(double) * dim);
		upper = (double*)malloc(sizeof(double) * dim);

		lower[0] = lx;
		lower[1] = ly;

		upper[0] = ux;
		upper[1] = uy;

		neval = i4_power(16, test) * 10000;
		int _seed = (int)seed;
		result = monte_carlo_nd(*f, dim, lower, upper, neval, &_seed);

		free(lower);
		lower = NULL;

		free(upper);
		upper = NULL;
	}

	return result;
}
