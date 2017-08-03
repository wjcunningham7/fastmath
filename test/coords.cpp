#include "Coordinates.h"
#include <cstring>

/////////////////////////////
//(C) Will Cunningham 2017 //
//         DK Lab          //
// Northeastern University //
/////////////////////////////

int main(int argc, char **argv)
{
	Coordinates c(3, 128);
	printf("Number of dimensions: %u\n", c.getDim());
	printf("Number of elements per dimension: %" PRIu64 "\n", c.size());

	for (int i = 0; i < c.getDim(); i++)
		for (int j = 0; j < c.size(); j++)
			c(i, j) = (float)(i*c.size()+j);

	for (int i = 0; i < c.getDim(); i++)
		for (int j = 0; j < c.size(); j++)
			printf("c(%d,%d) = %f\n", i, j, c(i,j));

	memset(c.dim(1), 0, c.size() * sizeof(float));

	for (int i = 0; i < c.getDim(); i++)
		for (int j = 0; j < c.size(); j++)
			printf("c(%d,%d) = %f\n", i, j, c(i,j));

	memcpy(c.dim(1), c.dim(2), c.size() * sizeof(float));

	for (int i = 0; i < c.getDim(); i++)
		for (int j = 0; j < c.size(); j++)
			printf("c(%d,%d) = %f\n", i, j, c(i,j));

	return 0;
}
