// shared memory, open and write

#include <iostream>
#include <sys/mman.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

using namespace std;

int main()
{
	int fd,flags;
	char *address;
	mode_t perms;
	string text;
	
	flags = O_RDWR;
	perms = S_IRUSR | S_IWUSR;

	//open a region
	fd = shm_open("/test_mmap",flags,perms);
	if(fd==-1)
		exit(EXIT_FAILURE);

	// set the size of the shared memory
	text = "hello, shared memory region";
	if(ftruncate(fd,text.length())==-1)
		exit(EXIT_FAILURE);

	//map the shared region
	address = mmap(NULL,text.length(),PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
	if(address==MAP_FAILED)
		exit(EXIT_FAILURE);

	//close the fd, no longer needed | OR, use it in read() system call instead of memcpy
	if(close(fd)==-1)
		exit(EXIT_FAILURE);

	// write to the memory region
	memcpy(address,text.c_str(),text.length());

	return 0;
}


	
	
