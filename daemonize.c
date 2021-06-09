#include <stdio.h>
#include <sys/resource.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

int daemonize()
{
    // clear file permission mask
    umask(0);
    
    // parent exit
    pid_t pid;
    if ((pid = fork()) < 0) {
        // fork error
        perror("fork error.");
        return -1;
    } else if (0 != pid) {
        printf("parent exit.\n");
        exit(0);
    }

    // child thread set as group leader
    setsid();

    if (chdir("/") < 0) {
        perror("Change dir failed.");
        return -1;
    }
    
    struct rlimit rl;
    if (getrlimit(RLIMIT_NOFILE, &rl) < 0) {
        perror("Get file description failed.\n");
        return -1;
    }
    
    // set 1024 if no limit
    if (rl.rlim_max == RLIM_INFINITY) {
        rl.rlim_max = 1024;
    }
    
    int i;
    for (i = 3; i < rl.rlim_max; ++i) {
        close(i);
    }

    return 0;
}

int main(void)
{
    if (0 == daemonize())
    {
        printf("daemonize ok.\n");
        sleep(20);
    } else {
        printf("daemonize failed.\n");
        sleep(20);
    }
    return 0;
}