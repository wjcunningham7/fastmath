#include "BenchFastMath.h"

int main(int argc, char **argv)
{
	double *data;
	double *results;
	double *error;

	long seed = -18100L;
	int nnum = 100000;
	int i;

	//Allocate memory for test data
	data = (double*)malloc(sizeof(double) * nnum);

	//Allocate memory for test results
	results = (double*)malloc(sizeof(double) * nnum);	

	//Allocate memory for errors
	error = (double*)malloc(sizeof(double) * nnum);

	//Generate values
	for (i = 0; i < nnum; i++)
		data[i] = ran2(&seed);

	//Method times
	double pow2_t0, pow2_t1, pow2_t2, pow2_t3;
	double pow3_t0, pow3_t1, pow3_t2, pow3_t3;
	double pow_t0, pow_t1, pow_t2;
	double sqrt_t0, sqrt_t1;
	double abs_t0, abs_t1;
	double log_t0, log_t1, log_t2;
	double sgn_t0, sgn_t1;
	double sin_t0, sin_t1, sin_t2;
	double cos_t0, cos_t1, cos_t2;
	double tan_t0, tan_t1, tan_t2;
	double acos_t0, acos_t1a, acos_t1b, acos_t1c, acos_t2a, acos_t2b, acos_t2c;
	double atan_t0, atan_t1a, atan_t1b, atan_t1c, atan_t2a, atan_t2b, atan_t2c;
	double sinh_t0, sinh_t1, sinh_t2;
	double cosh_t0, cosh_t1, cosh_t2;
	double asinh_t0, asinh_t2a, asinh_t2b, asinh_t2c;
	double acosh_t0, acosh_t2a, acosh_t2b, acosh_t2c;
	double gamma_t0, gamma_t1;

	//Perform Operations
	
	//POW2
	pow2_t0 = measure(&POW2, NULL, NULL, data, results, NULL, nnum, 0, STL, "POW2", NULL, 0, DEFAULT, false);
	pow2_t1 = measure(&POW2, NULL, NULL, data, results, NULL, nnum, 0, EXACT, "POW2", NULL, 0, DEFAULT, false);
	pow2_t2 = measure(&POW2, NULL, NULL, data, results, error, nnum, 0, FAST, "POW2", "dat/fastpow_2_error.dat", 0, DEFAULT, true);
	pow2_t3 = measure(&POW2, NULL, NULL, data, results, error, nnum, 0, FASTER, "POW2", "dat/fasterpow_2_error.dat", 0, DEFAULT, true);

	//POW3
	pow3_t0 = measure(&POW3, NULL, NULL, data, results, NULL, nnum, 0, STL, "POW3", NULL, 0, DEFAULT, false);
	pow3_t1 = measure(&POW3, NULL, NULL, data, results, NULL, nnum, 0, EXACT, "POW3", NULL, 0, DEFAULT, false);
	pow3_t2 = measure(&POW3, NULL, NULL, data, results, error, nnum, 0, FAST, "POW3", "dat/fastpow_3_error.dat", 0, DEFAULT, true);
	pow3_t3 = measure(&POW3, NULL, NULL, data, results, error, nnum, 0, FASTER, "POW3", "dat/fasterpow_3_error.dat", 0, DEFAULT, true);

	//POW
	double p = 2.0 * ran2(&seed);
	pow_t0 = measure(NULL, &POW, NULL, data, results, NULL, nnum, 1, STL, "POW", NULL, p, DEFAULT, false);
	pow_t1 = measure(NULL, &POW, NULL, data, results, error, nnum, 1, FAST, "POW", "dat/fastpow_error.dat", p, DEFAULT, true);
	pow_t2 = measure(NULL, &POW, NULL, data, results, error, nnum, 1, FASTER, "POW", "dat/fasterpow_error.dat", p, DEFAULT, true);

	//SQRT
	sqrt_t0 = measure(&SQRT, NULL, NULL, data, results, NULL, nnum, 0, STL, "SQRT", NULL, 0, DEFAULT, false);
	sqrt_t1 = measure(&SQRT, NULL, NULL, data, results, error, nnum, 0, BITWISE, "SQRT", "dat/sqrt_bit_error.dat", 0, DEFAULT, true);

	//ABS
	abs_t0 = measure(&ABS, NULL, NULL, data, results, NULL, nnum, 0, STL, "ABS", NULL, 0, DEFAULT, false);
	abs_t1 = measure(&ABS, NULL, NULL, data, results, error, nnum, 0, BITWISE, "ABS", "dat/abs_bit_error.dat", 0, DEFAULT, true);

	//LOG
	log_t0 = measure(&LOG, NULL, NULL, data, results, NULL, nnum, 0, STL, "LOG", NULL, 0, DEFAULT, false);
	log_t1 = measure(&LOG, NULL, NULL, data, results, error, nnum, 0, FAST, "LOG", "dat/fastlog_error.dat", 0, DEFAULT, true);
	log_t2 = measure(&LOG, NULL, NULL, data, results, error, nnum, 0, FASTER, "LOG", "dat/fasterlog_error.dat", 0, DEFAULT, true);

	//SGN
	sgn_t0 = measure(&SGN, NULL, NULL, data, results, NULL, nnum, 0, DEF, "SGN", NULL, 0, DEFAULT, false);
	sgn_t1 = measure(&SGN, NULL, NULL, data, results, NULL, nnum, 0, BITWISE, "SGN", NULL, 0, DEFAULT, false);

	//SIN
	sin_t0 = measure(&SIN, NULL, NULL, data, results, NULL, nnum, 0, STL, "SIN", NULL, 0, DEFAULT, false);
	sin_t1 = measure(&SIN, NULL, NULL, data, results, error, nnum, 0, FAST, "SIN", "dat/fastsin_error.dat", 0, DEFAULT, true);
	sin_t2 = measure(&SIN, NULL, NULL, data, results, error, nnum, 0, FASTER, "SIN", "dat/fastersin_error.dat", 0, DEFAULT, true);

	//COS
	cos_t0 = measure(&COS, NULL, NULL, data, results, NULL, nnum, 0, STL, "COS", NULL, 0, DEFAULT, false);
	cos_t1 = measure(&COS, NULL, NULL, data, results, error, nnum, 0, FAST, "COS", "dat/fastcos_error.dat", 0, DEFAULT, true);
	cos_t2 = measure(&COS, NULL, NULL, data, results, error, nnum, 0, FASTER, "COS", "dat/fastercos_error.dat", 0, DEFAULT, true);

	//TAN
	tan_t0 = measure(&TAN, NULL, NULL, data, results, NULL, nnum, 0, STL, "TAN", NULL, 0, DEFAULT, false);
	tan_t1 = measure(&TAN, NULL, NULL, data, results, error, nnum, 0, FAST, "TAN", "dat/fasttan_error.dat", 0, DEFAULT, true);
	tan_t2 = measure(&TAN, NULL, NULL, data, results, error, nnum, 0, FASTER, "TAN", "dat/fastertan_error.dat", 0, DEFAULT, true);

	//ACOS
	acos_t0 = measure(NULL, NULL, &ACOS, data, results, NULL, nnum, 2, STL, "ACOS", NULL, 0, DEFAULT, false);
	acos_t1a = measure(NULL, NULL, &ACOS, data, results, error, nnum, 2, CHEBYSHEV, "ACOS", "dat/acos_cheb_low_error.dat", 0, LOW_PRECISION, true);
	acos_t1b = measure(NULL, NULL, &ACOS, data, results, error, nnum, 2, CHEBYSHEV, "ACOS", "dat/acos_cheb_high_error.dat", 0, HIGH_PRECISION, true);
	acos_t1c = measure(NULL, NULL, &ACOS, data, results, error, nnum, 2, CHEBYSHEV, "ACOS", "dat/acos_cheb_vhigh_error.dat", 0, VERY_HIGH_PRECISION, true);
	acos_t2a = measure(NULL, NULL, &ACOS, data, results, error, nnum, 2, INTEGRATION, "ACOS", "dat/acos_wolf_low_error.dat", 0, LOW_PRECISION, true);
	acos_t2b = measure(NULL, NULL, &ACOS, data, results, error, nnum, 2, INTEGRATION, "ACOS", "dat/acos_wolf_high_error.dat", 0, HIGH_PRECISION, true);
	acos_t2c = measure(NULL, NULL, &ACOS, data, results, error, nnum, 2, INTEGRATION, "ACOS", "dat/acos_wolf_vhigh_error.dat", 0, VERY_HIGH_PRECISION, true);
	
	//ATAN
	atan_t0 = measure(NULL, NULL, &ATAN, data, results, NULL, nnum, 2, STL, "ATAN", NULL, 0, DEFAULT, false);
	atan_t1a = measure(NULL, NULL, &ATAN, data, results, error, nnum, 2, CHEBYSHEV, "ATAN", "dat/atan_cheb_low_error.dat", 0, LOW_PRECISION, true);
	atan_t1b = measure(NULL, NULL, &ATAN, data, results, error, nnum, 2, CHEBYSHEV, "ATAN", "dat/atan_cheb_high_error.dat", 0, HIGH_PRECISION, true);
	atan_t1c = measure(NULL, NULL, &ATAN, data, results, error, nnum, 2, CHEBYSHEV, "ATAN", "dat/atan_cheb_vhigh_error.dat", 0, VERY_HIGH_PRECISION, true);
	atan_t2a = measure(NULL, NULL, &ATAN, data, results, error, nnum, 2, INTEGRATION, "ATAN", "dat/atan_wolf_low_error.dat", 0, LOW_PRECISION, true);
	atan_t2b = measure(NULL, NULL, &ATAN, data, results, error, nnum, 2, INTEGRATION, "ATAN", "dat/atan_wolf_high_error.dat", 0, HIGH_PRECISION, true);
	atan_t2c = measure(NULL, NULL, &ATAN, data, results, error, nnum, 2, INTEGRATION, "ATAN", "dat/atan_wolf_vhigh_error.dat", 0, VERY_HIGH_PRECISION, true);

	//SINH
	sinh_t0 = measure(&SINH, NULL, NULL, data, results, NULL, nnum, 0, STL, "SINH", NULL, 0, DEFAULT, false);
	sinh_t1 = measure(&SINH, NULL, NULL, data, results, error, nnum, 0, FAST, "SINH", "dat/fastsinh_error.dat", 0, DEFAULT, true);
	sinh_t2 = measure(&SINH, NULL, NULL, data, results, error, nnum, 0, FASTER, "SINH", "dat/fastersinh_error.dat", 0, DEFAULT, true);

	//COSH
	cosh_t0 = measure(&COSH, NULL, NULL, data, results, NULL, nnum, 0, STL, "COSH", NULL, 0, DEFAULT, false);
	cosh_t1 = measure(&COSH, NULL, NULL, data, results, error, nnum, 0, FAST, "COSH", "dat/fastcosh_error.dat", 0, DEFAULT, true);
	cosh_t2 = measure(&COSH, NULL, NULL, data, results, error, nnum, 0, FASTER, "COSH", "dat/fastercosh_error.dat", 0, DEFAULT, true);

	//ASINH
	asinh_t0 = measure(NULL, NULL, &ASINH, data, results, NULL, nnum, 2, STL, "ASINH", NULL, 0, DEFAULT, false);
	asinh_t2a = measure(NULL, NULL, &ASINH, data, results, error, nnum, 2, INTEGRATION, "ASINH", "dat/asinh_wolf_low_error.dat", 0, LOW_PRECISION, true);
	asinh_t2b = measure(NULL, NULL, &ASINH, data, results, error, nnum, 2, INTEGRATION, "ASINH", "dat/asinh_wolf_high_error.dat", 0, HIGH_PRECISION, true);
	asinh_t2c = measure(NULL, NULL, &ASINH, data, results, error, nnum, 2, INTEGRATION, "ASINH", "dat/asinh_wolf_vhigh_error.dat", 0, VERY_HIGH_PRECISION, true);
	
	//ACOSH
	//acosh_t0 = measure(NULL, NULL, &ACOSH, data, results, NULL, nnum, 2, STL, "ACOSH", NULL, 0, DEFAULT, false);
	//acosh_t2a = measure(NULL, NULL, &ACOSH, data, results, error, nnum, 2, INTEGRATION, "ACOSH", "dat/acosh_wolf_low_error.dat", 0, LOW_PRECISION, true);
	//acosh_t2b = measure(NULL, NULL, &ACOSH, data, results, error, nnum, 2, INTEGRATION, "ACOSH", "dat/acosh_wolf_high_error.dat", 0, HIGH_PRECISION, true);
	//acosh_t2c = measure(NULL, NULL, &ACOSH, data, results, error, nnum, 2, INTEGRATION, "ACOSH", "dat/acosh_wolf_vhigh_error.dat", 0, VERY_HIGH_PRECISION, true);

	//GAMMA
	gamma_t0 = measure(&GAMMA, NULL, NULL, data, results, NULL, nnum, 0, STL, "GAMMA", NULL, 0, DEFAULT, false);
	gamma_t1 = measure(&GAMMA, NULL, NULL, data, results, error, nnum, 0, BOOST, "GAMMA", "dat/gamma_boost_error.dat", 0, DEFAULT, true);

	//Print Method Times
	std::ofstream os("dat/method_times.dat");

	os << "POW2\tpow\t" << (pow2_t0 / nnum) << std::endl;
	os << "POW2\tx*x\t" << (pow2_t1 / nnum) << std::endl;
	os << "POW2\tfastpow\t" << (pow2_t2 / nnum) << std::endl;
	os << "POW2\tfasterpow\t" << (pow2_t3 / nnum) << std::endl;
	
	os << "POW3\tpow\t" << (pow3_t0 / nnum) << std::endl;
	os << "POW3\tx*x*x\t" << (pow3_t1 / nnum) << std::endl;
	os << "POW3\tfastpow\t" << (pow3_t2 / nnum) << std::endl;
	os << "POW3\tfasterpow\t" << (pow3_t3 / nnum) << std::endl;

	os << "POW\tpow\t" << (pow_t0 / nnum) << std::endl;
	os << "POW\tfastpow\t" << (pow_t1 / nnum) << std::endl;
	os << "POW\tfasterpow\t" << (pow_t2 / nnum) << std::endl;

	os << "SQRT\tsqrt\t" << (sqrt_t0 / nnum) << std::endl;
	os << "SQRT\tbit\t" << (sqrt_t1 / nnum) << std::endl;
	
	os << "ABS\tfabs\t" << (abs_t0 / nnum) << std::endl;
	os << "ABS\tbit\t" << (abs_t1 / nnum) << std::endl;

	os << "LOG\tlog\t" << (log_t0 / nnum) << std::endl;
	os << "LOG\tfastlog\t" << (log_t1 / nnum) << std::endl;
	os << "LOG\tfasterlog\t" << (log_t2 / nnum) << std::endl;

	os << "SGN\tif\t" << (sgn_t0 / nnum) << std::endl;
	os << "SGN\tbit\t" << (sgn_t1 / nnum) << std::endl;

	os << "SIN\tsin\t" << (sin_t0 / nnum) << std::endl;
	os << "SIN\tfastsin\t" << (sin_t1 / nnum) << std::endl;
	os << "SIN\tfastersin\t" << (sin_t2 / nnum) << std::endl;

	os << "COS\tcos\t" << (cos_t0 / nnum) << std::endl;
	os << "COS\tfastcos\t" << (cos_t1 / nnum) << std::endl;
	os << "COS\tfastercos\t" << (cos_t2 / nnum) << std::endl;

	os << "TAN\ttan\t" << (tan_t0 / nnum) << std::endl;
	os << "TAN\tfasttan\t" << (tan_t1 / nnum) << std::endl;
	os << "TAN\tfastertan\t" << (tan_t2 / nnum) << std::endl;

	os << "ACOS\tacos\t" << (acos_t0 / nnum) << std::endl;
	os << "ACOS\tch_low\t" << (acos_t1a / nnum) << std::endl;
	os << "ACOS\tch_high\t" << (acos_t1b / nnum) << std::endl;
	os << "ACOS\tch_vhigh\t" << (acos_t1c / nnum) << std::endl;
	os << "ACOS\tw_low\t" << (acos_t2a / nnum) << std::endl;
	os << "ACOS\tw_high\t" << (acos_t2b / nnum) << std::endl;
	os << "ACOS\tw_vhigh\t" << (acos_t2c / nnum) << std::endl;

	os << "ATAN\tatan\t" << (atan_t0 / nnum) << std::endl;
	os << "ATAN\tch_low\t" << (atan_t1a / nnum) << std::endl;
	os << "ATAN\tch_high\t" << (atan_t1b / nnum) << std::endl;
	os << "ATAN\tch_vhigh\t" << (atan_t1c / nnum) << std::endl;
	os << "ATAN\tw_low\t" << (atan_t2a / nnum) << std::endl;
	os << "ATAN\tw_high\t" << (atan_t2b / nnum) << std::endl;
	os << "ATAN\tw_vhigh\t" << (atan_t2c / nnum) << std::endl;

	os << "SINH\tsinh\t" << (sinh_t0 / nnum) << std::endl;
	os << "SINH\tfastsinh\t" << (sinh_t1 / nnum) << std::endl;
	os << "SINH\tfastersinh\t" << (sinh_t2 / nnum) << std::endl;

	os << "COSH\tcosh\t" << (cosh_t0 / nnum) << std::endl;
	os << "COSH\tfastcosh\t" << (cosh_t1 / nnum) << std::endl;
	os << "COSH\tfastercosh\t" << (cosh_t2 / nnum) << std::endl;

	os << "ASINH\tasinh\t" << (asinh_t0 / nnum) << std::endl;
	os << "ASINH\tw_low\t" << (asinh_t2a / nnum) << std::endl;
	os << "ASINH\tw_high\t" << (asinh_t2b / nnum) << std::endl;
	os << "ASINH\tw_vhigh\t" << (asinh_t2c / nnum) << std::endl;

	//os << "ACOSH\tacosh\t" << (acosh_t0 / nnum) << std::endl;
	//os << "ACOSH\tw_low\t" << (acosh_t2a / nnum) << std::endl;
	//os << "ACOSH\tw_high\t" << (acosh_t2b / nnum) << std::endl;
	//os << "ACOSH\tw_vhigh\t" << (acosh_t2c / nnum) << std::endl;

	os << "GAMMA\ttgamma\t" << (gamma_t0 / nnum) << std::endl;
	os << "GAMMA\tboost\t" << (gamma_t1 / nnum) << std::endl;

	os.flush();
	os.close();

	//Free Memory
	free(data);
	free(results);
	free(error);
}

