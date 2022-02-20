#include "demo.hpp"

int List::get_num_options() {
	return 5;
}

Unicode::string_t List::get_option(int idx) {
	switch (idx) {
		case 0: return UTF8::decode("choice0");
		case 1: return UTF8::decode("chooooiice 1");
		case 2: return UTF8::decode("2");
		case 3: return UTF8::decode("");
		case 4: return UTF8::decode("aahhhaa");
	}
	throw std::runtime_error("out of bounds");
}

void List::draw_option(int idx) {
	int draw_y = y + idx;
	bool highlight = idx == selection_idx;
	if (highlight) { Terminal::set_invert(); }
	Terminal::mvaddstr(x, draw_y, get_option(idx));
	if (highlight) { Terminal::unset_invert(); }
}

void List::full_draw() {
	prior_selection_idx = selection_idx;
	needs_full_draw = false;
	for (int i = 0; i < get_num_options(); i++) {
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
bool running = true;

void Demo::draw() {
	int w, h;
	Terminal::get_size(w, h);

	list.quick_draw();
	Terminal::flush();
}

void Demo::event(const Terminal::Event &event) {
	switch (event.type) {

		case Terminal::EventType::EXIT:
			running = false;
			break;

		case Terminal::EventType::RESIZE:
			Terminal::clear();
			Demo::draw();
			break;

		case Terminal::EventType::TEXT:
			for (auto cp : event.e_text.text) {
				switch (cp) {
					case 'q':
						running = false;
						break;
					case 'j':
						list.move_selection_index(1);
						break;
					case 'k':
						list.move_selection_index(-1);
						break;
				}
			}
			draw();
			break;
	}
}

bool Demo::is_running() {
	return running;
}
