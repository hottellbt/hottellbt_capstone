#ifndef INCLUDED_ANSI_TERMINAL_HPP
#define INCLUDED_ANSI_TERMINAL_HPP

#include <iostream>
#include <functional>

#include "terminal.hpp"
#include "utf8.hpp"

class ANSITerminal : public Terminal::AbstractTerminal {
	public:
		ANSITerminal(
				std::function<std::string(Unicode::string_t)> encoder = UTF8::encode) :
			AbstractTerminal(),
			encoder(encoder) {}

		void print_raw(const std::string &str) { std::cout << str; }
		void print_raw(const char* str)        { std::cout << str; }
		void print_raw(char c)                 { std::cout << c; }

		void reset() override {
			this->print_raw("\x1b[0m");
		}

		void clear() override {
			print_raw("\x1b[2J");
		}

		void print(Unicode::string_t s) {
			print_raw(this->encoder(s));
		}

		void set_fg(uint8_t color_16) {
			print_raw("\x1b[38;5;");
			print_decimal(color_16);
			print_raw('m');
		}

		void set_bg(uint8_t color_16) {
			print_raw("\x1b[48;5;");
			print_decimal(color_16);
			print_raw('m');
		}

		void set_fg(uint8_t r, uint8_t g, uint8_t b) {
			print_raw("\x1b[38;2;");
			print_decimal(r);
			print_raw(';');
			print_decimal(g);
			print_raw(';');
			print_decimal(b);
			print_raw('m');
		}

		void set_bg(uint8_t r, uint8_t g, uint8_t b) {
			print_raw("\x1b[48;2;");
			print_decimal(r);
			print_raw(';');
			print_decimal(g);
			print_raw(';');
			print_decimal(b);
			print_raw('m');
		}

		void unset_bg()    override { print_raw("\x1b[49m"); };
		void unset_fg()    override { print_raw("\x1b[39m"); };
		void unset_bg_fg() override { print_raw("\x1b[39;49m"); };

		void set_bold()        override { print_raw("\x1b[1m"); }
		void set_faint()       override { print_raw("\x1b[2m"); }
		void set_italic()      override { print_raw("\x1b[3m"); }
		void set_underline()   override { print_raw("\x1b[4m"); }
		void set_blink()       override { print_raw("\x1b[5m"); }
		void set_invert()      override { print_raw("\x1b[7m"); }
		void set_strike()      override { print_raw("\x1b[9m"); }

		// unsets faint & bold
		void set_normal_intensity() override { print_raw("\x1b[22m"); }

		void unset_italic()    override { print_raw("\x1b[23m"); }
		void unset_underline() override { print_raw("\x1b[24m"); }
		void unset_blink()     override { print_raw("\x1b[25m"); }
		void unset_invert()    override { print_raw("\x1b[27m"); }
		void unset_strike()    override { print_raw("\x1b[29m"); }

		void move_cursor(int col, int row) override {
			print_raw("\x1b[");
			print_decimal(row);
			print_raw(';');
			print_decimal(col);
			print_raw("mh");
		}

	private:
		const std::function<std::string(Unicode::string_t)> encoder;

		void print_decimal(uint8_t d) { print_raw(std::to_string(d)); }
		void print_decimal(int d)     { print_raw(std::to_string(d)); }

};

#endif
