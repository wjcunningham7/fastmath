#include "Coordinates.h"
#include <cstring>

/////////////////////////////
//(C) Will Cunningham 2017 //
//         DK Lab          //
// Northeastern University //
/////////////////////////////

int main(int argc, char **argv)
{
	Coordinates c = Coordinates(3, 128);
	printf("Number of dimensions: %u\n", c.getDim());
	printf("Number of elements per dimension: %" PRIu64 "\n", c.size());

	for (int i = 0; i < c.getDim(); i++)
		for (int j = 0; j < c.size(); j++)
			c(i, j) = (COORD_TYPE)(i*c.size()+j);

	for (int i = 0; i < c.getDim(); i++)
		for (int j = 0; j < c.size(); j++)
			printf("c(%d,%d) = %f\n", i, j, c(i,j));

	printf("\nResetting first dimension to zero.\n");
	memset(c.dim(1), 0, c.size() * sizeof(COORD_TYPE));

	for (int i = 0; i < c.getDim(); i++)
		for (int j = 0; j < c.size(); j++)
			printf("c(%d,%d) = %f\n", i, j, c(i,j));

	Coordinates d = Coordinates(6, 32);
	for (int i = 0; i < d.getDim(); i++)
		for (int j = 0; j < d.size(); j++)
			d(i, j) = (COORD_TYPE)(i*d.size()+j);
	coordN dN = d.getCoordN(0);

	printf("\n6D coordinate:\n");
	for (int i = 0; i < dN.dim; i++)
		printf("\t%f\n", dN.x[i]);

	return 0;
}
