/*
   This file is written assuming we are running on Linux. Windows does
   something else entirely.
*/

#include "terminal.hpp"

#include <signal.h>
#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>

bool is_raw = false;

tcflag_t nonraw_c_iflag;
tcflag_t nonraw_c_oflag;
tcflag_t nonraw_c_lflag;
tcflag_t nonraw_c_cflag;

sig_atomic_t got_sigint = 0;
sig_atomic_t got_sigwinch = 0;

constexpr size_t read_buffer_len = 1028;
char read_buffer[read_buffer_len];

sig_atomic_t cached_window_size_valid = 0;
unsigned short cached_window_rows, cached_window_cols;

Encoding::UTF8::UTF8BufferedDecoder decoder;

void signal_handler(int signal) {
	switch (signal) {
		case SIGWINCH:
			got_sigwinch = 1;
			cached_window_size_valid = 0;
			break;
		case SIGINT:
		case SIGTERM:
			got_sigint = 1;
			break;
	}
}

void Terminal::next_event(Terminal::Event &event) {
	using Terminal::EventType;

	int bytes_read = read(0, read_buffer, read_buffer_len);

	if (bytes_read > 0) {
		auto decoded = decoder.decode(read_buffer, bytes_read);
		if (decoded.size() > 0) {
			event.type = EventType::TEXT;
			event.e_text.text = decoded;
			return;
		}
	}

	if (got_sigwinch == 1) {
		got_sigwinch = 0;
		event.type = EventType::RESIZE;
		return;
	}

	if (got_sigint == 1) {
		got_sigint = 0;
		event.type = EventType::EXIT;
		return;
	}

	event.type = EventType::NONE;
}

void Terminal::get_size(unsigned short &cols, unsigned short &rows) {
	if (cached_window_size_valid == 0) {
		winsize window_size;
		ioctl(0, TIOCGWINSZ, &window_size);
		cached_window_rows = window_size.ws_row;
		cached_window_cols = window_size.ws_col;
		cached_window_size_valid = 1;
	}

	rows = cached_window_rows;
	cols = cached_window_cols;
}

void Terminal::set_raw(bool raw) {
	if (is_raw == raw) { return; }
	is_raw = raw;


	static struct sigaction sigwinch_action;

	sigwinch_action.sa_handler = signal_handler;
	sigwinch_action.sa_flags = SA_NOCLDSTOP;

	sigaction(SIGWINCH, &sigwinch_action, 0);


    termios tty;
    tcgetattr(STDIN_FILENO, &tty);

	// cfmakeraw
	// https://linux.die.net/man/3/tcsetattr

	if (raw) {
		nonraw_c_iflag = tty.c_iflag;
		nonraw_c_oflag = tty.c_oflag;
		nonraw_c_lflag = tty.c_lflag;
		nonraw_c_cflag = tty.c_cflag;

		tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON);
		tty.c_oflag &= ~OPOST;
		tty.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
		tty.c_cflag &= ~(CSIZE | PARENB);
		tty.c_cflag |= CS8;
	} else {
		tty.c_iflag = nonraw_c_iflag;
		tty.c_oflag = nonraw_c_oflag;
		tty.c_lflag = nonraw_c_lflag;
		tty.c_cflag = nonraw_c_cflag;
	}

    tcsetattr(STDIN_FILENO, TCSANOW, &tty);
}

void Terminal::flush() {
	fflush(stdout);
}

