#ifndef INCLUDED_TERMINAL_HPP
#define INCLUDED_TERMINAL_HPP

#include <cstdint>
#include <string>

#include "unicode.hpp"


namespace Terminal {

	namespace Color16 {
		constexpr uint8_t BLACK   = 0;
		constexpr uint8_t RED     = 1;
		constexpr uint8_t GREEN   = 2;
		constexpr uint8_t YELLOW  = 3;
		constexpr uint8_t BLUE    = 4;
		constexpr uint8_t MAGENTA = 5;
		constexpr uint8_t CYAN    = 6;
		constexpr uint8_t WHITE   = 7;
		constexpr uint8_t BRIGHT_BLACK   = 8;
		constexpr uint8_t BRIGHT_RED     = 9;
		constexpr uint8_t BRIGHT_GREEN   = 10;
		constexpr uint8_t BRIGHT_YELLOW  = 11;
		constexpr uint8_t BRIGHT_BLUE    = 12;
		constexpr uint8_t BRIGHT_MAGENTA = 13;
		constexpr uint8_t BRIGHT_CYAN    = 14;
		constexpr uint8_t BRIGHT_WHITE   = 15;
	};

	enum class EventType : uint8_t {
		NONE,   // timeout waiting for next event, nothing happened, etc
		EXIT,   // user requested to exit/terminate gracefully
		TEXT,   // user typed things, and that input is ready to be processed
		RESIZE, // SIGWINCH, terminal resized
	};

	struct EventText {
		Unicode::string_t text {};
	};

	struct EventResize {
		unsigned int rows;
		unsigned int cols;
	};

	struct Event {
		EventType type = EventType::NONE;

		// could not make a union because nontrivial
		EventText   e_text;
		EventResize e_resize;
	};

	void enable_alt_buffer();
	void disable_alt_buffer();

	void clear();

	void print(Unicode::string_t);

	inline void print(Unicode::codepoint_t cp) {
		Unicode::string_t s;
		s.push_back(cp);
		print(s);
	}

	void print_raw(const std::string &str);
	void print_raw(const char* str);
	void print_raw(char c);

	void set_fg(uint8_t color_16);
	void set_bg(uint8_t color_16);

	void set_fg(uint8_t r, uint8_t g, uint8_t b);
	void set_bg(uint8_t r, uint8_t g, uint8_t b);

	void unset_fg();
	void unset_bg();
	void unset_bg_fg();

	void set_bold();
	void set_faint();
	void set_italic();
	void set_underline();
	void set_blink();
	void set_invert();
	void set_strike();

	// unsets faint & bold
	void set_normal_intensity();

	void unset_italic();
	void unset_underline();
	void unset_blink();
	void unset_invert();
	void unset_strike();

	void mv(int row, int col);
	void mv_home();

	void set_raw(bool raw);

	void next_event(Event &event);

	void flush();
};


/***** ANSI implementations *****/

#include <iostream>
#include <functional>

#include "utf8.hpp"

namespace Terminal {

	inline void print_raw(const std::string &str) { std::cout << str; }
	inline void print_raw(const char* str)        { std::cout << str; }
	inline void print_raw(char c)                 { std::cout << c; }

	inline void enable_alt_buffer() {
		print_raw("\x1b[?1049h");
	}

	inline void disable_alt_buffer() {
		print_raw("\x1b[?1049l");
	}

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

	inline void mv(int col, int row) {
		print_raw("\x1b[");
		print_raw(std::to_string(row));
		print_raw(';');
		print_raw(std::to_string(col));
		print_raw("H");
	}

	inline void mv_home() {
		print_raw("\x1b[H");
	}

};

#endif
