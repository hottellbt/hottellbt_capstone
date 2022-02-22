#ifndef INCLUDED_DEMO_HPP
#define INCLUDED_DEMO_HPP

#include "unicode.hpp"
#include "terminal.hpp"

namespace Demo {
	int get_terminal_width(Unicode::codepoint_t cp);

	void init();
	void event(const Terminal::Event &event);
	void draw();
	bool is_running();

	void setup_terminal();
	void cleanup_terminal();
};

#endif