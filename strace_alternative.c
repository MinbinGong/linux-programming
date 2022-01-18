#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <asm/ptrace.h>

static long pid;

int upeek(int pid, long off, long *res)
{
	long val;
	val = ptrace(PTRACE_PEEKUSER, pid, off, 0);
	if (-1 == val) {
		return -1;
	}

	*res = val;
	return 0;
}

void trace_syscall()
{
	long res;

	res = ptrace(PTRACE_SYSCALL, pid, (char *)1, 0);
	if (res < 0) {
		printf("Failed to execute until next syscall: %d\n", res);
	}
}

void sigchld_handler(int signum)
{
	long scno;
	int res;
	if (upeek(pid, 4 * ORIG_EAX, &scno) < 0) {
		return;
	}
	if (scno != 0) {
		printf("System call: %u\n", scno);
	}

	trace_syscall();
	return;
}

int main(int argc, char **argv)
{
	int res;

	if (argc != 2) {
		printf("Usage: ptrace <pid>\n");
		exit(-1);
	}

	pid = strtol(argv[1], NULL, 10);
	if (pid <= 0) {
		printf("No valid pid specified\n");
		exit(-1);
	} else {
		printf("Tracing requested for PID %u\n", pid);
	}

	struct sigaction sigact;
	sigact.sa_handler = sigchld_handler;
	sigaction(SIGCHLD, &sigact, NULL);

	res = ptrace(PTRACE_ATTACH, pid, 0, 0);
	if (res < 0) {
		printf("Failed to attach: %d\n", res);
		exit(-1);
	} else {
		printf("Attached to %u\n", pid);
	}

	for (;;) {
		wait(&res);
		if (res == 0) {
			exit(1);
		}
	}
}