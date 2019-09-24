#include <fastmath/printcolor.h>

namespace fastmath {

void printf_cyan()
{
	printf("\x1b[36m");
}

void printf_red()
{
	printf("\x1b[31m");
}

void printf_yel()
{
	printf("\x1b[33m");
}

void printf_mag()
{
	printf("\x1b[35m");
}

void printf_std()
{
	printf("\x1b[0m");
}

}
