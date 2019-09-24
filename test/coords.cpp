#include "coordinates.h"
#include "fastgraph.h"
#include "fastmath.h"
#include <cstring>

/////////////////////////////
//(C) Will Cunningham 2017 //
//         DK Lab          //
// Northeastern University //
/////////////////////////////

float sampler0(MersenneRNG &mrng, void *data)
{
	float max = ((float*)data)[0];
	return max * mrng.rng();
}

/*float distanceT2(float *crd0, float *crd1)
{
	return sqrtf(POW2(crd0[0] - crd1[0]) + POW2(crd0[1] - crd1[1]));
}

bool linked(float (*distance)(float *crd0, float *crd1), float *crd0, float *crd1, float threshold)
{
	return distance(crd0, crd1) < threshold;
}*/

int main(int argc, char **argv)
{
	/*Coordinates<float> c(32, 2);
	printf("Number of dimensions: %u\n", c.getDim());
	printf("Number of elements per dimension: %u\n", c.size());

	printf("\nSetting coordinates...\n"); fflush(stdout);
	for (unsigned i = 0; i < c.size(); i++)
		for (unsigned j = 0; j < c.getDim(); j++)
			c(i, j) = (float)(i*c.getDim()+j);

	printf("Reading coordinates...\n"); fflush(stdout);
	for (int i = 0; i < c.size(); i++)
		for (int j = 0; j < c.getDim(); j++)
			printf("c(%d,%d) = %f\n", i, j, c(i, j));

	printf("\nExtracting full coordinate...\n"); fflush(stdout);
	unsigned idx = 3;
	float *p = c(idx);
	for (unsigned i = 0; i < c.getDim(); i++)
		printf("p[%u] = %f\n", i, p[i]);*/

	/*printf("\nGenerating geometric nodes...\n"); fflush(stdout);
	GeometricNodes<float> nodes(16, 4);
	for (unsigned i = 0; i < nodes.size(); i++)
		for (unsigned j = 0; j < nodes.getDim(); j++)
			nodes.getNode(i, j) = (float)(i*nodes.getDim()+j);
	printf("Reading node coordinates...\n"); fflush(stdout);
	for (unsigned i = 0; i < nodes.size(); i++)
		for (unsigned j = 0; j < nodes.getDim(); j++)
			printf("n(%d,%d) = %f\n", i, j, nodes.getNode(i, j));
	printf("\nSetting degrees...\n"); fflush(stdout);
	for (unsigned i = 0; i < nodes.size(); i++)
		nodes.degrees[i] = i;
	printf("Reading degrees...\n"); fflush(stdout);
	for (unsigned i = 0; i < nodes.size(); i++)
		printf("Degree of node [%u] is %u\n", i, nodes.degrees[i]);*/

	printf("\nGenerating geometric graph...\n"); fflush(stdout);
	GeometricFastGraph<float, false> graph(16, 2, ST_DENSE);
	printf("Graph size: %u\n", graph.size());
	printf("Graph dimension: %u\n", graph.getDim());

	MersenneRNG mrng[omp_get_max_threads()];
	srand(time(NULL));
	long seed = (long)time(NULL);
	for (unsigned i = 0; i < omp_get_max_threads(); i++) {
		mrng[i].rng.engine().seed(seed ^ i);
		mrng[i].rng.distribution().reset();
		for (int j = 0; j < 1000; j++)
			mrng[i].rng();
	}

	printf("\nSetting samplers...\n"); fflush(stdout);
	graph.setSampler(0, &sampler0);
	graph.setSampler(1, &sampler0);
	float max = 1.0;
	printf("Sampling...\n");
	graph.sample(mrng, (void*)&max);

	printf("\nCoordinates:\n");
	for (unsigned i = 0; i < graph.size(); i++)
		for (unsigned j = 0; j < graph.getDim(); j++)
			printf("node(%u, %u) = %f\n", i, j, graph.getNode(i, j));
	fflush(stdout);

	return 0;
}
