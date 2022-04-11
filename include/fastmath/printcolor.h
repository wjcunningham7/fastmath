/* Copyright 2014-2022 Will Cunningham
 * 
 * This file is part of FastMath.
 *
 * Licensed under the GNU General Public License 3.0 (the "License").
 * A copy of the License may be obtained with this software package or at
 *
 *      https://www.gnu.org/licenses/gpl-3.0.en.html
 *
 * Use of this file is prohibited except in compliance with the License. Any
 * modifications or derivative works of this file must retain this copyright
 * notice, and modified files must contain a notice indicating that they have
 * been altered from the originals.
 *
 * FastMath is distributed in the hope that it will be useful, but WITHOUT 
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or 
 * FITNESS FOR A PARTICULAR PURPOSE. See the License for more details. */

#ifndef FASTMATH_PRINTCOLOR_H
#define FASTMATH_PRINTCOLOR_H

#include <stdio.h>

namespace fastmath {

void printf_cyan() { printf("\x1b[36m"); }

void printf_red() { printf("\x1b[31m"); }

void printf_yel() { printf("\x1b[33m"); }

void printf_mag() { printf("\x1b[35m"); }

void printf_std() { printf("\x1b[0m"); }

} // namespace fastmath

#endif
