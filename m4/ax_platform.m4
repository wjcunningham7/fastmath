# Copyright 2014-2022 Will Cunningham
#
# This file is part of FastMath.
#
# Licensed under the MIT License (the "License"). A copy of the
# License may be obtained with this software package or at
#
#     https://opensource.org/licenses/MIT
#
# FastMath is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#serial 1

AC_DEFUN([AX_PLATFORM],
[
AC_PROVIDE([AX_PLATFORM])

AC_ARG_WITH([platform], AS_HELP_STRING([--with-platform=PLATFORM], [Name of the platform added to the binary name]),
  [platform=$with_platform],
  [ if [[[ "$#" -ge 1 && "$1" != "" ]]] ; then
      platform="$1"
    else
      platform=""
    fi
  ])

AC_MSG_CHECKING([platform])
if [[[ $platform != "" ]]] ; then
  AC_MSG_RESULT([$platform])
  AC_SUBST([PLATFORM], [_$platform])
else
  AC_MSG_RESULT([generic])
fi
])
