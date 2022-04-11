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

AC_PREREQ([2.69])
AC_INIT([fastmath], m4_normalize(m4_include([VERSION])), [wjcunningham7@gmail.com])
AM_INIT_AUTOMAKE

AC_LANG([C++])

AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADERS([include/fastmath/config.h])
AC_CHECK_FILES([include/fastmath/fastmath.h],, AC_MSG_ERROR([Could not find the fastmath header files.]))

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
