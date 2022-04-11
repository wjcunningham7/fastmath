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

inline void printProgress(ProgressBar &pb) {
    printf("[");
    float pos = pb.get_pos();
    for (int i = 0; i < pb.bar_width; i++) {
        if (i < pos)
            printf("=");
        else if (i == pos)
            printf(">");
        else
            printf(" ");
    }
    printf("] %d %%\r", static_cast<int>(pb.get_progress() * 100));
    fflush(stdout);
    return;
}

inline void updateProgress(ProgressBar &pb, int step) {
    pb.step = step;
    if (!(pb.step % pb.get_unit()))
        printProgress(pb);
    return;
}

inline void completeProgress(ProgressBar &pb) {
    printf("[");
    for (int i = 0; i < pb.bar_width; i++)
        printf("=");
    printf("] 100%%\n");
    fflush(stdout);
    return;
}

}

#endif
