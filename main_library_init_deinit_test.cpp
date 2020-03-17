
#include <stdio.h>

extern int nonmain(void);

int main(void)
{
	nonmain();
	printf("Hello world from %s\n", __FUNCTION__);
	return 0;
}
