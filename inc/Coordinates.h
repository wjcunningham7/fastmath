#ifndef COORDINATES_H_
#define COORDINATES_H_

#include <algorithm>
#define __STDC_FORMAT_MACROS
#include <inttypes.h>
#include <stddef.h>
#include <stdio.h>

/////////////////////////////
//(C) Will Cunningham 2017 //
//         DK Lab          //
// Northeastern University //
/////////////////////////////

#ifndef COORD_SAFE
#define COORD_SAFE 0
#endif

#ifndef CUDA_ENABLED
struct __attribute__ ((aligned(8))) float2 {
	float x, y;
};

extern inline float2 make_float2(float x, float y)
{
	float2 f;
	f.x = x; f.y = y;
	return f;
}

struct float3 {
	float x, y, z;
};

extern inline float3 make_float3(float x, float y, float z)
{
	float3 f;
	f.x = x; f.y = y; f.z = z;
	return f;
}

struct __attribute ((aligned(16))) float4 {
	float w, x, y, z;
};

extern inline float4 make_float4(float w, float x, float y, float z)
{
	float4 f;
	f.w = w; f.x = x; f.y = y; f.z = z;
	return f;
}
#endif

class Coordinates
{
public:
	//Default Constructors
	Coordinates()
	{
		ndim = n = 0;
		coords = NULL;
	}

	Coordinates(unsigned int _ndim, uint64_t _n) : ndim(_ndim), n(_n)
	{
		#if COORD_SAFE
		if (!(_ndim & _n))
			fprintf(stderr, "Cannot initialize 'Coordinates' with size zero.\n");
		else
		#endif
			coords = new float[_ndim*_n]();
	}

	//Copy Constructor
	Coordinates(const Coordinates& other)
	{
		#if COORD_SAFE
		if (this == other)
			fprintf(stderr, "Error copying 'Coordinates' object into to itself.\n");
		else
		#endif
		{
			ndim = other.ndim;
			n = other.n;
			coords = new float[ndim*n]();
			std::copy(other.coords, other.coords + other.ndim * other.n, coords);
		}
	}

	//Assignment Operator
	Coordinates& operator= (const Coordinates& other)
	{
		#if COORD_SAFE
		if (this == other)
			fprintf(stderr, "Error assigning 'Coordinates' object to itself.\n");
		else
		#endif
		{
			float *_coords = new float[other.ndim*other.n];
			ndim = other.ndim;
			n = other.n;
			std::copy(other.coords, other.coords + ndim * n, _coords);
			delete [] coords;
			coords = _coords;
		}
		return *this;
	}

	//Equality Operator
	inline bool operator== (Coordinates const& other) const
	{
		if (n != other.n || ndim != other.ndim || coords != other.coords)
			return false;
		return true;
	}

	//Destructor
	~Coordinates()
	{
		#if COORD_SAFE
		if (coords != NULL)
		#endif
			delete [] coords;
		coords = NULL;
		ndim = n = 0;
	}

	inline unsigned int getDim() const
	{
		return ndim;
	}

	inline uint64_t size() const
	{
		return n;
	}

	inline float * dim(unsigned int _dim)
	{
		#if COORD_SAFE
		if (_dim >= ndim) {
			fprintf(stderr, "Invalid argument passed to 'dim'.\n");
			return coords;
		}
		#endif
		return coords + _dim * n;
	}

	inline float & operator() (unsigned int _dim, uint64_t idx)
	{
		#if COORD_SAFE
		if (_dim >= ndim || idx >= n) {
			fprintf(stderr, "Index out of bounds in 'Coordinates'.\n");
			return coords[0];
		}
		#endif
		return coords[_dim*n+idx];
	}

	inline float2 getFloat2(uint64_t idx)
	{
		#if COORD_SAFE
		if (ndim != 2 || idx >= n) {
			fprintf(stderr, "Invalid call to 'getFloat2' in 'Coordinates'.\n");
			return make_float2(0.0, 0.0);
		}
		#endif

		return make_float2(coords[idx], coords[n+idx]);
	}

	inline void setFloat2(float2 val, uint64_t idx)
	{
		#if COORD_SAFE
		if (idx >= n) {
			fprintf(stderr, "Index out of bounds in 'setFloat2' in 'Coordinates'.\n");
			return;
		}
		#endif

		coords[idx] = val.x;
		coords[n+idx] = val.y;
	}

	inline float3 getFloat3(uint64_t idx)
	{
		#if COORD_SAFE
		if (idx >= n) {
			fprintf(stderr, "Index out of bounds in 'getFloat3' in 'Coordinates'.\n");
			return;
		}
		#endif

		return make_float3(coords[idx], coords[n+idx], coords[(n<<1)+idx]);
	}

	inline void setFloat3(float3 val, uint64_t idx)
	{
		#if COORD_SAFE
		if (idx >= n) {
			fprintf(stderr, "Index out of bounds in 'setFloat3' in 'Coordinates'.\n");
			return;
		}
		#endif

		coords[idx] = val.x;
		coords[n+idx] = val.y;
		coords[(n<<1)+idx] = val.z;
	}

	inline float4 getFloat4(uint64_t idx)
	{
		#if COORD_SAFE
		if (idx >= n) {
			fprintf(stderr, "Index out of bounds in 'getFloat4' in 'Coordinates'.\n");
			return;
		}
		#endif
		
		return make_float4(coords[idx], coords[n+idx], coords[(n<<1)+idx], coords[n*3+idx]);
	}

	inline void setfloat4(float4 val, uint64_t idx)
	{
		#if COORD_SAFE
		if (idx >= n) {
			fprintf(stderr, "Index out of bounds in 'setFloat3' in 'Coordinates'.\n");
			return;
		}
		#endif

		coords[idx] = val.w;
		coords[n+idx] = val.x;
		coords[(n<<1)+idx] = val.y;
		coords[n*3+idx] = val.z;
	}

private:
	float *coords;
	unsigned int ndim;
	uint64_t n;
};

#endif
