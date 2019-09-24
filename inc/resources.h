#ifndef FASTMATH_RESOURCES_H
#define FASTMATH_RESOURCES_H

/*#ifdef __CUDACC_VER__
#undef __CUDACC_VER__
#define __CUDACC_VER__ 90000
#endif*/

//#error "This file is not ready to be used."

#include <cassert>
#include <cstdarg>
#include <cstring>
#include <fstream>
#include <hdf5.h>
#include <iostream>
#include <math.h>
#include <omp.h>
#include <stdint.h>
#include <stdio.h>
#include <string>
#include <sstream>
#include <vector>

#ifdef CUDA_ENABLED
#include <cuda.h>
#include <drvapi_error_string.h>
#endif

//This is needed for compilation with CUDA 10.0
#define BOOST_OPTIONAL_USE_OLD_DEFINITION_OF_NONE
#include <boost/optional.hpp>

//#include "config.h"
#include <fastmath/stopwatch.h>
#include <fastmath/printcolor.h>

#ifdef CUDA_ENABLED
#include <thrust/system/cuda/experimental/pinned_allocator.h>
#define checkCudaErrors(err) __checkCudaErrors (err, __FILE__, __LINE__)
#define getLastCudaError(msg) __getLastCudaError (msg, __FILE__, __LINE__)
#else
typedef int CUdevice;
typedef int CUcontext;
typedef int CUdeviceptr;
#endif

namespace fastmath {

#define CU_DEBUG false

enum Status {
	FAILED = 0,
	PASSED = 1,
	WAIVED = 2
};

#ifndef MAX_GPUS
#define MAX_GPUS 1
#endif

struct CuResources {
	CuResources() : gpus_active(0) {}

	//CUDA Driver API Variables
	CUdevice cuDevice[MAX_GPUS];
	CUcontext cuContext[MAX_GPUS];

