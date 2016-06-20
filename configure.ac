AC_PREREQ([2.63])
AC_INIT([FastMath], [1.1], [w.cunningham@northeastern.edu])
AM_INIT_AUTOMAKE
LT_PREREQ([2.2.6])
LT_INIT

AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_SRCDIR([src/FastMath.cpp])
AC_CONFIG_HEADERS([inc/config.h])

AC_PROG_CC
AC_PROG_CXX
AC_PROG_LIBTOOL
AM_PROG_LIBTOOL

AC_LANG([C++])
AX_CXX_COMPILE_STDCXX([11])

AM_CONDITIONAL(GCC_GE_481, test `g++ -dumpversion | awk '{print $1>=4.8.1?"1":"0"}'` = 1)
AX_PATH_GSL([1.13],, AC_MSG_ERROR([Could not find the required version of GSL.]))
BOOST_REQUIRE([1.55.0])
AC_OPENMP

AX_GCC_X86_CPU_SUPPORTS([avx2],, AC_MSG_RESULT([AVX2 features will be disabled.]))
AX_GCC_X86_CPU_SUPPORTS([popcnt],, AC_MSG_ERROR([CPU does not support popcnt instruction.]))
AX_GCC_ARCHFLAG([no],, AC_MSG_ERROR([CPU architecture could not be identified.]))

AC_CHECK_LIB([m], [cos],, AC_MSG_ERROR([Could not find the library "libm"]))
AC_CHECK_LIB([gslcblas], [cblas_dgemm],, AC_MSG_ERROR([Could not find the library "libgslcblas"]))
AC_CHECK_LIB([gsl], [gsl_blas_dgemm],, AC_MSG_ERROR([Could not find the library "libgsl"]))
AC_CHECK_LIB([stdc++], [main],, AC_MSG_ERROR([Could not find the library "libstdc++"]))

AC_CHECK_FILE([/home/$USER/local/lib64/libnint.a],, AC_MSG_ERROR([Could not find the library "libnint".]))
if [[ $(nm -C /home/$USER/local/lib64/libnint.a | grep monte_carlo_nd | wc -l) -eq 0 ]] ; then
  echo `Could not find the function "monte_carlo_nd" in "libnint".`
fi

AC_HEADER_STDC
AC_HEADER_TIME

AC_C_INLINE
AC_TYPE_SIZE_T
AC_TYPE_UINT64_T

AC_FUNC_MALLOC

# AC_SUBST([FAST_LOCAL_DIR])

CXXFLAGS="-g -O3 -DBOOST_NO_FENV_H -Wno-narrowing -I inc -I opt"
if [[ -z ${HAVE_AVX2_INSTRUCTIONS+x} ]] ; then
  CXXFLAGS+=" -mavx2 -DAVX2_ENABLED"
fi
if [[ -z ${HAVE_POPCNT_INSTRUCTIONS+x} ]] ; then
  CXXFLAGS+=" -mpopcnt"
fi
CXXFLAGS+=" $ax_cv_gcc_archflag"

# AC_CONFIG_SUBDIRS([opt/nintlib])
# AC_CONFIG_FILES([Makefile opt/nintlib/Makefile])

AC_CONFIG_FILES([Makefile opt/nintlib/Makefile])
AC_OUTPUT
