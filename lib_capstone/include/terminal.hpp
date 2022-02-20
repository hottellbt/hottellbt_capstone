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

	void set_raw(bool raw);

	void next_event(Event &event);

	void flush();

	// curses-like

	void mv_home(); // equivalent to mv(0, 0)

	void mv(int row, int col);

	void addstr(const Unicode::string_t &str);

	inline void mvaddstr(int row, int col, const Unicode::string_t &str) {
		mv(row, col);
		addstr(str);
	}

	inline void addcp(Unicode::codepoint_t cp) {
		Unicode::string_t s;
		s.push_back(cp);
		addstr(s);
	}

	inline void mvaddcp(int row, int col, Unicode::codepoint_t cp) {
		mv(row, col);
		addcp(cp);
	}

	void addraw(const std::string &str);
	void addraw(const char* str);
	void addraw(char c);

	inline void mvaddraw(int row, int col, const std::string &str) {
		mv(row, col);
		addraw(str);
	}

	inline void mvaddraw(int row, int col, const char* str) {
		mv(row, col);
		addraw(str);
	}

	inline void mvaddraw(int row, int col, char c) {
		mv(row, col);
		addraw(c);
	}
};


/***** ANSI implementations *****/

#include <iostream>
#include <functional>

#include "utf8.hpp"

namespace Terminal {

	inline void enable_alt_buffer() {
		addraw("\x1b[?1049h");
	}

	inline void disable_alt_buffer() {
		addraw("\x1b[?1049l");
	}

	inline void clear() {
		addraw("\x1b[2J");
	}

	inline void set_fg(uint8_t color_16) {
		addraw("\x1b[38;5;");
		addraw(std::to_string(color_16));
		addraw('m');
	}

	inline void set_bg(uint8_t color_16) {
		addraw("\x1b[48;5;");
		addraw(std::to_string(color_16));
		addraw('m');
	}

	inline void set_fg(uint8_t r, uint8_t g, uint8_t b) {
		addraw("\x1b[38;2;");
		addraw(std::to_string(r));
		addraw(';');
		addraw(std::to_string(g));
		addraw(';');
		addraw(std::to_string(b));
		addraw('m');
	}

	inline void set_bg(uint8_t r, uint8_t g, uint8_t b) {
		addraw("\x1b[48;2;");
		addraw(std::to_string(r));
		addraw(';');
		addraw(std::to_string(g));
		addraw(';');
		addraw(std::to_string(b));
		addraw('m');
	}

	inline void unset_bg()    { addraw("\x1b[49m"); };
	inline void unset_fg()    { addraw("\x1b[39m"); };
	inline void unset_bg_fg() { addraw("\x1b[39;49m"); };

	inline void set_bold()        { addraw("\x1b[1m"); }
	inline void set_faint()       { addraw("\x1b[2m"); }
	inline void set_italic()      { addraw("\x1b[3m"); }
	inline void set_underline()   { addraw("\x1b[4m"); }
	inline void set_blink()       { addraw("\x1b[5m"); }
	inline void set_invert()      { addraw("\x1b[7m"); }
	inline void set_strike()      { addraw("\x1b[9m"); }

	// unsets faint & bold
	inline void set_normal_intensity() { addraw("\x1b[22m"); }

	inline void unset_italic()    { addraw("\x1b[23m"); }
	inline void unset_underline() { addraw("\x1b[24m"); }
	inline void unset_blink()     { addraw("\x1b[25m"); }
	inline void unset_invert()    { addraw("\x1b[27m"); }
	inline void unset_strike()    { addraw("\x1b[29m"); }

	inline void addraw(const std::string &str) { std::cout << str; }
	inline void addraw(const char* str)        { std::cout << str; }
	inline void addraw(char c)                 { std::cout << c; }

	inline void mv(int col, int row) {
		addraw("\x1b[");
		addraw(std::to_string(row));
		addraw(';');
		addraw(std::to_string(col));
		addraw('H');
	}

	inline void mv_home() {
		addraw("\x1b[H");
	}

	inline void addstr(const Unicode::string_t &s) {
		addraw(UTF8::encode(s));
	}

};

#endif
