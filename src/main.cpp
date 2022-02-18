#include <iostream>

#include "unicode.hpp"
#include "ansi_terminal.hpp"
#include "highlight.hpp"

using Terminal::AbstractTerminal;

int main() {
	ANSITerminal term;

	auto hello = UTF8::decode("Hello, world!");
	auto endl = UTF8::decode("\n");

	Highlight::Highlight hi (0, 0);
	hi.apply_to_terminal(&term);

	term.print(hello);
	term.print(endl);

	term.reset();
	return 0;
}

void color256_demo(AbstractTerminal *term) {
	for (int i = 0; i <= 255; i++) {
		uint8_t i2 = (uint8_t) i;

		term->set_fg(i2);

		const std::string number = std::to_string(i2);

		if (number.size() < 3) { term->print_raw(' '); }
		if (number.size() < 2) { term->print_raw(' '); }

		term->print_raw(number);
		term->print_raw(' ');

		if ((i + 1) % 10 == 0) {
			term->print_raw('\n');
		}
	}

	term->print_raw('\n');
}
