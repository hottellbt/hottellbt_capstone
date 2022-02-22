#ifndef INCLUDED_COMPONENTS_HPP
#define INCLUDED_COMPONENTS_HPP

#include "unicode.hpp"

struct Boundary {
	unsigned int x;
	unsigned int y;
	unsigned int width;
	unsigned int height;

	Boundary() : x(0), y(0), width(0), height(0) {}
	Boundary(int x, int y, int w, int h) : x(x), y(y), width(w), height(h) {}
};

class Component {
	public:
		Component() {};

		void set_needs_redraw() {
			this->needs_full_draw = true;
		}

		void set_bounds(const Boundary &bounds) {
			this->bounds = bounds;
			this->needs_full_draw = true;
		}

		void full_draw() {
			do_full_draw();
			this->needs_full_draw = false;
		}

		void quick_draw() {
			if (this->needs_full_draw) {
				full_draw();
				return;
			}
			do_quick_draw();
		}

	protected:
		virtual void do_full_draw() = 0;
		virtual void do_quick_draw() = 0;

		bool needs_full_draw = true;
		Boundary bounds;
};

class List : public Component {
	public:
		void do_full_draw();
		void do_quick_draw();

		int get_num_options();
		Unicode::string_t get_option(int idx);

		int get_selection_index() { return this->selection_idx; }

		void set_selection_index(int idx) { this->selection_idx = idx; }

		void move_selection_index(int move) {
			int idx = selection_idx + move;

			if (idx < 0) {
				set_selection_index(0);
			} else if (idx >= get_num_options()) {
				set_selection_index(get_num_options() - 1);
			} else {
				set_selection_index(get_selection_index() + move);
			}
		}

	private:
		int prior_selection_idx = 0;
		int selection_idx = 0;

		void draw_option(int idx);
};

#endif
