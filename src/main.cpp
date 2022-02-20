#include <iostream>
#include <exception>
#include <cerrno>
#include <cstring>

#include "unicode.hpp"
#include "terminal.hpp"

int main();
void loop();
void cleanup();

int main() {
	Terminal::enable_alt_buffer();
	Terminal::flush();
	Terminal::clear();
	Terminal::mv_home();
	Terminal::flush();
	Terminal::set_raw(true);

	try {
		loop();
	} catch (const std::exception& e) {
		cleanup();

		std::cerr << e.what() << std::endl;

		if (errno != 0) {
			std::cerr << std::strerror(errno) << std::endl;
		}

		return 128;
	}

	cleanup();
	return 0;
}

void cleanup() {
	Terminal::flush();
	Terminal::set_raw(false);
	Terminal::disable_alt_buffer();
}

void loop() {

	Terminal::Event event;

	bool running = true;

	while (running) {

		Terminal::next_event(event);

		switch (event.type) {

			case Terminal::EventType::EXIT:
				Terminal::addraw("Exit\n\r");
				running = false;
				break;

			case Terminal::EventType::RESIZE:
				int rows, cols;
				Terminal::get_size(cols, rows);

				Terminal::addraw("Resize: rows=");
				Terminal::addraw(std::to_string(rows));
				Terminal::addraw(", cols=");
				Terminal::addraw(std::to_string(cols));
				Terminal::addraw("\n\r");
				break;

			case Terminal::EventType::TEXT:
				Terminal::addraw("Text: ");

				for (auto cp : event.e_text.text) {
					if (cp == 'q') {
						running = false;
					}
				}

				Terminal::addstr(event.e_text.text);

				Terminal::addraw("\n\r");
				break;
		}
	}
}

