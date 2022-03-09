#ifndef INCLUDED_TWIG_HPP
#define INCLUDED_TWIG_HPP

#include "unicode.hpp"

namespace twig {

	struct Point {

		unsigned short x, y;

		Point() {}

		Point(
				unsigned short x,
				unsigned short y) :
			x(x), y(y) {}

		bool operator==(const Point& o) const {
			return x == o.x && y == o.y;
		}

		bool operator!=(const Point& o) const {
			return x != o.x || y != o.y;
		}

	};

	struct Dimension {

		unsigned short width, height;

		Dimension() {}

		Dimension(
				unsigned short width,
				unsigned short height) :
			width(width), height(height) {}

		bool operator==(const Dimension& o) const {
			return width == o.width && height == o.height;
		}

		bool operator!=(const Dimension& o) const {
			return width != o.width || height != o.height;
		}

	};

	struct Rectangle {

		unsigned short x, y, width, height;

		Rectangle() {}

		Rectangle(
				unsigned short x,
				unsigned short y,
				unsigned short width,
				unsigned short height) :
			x(x), y(y), width(width), height(height) {}

		Rectangle(
				unsigned short width,
				unsigned short height) :
			x(0), y(0), width(width), height(height) {}

		Rectangle(
				const Point& point,
				const Dimension& dimension) :
			x(point.x), y(point.y), width(dimension.width), height(dimension.height) {}

		bool is_zero() const {
			return width == 0 || height == 0;
		}

		bool operator==(const Rectangle& o) const {
			return x == o.x
				&& y == o.y
				&& width == o.width
				&& height == o.height;
		}

		bool operator!=(const Rectangle& o) const {
			return x != o.x
				|| y != o.y
				|| width != o.width
				|| height != o.height;
		}

	};

	template<typename T> class ListModel {
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


	template<typename T> class ListView {
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

}

#endif
