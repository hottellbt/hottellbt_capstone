#include <iostream>
#include <exception>
#include <cerrno>
#include <cstring>

#include "unicode.hpp"
#include "terminal.hpp"
#include "demo.hpp"

bool is_set_up = false;

int main();

int main() {
	Demo::setup_terminal();

	try {

		Demo::init();
		Demo::draw();

		Terminal::Event event;

		while (Demo::is_running()) {
			Terminal::next_event(event);
			Demo::event(event);
			Demo::draw();
		}

	} catch (const std::exception& e) {
		Demo::cleanup_terminal();
		std::cerr << "Uncaught exception: " << e.what() << std::endl;
		if (errno != 0) {
			std::cerr << "errno " << std::to_string(errno) << ": "
				<< std::strerror(errno) << std::endl;
		}
		return 128;
	}

	Demo::cleanup_terminal();
	return 0;
}

void Demo::setup_terminal() {
	if (is_set_up) return;
	is_set_up = true;

	Terminal::enable_alt_buffer();
	Terminal::hide_cursor();
	Terminal::clear();
	Terminal::mv(0, 0);
	Terminal::set_raw(true);
}

void Demo::cleanup_terminal() {
	if (!is_set_up) return;
	is_set_up = false;

	Terminal::set_raw(false);
	Terminal::disable_alt_buffer();
	Terminal::show_cursor();
	Terminal::unset_bg();
	Terminal::unset_fg();
	Terminal::flush();
}

