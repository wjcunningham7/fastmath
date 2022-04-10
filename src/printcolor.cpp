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

#include <fastmath/printcolor.h>

namespace fastmath {

void printf_cyan() { printf("\x1b[36m"); }

void printf_red() { printf("\x1b[31m"); }

void printf_yel() { printf("\x1b[33m"); }

void printf_mag() { printf("\x1b[35m"); }

void printf_std() { printf("\x1b[0m"); }

} // namespace fastmath