double measure(double (*solve0)(const double x, const enum FastMethod fm), double (*solve1)(const double x, const double p, const enum FastMethod fm), double (*solve2)(const double x, const enum FastMethod fm, const enum Precision p), double * const data, double * const results, double * const error, const int &nnum, const int func, const enum FastMethod fm, const char *funcname, const char *filename, const double exponent, const enum Precision p, const bool calc_error)
{
	assert (data != NULL);
	assert (results != NULL);
	assert (funcname != NULL);

	if (calc_error) {
		assert (error != NULL);
		assert (filename != NULL);
	}

	assert (nnum > 0);
	assert (func == 0 || func == 1 || func == 2);

	Stopwatch watch = Stopwatch();
	double time = 0.0;
	int i;
	
	printf("Measuring %s Method %d.....\n", funcname, fm);
	fflush(stdout);

	if (func == 0) {
		assert (solve0 != NULL);
		stopwatchStart(&watch);
		for (i = 0; i < nnum; i++)
			results[i] = solve0(data[i], fm);
		stopwatchStop(&watch);

		if (calc_error)
			for (i = 0; i < nnum; i++)
				error[i] = fabs(results[i] - solve0(data[i], DEF)) / solve0(data[i], DEF);
	} else if (func == 1) {
		assert (solve1 != NULL);
		stopwatchStart(&watch);
		for (i = 0; i < nnum; i++)
			results[i] = solve1(data[i], exponent, fm);
		stopwatchStop(&watch);

		if (calc_error)
			for (i = 0; i < nnum; i++)
				error[i] = fabs(results[i] - solve1(data[i], exponent, DEF)) / solve1(data[i], exponent, DEF);
	} else if (func == 2) {
		assert (solve2 != NULL);
		stopwatchStart(&watch);
		for (i = 0; i < nnum; i++)
			results[i] = solve2(data[i], fm, p);
		stopwatchStop(&watch);

		if (calc_error)
			for (i = 0; i < nnum; i++)
				error[i] = fabs(results[i] - solve2(data[i], DEF, p)) / solve2(data[i], DEF, p);
	}

	if (calc_error)
		printErrors(data, error, nnum, filename);

	time = watch.elapsedTime;
	stopwatchReset(&watch);

	printf("\tCompleted.\n\n");
	fflush(stdout);

	return time;
}

void printErrors(const double * const data, const double * const error, const int &nnum, const char *filename)
{
	assert (data != NULL);
	assert (error != NULL);
	assert (nnum > 0);
	assert (filename != NULL);

	std::ofstream os(filename);
	int i;

	for (i = 0; i < nnum; i++)
		os << data[i] << " " << error[i] << std::endl;

	os.flush();
	os.close();
}
