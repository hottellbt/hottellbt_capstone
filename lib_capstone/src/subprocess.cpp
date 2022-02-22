#include "subprocess.hpp"

#include <sys/types.h>
#include <unistd.h>

#include <cstdio>

using Subprocesses::subprocess_error;

void Subprocesses::run(
		const char* path,
		const char* args) {

	if (!pipe) {
		throw subprocess_error("pipe");
	}

	pid_t pid;

	if ((pid = fork()) == -1) {
		throw subprocess_error("fork");
	} else if (pid == 0) {
		return;
	}

	// we are in child thread

	// the exec replaces the current process image
	// i.e. we will never return on success. this is effectively exit()

	execl(path, args);

	// exec above failed
}
