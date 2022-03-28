#include <vector>
#include <iostream>
#include <exception>

#include <cassert>
#include <cstring>

#include "unicode.hpp"
#include "encoding.hpp"

#include "todo.hpp"

#include "twig_app.hpp"
#include "twig_widget_container.hpp"
#include "twig_widget_list.hpp"
#include "twig_os.hpp"

using twig::widget::Widget;
using twig::widget::Graphics;
using twig::widget::WRect;
using twig::widget::WDim;
using twig::widget::WInt;
using twig::widget::WPoint;

inline Unicode::string_t escape(const Unicode::string_t s) {
	Unicode::string_t ret;

	for (auto c : s) {
		switch (c) {
			case '\n':
				ret.push_back('\\');
				ret.push_back('n');
				continue;
			default:
				ret.push_back(c);
		}
	}

	return ret;
}

class DesktopWidget : public Widget {
	public:
		DesktopWidget() {}
		~DesktopWidget() {}

		void paint(Graphics& g) override {
			g.clear_fast();

			if (left_widget != nullptr) {
				left_widget->paint(g);
			}

			if (right_widget != nullptr) {
				right_widget->paint(g);
			}
		}

		void when_resized(const WDim& new_size) override {
			WInt half_width = (WInt) (new_size.width / 2);

			if (left_widget != nullptr) {
				left_widget->set_position({0, 0});
				left_widget->set_size({
					half_width,
					new_size.height
				});
			}

			if (right_widget != nullptr) {
				right_widget->set_position({half_width, 0});
				right_widget->set_size({
					(WInt) (new_size.width-half_width),
					new_size.height
				});
			}
		}

		Widget* left_widget = nullptr;
		Widget* right_widget = nullptr;
};

class DemoListModel : public twig::widget::ListModel<todo::Item> {
	public:
		DemoListModel() {
			items.push_back({});
			items.push_back({});
			items.push_back({});
			items.push_back({});
		}

		std::vector<todo::Item> items;

		size_t num_elements() { return items.size(); }

		todo::Item get_element(size_t idx) { return items[idx]; }

		todo::Item* get_element_ptr(size_t idx) { return &items[idx]; }

		void erase(const size_t erase_idx) {
			assert(num_elements() > 0);
			assert(erase_idx < num_elements());

			if (erase_idx < get_selection_index()
					|| (get_selection_index() == num_elements() - 1 && get_selection_index() > 0)) {
				set_selection_index(get_selection_index() - 1);
			}
			items.erase(items.begin() + erase_idx);
		}

		void push_back(const todo::Item& item) {
			items.push_back(item);
		}

		void insert(const size_t idx, const todo::Item& item) {
			items.insert(items.begin() + idx, item);
		}

	private:
};

class DemoListPainter : public twig::widget::ListPainter<todo::Item> {
	public:
		DemoListPainter(twig::widget::ListModel<todo::Item> *model) : ListPainter(model) {}

	protected:
		void draw_row(
				Graphics& g,
				const WRect& bounds,
				const todo::Item& value,
				size_t index,
				bool selected) override;
};

void DemoListPainter::draw_row(
		Graphics& g,
		const WRect& bounds,
		const todo::Item& value,
		size_t index,
		bool selected) {

	if (bounds.is_zero()) return;

	bool highlight = index == model->get_selection_index();

	if (highlight) { 
		g.set_standout(true);
		g.set_bold(true);
	}

	auto x = bounds.x;
	auto y = bounds.y;
	g.mv(x, y);

	if (bounds.width > 6) {
		bool add_brackets = bounds.width > 8;

		if (add_brackets) g.add_ch('[');

		switch (value.status) {
			case todo::Status::IN_PROGRESS:
				g.add_ch('~');
				break;
			case todo::Status::DONE:
				g.add_ch('@');
				break;
			case todo::Status::CANCELED:
				g.add_ch('X');
				break;
			case todo::Status::NONE:
			default:
				g.add_ch(' ');
				break;
		}
		if (add_brackets) g.add_ch(']');
		g.add_ch(' ');

		if (add_brackets) x += 4;
		else x += 2;
	}

	g.add_unicode_str(value.title);
	x += g.get_str_width(value.title);

	for (; x < bounds.width; x++) {
		g.add_ch(' ');
	}

	if (highlight) {
		g.set_standout(false);
		g.set_bold(false);
	}
}

class DebugWidget : public Widget {
	private:
		char c;
	public:
		DebugWidget(char c) : c(c) {}
		~DebugWidget() {}

		void paint(Graphics& g) override {
			WPoint pos = get_position();
			WDim size = get_size();

			for (auto y = 0; y < size.height; y++) {
				g.mv(pos.x, pos.y + y);
				for (auto x = 0; x < size.width; x++) {
					g.add_ch(c);
				}
			}
		}
};

DebugWidget *debug_widget = new DebugWidget('%');

DesktopWidget *desktop = new DesktopWidget();

DemoListModel *list_model = new DemoListModel();

DemoListPainter *list_view = new DemoListPainter(list_model);

twig::widget::ListController<todo::Item> *list_controller =
	new twig::widget::ListController<todo::Item>(list_model);

class MyTwigApp : public twig::TwigApp {

	public:
		bool is_running() override { return this->running; }

		Widget* get_root_widget() { return desktop; }

		void when_typed(const Unicode::codepoint_t& input) override;

		void when_typed_special(const twig::special_key& key) override;

		void when_starting() override {
			desktop->left_widget = list_view;
			desktop->right_widget = debug_widget;
		}

	private:
		bool running = true;

};

void MyTwigApp::when_typed_special(const twig::special_key& key) {
	list_controller->when_typed_special(key);
}

void MyTwigApp::when_typed(const Unicode::codepoint_t& key) {
	todo::Item* item;

	switch (key) {
		case 'q':
			running = false;
			return;
		case 'i':
			list_model->insert(list_model->get_selection_index(), {});
			return;
		case 'I':
			list_model->insert(0, {});
			list_model->set_selection_index(0);
			return;
		case 'a':
			if (list_model->is_empty()) {
				list_model->push_back({});
			} else {
				list_model->insert(list_model->get_selection_index() + 1, {});
				list_model->set_selection_index(list_model->get_selection_index() + 1);
			}
			return;
		case 'A':
			list_model->push_back({});
			list_model->set_selection_index(list_model->num_elements() - 1);
			return;
		case 'x':
		case 'd':
			if (!list_model->is_empty())
				list_model->erase(list_model->get_selection_index());
			return;
		case ']':
			if (list_model->is_empty()) break;
			item = list_model->get_element_ptr(list_model->get_selection_index());
			item->status = todo::Status::IN_PROGRESS;
			return;
		case 'c':
			if (list_model->is_empty()) break;
			item = list_model->get_element_ptr(list_model->get_selection_index());
			std::string encoded = encoding::encode(encoding::Encoding::UTF8, item->title);
			std::string new_name = twig::os::subprocess::open_editor_line(encoded.c_str());
			auto decoded = encoding::decode(encoding::Encoding::UTF8, new_name.c_str(), new_name.size());
			item->title = escape(decoded);
			return;
	}

	list_controller->when_typed(key);
}

int main() {
	MyTwigApp app;
	return twig::curses::run_twig_app(&app);
}

