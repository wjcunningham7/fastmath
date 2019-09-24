#ifndef FASTMATH_FASTGRAPH_H
#define FASTMATH_FASTGRAPH_H

/////////////////////////////
//(C) Will Cunningham 2019 //
//    Perimeter Institute  //
/////////////////////////////

#include <fastmath/fastbitset.h>
#include <fastmath/resources.h>

namespace fastmath {

#ifdef CUDA_ENABLED
template<typename T>
using pinned_vector = std::vector<T, thrust::system::cuda::experimental::pinned_allocator<T>>;
#else
template<typename T>
using pinned_vector = std::vector<T>;
#endif

template<bool gpu = false>
class Nodes
{
public:
	//Creation Constructor
	Nodes(unsigned _N)
	{
		allocNodes(_N);
	}

	//Copy Constructor
	Nodes(const Nodes &other)
	{
		allocNodes(other.N);
		std::copy(other.degrees, other.degrees + other.N, degrees);
		std::copy(other.components, other.components + other.N, components);
	}

	//Destructor
	~Nodes()
	{
		destroyNodes();
	}

	unsigned *degrees;
	int *components;

private:
	unsigned N;
	bool gpu;

	void allocNodes(unsigned _N)
	{
		N = _N;
		degrees = MALLOC<unsigned, true, gpu>(N);
		components = MALLOC<int>(N);
		if (degrees == NULL || components == NULL)
			freeNodes();
	}

	void freeNodes()
	{
		N = 0;
		FREE<unsigned, gpu>(degrees);
		FREE(components);
	}
}

enum StorageType {
	ST_DENSE = 1,
	ST_SPARSE = ST_DENSE << 1
};

template<bool gpu = false>
class FastGraph
{
public:
	//Creation Constructor
	FastGraph(unsigned _N, StorageType _st)
	{
		allocGraph(_N, _st);
	}

	//Destructor
	~FastGraph()
	{
		freeGraph();
	}

	//Adjacency Matrix (Dense)
	Bitvector adj;

	//Adjacency List (Sparse)
	pinned_vector<unsigned> links;
	pinned_vector<uint64_t> linkidx;

	//Graph Nodes
	Nodes nodes;

	unsigned N;	//Number of Nodes
	uint64_t L;	//Number of Links
	StorageType st;	//Storage Type

private:
	void allocGraph(unsigned _N, StorageType _st)
	{
		N = _N;
		st = _st;
		nodes = Nodes<gpu>(N);

		if (st & ST_DENSE) {
			FastBitset fb(N);
			adj.reserve(N);
			for (unsigned i = 0; i < N; i++)
				adj.push_back(fb);
		}

		if (st & ST_SPARSE) {
			uint64_t npairs = ((uint64_t)N * (N - 1)) >> 1;
			links.reserve(npairs / 10);
			linkidx.resize(N);
		}
	}

	void freeGraph()
	{
		N = 0; L = 0;
		if (st & ST_DENSE)
			VFREE(adj);
		if (st & ST_SPARSE) {
			VFREE(links);
			VFREE(linkidx);
		}
	}
};

}

#endif
