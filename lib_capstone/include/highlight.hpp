#ifndef INCLUDED_HIGHLIGHT_HPP
#define INCLUDED_HIGHLIGHT_HPP

#include "terminal.hpp"

#include <optional>
#include <vector>
#include <map>
#include <cstdint>

namespace highlight {

	class Highlight {
		public:
			Terminal::Color fg;
			Terminal::Color bg;

			bool operator==(const Highlight &c) const {
				return fg == c.fg
					&& bg == c.bg;
			}

			bool operator!=(const Highlight &c) const {
				return fg != c.fg
					|| bg != c.bg;
			}

			void activate() {
				Terminal::set_fg(fg);
				Terminal::set_bg(bg);
			}
	};

	class OptionalHighlight {
		public:

			OptionalHighlight() :
				fg(std::nullopt), bg(std::nullopt) {}

			OptionalHighlight(
					const std::optional<const std::string> fg,
					const std::optional<const std::string> bg) :
				fg(Terminal::Color::from_opt_string(fg)),
				bg(Terminal::Color::from_opt_string(bg)) {}

			OptionalHighlight(
					const std::optional<Terminal::Color> fg,
					const std::optional<Terminal::Color> bg) :
				fg(fg), bg(bg) {}

			void imprint(const OptionalHighlight &hi) {
				if (!this->fg && hi.fg) { this->fg = hi.fg; }
				if (!this->bg && hi.bg) { this->bg = hi.bg; }
			}

			Highlight resolve() {
				Highlight ret;
				if (this->fg) ret.fg = *(this->fg);
				if (this->bg) ret.bg = *(this->bg);
				return ret;
			}

			std::optional<Terminal::Color> fg;
			std::optional<Terminal::Color> bg;

	};

	class ColorScheme {
		public:

			OptionalHighlight get_partial_highlight(
					const std::string name_utf8) {

				auto lookup = partial_highlights.find(name_utf8);
				if (lookup != partial_highlights.end()) return lookup->second;

				OptionalHighlight ret;
				return ret;
			}

			Highlight get_highlight(std::vector<std::string> names_utf8) {
				OptionalHighlight ret;

				for (auto it = names_utf8.begin(); it != names_utf8.end(); it++) {
					ret.imprint(get_partial_highlight(*it));
				}

				return ret.resolve();
			}

			void set_partial_highlight(
					const std::string& name_utf8,
					const OptionalHighlight& highlight) {
				partial_highlights[name_utf8] = highlight;
			}

		private:

			std::map<std::string, OptionalHighlight> partial_highlights;

	};

};

#endif
