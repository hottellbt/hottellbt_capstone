#ifndef INCLUDED_HIGHLIGHTING_HPP
#define INCLUDED_HIGHLIGHTING_HPP

#include "terminal.hpp"

#include <cstdint>
#include <optional>

namespace Highlighting {

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

			void apply_to_terminal_fg(Terminal::AbstractTerminal *term) const {
				switch (this->color_type) {
					case ColorType::USE_DEFAULT:
						term->unset_fg();
						break;
					case ColorType::COLOR_256:
						term->set_fg(color_256);
						break;
					case ColorType::COLOR_RGB:
						term->set_fg(color_rgb_r, color_rgb_g, color_rgb_b);
						break;
				}
			}

			void apply_to_terminal_bg(Terminal::AbstractTerminal *term) const {
				switch (this->color_type) {
					case ColorType::USE_DEFAULT:
						term->unset_bg();
						break;
					case ColorType::COLOR_256:
						term->set_bg(color_256);
						break;
					case ColorType::COLOR_RGB:
						term->set_bg(color_rgb_r, color_rgb_g, color_rgb_b);
						break;
				}
			}

		private:
			ColorType color_type;
			uint8_t color_256;
			uint8_t color_rgb_r;
			uint8_t color_rgb_g;
			uint8_t color_rgb_b;
	};

	std::optional<Color> color_from_string(const std::string &str);

	class Highlight {
		public:
			Highlight() {}

			Highlight(
					const std::string &fg_str,
					const std::string &bg_str) :
				fg(color_from_string(fg_str)), bg(color_from_string(bg_str)) {}
			
			Highlight(
					const std::optional<Color> &fg,
					const std::optional<Color> &bg) :
				fg(fg), bg(bg) {}

			void apply_to_terminal(Terminal::AbstractTerminal *term) const {
				if (fg) { fg->apply_to_terminal_fg(term); }
				if (bg) { bg->apply_to_terminal_bg(term); }
			}

			void imprint(const Highlight &hi) {
				if (!this->fg && hi.fg) { this->fg = hi.fg; }
				if (!this->bg && hi.bg) { this->bg = hi.bg; }
			}

		private:
			std::optional<Color> fg;
			std::optional<Color> bg;
	};
};

#endif
