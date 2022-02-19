#include <iostream>

#include "unicode.hpp"

#include "ansi_terminal.hpp"


int main() {
	Terminal::set_raw(true);

	Terminal::Event event;

	bool running = true;

	while (running) {

		Terminal::next_event(event);

		switch (event.type) {

			case Terminal::EventType::EXIT:
				Terminal::print(UTF8::decode("Exit\n\r"));
				running = false;
				break;

			case Terminal::EventType::RESIZE:
				Terminal::print(UTF8::decode("Resize: rows="));
				Terminal::print_raw(std::to_string(event.e_resize.rows));
				Terminal::print(UTF8::decode(", cols="));
				Terminal::print_raw(std::to_string(event.e_resize.cols));
				Terminal::print_raw("\n\r");
				break;

			case Terminal::EventType::TEXT:
				Terminal::print(UTF8::decode("Text: "));

				for (auto cp : event.e_text.text) {
					if (cp == 'q') {
						running = false;
					}
				}

				Terminal::print(event.e_text.text);

				Terminal::print_raw("\n\r");
				break;
		}
	}

	Terminal::set_raw(false);
	Terminal::print_raw("\n");
	return 0;
}

