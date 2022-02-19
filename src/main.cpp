#include <iostream>

#include "unicode.hpp"

#include "ansi_terminal.hpp"


int main() {
	Terminal::set_raw(true);

	Terminal::Event event;

	bool running = true;

	while (running) {
		Terminal::next_event(event);
		Terminal::print_raw(std::to_string((uint8_t) event.type));
		Terminal::print_raw(' ');
		Terminal::flush();

		switch (event.type) {
			case Terminal::EventType::EXIT:
				running = false;
				break;
			case Terminal::EventType::KEY:
				running = event.key_event.codepoint != 'q';
				break;
		}
	}

	Terminal::set_raw(false);
	Terminal::print_raw("\n");
	return 0;
}

