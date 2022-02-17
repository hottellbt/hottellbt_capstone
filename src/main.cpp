#include <iostream>

#include "unicode.hpp"

#include "ansi_terminal.hpp"


int main() {
	ANSITerminal term;

	term.set_bold();
	term.set_italic();
	term.print(UTF8::decode("Hello, world!\n"));
	term.reset();

	term.set_bold();
	term.print(UTF8::decode("Hello, world!\n"));
	term.reset();

	term.set_underline();
	term.print(UTF8::decode("Hello, world!"));
	term.reset();

	term.print(UTF8::decode("\n"));


	for (int i = 0; i <= 255; i++) {
		uint8_t i2 = (uint8_t) i;

		term.set_fg(i2);

		const std::string number = std::to_string(i2);

		if (number.size() < 3) { term.print_raw(' '); }
		if (number.size() < 2) { term.print_raw(' '); }

		term.print_raw(number);
		term.print_raw(' ');

		if ((i + 1) % 10 == 0) {
			term.print_raw('\n');
		}
	}

	term.print_raw('\n');

	term.reset();
	return 0;
}

