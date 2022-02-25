#ifndef INCLUDED_HIGHLIGHT_HPP
#define INCLUDED_HIGHLIGHT_HPP

#include "terminal.hpp"

#include <cstdint>
#include <optional>

namespace Highlight {

	class Highlight {
		public:
			Highlight() {}

			Highlight(
					const std::string &fg_str,
					const std::string &bg_str) :
				fg(Terminal::Color::from_string(fg_str)),
				bg(Terminal::Color::from_string(bg_str)) {}
			
			Highlight(
					const std::optional<Terminal::Color> &fg,
					const std::optional<Terminal::Color> &bg) :
				fg(fg), bg(bg) {}

			void imprint(const Highlight &hi) {
				if (!this->fg && hi.fg) { this->fg = hi.fg; }
				if (!this->bg && hi.bg) { this->bg = hi.bg; }
			}

		private:
			std::optional<Terminal::Color> fg;
			std::optional<Terminal::Color> bg;
	};
};

#endif
