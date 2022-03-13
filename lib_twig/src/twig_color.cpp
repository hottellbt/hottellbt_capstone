#include "twig_color.hpp"

#include <stdexcept>
#include <cstdint>
#include <map>

namespace Color16 = twig::color::Color16;
using ColorType = twig::color::ColorType;
using twig::color::Color;

const std::map<const std::string, const uint8_t> aliases {
	{"black",         Color16::BLACK         },
	{"red",           Color16::RED           },
	{"green",         Color16::GREEN         },
	{"yellow",        Color16::YELLOW        },
	{"blue",          Color16::BLUE          },
	{"magenta",       Color16::MAGENTA       },
	{"cyan",          Color16::CYAN          },
	{"white",         Color16::WHITE         },
	{"brightblack",   Color16::BRIGHT_BLACK  },
	{"brightred",     Color16::BRIGHT_RED    },
	{"brightgreen",   Color16::BRIGHT_GREEN  },
	{"brightyellow",  Color16::BRIGHT_YELLOW },
	{"brightblue",    Color16::BRIGHT_BLUE   },
	{"brightmagenta", Color16::BRIGHT_MAGENTA},
	{"brightcyan",    Color16::BRIGHT_CYAN   },
	{"brightwhite",   Color16::BRIGHT_WHITE  },
	// vim uses light as a synonym for bright
	{"lightblack",    Color16::BRIGHT_BLACK  },
	{"lightred",      Color16::BRIGHT_RED    },
	{"lightgreen",    Color16::BRIGHT_GREEN  },
	{"lightyellow",   Color16::BRIGHT_YELLOW },
	{"lightblue",     Color16::BRIGHT_BLUE   },
	{"lightmagenta",  Color16::BRIGHT_MAGENTA},
	{"lightcyan",     Color16::BRIGHT_CYAN   },
	{"lightwhite",    Color16::BRIGHT_WHITE  },
	// Vim uses gray/grey as a synonym for bright/light black
	{"gray",          Color16::BRIGHT_BLACK  },
	{"grey",          Color16::BRIGHT_BLACK  },
	{"lightgray",     Color16::BRIGHT_BLACK  },
	{"lightgrey",     Color16::BRIGHT_BLACK  },
};

std::optional<Color> Color::from_string(
		const std::string &str) {

	auto lookup = aliases.find(str);
	if (lookup != aliases.end()) { return Color(lookup->second); }

	const size_t str_size = str.size();

	// empty string is interpreted as no value
	if (str_size == 0) {
		return std::nullopt;
	}

	// specify a 0-255 number
	if (str_size <= 3) {
		try {
			size_t idx = 0;
			unsigned long value = stoul(str, &idx, 10);

			if (value <= 255 && idx == str_size) {
				return Color((uint8_t) value);
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
				return Color(r, g, b);
			}
		} catch (const std::invalid_argument &e) {}
	}

	return std::nullopt;
}

Color twig::color::reduce_to_256(const Color& c) {
	switch (c.color_type) {

		case ColorType::USE_DEFAULT:
		case ColorType::COLOR_256:
			return c;

		case ColorType::COLOR_RGB:
			break;
	}

	// FIXME
	return c;
}

Color twig::color::reduce_to_16(const Color& c) {

	switch (c.color_type) {

		case ColorType::USE_DEFAULT:
			return c;

		case ColorType::COLOR_RGB:
			return reduce_to_16(reduce_to_256(c));

		case ColorType::COLOR_256:
			break;
	}

	// FIXME
	return c;
}
