#include "FastNumInt.h"

//GNU Scientific Library
//Numerical Integration
double integrate1D(double (*f)(double x, void *params), void *params, struct IntData *idata, enum FastIntMethod fim)
{
	assert (f != NULL);
	assert (idata != NULL);

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

double integrate2D(double (*f)(double x, double y, void *params), void *params, double lx, double ly, double ux, double uy, double tol, int m)
{
	assert (f != NULL);

	double result = 0.0;

	return result;
}
