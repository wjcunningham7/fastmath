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

#ifndef FASTMATH_PROGRESSBAR_H
#define FASTMATH_PROGRESSBAR_H

#include <math.h>
#include <stdio.h>

namespace fastmath {

struct ProgressBar {
	ProgressBar() : bar_width(70), steps(0), step(0) {}
	ProgressBar(int _steps) : bar_width(70), steps(_steps), step(0) {}

	int bar_width;
	int steps;
	int step;

	inline float get_progress() { return static_cast<float>(step) / steps; }
	inline int get_pos() { return bar_width * get_progress(); }
	inline int get_unit() { return (int)ceil(float(steps) / 100); }
};

void printProgress(ProgressBar &pb);
void updateProgress(ProgressBar &pb, int step);
void completeProgress(ProgressBar &pb);

}

#endif
