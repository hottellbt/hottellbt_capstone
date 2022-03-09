#ifndef INCLUDED_DEMO_HPP
#define INCLUDED_DEMO_HPP

#include "unicode.hpp"
#include "terminal.hpp"

namespace Demo {
	void init();
	void event(const Terminal::Event &event);
	void draw();
	bool is_running();

	void setup_terminal();
	void cleanup_terminal();
};

#endif
