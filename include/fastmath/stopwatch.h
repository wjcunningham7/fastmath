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

#ifndef FASTMATH_STOPWATCH_H
#define FASTMATH_STOPWATCH_H

#include <assert.h>
#include <stdlib.h>
#include <sys/time.h>

namespace fastmath {

struct Stopwatch {
    Stopwatch()
        : startTime((struct timeval){0, 0}), stopTime((struct timeval){0, 0}),
          elapsedTime(0.0) {}

    struct timeval startTime;
    struct timeval stopTime;
    double elapsedTime;
};

inline void stopwatchStart(struct Stopwatch *sw) {
    assert(sw != NULL);
    gettimeofday(&sw->startTime, NULL);
}

inline void stopwatchStop(struct Stopwatch *sw) {
    assert(sw != NULL);
    assert(!(sw->startTime.tv_sec == 0 && sw->startTime.tv_usec == 0));
    gettimeofday(&sw->stopTime, NULL);
    long ds = sw->stopTime.tv_sec - sw->startTime.tv_sec;
    long dus = sw->stopTime.tv_usec - sw->startTime.tv_usec;
    sw->elapsedTime += ds + 0.000001 * dus;
}

inline void stopwatchReset(struct Stopwatch *sw) {
    assert(sw != NULL);
    sw->startTime = (struct timeval){0, 0};
    sw->stopTime = (struct timeval){0, 0};
    sw->elapsedTime = 0.0;
}

}

#endif
