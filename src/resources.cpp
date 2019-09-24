#include <fastmath/resources.h>

namespace fastmath {

#ifdef CUDA_ENABLED

//This will output the proper CUDA error strings in the event that a CUDA host call returns an error
void __checkCudaErrors(CUresult err, const char *file, const int line)
{
	if (CUDA_SUCCESS != err) {
    		fprintf(stderr, "checkCudaErrors() Driver API error = %04d \"%s\" from file <%s>, line %i.\n", err, getCudaDrvErrorString(err), file, line);
		#ifdef MPI_ENABLED
		MPI_Abort(MPI_COMM_WORLD, 3);
		#else
    		exit(3);
		#endif
	}
}

//This will output the proper error string when calling cudaGetLastError
void __getLastCudaError(const char *errorMessage, const char *file, const int line)
{
	cudaError_t err = cudaGetLastError();
	if (cudaSuccess != err) {
    		fprintf(stderr, "%s(%i) : getLastCudaError() CUDA error : %s : (%d).\n", file, line, errorMessage, (int)err);
		fprintf(stderr, "CUDA Error Message: %s\n", cudaGetErrorString(err));
		#ifdef MPI_ENABLED
		MPI_Abort(MPI_COMM_WORLD, 4);
		#else
       		exit(4);
		#endif
	}
}

#endif

//MPI Print Variadic Function
//Allows only the master process to print to stdout
//If MPI is not enabled, rank == 0
int printf_mpi(int rank, const char * format, ...)
{
	int retval = 0;

	if (rank == 0) {
		va_list argp;
		va_start(argp, format);
		vprintf(format, argp);
		va_end(argp);
	}

	return retval;
}

//Debug Print Variadic Function
int printf_dbg(const char * format, ...)
{
	printf_mag();
	va_list argp;
	va_start(argp, format);
	vprintf(format, argp);
	va_end(argp);
	printf_std();
	fflush(stdout);

	return 0;
}

//These next three functions are loosely derived from shrQATest.h in the CUDA SDK

int printStart(const char **argv, const int &rank)
{
	int exename_start = findExeNameStart(argv[0]);
	printf_mpi(rank, "[%s] starting...\n\n", &(argv[0][exename_start]));
	fflush(stdout);
	if (rank == 0)
		printCPUInfo();
	return exename_start;
}

void printFinish(const char **argv, const int &exename_start, const int &rank, int iStatus)
{
	const char *sStatus[] = { "FAILED", "PASSED", "WAIVED", NULL };
	printf_mpi(rank, "[%s] results...\n%s\n\n", &(argv[0][exename_start]), sStatus[iStatus]);
	fflush(stdout);
}

int findExeNameStart(const char *exec_name)
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

//Print Info about the CPU
void printCPUInfo()
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
			printf("> Using Processor: %s\n", info.c_str());
			fflush(stdout);
			break;
		}
	}
}

//Print 'CHECKPOINT' for debugging
void printChk()
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

#ifdef CUDA_ENABLED

//Initialize connection to GPU
void connectToGPU(CuResources *cu, int argc, char **argv, const int &rank)
{
	//No null pointers
	#if CU_DEBUG
	assert (cu != NULL);
	#endif

	//Pick CUDA Device
	CUresult status;
	int devCount, major = 0, minor = 0;

	checkCudaErrors(cuInit(0));
	checkCudaErrors(cuDeviceGetCount(&devCount));
	assert (MAX_GPUS == devCount);	//If not, something is wrong
	//#if CU_DEBUG
	//assert(cu->gpuID > -1 && cu->gpuID < devCount);
	//#endif
	for (unsigned i = 0; i < devCount; i++) {
		cu->cuDevice[i] = findCudaDevice(i, rank);

		//checkCudaErrors(cuDeviceComputeCapability(&major, &minor, cu->cuDevice[i]));
		checkCudaErrors(cuDeviceGetAttribute(&major, CU_DEVICE_ATTRIBUTE_COMPUTE_CAPABILITY_MAJOR, cu->cuDevice[i]));
		checkCudaErrors(cuDeviceGetAttribute(&minor, CU_DEVICE_ATTRIBUTE_COMPUTE_CAPABILITY_MINOR, cu->cuDevice[i]));

		//Statistics about the Device
		printf_mpi(rank, "  GPU device has SM %d.%d compute capabilities\n", major, minor);
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
		if (devCount > 1)
			checkCudaErrors(cuCtxPopCurrent(&cu->cuContext[i]));
	}
}

//Find available GPUs
CUdevice findCudaDevice(int id, const int &rank)
{
    	CUdevice device;
      	char name[100];

	checkCudaErrors(cuDeviceGet(&device, id));
	cuDeviceGetName(name, 100, device);
	printf_mpi(rank, "> Using CUDA Device [%d]: %s\n", id, name);
	fflush(stdout);

    	return device;
}

#endif

template<>
hid_t getH5DataType<unsigned>() { return H5T_STD_U32LE; }

template<>
hid_t getH5DataType<float>() { return H5T_IEEE_F32LE; }

template<>
hid_t getH5DataType<double>() { return H5T_IEEE_F64LE; }

bool write_to_h5(hid_t &handle, void *data, hsize_t dim, hsize_t *shape, hsize_t *maxshape, hsize_t *chunk_size, hid_t datatype, const char *dset_name, bool create, bool append)
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

template<typename T>
void save_h5_scalar(const char *filename, const char *dataname, T value)
{
	hid_t file;	//File handle
	hsize_t num_entries = 1;
	if (access(filename, F_OK) == -1) {
		//The file does not already exist; create it
		file = H5Fcreate(filename, H5F_ACC_EXCL, H5P_DEFAULT, H5P_DEFAULT);

		//Create a chunked dataset, so it may later be extended
		hsize_t max_entries = H5S_UNLIMITED;
		hsize_t chunk_size = 128;

		//Write the value
		write_to_h5(file, &value, 1, &num_entries, &max_entries, &chunk_size, getH5DataType<T>(), dataname, true, true);
	} else {
		//The file already exists; append to it
		file = H5Fopen(filename, H5F_ACC_RDWR, H5P_DEFAULT);
		write_to_h5(file, &value, 1, &num_entries, NULL, NULL, getH5DataType<T>(), dataname, false, true);
	}
	H5Fclose(file);
}

template<typename T>
void save_h5_vector(const char *filename, const char *dataname, const char *samplename, T* values, hsize_t *shape, hsize_t dim)
{
	hid_t file;
	hid_t group;
	if (access(filename, F_OK) == -1) {
		file = H5Fcreate(filename, H5F_ACC_EXCL, H5P_DEFAULT, H5P_DEFAULT);
		group = H5Gcreate(file, dataname, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
	} else {
		file = H5Fopen(filename, H5F_ACC_RDWR, H5P_DEFAULT);
		group = H5Gopen(file, dataname, H5P_DEFAULT);
	}
	
	write_to_h5(group, values, dim, shape, NULL, NULL, getH5DataType<T>(), samplename, true, false);

	H5Gclose(group);
	H5Fclose(file);
}

}
