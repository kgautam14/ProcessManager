#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int
main(int argc, char* argv[])
{
		const char* tag = "child 2\n";
		int interval = 20;
		while(1) {
			printf("%s", tag);
			sleep(interval);
		}
	
}