	//Number of GPUs which will be used
	//This is determined by the particular algorithms
	unsigned gpus_active;
};		

#ifdef CUDA_ENABLED
void __checkCudaErrors(CUresult err, const char *file, const int line);
void __getLastCudaError(const char *errorMessage, const char *file, const int line);
#endif

#define BENCH_START Stopwatch BENCH_SW = Stopwatch(); \
		     double BENCH_MEAN = 0.0; int BENCH_I = 0, BENCH_ITER = 8; \
		     printf_red(); \
		     for ( ; BENCH_I < BENCH_ITER ; BENCH_I++) { \
		         stopwatchStart(&BENCH_SW);

#define BENCH_STOP       stopwatchStop(&BENCH_SW); \
                         printf("BENCH [%d]: %.6f sec\n", BENCH_I, BENCH_SW.elapsedTime); \
			 if (BENCH_I + 3 >= BENCH_ITER) BENCH_MEAN += BENCH_SW.elapsedTime; \
			 stopwatchReset(&BENCH_SW); \
                     } \
		     printf_std(); printf_cyan(); \
                     printf("BENCH MEAN: %.6f sec\n", BENCH_MEAN / 3.0); \
		     printf_std();


//Memory Management
template<typename T, bool zero = false, bool gpu = false>
inline T* MALLOC(const size_t entries, const boost::optional<T*> init = (const boost::optional<T*>)boost::none)
{
	T *data = NULL;
	#ifdef CUDA_ENABLED
	if (gpu) {
		checkCudaErrors(cuMemHostAlloc((void**)&data, entries * sizeof(T), CU_MEMHOSTALLOC_PORTABLE));
		if (zero)
			memset(data, 0, entries * sizeof(T));
		else if (init)
			memcpy(data, init.get(), entries * sizeof(T));
	} else
	#endif
	{
		try {
			if (zero)
				data = (T*)calloc(entries, sizeof(T));
			else {
				data = (T*)malloc(entries * sizeof(T));
				if (init)
					memcpy(data, init.get(), entries * sizeof(T));
			}
		} catch (const std::bad_alloc&) {
			fprintf(stderr, "Memory allocation failed!\n");
			fflush(stdout);
		}
	}

	return data;
}

template<typename T, bool gpu = false>
inline void FREE(T* &data)
{
	#ifdef CUDA_ENABLED
	if (gpu)
		cuMemFreeHost(data);
	else
	#endif
		free(data);

	data = NULL;
}

#ifdef CUDA_ENABLED
template<typename T, unsigned val = 0, int stream = 0>
inline CUdeviceptr CUDA_MALLOC(const size_t entries, const boost::optional<T*> init = boost::none)
{
	CUdeviceptr d = 0;
	checkCudaErrors(cuMemAlloc(&d, entries * sizeof(T)));
	if (init)
		checkCudaErrors(cuMemcpyHtoDAsync(d, (void*)init.get(), entries * sizeof(T), stream));
	else {
		if (sizeof(T) == sizeof(uint8_t))
			checkCudaErrors(cuMemsetD8Async(d, (uint8_t)val, entries, stream));
		else
			checkCudaErrors(cuMemsetD32Async(d, val, entries * sizeof(T) / sizeof(unsigned), stream));
	}

	return d;
}

template<typename T, unsigned val = 0>
inline CUdeviceptr CUDA_MALLOC_MANAGED(const size_t entries, const boost::optional<T*> init = boost::none)
{
	CUdeviceptr d = 0;
	checkCudaErrors(cuMemAllocManaged(&d, entries * sizeof(T), CU_MEM_ATTACH_GLOBAL));
	if (init)
		memcpy((T*)d, init.get(), entries * sizeof(T));
	else
		memset((T*)d, 0, entries * sizeof(T));

	return d;
}

template<typename T = int>
inline void CUDA_FREE(CUdeviceptr &d)
{
	cuMemFree(d);
	d = 0;
}
#endif

//Free a vector
template<typename T>
inline void VFREE(std::vector<T> &v)
{
	v.clear();
	v.shrink_to_fit();
}

//Free a pinned vector
#ifdef CUDA_ENABLED
template<typename T>
inline void VFREE(std::vector<T,thrust::system::cuda::experimental::pinned_allocator<T>> &v)
{
	v.clear();
	v.shrink_to_fit();
}
#endif

//Free a deque
/*template<typename T>
inline void VFREE(std::deque<T> &d)
{
	d.clear();
	d.shrink_to_fit();
}*/

int printf_mpi(int rank, const char * format, ...);
int printf_dbg(const char * format, ...);

int printStart(const char **argv, const int &rank);
void printFinish(const char **argv, const int &exename_start, const int &rank, int iStatus);
int findExeNameStart(const char *exec_name);
void printCPUInfo();

void printChk();

#ifdef CUDA_ENABLED
void connectToGPU(CuResources *cu, int argc, char **argv, const int &rank);
CUdevice findCudaDevice(int id, const int &rank);

inline void sync_all_gpus(CuResources * const cu)
{
	for (unsigned k = 0; k < cu->gpus_active; k++) {
		if (cu->gpus_active > 1)
			cuCtxPushCurrent(cu->cuContext[k]);
		checkCudaErrors(cuCtxSynchronize());
		if (cu->gpus_active > 1)
			cuCtxPopCurrent(&cu->cuContext[k]);
	}
}
#endif

template<typename T>
hid_t getH5DataType();

bool write_to_h5(hid_t &handle, void *data, hsize_t dim, hsize_t *shape, hsize_t *maxshape, hsize_t *chunk_size, hid_t datatype, const char *dset_name, bool create, bool append);

template<typename T>
void save_h5_scalar(const char *filename, const char *dataname, T value);

template<typename T>
void save_h5_vector(const char *filename, const char *dataname, const char *samplename, T* values, hsize_t *shape, hsize_t dim);

}

#endif
