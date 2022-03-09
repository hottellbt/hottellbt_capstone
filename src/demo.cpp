#include <vector>

#include "demo.hpp"
#include "unicode.hpp"
#include "components.hpp"
#include "os_helper.hpp"
#include "highlight.hpp"
#include "encoding.hpp"
#include "terminal.hpp"
#include "todo.hpp"

bool running = true;
highlight::ColorScheme color_scheme;
std::string status = "Hello world";

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

class DemoListModel : public ListModel<todo::Item> {
	public:
		DemoListModel() {
			items.push_back({});
		}

		std::vector<todo::Item> items;

		size_t num_elements() { return items.size(); }

		todo::Item get_element(size_t idx) { return items[idx]; }

	private:
};

template<typename T>
class ListView {
	public:
		ListView(ListModel<T> *model) :model(model) {}
		virtual ~ListView() {}

		void set_needs_redraw() {
			needs_redraw = true;
		}

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

};

class DemoListView : public ListView<todo::Item> {
	public:
		DemoListView(ListModel<todo::Item> *model) : ListView(model) {}

	protected:
		void draw_row(
				const Rectangle& bounds,
				const todo::Item& value,
				size_t index,
				bool selected) override;
};

void DemoListView::draw_row(
		const Rectangle& bounds,
		const todo::Item& value,
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

	auto x = bounds.x;
	auto y = bounds.y;
	Terminal::mv(x, y);

	if (bounds.width > 6) {
		Terminal::addliteral("[");
		Terminal::addliteral("] ");
	}

	Terminal::addstr(value.title);

	if (highlight) {
		Terminal::unset_invert();
		Terminal::set_normal_intensity();
	}
}

ListModel<todo::Item> *list_model = new DemoListModel();
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
	unsigned short x = 0, y = 0, width, height;
	Terminal::get_size(width, height);

	list_view->draw({x, y, width, (unsigned short) (height - 1)});

	color_scheme.get_highlight({"StatusLine"}).activate();
	Terminal::mvaddraw(0, height - 1, status);

	Terminal::flush();
}

void Demo::event(const Terminal::Event &event) {

	switch (event.type) {

		case Terminal::EventType::EXIT:
			running = false;
			break;

		case Terminal::EventType::RESIZE:
			Terminal::clear();
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

