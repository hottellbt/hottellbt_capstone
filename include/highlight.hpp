#ifndef INCLUDED_HIGHLIGHT_HPP
#define INCLUDED_HIGHLIGHT_HPP

#include "terminal.hpp"
#include <cstdint>
#include <optional>

namespace Highlight {

	enum class ColorType : uint8_t {
		USE_DEFAULT,
		COLOR_256,
		COLOR_RGB
	};

	class HighlightColor {
		public:
			HighlightColor() :
				color_type(ColorType::USE_DEFAULT) {}

			HighlightColor(uint8_t color256) :
				color_type(ColorType::COLOR_256), color_256(color_256) {}

			HighlightColor(uint8_t r, uint8_t g, uint8_t b) :
				color_type(ColorType::COLOR_RGB),
				color_rgb_r(r), color_rgb_g(g), color_rgb_b(b) {}

			void apply_to_terminal_fg(Terminal::AbstractTerminal *term) {
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

			void apply_to_terminal_bg(Terminal::AbstractTerminal *term) {
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
			union {
				uint8_t color_256;
				uint8_t color_rgb_r;
			};
			uint8_t color_rgb_g;
			uint8_t color_rgb_b;
	};

	class Highlight {
		public:
			Highlight() {}
			
			Highlight(
					std::optional<HighlightColor> fg,
					std::optional<HighlightColor> bg) :
				fg(fg), bg(bg) {}

			void apply_to_terminal(Terminal::AbstractTerminal *term) {
				if (fg) { fg->apply_to_terminal_fg(term); }
				if (bg) { bg->apply_to_terminal_bg(term); }
			}

			void imprint(const Highlight &hi) {
				if (!this->fg && hi.fg) { this->fg = hi.fg; }
				if (!this->bg && hi.bg) { this->bg = hi.bg; }
			}

		private:
			std::optional<HighlightColor> fg;
			std::optional<HighlightColor> bg;
	};
};

#endif
