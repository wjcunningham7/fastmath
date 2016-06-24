AC_PREREQ([2.63])
AC_INIT([FastMath], [1.1], [w.cunningham@northeastern.edu])
AM_INIT_AUTOMAKE

AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_SRCDIR([src/FastMath.cpp])
AC_CONFIG_HEADERS([inc/config.h])

AC_PROG_CC
AC_PROG_CXX
AC_PROG_RANLIB

AC_LANG([C++])

AX_COMPILER_VERSION
AX_COMPARE_VERSION([$ax_cv_cxx_compiler_version], [ge], [4.8.1],, AC_MSG_ERROR([GNU compiler must be at least version 4.8.1]))
AX_CXX_COMPILE_STDCXX([11])
AX_PATH_GSL([1.13],, AC_MSG_ERROR([Could not find the required version of GSL.]))
BOOST_REQUIRE([1.55.0])

AC_CHECK_LIB([m], [cos],, AC_MSG_ERROR([Could not find the library "libm"]))
AC_CHECK_LIB([gslcblas], [cblas_dgemm],, AC_MSG_ERROR([Could not find the library "libgslcblas"]))
AC_CHECK_LIB([gsl], [gsl_blas_dgemm],, AC_MSG_ERROR([Could not find the library "libgsl"]))
AC_CHECK_LIB([stdc++], [main],, AC_MSG_ERROR([Could not find the library "libstdc++"]))

AC_CHECK_FILES([src/stopwatch.cpp src/FastMath.cpp src/FastNumInt.cpp],, AC_MSG_ERROR([Could not find fastmath source files.]))
AC_CHECK_FILES([inc/stopwatch.h inc/FastMath.h inc/FastNumInt.h inc/FastBitset.h inc/fastapprox.h],, AC_MSG_ERROR([Could not find fastmath header files.]))
AC_CHECK_FILES([nint/nint.cpp nint/nint.h],, AC_MSG_ERROR([Could not find nint files.]))

AC_CHECK_HEADERS([assert.h cstdlib algorithm cstring inttypes.h limits.h math.h new stddef.h stdio.h stdlib.h sstream stdint.h],, AC_MSG_ERROR([Could not find system header files.]))
AC_CHECK_HEADER([emmintrin.h],, AC_MSG_ERROR([Could not find SSE header file.]))
AC_CHECK_HEADERS([gsl/gsl_errno.h gsl/gsl_integration.h],, AC_MSG_ERROR([Could not find GSL header files.]))
AC_CHECK_HEADER([boost/functional/hash/hash.hpp],, AC_MSG_ERROR([Could not find Boost header files.]))
AC_HEADER_STDC
AC_HEADER_TIME

AC_CHECK_FUNCS([gettimeofday memcpy memset])
AC_FUNC_MALLOC

AC_TYPE_SIZE_T
AC_TYPE_UINT64_T
AC_C_INLINE
AC_C_CONST
AC_C_VOLATILE

CXXFLAGS="$CXXFLAGS -g -O3 -DBOOST_NO_FENV_H -Wno-narrowing -mpopcnt"
AX_GCC_X86_CPU_SUPPORTS([avx2], [CXXFLAGS="$CXXFLAGS -mavx2 -DAVX2_ENABLED";], [CXXFLAGS+=""])
AX_GCC_X86_CPU_SUPPORTS([popcnt],, AC_MSG_ERROR([CPU does not support popcnt instruction.]))
AX_GCC_ARCHFLAG([no],, AC_MSG_ERROR([CPU architecture could not be identified.]))
CXXFLAGS="$CXXFLAGS $ax_cv_gcc_archflag"

xtra=($(for file in $(find inc/intrinsics -type f) ; do echo intrinsics/$(basename $file) ; done))
AC_SUBST([xtra], ["${xtra[[@]]}"])

AC_CONFIG_FILES([Makefile nint/Makefile src/Makefile inc/Makefile])
AC_OUTPUT
