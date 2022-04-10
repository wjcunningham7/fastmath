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

#include "resources.h"

using namespace fastmath;

int main(int argc, char **argv)
{
	const char *filename = "h5test.h5";
	remove(filename);

	//Save a scalar
	const char *dataname = "x";
	float x = 0.5;
	save_h5_scalar(filename, dataname, x);

	//Save a vector
	const char *dataname_vec = "xvec";
	char samplename[80];
	sprintf(samplename, "%ld", (long)time(NULL));
	uint64_t len = 32;
	unsigned *xvec = MALLOC<unsigned>(len);
	for (unsigned i = 0; i < len; i++)
		xvec[i] = i;
	save_h5_vector(filename, dataname_vec, samplename, xvec, (hsize_t*)&len, 1);
	FREE(xvec);

	//Save an attribute
	float y = 1.0;
	const char *attrname = "y";
	save_h5_metadata(filename, attrname, y);

	//Save another attribute
	const char *myname = "will";
	const char *attrname2 = "myname";
	save_h5_metadata(filename, attrname2, myname);

	//Save another scalar
	float x2 = 0.7;
	save_h5_scalar(filename, dataname, x2);

	//Save another vector
	sleep(1);
	sprintf(samplename, "%ld", (long)time(NULL));
	xvec = MALLOC<unsigned>(len);
	for (unsigned i = 0; i < len; i++)
		xvec[i] = len - i - 1;
	save_h5_vector(filename, dataname_vec, samplename, xvec, (hsize_t*)&len, 1);
}
