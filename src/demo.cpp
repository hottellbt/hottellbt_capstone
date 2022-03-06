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

void Demo::init() {
}

void Demo::draw() {
	Bounds bounds;
	bounds.x = 0;
	bounds.y = 0;
	Terminal::get_size(bounds.width, bounds.height);

	list.quick_draw(bounds);

	Terminal::set_fg(Terminal::Color16::BRIGHT_BLACK);
	Terminal::mvaddraw(0, 2, hack_result);
	Terminal::unset_fg();

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
