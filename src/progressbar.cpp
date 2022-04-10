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

#include <fastmath/progressbar.h>

namespace fastmath {

void printProgress(ProgressBar &pb)
{
	printf("[");
	float pos = pb.get_pos();
	for (int i = 0; i < pb.bar_width; i++) {
		if (i < pos) printf("=");
		else if (i == pos) printf(">");
		else printf(" ");
	}
	printf("] %d %%\r", static_cast<int>(pb.get_progress() * 100));
	fflush(stdout);
	return;
}

void updateProgress(ProgressBar &pb, int step)
{
	pb.step = step;
	if (!(pb.step % pb.get_unit()))
		printProgress(pb);
	return;
}

void completeProgress(ProgressBar &pb)
{
	printf("[");
	for (int i = 0; i < pb.bar_width; i++)
		printf("=");
	printf("] 100%%\n");
	fflush(stdout);
	return;
}

}
