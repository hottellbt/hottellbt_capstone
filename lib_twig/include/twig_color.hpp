#ifndef INCLUDED_TWIG_COLOR_HPP
#define INCLUDED_TWIG_COLOR_HPP

#include <optional>
#include <string>

namespace twig::color {

	namespace Color16 {
		constexpr uint8_t BLACK   = 0;
		constexpr uint8_t RED     = 1;
		constexpr uint8_t GREEN   = 2;
		constexpr uint8_t YELLOW  = 3;
		constexpr uint8_t BLUE    = 4;
		constexpr uint8_t MAGENTA = 5;
		constexpr uint8_t CYAN    = 6;
		constexpr uint8_t WHITE   = 7;
		constexpr uint8_t BRIGHT_BLACK   = 8;
		constexpr uint8_t BRIGHT_RED     = 9;
		constexpr uint8_t BRIGHT_GREEN   = 10;
		constexpr uint8_t BRIGHT_YELLOW  = 11;
		constexpr uint8_t BRIGHT_BLUE    = 12;
		constexpr uint8_t BRIGHT_MAGENTA = 13;
		constexpr uint8_t BRIGHT_CYAN    = 14;
		constexpr uint8_t BRIGHT_WHITE   = 15;
	};

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

			bool operator==(const Color &c) const {
				if (color_type != c.color_type) return false;

				switch (color_type) {

					case ColorType::COLOR_256:
						if (color_256 != c.color_256) return false;
						break;

					case ColorType::COLOR_RGB:
						if (color_rgb_r != c.color_rgb_r
								|| color_rgb_g != c.color_rgb_g
								|| color_rgb_b != c.color_rgb_b) return false;
						break;

					default: break;
				}

				return true;
			}

			bool operator!=(const Color &c) const {
				return !operator==(c);
			}

			static std::optional<Color> from_string(const std::string&);

			inline static std::optional<Color> from_opt_string(
					const std::optional<const std::string>& str) {
				if (!str) return std::nullopt;
				return from_string(*str);
			}

			ColorType color_type;
			union {
				uint8_t color_256;
				uint8_t color_rgb_r;
			};
			uint8_t color_rgb_g;
			uint8_t color_rgb_b;
	};

	class ColorPair {
		public:
			Color fg;
			Color bg;

			bool operator==(const ColorPair &c) const {
				return fg == c.fg
					&& bg == c.bg;
			}

			bool operator!=(const ColorPair &c) const {
				return fg != c.fg
					|| bg != c.bg;
			}
	};

	Color reduce_to_256(const Color& c);
	Color reduce_to_16(const Color& c);

};

#endif
