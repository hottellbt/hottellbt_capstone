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

	class AbstractTerminal {
		public:
			AbstractTerminal() {}

			virtual void reset() = 0;
			virtual void clear() = 0;

			virtual void print(Unicode::string_t) = 0;

			virtual void print(Unicode::codepoint_t cp) {
				Unicode::string_t s;
				s.push_back(cp);
				this->print(s);
			}

			virtual void print_raw(const std::string &str) = 0;
			virtual void print_raw(const char* str) = 0;
			virtual void print_raw(char c) = 0;

			virtual void set_fg(uint8_t color_16) = 0;
			virtual void set_bg(uint8_t color_16) = 0;

			virtual void set_fg(uint8_t r, uint8_t g, uint8_t b) = 0;
			virtual void set_bg(uint8_t r, uint8_t g, uint8_t b) = 0;

			virtual void unset_fg() = 0;
			virtual void unset_bg() = 0;
			virtual void unset_bg_fg() = 0;

			virtual void set_bold()        = 0;
			virtual void set_faint()       = 0;
			virtual void set_italic()      = 0;
			virtual void set_underline()   = 0;
			virtual void set_blink()       = 0;
			virtual void set_invert()      = 0;
			virtual void set_strike()      = 0;

			// unsets faint & bold
			virtual void set_normal_intensity() = 0;

			virtual void unset_italic()    = 0;
			virtual void unset_underline() = 0;
			virtual void unset_blink()     = 0;
			virtual void unset_invert()    = 0;
			virtual void unset_strike()    = 0;

			virtual void move_cursor(int row, int col) = 0;
	};
};

#endif
