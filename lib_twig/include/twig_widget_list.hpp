#ifndef INCLUDED_TWIG_WIDGET_LIST_HPP
#define INCLUDED_TWIG_WIDGET_LIST_HPP

#include "twig_widget.hpp"

namespace twig::widget {

	template<typename T> class ListModel {
		public:
			ListModel() {}
			virtual ~ListModel() {}

			size_t get_selection_index() { return selection_index; }
			virtual size_t num_elements() = 0;
			virtual T get_element(size_t idx) = 0;

			void scroll_up(size_t magnitude = 1) {
				if (selection_index >= magnitude) {
					selection_index -= magnitude;
				}
			}

			void scroll_down(size_t magnitude = 1) {
				if (selection_index < num_elements() - magnitude) {
					selection_index += magnitude;
				}
			}

			void home() { selection_index = 0; }
			void end() { selection_index = num_elements() - 1; }

		protected:
			size_t selection_index = 0;
	};

	template<typename T> class ListController {
		public:
			ListController(ListModel<T>* model) : model(model) {}

			void when_typed(const Unicode::codepoint_t key) {
				switch (key) {
					case 'j':
						model->scroll_down();
						break;
					case 'k':
						model->scroll_up();
						break;
					case 'g':
						model->home();
						break;
					case 'G':
						model->end();
						break;
				}
			}

			void when_typed_special(const twig::special_key& key) {
				using X = twig::special_key;
				switch (key) {
					case X::UP:
						model->scroll_up();
						break;
					case X::DOWN:
					case X::BACKSPACE:
						model->scroll_down();
						break;
					case X::HOME:
						model->home();
						break;
					case X::END:
						model->end();
						break;
					case X::NPAGE:
						break;
					case X::PPAGE:
						break;
					default:
						break;
				}
			}

		protected:
			ListModel<T>* const model;
	};

	template<typename T> class ListPainter : public Widget {
		public:
			ListPainter(ListModel<T> *model) : model(model) {}
			virtual ~ListPainter() {}

			void repaint() override {
				auto& g = get_graphics();
				const WDim bounds = get_size();

				g->clear_fast();

				// for now we assume that every row has a height of 1
				const unsigned short row_height = 1;

				const size_t max_row = model->num_elements();

				unsigned short draw_y = 0;
				size_t row = 0;
				WRect row_bounds;

				for (;
						draw_y < bounds.height && row < max_row;
						row++, draw_y += row_height) {

					row_bounds = {
						0,
						draw_y,
						bounds.width,
						row_height
					};

					draw_row(
							g,
							row_bounds,
							model->get_element(row),
							row,
							row == model->get_selection_index());
				}
			}

		protected:
			ListModel<T>* const model;

			virtual void draw_row(
					std::unique_ptr<Graphics>& g,
					const WRect& bounds,
					const T& value,
					size_t index,
					bool selected) = 0;

	};

}

#endif
