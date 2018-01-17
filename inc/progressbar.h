#ifndef PROGRESSBAR_H_
#define PROGRESSBAR_H_

/////////////////////////////
//(C) Will Cunningham 2018 //
//         DK Lab          //
// Northeastern University //
/////////////////////////////

#include <math.h>
#include <stdio.h>

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

#endif
