#include <iostream>

#include "unicode.hpp"

#include "ansi_terminal.hpp"


int main() {
	ANSITerminal term;
	term.set_raw();

	Terminal::Event event;

	bool running = true;

	while (running) {
		term.next_event(event);
		term.print_raw(std::to_string((uint8_t) event.type));
		term.print_raw(' ');
		term.flush();

		switch (event.type) {
			case Terminal::EventType::EXIT:
				running = false;
				break;
		}
	}

	return 0;
}

