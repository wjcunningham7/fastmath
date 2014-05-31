#include <stdio.h>

#include "FastMath.h"
#include "FastNumInt.h"

double tauToEtaUniverse(double tau, void *params)
{
	return POW(SINH(1.5*tau, STL), (-2.0f / 3.0f), STL);
}

int main(int argc, char **argv)
{
	IntData idata = IntData();
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
	printf("COMPLETED\n\n");
}
