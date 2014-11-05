#include <fastmath/stopwatch.h>

void stopwatchStart(struct Stopwatch *sw)
{
	assert (sw != NULL);
	gettimeofday(&sw->startTime, NULL);
}

void stopwatchStop(struct Stopwatch *sw)
{
	assert (sw != NULL);
	assert (sw->startTime.tv_sec != 0 && sw->startTime.tv_usec != 0);
	gettimeofday(&sw->stopTime, NULL);
	long ds = sw->stopTime.tv_sec - sw->startTime.tv_sec;
	long dus = sw->stopTime.tv_usec - sw->startTime.tv_usec;
	sw->elapsedTime = ds + 0.000001 * dus;
}

void stopwatchReset(struct Stopwatch *sw)
{
	assert (sw != NULL);
	sw->startTime = (struct timeval){0,0};
	sw->stopTime = (struct timeval){0,0};
	sw->elapsedTime = 0.0;
}
