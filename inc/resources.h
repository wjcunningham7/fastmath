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

#ifndef FASTMATH_RESOURCES_H
#define FASTMATH_RESOURCES_H

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
#endif

//This is needed for compilation with CUDA 10.0
#define BOOST_OPTIONAL_USE_OLD_DEFINITION_OF_NONE
#include <boost/optional.hpp>

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

//We assume we use no more than 16 GPUs per host
//As of Mar. 2020 no devices exist which violate this
#ifndef MAX_GPUS
#define MAX_GPUS 16
#endif

struct CuResources {
	CuResources() : dev_count(0), gpus_active(0) {}

	//CUDA Driver API Variables
	CUdevice cuDevice[MAX_GPUS];
	CUcontext cuContext[MAX_GPUS];

	//Global memory size of each GPU
	//It is assumed all GPUs on a system have equal memory
	size_t dev_memory[MAX_GPUS];

	//The number of GPUs detected by cuInit
	int dev_count;

	//Number of GPUs which will be used
	//This is determined by the particular algorithms
	unsigned gpus_active;
};

#ifdef CUDA_ENABLED
//This will output the proper CUDA error strings in the event that a CUDA host call returns an error
inline void __checkCudaErrors(CUresult err, const char *file, const int line)
{
	if (CUDA_SUCCESS != err) {
    		fprintf(stderr, "checkCudaErrors() Driver API error = %04d from file <%s>, line %i.\n", err, file, line);
		#ifdef MPI_ENABLED
		MPI_Abort(MPI_COMM_WORLD, 3);
		#else
    		exit(3);
		#endif
	}
}

//This will output the proper error string when calling cudaGetLastError
inline void __getLastCudaError(const char *errorMessage, const char *file, const int line)
{
	cudaError_t err = cudaGetLastError();
	if (cudaSuccess != err) {
    		fprintf(stderr, "%s(%i) : getLastCudaError() CUDA error : %s : (%d).\n", file, line, errorMessage, (int)err);
		#ifdef MPI_ENABLED
		MPI_Abort(MPI_COMM_WORLD, 4);
		#else
       		exit(4);
		#endif
	}
}
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

int printf_mpi(int rank, const char * format, ...);
int printf_dbg(const char * format, ...);
void printFinish(const char **argv, const int &exename_start, const int &rank, int iStatus);

inline int findExeNameStart(const char *exec_name)
{
	int exename_start = (int)strlen(exec_name);

	while (exename_start > 0 &&
	       exec_name[exename_start] != '\\' &&
	       exec_name[exename_start] != '/')
		exename_start--;

	if (exec_name[exename_start] == '\\' ||
	    exec_name[exename_start] == '/')
		return exename_start + 1;
	else
		return exename_start;
}

inline void printCPUInfo(const int rank)
{
	std::string line;
	std::ifstream finfo("/proc/cpuinfo");

	while (getline(finfo, line)) {
		std::stringstream str(line);
		std::string itype;
		std::string info;

		if (getline(str, itype, ':') &&
		    getline(str, info) &&
		    itype.substr(0, 10) == "model name") {
			printf("> ");
			#ifdef MPI_ENABLED
			printf("Rank [%d] ", rank);
			#endif
			printf("Using Processor: %s\n", info.c_str());
			fflush(stdout);
			break;
		}
	}
}

inline int printStart(const char **argv, const int mpi_procs, const int rank)
{
	int exename_start = findExeNameStart(argv[0]);
	printf_mpi(rank, "[%s] starting...\n\n", &(argv[0][exename_start]));
	printf_mpi(rank, "Detecting Compute Hardware...\n");
	fflush(stdout);
	#ifdef MPI_ENABLED
	MPI_Barrier(MPI_COMM_WORLD);
	for (int i = 0; i < mpi_procs; i++) {
		if (rank == i)
			printCPUInfo(rank);
		MPI_Barrier(MPI_COMM_WORLD);
	}
	#else
	printCPUInfo(0);
	#endif
	return exename_start;
}

inline void printChk()
{
	#ifdef MPI_ENABLED
	MPI_Barrier(MPI_COMM_WORLD);
	#endif
	printf("CHECKPOINT\n");
	#ifdef MPI_ENABLED
	MPI_Finalize();
	#endif
	exit(99);
}

