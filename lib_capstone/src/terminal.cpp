#include "terminal.hpp"

#include <signal.h>
#include <termios.h>
#include <unistd.h>

sig_atomic_t got_sigint = 0;
sig_atomic_t got_sigwinch = 0;

constexpr size_t read_buffer_len = 1028;
char read_buffer[read_buffer_len];

sig_atomic_t cached_window_size_valid = 0;
unsigned short cached_window_rows, cached_window_cols;

encoding::UTF8::UTF8BufferedDecoder decoder;

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

