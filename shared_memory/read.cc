// shared memory, open and write

#include <iostream>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

using namespace std;

int main()
{
	int fd,flags,length;
	char *address;
	mode_t perms;
	string text;
	struct stat sb;
	
	flags = O_RDWR;
	perms = S_IRUSR | S_IWUSR;

	//open a region
	fd = shm_open("/test_mmap",flags,perms);
	if(fd==-1)
		exit(EXIT_FAILURE);

	// get the size of the shared memory
	if(fstat(fd,&sb)==-1)
		exit(EXIT_FAILURE);

	//map the shared region
	address = mmap(NULL,sb.st_size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
	if(address==MAP_FAILED)
		exit(EXIT_FAILURE);

	//close the fd, no longer needed
	if(close(fd)==-1)
		exit(EXIT_FAILURE);

	// read from the memory region
	write(STDOUT_FILENO,address,sb.st_size);

	return 0;
}


	
	
