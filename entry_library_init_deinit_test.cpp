
#include <stdio.h>

int nonmain(void)
{
	printf("Hello world from %s\n", __FUNCTION__);
	return 0;
}
