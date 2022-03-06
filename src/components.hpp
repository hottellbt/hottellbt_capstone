#ifndef INCLUDED_COMPONENTS_HPP
#define INCLUDED_COMPONENTS_HPP

#include "unicode.hpp"

struct Bounds {
	unsigned short x, y, width, height;

	Bounds() {}

	Bounds(int x, int y, int width, int height) :
		x(x), y(y), width(width), height(height) {}

	Bounds(int width, int height) :
		x(0), y(0), width(width), height(height) {}

};

class Component {
	public:
		Component() {};

		void set_needs_full_draw() {
			this->needs_full_draw = true;
		}

		void full_draw(const Bounds& bounds) {
			do_full_draw(bounds);
			this->needs_full_draw = false;
		}

		void quick_draw(const Bounds& bounds) {
			if (this->needs_full_draw) {
				full_draw(bounds);
				return;
			}
			do_quick_draw(bounds);
		}

	protected:
		virtual void do_full_draw(const Bounds& bounds) = 0;
		virtual void do_quick_draw(const Bounds& bounds) = 0;

		bool needs_full_draw = true;
};

class List : public Component {
	public:
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

	protected:
		void do_full_draw(const Bounds& bounds);
		void do_quick_draw(const Bounds& bounds);

	private:
		int prior_selection_idx = 0;
		int selection_idx = 0;

		void draw_row(int idx, const Bounds& bounds);
};

#endif
