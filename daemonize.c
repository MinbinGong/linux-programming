#include <stdio.h>
#include <sys/resource.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

int daemonize()
{
    // clear file permission mask
    umask(0);
    
}