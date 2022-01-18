#include <fcntl.h>
#include <malloc.h>
#include <stdio.h>
#include <unistd.h>

int main()
{
	int handle, bytes;
	void *ptr;

	handle = open("/tmp/test.txt", O_RDONLY);
	ptr = (void *)malloc(150);
	bytes = read(handle, ptr, 150);
	if (0 == bytes) {
		printf("Empty file\n");
		return -1;
	}
	printf("%s", (char *)ptr);

	close(handle);
	return 0;
}