inline void printChk(int flag)
{
	#ifdef MPI_ENABLED
	MPI_Barrier(MPI_COMM_WORLD);
	#endif
	printf("CHECKPOINT");
	if (flag) printf(" %d", flag);
	printf("\n");
	#ifdef MPI_ENABLED
	MPI_Finalize();
	#endif
	exit(99);
}

#ifdef CUDA_ENABLED
//Find available GPUs
inline CUdevice findCudaDevice(int id, const int &rank)
{
    	CUdevice device;
      	char name[100];

	checkCudaErrors(cuDeviceGet(&device, id));
	cuDeviceGetName(name, 100, device);
	printf("> ");
	#ifdef MPI_ENABLED
	printf("Rank [%d] ", rank);
	#endif
	printf("Using CUDA Device [%d]: %s\n", id, name);
	fflush(stdout);

    	return device;
}

//Returns cores per multiprocessor (last taken from CUDA 10.2)
inline int _ConvertSMVer2CoresDRV(int major, int minor) {
  // Defines for GPU Architecture types (using the SM version to determine the #
  // of cores per SM
  typedef struct {
    int SM;  // 0xMm (hexidecimal notation), M = SM Major version, and m = SM
             // minor version
    int Cores;
  } sSMtoCores;

  sSMtoCores nGpuArchCoresPerSM[] = {
      {0x30, 192},
      {0x32, 192},
      {0x35, 192},
      {0x37, 192},
      {0x50, 128},
      {0x52, 128},
      {0x53, 128},
      {0x60,  64},
      {0x61, 128},
      {0x62, 128},
      {0x70,  64},
      {0x72,  64},
      {0x75,  64},
      {-1, -1}};

  int index = 0;

  while (nGpuArchCoresPerSM[index].SM != -1) {
    if (nGpuArchCoresPerSM[index].SM == ((major << 4) + minor)) {
      return nGpuArchCoresPerSM[index].Cores;
    }

    index++;
  }

  // If we don't find the values, we default use the previous one to run
  // properly
  printf(
      "MapSMtoCores for SM %d.%d is undefined.  Default to use %d Cores/SM\n",
      major, minor, nGpuArchCoresPerSM[index - 1].Cores);
  return nGpuArchCoresPerSM[index - 1].Cores;
}

//Initialize connection to GPU
inline void connectToGPU(CuResources *cu, int argc, char **argv, const int rank)
{
	//No null pointers
	#if CU_DEBUG
	assert (cu != NULL);
	#endif

	//Pick CUDA Device
	CUresult status;
	int major = 0, minor = 0;

	//Local Processes (MPI ranks that share the same physical computer)
	//This assumes all GPUs are exposed to all MPI processes
	//If that is not the case, such as for a SLURM environment with GRES partitions,
	//set mpi_local_procs=1, mpi_local_rank=0
	#ifdef MPI_ENABLED
	int mpi_local_procs = atoi(getenv("OMPI_COMM_WORLD_LOCAL_SIZE"));
	int mpi_local_rank = atoi(getenv("OMPI_COMM_WORLD_LOCAL_RANK"));
	#else
	int mpi_local_procs = 1;
	int mpi_local_rank = 0;
	#endif

	checkCudaErrors(cuInit(0));
	checkCudaErrors(cuDeviceGetCount(&cu->dev_count));
	printf_mpi(rank, "--------------------\n");
	unsigned gpus_per_proc = cu->dev_count / mpi_local_procs;
	for (int j = 0; j < mpi_local_procs; j++) {
		if (j != mpi_local_rank) continue;
		for (unsigned i = 0; i < gpus_per_proc; i++) {
			cu->cuDevice[i] = findCudaDevice(j * gpus_per_proc + i, rank);

			checkCudaErrors(cuDeviceGetAttribute(&major, CU_DEVICE_ATTRIBUTE_COMPUTE_CAPABILITY_MAJOR, cu->cuDevice[i]));
			checkCudaErrors(cuDeviceGetAttribute(&minor, CU_DEVICE_ATTRIBUTE_COMPUTE_CAPABILITY_MINOR, cu->cuDevice[i]));

			//Statistics about the Device
			printf("  Compute Capability: %d.%d\n", major, minor);
			int version = (major * 0x10 + minor);
			if (version < 0x30) {
				printf("Program requires a minimum CUDA compute capability of 3.0\n");
				printFinish((const char**)argv, findExeNameStart(argv[0]), 0, FAILED);
				#ifdef MPI_ENABLED
				MPI_Abort(MPI_COMM_WORLD, 1);
				#else
				exit(1);
				#endif
			}

			//Get number of SMPs
			int mpcount;
			checkCudaErrors(cuDeviceGetAttribute(&mpcount, CU_DEVICE_ATTRIBUTE_MULTIPROCESSOR_COUNT, cu->cuDevice[i]));
			//Get global memory
			checkCudaErrors(cuDeviceTotalMem(&cu->dev_memory[i], cu->cuDevice[i]));
			printf("  Memory: %.2fGB\n  Cores: %d\n", (float)cu->dev_memory[i]/1073741824.0f, mpcount * _ConvertSMVer2CoresDRV(major, minor));
			fflush(stdout);

			//Create Context
			status = cuCtxCreate(&cu->cuContext[i], CU_CTX_SCHED_SPIN, cu->cuDevice[i]);
			if (status != CUDA_SUCCESS) {
				printf("Could not create CUDA context!\n");
				cuCtxDestroy(cu->cuContext[i]);
				printFinish((const char**)argv, findExeNameStart(argv[0]), 0, FAILED);
				#ifdef MPI_ENABLED
				MPI_Abort(MPI_COMM_WORLD, 2);
				#else
				exit(2);
				#endif
			}
			fflush(stdout);

			//Pop the Context
			if (gpus_per_proc > 1)
				checkCudaErrors(cuCtxPopCurrent(&cu->cuContext[i]));
		}
	}
	//GPUs are only exposed to their respective MPI threads
	cu->dev_count = gpus_per_proc;
}

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

