#ifndef INCLUDED_TERMINAL_HPP
#define INCLUDED_TERMINAL_HPP

#include <cstdint>
#include <string>
#include <optional>

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

	enum class ColorType : uint8_t {
		USE_DEFAULT,
		COLOR_256,
		COLOR_RGB
	};

	class Color {
		public:
			Color() :
				color_type(ColorType::USE_DEFAULT) {}

			Color(uint8_t color_256) :
				color_type(ColorType::COLOR_256), color_256(color_256) {}

			Color(uint8_t r, uint8_t g, uint8_t b) :
				color_type(ColorType::COLOR_RGB),
				color_rgb_r(r), color_rgb_g(g), color_rgb_b(b) {}

			bool operator==(const Color &c) const {
				if (color_type != c.color_type) return false;

				switch (color_type) {

					case ColorType::COLOR_256:
						if (color_256 != c.color_256) return false;
						break;

					case ColorType::COLOR_RGB:
						if (color_rgb_r != c.color_rgb_r
								|| color_rgb_g != c.color_rgb_g
								|| color_rgb_b != c.color_rgb_b) return false;
						break;

					default: break;
				}

				return true;
			}

			bool operator!=(const Color &c) const {
				return !operator==(c);
			}

			static std::optional<Color> from_string(const std::string&);

			ColorType color_type;
			uint8_t color_256;
			uint8_t color_rgb_r;
			uint8_t color_rgb_g;
			uint8_t color_rgb_b;
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

	struct Event {
		EventType type = EventType::NONE;
		EventText   e_text;
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

	inline void set_fg(const Color &c) {
		switch (c.color_type) {
			case ColorType::USE_DEFAULT:
				Terminal::unset_fg();
				break;
			case ColorType::COLOR_256:
				Terminal::set_fg(c.color_256);
				break;
			case ColorType::COLOR_RGB:
				Terminal::set_fg(c.color_rgb_r, c.color_rgb_g, c.color_rgb_b);
				break;
		}
	}

	inline void set_bg(const Color &c) {
		switch (c.color_type) {
			case ColorType::USE_DEFAULT:
				Terminal::unset_bg();
				break;
			case ColorType::COLOR_256:
				Terminal::set_bg(c.color_256);
				break;
			case ColorType::COLOR_RGB:
				Terminal::set_bg(c.color_rgb_r, c.color_rgb_g, c.color_rgb_b);
				break;
		}
	}

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

	void get_size(int &cols, int &rows);

	void show_cursor();
	void hide_cursor();

	// curses-like

	void mv(int col, int row);

	void addstr(const Unicode::string_t &str);

	inline void mvaddstr(int col, int row, const Unicode::string_t &str) {
		mv(col, row);
		addstr(str);
	}

	inline void addcp(Unicode::codepoint_t cp) {
		Unicode::string_t s;
		s.push_back(cp);
		addstr(s);
	}

	inline void mvaddcp(int col, int row, Unicode::codepoint_t cp) {
		mv(col, row);
		addcp(cp);
	}

	void addraw(const std::string &str);
	void addraw(const char* str);
	void addraw(char c);

	inline void mvaddraw(int col, int row, const std::string &str) {
		mv(col, row);
		addraw(str);
	}

	inline void mvaddraw(int col, int row, const char* str) {
		mv(col, row);
		addraw(str);
	}

	inline void mvaddraw(int col, int row, char c) {
		mv(col, row);
		addraw(c);
	}
};


/***** ANSI implementations *****/

#include <iostream>
#include <functional>

#include "encoding.hpp"

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

	inline void show_cursor() { addraw("\x1b[?25h"); }
	inline void hide_cursor() { addraw("\x1b[?25l"); }

	inline void addraw(const std::string &str) { std::cout << str; }
	inline void addraw(const char* str)        { std::cout << str; }
	inline void addraw(char c)                 { std::cout << c; }

	inline void mv(int col, int row) {
		if (col == 0 && row == 0) {
			addraw("\x1b[H");
			return;
		}

		addraw("\x1b[");
		addraw(std::to_string(row + 1));
		addraw(';');
		addraw(std::to_string(col + 1));
		addraw('H');
	}

	inline void addstr(const Unicode::string_t &s) {
		addraw(Encoding::UTF8::encode(s));
	}

};

#endif
