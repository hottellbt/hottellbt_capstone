#include "demo.hpp"
#include "unicode.hpp"
#include "components.hpp"
#include "os_helper.hpp"
#include "highlight.hpp"
#include "encoding.hpp"
#include "terminal.hpp"

bool running = true;
Highlight::ColorScheme color_scheme;
std::string status = "Hello world";

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

template<typename T>
class ListModel {
	public:
		ListModel() {}
		virtual ~ListModel() {}

		virtual size_t num_elements() = 0;
		virtual T get_element(size_t idx) = 0;

		size_t selection_index = 0;

		void scroll_up() {
			if (selection_index > 0) {
				selection_index--;
			}
		}

		void scroll_down() {
			if (selection_index < num_elements() - 1) {
				selection_index++;
			}
		}
		
		void home() { selection_index = 0; }
		void end() { selection_index = num_elements() - 1; }
};

class DemoListModel : public ListModel<Unicode::string_t> {
	public:
		size_t num_elements() { return 4; }

		Unicode::string_t get_element(size_t idx) {
			switch (idx) {
				case 0: return Encoding::decode_literal("Dummy 1");
				case 1: return Encoding::decode_literal("Dummy 2");
				case 2: return Encoding::decode_literal("Dummy 3");
				case 3: return Encoding::decode_literal("Exit");
			}
			throw std::runtime_error(
					(std::string) "out of bounds: " + std::to_string(idx));
		}

};

template<typename T>
class ListView {
	public:
		ListView(ListModel<T> *model) :model(model) {}
		virtual ~ListView() {}

		void draw(const Rectangle& bounds) {
			// for now we assume that every row has a height of 1
			const unsigned short row_height = 1;

			// can we skip re-drawing most rows?
			const bool quick = !needs_redraw && bounds == last_draw_bounds;

			needs_redraw = false;
			last_draw_bounds = bounds;

			const size_t max_row = model->num_elements();

			unsigned short draw_y = bounds.y;
			size_t row = 0;
			Rectangle row_bounds;

			for (;
					draw_y < bounds.height && row < max_row;
					row++, draw_y += row_height) {

				bool do_draw = true;

				if (quick) {
					// establish if we can skip drawing this row
					do_draw = row == last_draw_selection || row == model->selection_index;
				}

				if (do_draw) {
					row_bounds = {
						bounds.x,
						draw_y,
						bounds.width,
						row_height
					};

					draw_row(
							row_bounds,
							model->get_element(row),
							row,
							row == model->selection_index);
				}
			}

			last_draw_selection = model->selection_index;
		}

	protected:
		ListModel<T>* const model;

		virtual void draw_row(
				const Rectangle& bounds,
				const T& value,
				size_t index,
				bool selected) = 0;

	private:
		bool needs_redraw = true;
		Rectangle last_draw_bounds;
		size_t last_draw_selection;

		bool needs_full_draw() {
			return false;
		}

};

class DemoListView : public ListView<Unicode::string_t> {
	public:
		DemoListView(ListModel<Unicode::string_t> *model) : ListView(model) {}

	protected:
		void draw_row(
				const Rectangle& bounds,
				const Unicode::string_t& value,
				size_t index,
				bool selected);
};

void DemoListView::draw_row(
		const Rectangle& bounds,
		const Unicode::string_t& value,
		size_t index,
		bool selected) {

	if (bounds.is_zero()) return;

	bool highlight = index == model->selection_index;

	if (highlight) { 
		color_scheme.get_highlight({"list", "listSelect"}).activate();
		Terminal::set_invert();
		Terminal::set_bold();
	} else {
		color_scheme.get_highlight({"list", "listNormal"}).activate();
	}

	int option_str_width;

	Unicode::string_t option_str = normalize_string(
			value, bounds.width, &option_str_width);

	Terminal::mvaddstr(bounds.x, bounds.y, option_str);

	for (int i = option_str_width; i < bounds.width; i++) {
		Terminal::addraw(' ');
	}
		
	if (highlight) {
		Terminal::unset_invert();
		Terminal::set_normal_intensity();
	}
}

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


ListModel<Unicode::string_t> *list_model = new DemoListModel();
DemoListView *list_view = new DemoListView(list_model);

void Demo::init() {
	color_scheme.set_partial_highlight("Normal",     {"lightgrey", ""});
	color_scheme.set_partial_highlight("StatusLine", {"blue", ""});

	color_scheme.set_partial_highlight("todoPriorityHighest", {"brightred", ""});
	color_scheme.set_partial_highlight("todoPriorityHigher",  {"red", ""});
	color_scheme.set_partial_highlight("todoPriorityHigh",    {"yellow", ""});

	color_scheme.set_partial_highlight("todoPriorityLowest",  {"brightblue", ""});
	color_scheme.set_partial_highlight("todoPriorityLower",   {"blue", ""});
	color_scheme.set_partial_highlight("todoPriorityLow",     {"green", ""});
}

void Demo::draw() {
	Rectangle bounds;
	bounds.x = 0;
	bounds.y = 0;
	Terminal::get_size(bounds.width, bounds.height);

	list_view->draw(bounds);

	color_scheme.get_highlight({"StatusLine"}).activate();
	Terminal::mvaddraw(0, bounds.height - 1, status);

	Terminal::flush();
}

void Demo::event(const Terminal::Event &event) {

	switch (event.type) {

		case Terminal::EventType::EXIT:
			running = false;
			break;

		case Terminal::EventType::RESIZE:
			break;

		case Terminal::EventType::TEXT:
			for (auto cp : event.e_text.text) {
				switch (cp) {
					case 'q':
						running = false;
						break;
					case 'j':
						list_model->scroll_down();
						break;
					case 'k':
						list_model->scroll_up();
						break;
					case 'g':
						list_model->home();
						break;
					case 'G':
						list_model->end();
						break;
					case 'l':
						if (list_model->selection_index == 3) {
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

