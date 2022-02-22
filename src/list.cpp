#include "list.hpp"

void List::draw_option(int idx) {
	const int draw_x = bounds.x;
	const int draw_y = bounds.y + idx;
	const int width = bounds.width;
	const int height = bounds.height;

	if (width < 1 || height < 1) {
		return;
	}

	bool highlight = idx == selection_idx;

	if (highlight) { 
		Terminal::set_invert();
		Terminal::set_bold();
	}

	int option_str_width;
	Unicode::string_t option_str = normalize_string(
			get_option(idx), width, &option_str_width);
	Terminal::mvaddstr(draw_x, draw_y, option_str);

	for (int i = option_str_width; i < width; i++) {
		Terminal::addraw(' ');
	}
		
	if (highlight) {
		Terminal::unset_invert();
		Terminal::set_normal_intensity();
	}
}

void List::do_full_draw() {
	for (int i = 0; i < get_num_options(); i++) {
		draw_option(i);
	}
	prior_selection_idx = selection_idx;
}

void List::do_quick_draw() {
	if (prior_selection_idx == selection_idx) return;

	draw_option(prior_selection_idx);
	draw_option(selection_idx);
	prior_selection_idx = selection_idx;
}

int List::get_num_options() {
	return 5;
}

Unicode::string_t List::get_option(int idx) {
	switch (idx) {
		case 0: return UTF8::decode("Open Editor");
		case 1: return UTF8::decode("c1 45");
		case 2: return UTF8::decode("c2 abc 123");
		case 3: return UTF8::decode("01234567890123456789");
		case 4: return UTF8::decode("0123");
	}
	throw std::runtime_error("out of bounds");
}

