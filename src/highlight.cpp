#include "highlight.hpp"
#include "terminal.hpp"

#include <iostream>
#include <cstdint>
#include <map>

std::map<const std::string, const uint8_t> aliases {
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

std::optional<Highlight::HighlightColor> Highlight::color_from_string(const std::string &str) {
	auto lookup = aliases.find(str);
	if (lookup != aliases.end()) { return HighlightColor(lookup->second); }

	return std::nullopt;
}

