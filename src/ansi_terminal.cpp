/*
   This file is written assuming we are running on Linux. Windows does
   something else entirely.
*/

#include "terminal.hpp"
#include "ansi_terminal.hpp"

#include <signal.h>
#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>

sig_atomic_t got_sigwinch = 0;
sig_atomic_t got_sigint = 0;

void signal_handler(int signal) {
	switch (signal) {
		case SIGWINCH:
			got_sigwinch = 1;
			break;
		case SIGINT:
		case SIGTERM:
			got_sigint = 1;
			break;
	}
}

void ANSITerminal::next_event(Terminal::Event &event) {
	using Terminal::EventType;

	int bytes_read;
	constexpr int buffer_len = 1;
	char buffer[buffer_len];

	bytes_read = read(0, buffer, buffer_len);
	if (bytes_read > 0) {
		event.type = EventType::KEY;
		return;
	}

	if (got_sigwinch == 1) {
		got_sigwinch = 0;
		winsize window_size;
		ioctl(0, TIOCGWINSZ, &window_size);

		event.type = EventType::RESIZE;
		event.resize_event.rows = window_size.ws_row;
		event.resize_event.cols = window_size.ws_col;
		return;
	}

	if (got_sigint == 1) {
		got_sigint = 0;

		event.type = EventType::EXIT;
		return;
	}

	event.type = EventType::NONE;
}

void ANSITerminal::set_raw() {
	//signal(SIGWINCH, signal_handler);

	static struct sigaction sigwinch_action;

	sigwinch_action.sa_handler = signal_handler;
	sigwinch_action.sa_flags = SA_NOCLDSTOP;

	sigaction(SIGWINCH, &sigwinch_action, 0);

    termios tty;
    tcgetattr(STDIN_FILENO, &tty);

	// cfmakeraw
	// https://linux.die.net/man/3/tcsetattr
	tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON);
	tty.c_oflag &= ~OPOST;
	tty.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
	tty.c_cflag &= ~(CSIZE | PARENB);
	tty.c_cflag |= CS8;

    tcsetattr(STDIN_FILENO, TCSANOW, &tty);
}

void ANSITerminal::flush() {
	fflush(stdout);
}

