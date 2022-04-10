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

#include <fastmath/stopwatch.h>

/* These functions are used for benchmarking.
 * Precision is to the microsecond. */

namespace fastmath {

void stopwatchStart(struct Stopwatch *sw) {
    assert(sw != NULL);
    gettimeofday(&sw->startTime, NULL);
}

void stopwatchStop(struct Stopwatch *sw) {
    assert(sw != NULL);
    assert(!(sw->startTime.tv_sec == 0 && sw->startTime.tv_usec == 0));
    gettimeofday(&sw->stopTime, NULL);
    long ds = sw->stopTime.tv_sec - sw->startTime.tv_sec;
    long dus = sw->stopTime.tv_usec - sw->startTime.tv_usec;
    sw->elapsedTime += ds + 0.000001 * dus;
}

void stopwatchReset(struct Stopwatch *sw) {
    assert(sw != NULL);
    sw->startTime = (struct timeval){0, 0};
    sw->stopTime = (struct timeval){0, 0};
    sw->elapsedTime = 0.0;
}

} // namespace fastmath
