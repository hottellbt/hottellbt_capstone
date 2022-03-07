#include "demo.hpp"
#include "unicode.hpp"
#include "components.hpp"
#include "os_helper.hpp"
#include "highlight.hpp"
#include "encoding.hpp"
#include "terminal.hpp"

Highlight::ColorScheme scheme;

int Demo::get_terminal_width(Unicode::codepoint_t cp) {
	using X = Unicode::EastAsianWidth;
	const auto width_prop = Unicode::get_east_asian_width(cp);

	switch(width_prop) {
		case X::F:
		case X::W:
			return 2;
		default:
			return 1;
	}
}

List list;
bool running = true;

std::string hack_result = "open the editor";

void Demo::init() {
	scheme.set_partial_highlight("Normal",     {"lightgrey", ""});
	scheme.set_partial_highlight("StatusLine", {"blue", ""});

	scheme.set_partial_highlight("listNormal", {"lightgreen", ""});
	scheme.set_partial_highlight("listSelect", {"lightblue", ""});

}

void Demo::draw() {
	Bounds bounds;
	bounds.x = 0;
	bounds.y = 0;
	Terminal::get_size(bounds.width, bounds.height);

	list.quick_draw(bounds);

	scheme.get_highlight({"StatusLine"}).activate();
	Terminal::mvaddraw(0, 2, hack_result);

	Terminal::flush();
}

void Demo::event(const Terminal::Event &event) {

	switch (event.type) {

		case Terminal::EventType::EXIT:
			running = false;
			break;

		case Terminal::EventType::RESIZE:
			list.set_needs_full_draw();
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
					case 'g':
						list.set_selection_index(0);
						break;
					case 'G':
						list.set_selection_index(list.get_num_options() - 1);
						break;
					case 'l':
						if (list.get_selection_index() == 0) {
							Demo::cleanup_terminal();
							hack_result = OS::Subprocess::open_editor_line(hack_result);
							Demo::setup_terminal();
							list.set_needs_full_draw();
						} else if (list.get_selection_index() == 1) {
							running = false;
						}
						break;
				}
			}
			draw();
			break;

		default:
			break;
	}
}

bool Demo::is_running() {
	return running;
}

Unicode::string_t normalize_string(
		const Unicode::string_t &s,
		int max_width,
		int *actual_width) {

	static constexpr Unicode::codepoint_t ellipses = 0x2026;
	static const int ellipses_width = Demo::get_terminal_width(ellipses);

	const int s_size = s.size();

	int width = 0;
	Unicode::string_t ret;

	for (size_t i = 0; i < s_size; i++) {
		auto cp = s[i];
		
		int cp_width = Demo::get_terminal_width(cp);

		if (width + cp_width + ellipses_width > max_width) {

			if (i == s_size - 1 && (width + cp_width <= max_width)) {
				ret.push_back(cp);
				width += cp_width;
			} else {
				ret.push_back(ellipses);
				width += ellipses_width;
			}
			break;
		} else {
			width += cp_width;
			ret.push_back(cp);
		}
	}

	if (actual_width != nullptr) {
		*actual_width = width;
	}

	return ret;
}

void List::draw_row(int row, const Bounds& bounds) {

	auto draw_y = bounds.y + row;
	auto draw_x = bounds.x;

	if (draw_y > bounds.height) {
		return;
	}

	// the index that this row represents
	auto idx = row;

	if (idx >= get_num_options()) {
		return;
	}

	bool highlight = idx == selection_idx;

	if (highlight) { 
		scheme.get_highlight({"list", "listSelect"}).activate();
		Terminal::set_invert();
		Terminal::set_bold();
	} else {
		scheme.get_highlight({"list", "listNormal"}).activate();
	}

	int option_str_width;
	Unicode::string_t option_str = normalize_string(
			get_option(idx), bounds.width, &option_str_width);
	Terminal::mvaddstr(draw_x, draw_y, option_str);

	for (int i = option_str_width; i < bounds.width; i++) {
		Terminal::addraw(' ');
	}
		
	if (highlight) {
		Terminal::unset_invert();
		Terminal::set_normal_intensity();
	}
}

void List::do_full_draw(const Bounds& bounds) {
	int i = 0;
	for (; i < bounds.height; i++) {
		draw_row(i, bounds);
	}
	prior_selection_idx = selection_idx;
}

void List::do_quick_draw(const Bounds& bounds) {
	if (prior_selection_idx == selection_idx) return;

	draw_row(prior_selection_idx, bounds);
	draw_row(selection_idx, bounds);
	prior_selection_idx = selection_idx;
}

int List::get_num_options() {
	return 2;
}

Unicode::string_t List::get_option(int idx) {
	switch (idx) {
		case 0: return Encoding::decode_literal("Open Editor");
		case 1: return Encoding::decode_literal("Exit");
	}
	throw std::runtime_error("out of bounds");
}

