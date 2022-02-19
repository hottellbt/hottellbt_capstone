#ifndef INCLUDED_ANSI_TERMINAL_HPP
#define INCLUDED_ANSI_TERMINAL_HPP

#include <iostream>
#include <functional>

#include "terminal.hpp"
#include "utf8.hpp"

namespace Terminal {

	inline void print_raw(const std::string &str) { std::cout << str; }
	inline void print_raw(const char* str)        { std::cout << str; }
	inline void print_raw(char c)                 { std::cout << c; }

	inline void clear() {
		print_raw("\x1b[2J");
	}

	inline void print(Unicode::string_t s) {
		print_raw(UTF8::encode(s));
	}

	inline void set_fg(uint8_t color_16) {
		print_raw("\x1b[38;5;");
		print_raw(std::to_string(color_16));
		print_raw('m');
	}

	inline void set_bg(uint8_t color_16) {
		print_raw("\x1b[48;5;");
		print_raw(std::to_string(color_16));
		print_raw('m');
	}

	inline void set_fg(uint8_t r, uint8_t g, uint8_t b) {
		print_raw("\x1b[38;2;");
		print_raw(std::to_string(r));
		print_raw(';');
		print_raw(std::to_string(g));
		print_raw(';');
		print_raw(std::to_string(b));
		print_raw('m');
	}

	inline void set_bg(uint8_t r, uint8_t g, uint8_t b) {
		print_raw("\x1b[48;2;");
		print_raw(std::to_string(r));
		print_raw(';');
		print_raw(std::to_string(g));
		print_raw(';');
		print_raw(std::to_string(b));
		print_raw('m');
	}

	inline void unset_bg()    { print_raw("\x1b[49m"); };
	inline void unset_fg()    { print_raw("\x1b[39m"); };
	inline void unset_bg_fg() { print_raw("\x1b[39;49m"); };

	inline void set_bold()        { print_raw("\x1b[1m"); }
	inline void set_faint()       { print_raw("\x1b[2m"); }
	inline void set_italic()      { print_raw("\x1b[3m"); }
	inline void set_underline()   { print_raw("\x1b[4m"); }
	inline void set_blink()       { print_raw("\x1b[5m"); }
	inline void set_invert()      { print_raw("\x1b[7m"); }
	inline void set_strike()      { print_raw("\x1b[9m"); }

	// unsets faint & bold
	inline void set_normal_intensity() { print_raw("\x1b[22m"); }

	inline void unset_italic()    { print_raw("\x1b[23m"); }
	inline void unset_underline() { print_raw("\x1b[24m"); }
	inline void unset_blink()     { print_raw("\x1b[25m"); }
	inline void unset_invert()    { print_raw("\x1b[27m"); }
	inline void unset_strike()    { print_raw("\x1b[29m"); }

	inline void move_cursor(unsigned int col, unsigned int row) {
		print_raw("\x1b[");
		print_raw(std::to_string(row));
		print_raw(';');
		print_raw(std::to_string(col));
		print_raw("mh");
	}

};

#endif
