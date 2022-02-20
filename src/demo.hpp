#include "unicode.hpp"
#include "terminal.hpp"

class List {
	public:
		void full_draw();
		void quick_draw();

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
		bool needs_full_draw = true;
		int x = 0, y = 0, width = 10, height = 10;
		int prior_selection_idx = 0;
		int selection_idx = 0;

		void draw_option(int idx);
};

namespace Demo {
	void event(const Terminal::Event &event);
	void draw();
	bool is_running();
};

