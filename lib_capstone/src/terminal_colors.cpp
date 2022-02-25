#include "highlight.hpp"
#include "terminal.hpp"

#include <stdexcept>
#include <cstdint>
#include <map>

const std::map<const std::string, const uint8_t> aliases {
	{"black",         Terminal::Color16::BLACK         },
	{"red",           Terminal::Color16::RED           },
	{"green",         Terminal::Color16::GREEN         },
	{"yellow",        Terminal::Color16::YELLOW        },
	{"blue",          Terminal::Color16::BLUE          },
	{"magenta",       Terminal::Color16::MAGENTA       },
	{"cyan",          Terminal::Color16::CYAN          },
	{"white",         Terminal::Color16::WHITE         },
	{"brightblack",   Terminal::Color16::BRIGHT_BLACK  },
	{"brightred",     Terminal::Color16::BRIGHT_RED    },
	{"brightgreen",   Terminal::Color16::BRIGHT_GREEN  },
	{"brightyellow",  Terminal::Color16::BRIGHT_YELLOW },
	{"brightblue",    Terminal::Color16::BRIGHT_BLUE   },
	{"brightmagenta", Terminal::Color16::BRIGHT_MAGENTA},
	{"brightcyan",    Terminal::Color16::BRIGHT_CYAN   },
	{"brightwhite",   Terminal::Color16::BRIGHT_WHITE  },
	// Vim uses lightgray, lightgrey, gray, and grey as synonyms
	{"gray",          Terminal::Color16::BRIGHT_BLACK  },
	{"grey",          Terminal::Color16::BRIGHT_BLACK  },
	{"lightgray",     Terminal::Color16::BRIGHT_BLACK  },
	{"lightgrey",     Terminal::Color16::BRIGHT_BLACK  },
};

std::optional<Terminal::Color> Terminal::Color::from_string(const std::string &str) {
	auto lookup = aliases.find(str);
	if (lookup != aliases.end()) { return Terminal::Color(lookup->second); }

	const size_t str_size = str.size();

	if (str_size == 0) {
		return std::nullopt;
	}

	// specify a 0-255 number
	if (str_size <= 3) {
		try {
			size_t idx = 0;
			unsigned long value = stoul(str, &idx, 10);

			if (value <= 255 && idx == str_size) {
				return Terminal::Color((uint8_t) value);
			}
		} catch (const std::invalid_argument &e) {}
	}

	// specify #AABBCC
	if (str_size == 6) {
		try {
			size_t idx = 0;
			unsigned long value = stoul(str, &idx, 16);

			//std::cerr << str << std::endl;

			if (value <= 0xFFFFFF) {
				uint8_t r = (value >> 16) & 0xFF;
				uint8_t g = (value >>  8) & 0xFF;
				uint8_t b = (value      ) & 0xFF;
				return Terminal::Color(r, g, b);
			}
		} catch (const std::invalid_argument &e) {}
	}

	return std::nullopt;
}

