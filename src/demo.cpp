#include "demo.hpp"
#include "unicode.hpp"
#include "components.hpp"
#include "os_helper.hpp"

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
int hack_result_x, hack_result_y;

void on_wake_up() {
	list.set_needs_redraw();
}

void on_resize() {
	int w, h;
	Terminal::get_size(w, h);
	list.set_bounds({0, 0, w, 2});
	
	hack_result_x = 0, hack_result_y = 2;
}

void Demo::init() {
	on_resize();
}

void Demo::draw() {
	list.quick_draw();

	Terminal::set_fg(Terminal::Color16::BRIGHT_BLACK);
	Terminal::mvaddraw(hack_result_x, hack_result_y, hack_result);
	Terminal::unset_fg();

	Terminal::flush();
}

void Demo::event(const Terminal::Event &event) {

	switch (event.type) {

		case Terminal::EventType::EXIT:
			running = false;
			break;

		case Terminal::EventType::RESIZE:
			Terminal::clear();
			on_resize();
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
							on_wake_up();
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
