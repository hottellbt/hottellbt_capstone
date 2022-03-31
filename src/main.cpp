#include <vector>
#include <iostream>
#include <exception>

#include <cassert>
#include <cstring>

#include "unicode.hpp"
#include "encoding.hpp"

#include "todo.hpp"

#include "twig_app.hpp"
#include "twig_widget_list.hpp"
#include "twig_os.hpp"

using twig::widget::Widget;
using twig::widget::Graphics;
using twig::widget::WRect;
using twig::widget::WDim;

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
				std::unique_ptr<Graphics>& g,
				const WRect& bounds,
				const todo::Item& value,
				size_t index,
				bool selected) override;
};

void DemoListPainter::draw_row(
		std::unique_ptr<Graphics>& g,
		const WRect& bounds,
		const todo::Item& value,
		size_t index,
		bool selected) {

	if (bounds.is_zero()) return;

	bool highlight = index == model->get_selection_index();

	if (highlight) { 
		g->set_standout(true);
		g->set_bold(true);
	}

	auto x = bounds.x;
	auto y = bounds.y;
	g->mv(x, y);

	if (bounds.width > 6) {
		bool add_brackets = bounds.width > 8;

		if (add_brackets) g->add_ch('[');

		switch (value.status) {
			case todo::Status::IN_PROGRESS:
				g->add_ch('~');
				break;
			case todo::Status::DONE:
				g->add_ch('@');
				break;
			case todo::Status::CANCELED:
				g->add_ch('X');
				break;
			case todo::Status::NONE:
			default:
				g->add_ch(' ');
				break;
		}
		if (add_brackets) g->add_ch(']');
		g->add_ch(' ');

		if (add_brackets) x += 4;
		else x += 2;
	}

	g->add_unicode_str(value.title);
	x += g->get_str_width(value.title);

	for (; x < bounds.width; x++) {
		g->add_ch(' ');
	}

	if (highlight) {
		g->set_standout(false);
		g->set_bold(false);
	}
}

DemoListModel *list_model = new DemoListModel();

DemoListPainter *list_view = new DemoListPainter(list_model);

twig::widget::ListController<todo::Item> *list_controller =
	new twig::widget::ListController<todo::Item>(list_model);

class MyTwigApp : public twig::TwigApp {

	public:
		bool is_running() override { return this->running; }

		Widget* get_root_widget() { return list_view; }

		void when_typed(const Unicode::codepoint_t& input) override;

		void when_typed_special(const twig::special_key& key) override;

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

			// FIXME assuming terminal is in UTF8
			auto encoded = encoding::auto_encode_or_throw(encoding::Encoding::UTF8, &item->title);

			char* raw_data = (char*) malloc(sizeof(char) * encoded.size());
			for (size_t i = 0; i < encoded.size(); i++) raw_data[i] = encoded[i];

			std::string new_name = twig::os::subprocess::open_editor_line(raw_data);

			free(raw_data);

			auto decoded = encoding::auto_decode_or_throw(encoding::Encoding::UTF8, new_name.c_str(), new_name.size());
			item->title = escape(decoded);
			return;
	}

	list_controller->when_typed(key);
}

int main() {
	MyTwigApp app;
	return twig::curses::run_twig_app(&app);
}

