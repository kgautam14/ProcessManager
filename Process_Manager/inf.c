#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int
main(int argc, char* argv[])
{
		const char* tag = "child 0\n";
		int interval = 20000;
		while(1) {
			// printf("%s", tag);
			sleep(interval);
		}
	
}

