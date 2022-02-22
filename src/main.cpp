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
	Terminal::hide_cursor();
	Terminal::clear();
	Terminal::mv_home();
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

class List {
	public:
		void full_draw();
		void quick_draw();

		size_t get_num_options() { return 5; }

		Unicode::string_t get_option(size_t idx) {
			switch (idx) {
				case 0: return UTF8::decode("choice0");
				case 1: return UTF8::decode("chooooiice 1");
				case 2: return UTF8::decode("2");
				case 3: return UTF8::decode("");
				case 4: return UTF8::decode("aahhhaa");
			}
			throw std::runtime_error("out of bounds");
		}

		int selection_idx = 0;
	private:
		void draw_option(size_t idx);
		bool needs_full_draw = true;
		int x = 0, y = 0, width = 10, height = 10;
		int prior_selection_idx = 0;
};

void List::draw_option(size_t idx) {
	int draw_y = y + idx;
	bool highlight = idx == selection_idx;
	if (highlight) { Terminal::set_invert(); }
	Terminal::mvaddstr(x, draw_y, get_option(idx));
	if (highlight) { Terminal::unset_invert(); }
}

void List::full_draw() {
	prior_selection_idx = selection_idx;
	needs_full_draw = false;
	for (size_t i = 0; i < get_num_options(); i++) {
		draw_option(i);
	}
}

void List::quick_draw() {
	if (needs_full_draw) {
		this->full_draw();
		return;
	}
	if (prior_selection_idx == selection_idx) {
		return;
	}
	draw_option(prior_selection_idx);
	draw_option(selection_idx);
	prior_selection_idx = selection_idx;
}

List list;

void draw() {
	int x, y, w, h;
	Terminal::get_size(w, h);

	list.quick_draw();
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
					switch (cp) {
						case 'q':
							running = false;
							break;
						case 'j':
							list.selection_idx++;
							break;
						case 'k':
							list.selection_idx--;
							break;
					}
				}
				draw();
				break;

			default:
				break;
		}
	}
}

