#include <fastmath/progressbar.h>

/////////////////////////////
//(C) Will Cunningham 2018 //
//         DK Lab          //
// Northeastern University //
/////////////////////////////

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
