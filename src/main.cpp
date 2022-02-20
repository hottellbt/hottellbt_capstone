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
	Terminal::show_cursor();
}

void draw() {
	int w, h;
	Terminal::mvaddraw(0, 0, "press q");
	Terminal::hide_cursor();
	Terminal::get_size(w, h);
	Terminal::mvaddraw(w/2, h/2, ":-)");
	Terminal::flush();
}

void loop() {

	Terminal::Event event;

	bool running = true;

	draw();

	while (running) {

		Terminal::next_event(event);

		switch (event.type) {

			case Terminal::EventType::EXIT:
				running = false;
				break;

			case Terminal::EventType::RESIZE:
				Terminal::clear();
				draw();
				break;

			case Terminal::EventType::TEXT:
				for (auto cp : event.e_text.text) {
					if (cp == 'q') {
						running = false;
					}
				}
				break;
		}
	}
}

