#ifndef STOPWATCH_H_
#define STOPWATCH_H_

#include <assert.h>
#include <cstdlib>
#include <sys/time.h>

struct Stopwatch {
	Stopwatch() : startTime((struct timeval){0,0}), stopTime((struct timeval){0,0}), elapsedTime(0.0) {}

	struct timeval startTime;
	struct timeval stopTime;
	double elapsedTime;
};

void stopwatchStart(struct Stopwatch *sw);
void stopwatchStop(struct Stopwatch *sw);
void stopwatchReset(struct Stopwatch *sw);

#endif
