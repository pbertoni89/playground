/*
 * https://stackoverflow.com/questions/5656530/how-to-use-shared-memory-with-linux-in-c
 */

#include <string.h>
#include <cstdio>
#include <iostream>
#include <stdlib.h>
// #include <sys/types.h>
#include <sys/mman.h>
#include <unistd.h>


void * create_shared_memory(size_t size)
{
	// Our memory buffer will be readable and writable:
	int protection = PROT_READ | PROT_WRITE;

	// The buffer will be shared (meaning other processes can access it), but
	// anonymous (meaning third-party processes cannot obtain an address for it),
	// so only this process and its children will be able to use it:
	int visibility = /* MAP_ANONYMOUS | */ MAP_SHARED;

	// The remaining parameters to `mmap()` are not important for this use case,
	// but the manpage for `mmap` explains their purpose.
	return mmap(nullptr, size, protection, visibility, -1, 0);
}


int main()
{
	const char * parent_message = "hello";  // parent process will write this message
	const char * child_message = "goodbye"; // child process will then write this one

	const size_t SZ_SHMEM = 128;
	void * shmem = create_shared_memory(SZ_SHMEM);
	char * cmem = static_cast<char*> (shmem);
	std::cout << "sharing " << SZ_SHMEM << " bytes at address " << shmem << std::endl;

	memcpy(shmem, parent_message, sizeof(parent_message));

	const int pid = fork();

	if (pid == 0)
	{
		std::cout << "Child read: " << cmem << std::endl;
		memcpy(shmem, child_message, sizeof(child_message));
		std::cout << "Child wrote: " << cmem << std::endl;
	}
	else
	{
		std::cout << "Parent read: " << cmem << std::endl;
		sleep(1);  // let concurrent execution of forked child
		std::cout << "After 1s, parent read: " << cmem << std::endl;

		std::cout << "Enter to exit:";
		getc(stdin);
		munmap(shmem, SZ_SHMEM);
	}
}
