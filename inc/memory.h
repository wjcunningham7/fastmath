#ifndef FASTMATH_MEMORY_H
#define FASTMATH_MEMORY_H

#error "This file is not ready to be used."

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

#endif
