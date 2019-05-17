###########################
#(C) Will Cunningham 2014 #
#         DK Lab          #
# Northeastern University #
###########################

AC_DEFUN([AX_FASTMATH],
[
AC_PROVIDE([AX_FASTMATH])
AC_REQUIRE([AC_HEADER_STDC])
AC_REQUIRE([AX_PATH_GSL])

AC_ARG_WITH([fastmath], AS_HELP_STRING([--with-fastmath=PREFIX], [Prefix of your fastmath installation]),
  [fastmath_prefix=$with_fastmath],
  [ if [[[ "$#" -ge 1 && "$1" != "" ]]] ; then
      fastmath_prefix="$1"
    else
      fastmath_prefix="../"
    fi
  ])

if [[[ $fastmath_prefix != "" ]]] ; then
  AC_MSG_CHECKING([for fastmath directory])
  if [[[ -d $fastmath_prefix/fastmath ]]] ; then
    AC_MSG_RESULT([found])
    FASTMATH_HOME=$fastmath_prefix/fastmath
    VALID_FASTMATH=yes
  else
    AC_MSG_RESULT([not found!])
    AC_MSG_WARN([fastmath was not found in $fastmath_prefix])
    VALID_FASTMATH=no
  fi
else
  AC_MSG_WARN([Could not find the installation directory for fastmath.])
  VALID_FASTMATH=no
fi

if [[[ $VALID_FASTMATH == "yes" ]]] ; then
  FASTMATH_INCLUDE="-I $FASTMATH_HOME/include"
  FASTMATH_LDFLAGS="-L $FASTMATH_HOME/lib -lfastmath$2"

  cxx_flags=$CXXFLAGS
  ld_flags=$LDFLAGS

  CXXFLAGS="$CXXFLAGS $FASTMATH_INCLUDE"
  LDFLAGS="$LDFLAGS $FASTMATH_LDFLAGS"

  AC_CHECK_HEADER([FastMath.h],, AC_MSG_WARN([Could not find FastMath.h]); VALID_FASTMATH=no, [#include <FastMath.h>])
  AC_CHECK_HEADER([nint.h],, AC_MSG_WARN([Could not find nint.h]); VALID_FASTMATH=no, [#include <nint.h>])
  AC_CHECK_HEADER([FastNumInt.h],, AC_MSG_WARN([Could not find FastNumInt.h]); VALID_FASTMATH=no, [#include <FastNumInt.h>])
dnl This next line sometimes gives errors for newer compilers, so we will exclude it
  AC_CHECK_HEADER([FastBitset.h],, AC_MSG_WARN([Could not find FastBitset.h]); VALID_FASTMATH=no, [#include <FastBitset.h>])
  AC_CHECK_HEADER([stopwatch.h],, AC_MSG_WARN([Could not find stopwatch.h]); VALID_FASTMATH=no, [#include <stopwatch.h>])
  AC_CHECK_FILE([$FASTMATH_HOME/lib/libfastmath$2.a],, AC_MSG_WARN([Could not find libfastmath]); VALID_FASTMATH=no)

  CXXFLAGS=$cxx_flags
  LDFLAGS=$ld_flags
fi

AC_MSG_CHECKING([if fastmath will be used])
if [[[ $VALID_FASTMATH == "yes" ]]] ; then
  AC_MSG_RESULT([yes])
  AC_SUBST([FASTMATH_FLAGS], [$FASTMATH_INCLUDE])
  AC_SUBST([FASTMATH_LDFLAGS], [$FASTMATH_LDFLAGS])
  AC_SUBST([FASTMATH_ENABLED], [yes])
else
  AC_MSG_RESULT([no])
  if [[[ "$#" -ge 2 && "$3" == "mandatory" ]]] ; then
    AC_MSG_ERROR([Fastmath is required for this package.])
  fi
fi
])
