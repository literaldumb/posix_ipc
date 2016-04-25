// shared memory, open and write

#include <iostream>
#include <sys/mman.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

using namespace std;

int main()
{
	if(shm_unlink("/test_mmap")==-1)
		exit(EXIT_FAILURE);

	return 0;
}


	
	
