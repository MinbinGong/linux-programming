//只允许一个程序运行
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>

#define PROC_NAME "single_instance"
#define PID_FILE_PATH "/var/run/"

static int lockFile(int fd);
static int isRunning(const char *procname);

int main(void)
{
    if (isRunning(PROC_NAME)) {
        exit(-1);
    }
    printf("run ok\n");
    sleep(20);
    return 0;
}

static int lockFile(int fd)
{
    struct flock fl;
    fl.l_type = F_WRLCK;
    fl.l_start = 0;
    fl.l_whence = SEEK_SET;
    fl.l_len = 0;
    fl.l_pid = -1;

    return (fcntl(fd. F_SETLK, &fl));
}

static int isRunning(const char *procname)
{
    char buf[16] = {0};
    char filename[128] = {0};
    sprintf(filename, "%s%s.pid", PID_FILE_PATH, procname);
    int fd = open(filename, O_CREAT|O_RDWR);
    if (fd < 0) {
        printf("open file %s failed.\n", filename);
        return 1;
    }
    
    if (-1 == lockFile(fd)) {
        printf("%s is already running.\n", procname);
        close(fd);
        return 1;
    } else {
        ftruncate(fd, 0);
        sprintf(buf, "%ld", (long)getpid());
        write(fd, buf, strlen(buf) + 1);
        return 0;
    }
}