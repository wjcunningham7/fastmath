#include <stdio.h>

#include "FastMath.h"
#include "FastNumInt.h"

double tauToEtaUniverse(double tau, void *params)
{
	return POW(SINH(1.5f * tau, STL), (-2.0f / 3.0f), STL);
}

float _2F1_r(const float &r, void * const param)
{
	return -1.0f / POW3(r, EXACT);
}

double xi(double r)
{
	double _xi = 0.0;
	float err = 0.0f;
	float f;
	int nterms = 10;

	if (ABS(r - 1.0f, STL) < 0.05)
		nterms = 20;

	if (r < 1.0f) {
		//Since 1/f(x) = f(1/x) we can use _r
		double _r = 1.0f / r;
		_2F1(&_2F1_r, _r, NULL, 1.0f / 6.0f, 0.5f, 7.0f / 6.0f, &f, &err, &nterms);
		_xi = 2.0 * SQRT(r, STL) * f;
	} else {
		_2F1(&_2F1_r, r, NULL, 1.0f / 3.0f, 0.5f, 4.0f / 3.0f, &f, &err, &nterms);
		_xi = SQRT(4.0f / M_PI, STL) * GAMMA(7.0f / 6.0f, STL) * GAMMA(1.0f / 3.0f, STL) - f / r;
	}

	//assert (ABS(err, STL) < 1e-4);
	//printf("\t2F1 Error:  %f\n", err);
	//printf("\t2F1 Result: %f\n", f);

	return _xi;
}

double rescaledDegreeUniverse(int dim, double x[], double *params)
{
	//Identify x[0] with x coordinate
	//Identify x[1] with r coordinate

	double z;

	//printf("\tx: %f\n", x[0]);
	//printf("\tr: %f\n", x[1]);

	z = POW3(ABS(xi(x[0]) - xi(x[1]), STL), EXACT) * POW2(x[0], EXACT) * POW3(x[1], EXACT) * SQRT(x[1], STL);
	z /= (SQRT(1.0 + 1.0 / POW3(x[0], EXACT), STL) * SQRT(1.0 + POW3(x[1], EXACT), STL));

	return z;
}

int main(int argc, char **argv)
{
	//1D Numerical Integration Test
	
	/*IntData idata = IntData();
	double tau = 0.5;
	double eta;

	if (argc > 2) {
		idata.lower = atof(argv[1]);
		tau = atof(argv[2]);
	} else if (argc > 1)
		tau = atof(argv[1]);

	idata.upper = tau;
	idata.workspace = gsl_integration_workspace_alloc(idata.nintervals);
	eta = integrate1D(&tauToEtaUniverse, NULL, &idata, QAGS);
	gsl_integration_workspace_free(idata.workspace);

	printf("Result for QAGS 1D Numerical Integration:\n");
	printf("-----------------------------------------\n");
	printf(" > tau: %.5f\n", tau);
	printf(" > eta: %.8f\n", eta);
	printf(" > err: %.8f\n", idata.abserr);
	printf("\n");
	printf("COMPLETED\n\n");*/

	//2D Numerical Integration Test
	
	float tau0 = 0.8458;
	double r0 = POW(SINH(1.5f * tau0, STL), 2.0f / 3.0f, STL);
	long seed = 18100;

	printf("tau0: %f\n", tau0);
	printf("r0:   %f\n", r0);

	double k = integrate2D(&rescaledDegreeUniverse, 0.0, 0.0, r0, r0, NULL, seed, 0);
	k *= 8 * M_PI / (SINH(3.0f * tau0, STL) - 3.0f * tau0);
	printf("k: %f\n", k);
	printf("\n");
	printf("COMPLETED\n\n");
}
