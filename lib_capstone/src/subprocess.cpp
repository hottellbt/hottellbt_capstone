#include "subprocess.hpp"

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <cstdio>

using Subprocesses::subprocess_error;

void Subprocesses::run(
		const char* path,
		char* const* argv) {

	if (!pipe) {
		throw subprocess_error("pipe");
	}

	pid_t pid;

	if ((pid = fork()) == -1) {
		throw subprocess_error("fork");
	} else if (pid != 0) {
		int status;
		int options = 0;
		waitpid(pid, &status, options);
		return;
	}

	// we are in child thread

	// the exec replaces the current process image
	// i.e. we will never return on success. this is effectively exit()

	execv(path, argv);
	throw subprocess_error("execv");

	// exec above failed
}

void Subprocesses::open_editor() {
	char *argv[1] { (char*) nullptr }; 
	run("/usr/bin/vim", argv);
}