template<>
inline hid_t getH5DataType<bool>() { return H5T_NATIVE_UCHAR; }

template<>
inline hid_t getH5DataType<unsigned>() { return H5T_STD_U32LE; }

template<>
inline hid_t getH5DataType<int>() { return H5T_STD_I32LE; }

template<>
inline hid_t getH5DataType<long>() { return H5T_STD_I64LE; }

template<>
inline hid_t getH5DataType<uint64_t>() { return H5T_STD_U64LE; }

template<>
inline hid_t getH5DataType<float>() { return H5T_IEEE_F32LE; }

template<>
inline hid_t getH5DataType<double>() { return H5T_IEEE_F64LE; }

template<>
inline hid_t getH5DataType<const char*>()
{
	hid_t dt = H5Tcopy(H5T_C_S1);
	H5Tset_size(dt, H5T_VARIABLE);
	return dt;
}

inline bool write_to_h5(hid_t &handle, void *data, hsize_t dim, hsize_t *shape, hsize_t *maxshape, hsize_t *chunk_size, hid_t datatype, const char *dset_name, bool create, bool append)
{
	hid_t dataspace, memspace, props, dataset;
	if (create && append) {
		dataspace = H5Screate_simple(dim, shape, maxshape);
		props = H5Pcreate(H5P_DATASET_CREATE);
		H5Pset_layout(props, H5D_CHUNKED);
		H5Pset_chunk(props, dim, chunk_size);
		dataset = H5Dcreate(handle, dset_name, datatype, dataspace, H5P_DEFAULT, props, H5P_DEFAULT);
		H5Dwrite(dataset, datatype, H5S_ALL, dataspace, H5P_DEFAULT, data);
		H5Dclose(dataset);
		H5Pclose(props);
		H5Sclose(dataspace);
	} else if (create && !append) {
		dataspace = H5Screate_simple(dim, shape, NULL);
		dataset = H5Dcreate(handle, dset_name, datatype, dataspace, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
		H5Dwrite(dataset, datatype, H5S_ALL, dataspace, H5P_DEFAULT, data);
		H5Dclose(dataset);
		H5Sclose(dataspace);
	} else if (!create && append) {
		dataset = H5Dopen(handle, dset_name, H5P_DEFAULT);
		dataspace = H5Dget_space(dataset);
		hsize_t dataspace_shape[dim];
		H5Sget_simple_extent_dims(dataspace, dataspace_shape, NULL);
		memspace = H5Screate_simple(dim, shape, NULL);

		hsize_t newshape[dim];
		for (unsigned i = 0; i < dim; i++)
			newshape[i] = shape[i] + dataspace_shape[i];
		H5Dset_extent(dataset, newshape);
		dataspace = H5Dget_space(dataset);

		H5Sselect_hyperslab(dataspace, H5S_SELECT_SET, dataspace_shape, NULL, shape, NULL);
		H5Dwrite(dataset, datatype, memspace, dataspace, H5P_DEFAULT, data);
		H5Dclose(dataset);
		H5Sclose(dataspace);
		H5Sclose(memspace);
	} else
		return false;

	return true;
}

//Use this for a value which is saved once per simulation
template<typename T>
inline void save_h5_scalar(const char *filename, const char *dataname, T value)
{
	hid_t file;	//File handle
	hsize_t num_entries = 1;
	if (access(filename, F_OK) == -1)
		//The file does not already exist; create it
		file = H5Fcreate(filename, H5F_ACC_EXCL, H5P_DEFAULT, H5P_DEFAULT);
	else
		//The file already exists; append to it
		file = H5Fopen(filename, H5F_ACC_RDWR, H5P_DEFAULT);

	//Check if the dataset exists already
	if (!H5Lexists(file, dataname, 0)) {
		//Create a chunked dataset, so it may later be extended
		hsize_t max_entries = H5S_UNLIMITED;
		hsize_t chunk_size = 128;

		//Write the value
		write_to_h5(file, &value, 1, &num_entries, &max_entries, &chunk_size, getH5DataType<T>(), dataname, true, true);
	} else
		write_to_h5(file, &value, 1, &num_entries, NULL, NULL, getH5DataType<T>(), dataname, false, true);

	H5Fclose(file);
}

//Use this for a set of values saved at the end of a simulation
//We do not need to worry about chunking since the samplename should be
//the random seed associated with the simulation
template<typename T>
inline void save_h5_vector(const char *filename, const char *dataname, const char *samplename, T* values, hsize_t *shape, hsize_t dim)
{
	hid_t file;
	hid_t group;
	if (access(filename, F_OK) == -1) {
		file = H5Fcreate(filename, H5F_ACC_EXCL, H5P_DEFAULT, H5P_DEFAULT);
		group = H5Gcreate(file, dataname, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
	} else {
		file = H5Fopen(filename, H5F_ACC_RDWR, H5P_DEFAULT);
		if (!H5Lexists(file, dataname, 0))
			group = H5Gcreate(file, dataname, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
		else
			group = H5Gopen(file, dataname, H5P_DEFAULT);
	}

	write_to_h5(group, values, dim, shape, NULL, NULL, getH5DataType<T>(), samplename, true, false);

	H5Gclose(group);
	H5Fclose(file);
}

//Use this to write metadata
//It should be the same among different samples (graphs) with the same flags
//If the value varies among samples, use save_h5_scalar instead
template<typename T>
inline void save_h5_metadata(const char *filename, const char *attrname, T value)
{
	//If the file already exists, then the metadata was already added
	hid_t file;
	if (access(filename, F_OK) == -1)
		file = H5Fcreate(filename, H5F_ACC_EXCL, H5P_DEFAULT, H5P_DEFAULT);
	else
		file = H5Fopen(filename, H5F_ACC_RDWR, H5P_DEFAULT);

	if (!H5Aexists(file, attrname)) {
		hsize_t dims = 1;
		hid_t attrspace = H5Screate_simple(1, &dims, NULL);
		hid_t attr = H5Acreate2(file, attrname, getH5DataType<T>(), attrspace, H5P_DEFAULT, H5P_DEFAULT);
		H5Awrite(attr, getH5DataType<T>(), &value);
		H5Aclose(attr);
		H5Sclose(attrspace);
	}

	H5Fclose(file);
}

//For efficient access you want nrow > ncol
//We will write one row at a time
//For usage with MPI: each rank writes one column
template<typename T>
inline void save_h5_matrix_init(const char *filename, const char *dataname, std::vector<std::pair<hid_t,std::string>> &datasets, hid_t &file, hid_t &group, hid_t &dataspace, hid_t &memspace, hsize_t rowdim[2], const hsize_t nrow, const hsize_t ncol)
{
	hsize_t dim = 2;
	rowdim[0] = 1;
	rowdim[1] = ncol;
	hsize_t matrixdim[] = { nrow, ncol };

	hid_t fapl_id = H5P_DEFAULT;
	hid_t lcpl_id = H5Pcreate(H5P_LINK_CREATE);
	H5Pset_create_intermediate_group(lcpl_id, 1);
	#ifdef MPI_ENABLED
	fapl_id = H5Pcreate(H5P_FILE_ACCESS);
	H5Pset_fapl_mpio(fapl_id, MPI_COMM_WORLD, MPI_INFO_NULL);
	#endif

	if (!file) {
		if (access(filename, F_OK) == -1) {
			file = H5Fcreate(filename, H5F_ACC_EXCL, H5P_DEFAULT, fapl_id);
		} else {
			file = H5Fopen(filename, H5F_ACC_RDWR, fapl_id);
		}
	}

	if (!group) {
		bool exists = H5Lexists(file, dataname, 0) > 0;
		if (!exists) {
			group = H5Gcreate(file, dataname, lcpl_id, H5P_DEFAULT, H5P_DEFAULT);
		} else {
			group = H5Gopen(file, dataname, H5P_DEFAULT);
		}
	}

	if (!dataspace)
		dataspace = H5Screate_simple(dim, matrixdim, NULL);

	if (!memspace) {
		#ifdef MPI_ENABLED
		rowdim[1] = 1;
		#endif
		memspace = H5Screate_simple(dim, rowdim, NULL);
		#ifdef MPI_ENABLED
		rowdim[1] = ncol;
		#endif
	}


	for (size_t i = 0; i < datasets.size(); i++)
		datasets[i].first = H5Dcreate(group, datasets[i].second.c_str(), getH5DataType<T>(), dataspace, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

	H5Pclose(lcpl_id);
	#ifdef MPI_ENABLED
	H5Pclose(fapl_id);
	#endif
}

//If using MPI, call this using rank 0 *before* calling the standard init with all ranks
inline void save_h5_matrix_init_mpi(const char *filename, const char *dataname, hid_t &file, hid_t &group)
{
	#ifndef MPI_ENABLED
	fprintf(stderr, "Cannot call save_h5_matrix_init_mpi() when MPI is not enabled!\n");
	assert (false);
	#endif

	hid_t lcpl_id = H5Pcreate(H5P_LINK_CREATE);
	H5Pset_create_intermediate_group(lcpl_id, 1);
	hid_t fapl_id = H5P_DEFAULT;

	if (access(filename, F_OK) == -1)
		file = H5Fcreate(filename, H5F_ACC_EXCL, H5P_DEFAULT, fapl_id);
	else
		file = H5Fopen(filename, H5F_ACC_RDWR, fapl_id);

	herr_t (*old_func)(hid_t, void*);
	void *old_client_data;
	H5Eget_auto(H5E_DEFAULT, &old_func, &old_client_data);
	H5Eset_auto(H5E_DEFAULT, NULL, NULL);
	bool exists = H5Lexists(file, dataname, 0) > 0;
	H5Eset_auto(H5E_DEFAULT, old_func, old_client_data);

	group = 0;
	if (!exists)
		group = H5Gcreate(file, dataname, lcpl_id, H5P_DEFAULT, H5P_DEFAULT);

	if (group)
		H5Gclose(group);
	H5Fclose(file);

	file = group = 0;
}


//Initialize 'offset' as { 0, 0 }
template<typename T>
inline void save_h5_matrix_row(const T * const rowdata, const hid_t dataset, const hid_t dataspace, const hid_t memspace, const hsize_t rowdim[2], hsize_t offset[2], const hid_t plist)
{
	H5Sselect_hyperslab(dataspace, H5S_SELECT_SET, offset, NULL, rowdim, NULL);
	if (rowdata == NULL) {
		H5Sselect_none(dataspace);
		H5Sselect_none(memspace);
	}
	H5Dwrite(dataset, getH5DataType<T>(), memspace, dataspace, plist, rowdata);
	offset[0]++;
}

inline void save_h5_matrix_close(hid_t &file, hid_t &group, hid_t &dataspace, hid_t &memspace, std::vector<std::pair<hid_t,std::string>> &datasets)
{
	for (size_t i = 0; i < datasets.size(); i++)
		H5Dclose(datasets[i].first);
	H5Sclose(memspace);
	H5Sclose(dataspace);
	H5Gclose(group);
	H5Fclose(file);
}

}

#endif
