# Copyright 2014-2022 Will Cunningham
#
# This file is part of FastMath.
#
# Licensed under the GNU General Public License 3.0 (the "License").
# A copy of the License may be obtained with this software package or at
#
#     https://www.gnu.org/licenses/gpl-3.0.en.html
#
# Use of this file is prohibited except in compliance with the License. Any
# modifications or derivative works of this file must retain this copyright
# notice, and modified files must contain a notice indicating that they have
# been altered from the originals.
#
# FastMath is distributed in the hope that it will be useful, but WITHOUT 
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or 
# FITNESS FOR A PARTICULAR PURPOSE. See the License for more details.
#
#serial 1

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

  cxx_flags=$CXXFLAGS

  CXXFLAGS="$CXXFLAGS $FASTMATH_INCLUDE"

  AC_CHECK_HEADER([fastmath/fastmath.h],, AC_MSG_WARN([Could not find fastmath headers]); VALID_FASTMATH=no, [#include <fastmath/fastmath.h>])

  CXXFLAGS=$cxx_flags
fi

AC_MSG_CHECKING([if fastmath will be used])
if [[[ $VALID_FASTMATH == "yes" ]]] ; then
  AC_MSG_RESULT([yes])
  AC_SUBST([FASTMATH_FLAGS], [$FASTMATH_INCLUDE])
  AC_SUBST([FASTMATH_ENABLED], [yes])
else
  AC_MSG_RESULT([no])
  if [[[ "$#" -ge 1 && "$2" == "mandatory" ]]] ; then
    AC_MSG_ERROR([Fastmath is required for this package.])
  fi
fi
])
