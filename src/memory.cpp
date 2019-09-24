#include <fastmath/memory.h>

namespace fastmath {

int parseLine(char* line) {
	// This assumes that a digit will be found and the line ends in " Kb".
	int i = strlen(line);
	const char* p = line;
	while (*p <'0' || *p > '9') p++;
	line[i-3] = '\0';
	i = atoi(p);
	return i;
}

}
