#ifndef INCLUDED_TWIG_WIDGET_HPP
#define INCLUDED_TWIG_WIDGET_HPP

#include "twig_constants.hpp"
#include "twig_geometry.hpp"
#include "twig_color.hpp"

#include "unicode.hpp"
#include "encoding.hpp"

#include <stdexcept>
#include <memory>
#include <string>
#include <optional>

#include <cassert>

namespace twig::widget {

	typedef unsigned short int WInt;

	typedef twig::geom::Point<WInt>     WPoint;
	typedef twig::geom::Dimension<WInt> WDim;
	typedef twig::geom::Rectangle<WInt> WRect;

	class Graphics {
		public:
			void* magic;

			Graphics() : Graphics(nullptr) {}
			Graphics(void* magic) : magic(magic) {}
			~Graphics();

			WDim get_size(void);
			WPoint get_position(void);

			void clear_fast(void);
			void clear_full(void);

			void set_normal(void);

			void set_color_pair(const twig::color::ColorPair &c);

			void set_standout  (const bool b);
			void set_underline (const bool b);
			void set_reverse   (const bool b);
			void set_blink     (const bool b);
			void set_dim       (const bool b);
			void set_bold      (const bool b);
			void set_protect   (const bool b);
			void set_invisible (const bool b);
			void set_italic    (const bool b);

			void mv(WInt x, WInt y);

			void add_ch(char c);

			void add_str_raw(const char* str, const size_t len);
			void add_str_raw(const char* str);

			inline void add_str_raw(const std::string& str) {
				add_str_raw(str.c_str(), str.size());
			}

			inline void add_str_raw(const std::vector<char> v) {
				for (size_t i = 0; i < v.size(); i++) {
					add_ch(v[i]);
				}
			}

			inline void add_unicode_str(const Unicode::string_t& str) {
				add_str_raw(encoding::auto_encode_or_throw(encoding::Encoding::UTF8, &str));
			}

			inline void add_str_utf8(const char* str, const size_t len) {
				// FIXME we are assuming that the terminal uses UTF-8
				add_str_raw(str, len);
			}

			inline void add_str_utf8(const char* str) {
				// FIXME we are assuming that the terminal uses UTF-8
				add_str_raw(str);
			}

			inline void add_str_utf8(const std::string& str) {
				add_str_utf8(str.c_str(), str.size());
			}

			inline void mv_add_ch(
					WInt x, WInt y,
					char c) {
				mv(x, y);
				add_ch(c);
			}

			inline void mv_add_str_raw(
					WInt x, WInt y,
					const char* str, const size_t len) {
				mv(x, y);
				add_str_raw(str, len);
			}

			inline void mv_add_str_raw(
					WInt x, WInt y,
					const char* str) {
				mv(x, y);
				add_str_raw(str);
			}

			inline void mv_add_str_raw(
					WInt x, WInt y,
					const std::string& str) {
				mv(x, y);
				add_str_raw(str.c_str(), str.size());
			}

			inline void mv_add_str_utf8(
					WInt x, WInt y,
					const char* str, const size_t len) {
				mv(x, y);
				add_str_utf8(str, len);
			}

			inline void mv_add_str_utf8(
					WInt x, WInt y,
					const char* str) {
				mv(x, y);
				add_str_utf8(str);
			}

			inline void mv_add_str_utf8(
					WInt x, WInt y,
					const std::string& str) {
				mv(x, y);
				add_str_utf8(str);
			}

			void when_owner_resized(const WDim& new_size);

			inline unsigned short get_str_width(Unicode::codepoint_t cp) {
				using X = Unicode::EastAsianWidth;
				const auto width_prop = Unicode::get_east_asian_width(cp);
				switch(width_prop) {
					case X::F:
					case X::W:
						return 2;
					default:
						return 1;
				}
			}

			template <typename T>
			inline unsigned short get_str_width(T s, size_t start, size_t end) {
				unsigned short sum = 0;
				for (size_t i = start; i < end; i++) {
					sum += get_str_width(s[i]);
				}
				return sum;
			}

			inline unsigned short get_str_width(Unicode::string_t s) {
				return get_str_width(s, 0, s.size());
			}
	};

	inline std::unique_ptr<Graphics> request_graphics() {
		return std::unique_ptr<Graphics>(new Graphics());
	}

	class Widget {
		public:
			Widget() {
				if (is_graphical()) {
					graphics = request_graphics();
				} else {
					graphics = nullptr;
				}
			}

			virtual ~Widget() {}

			virtual void repaint() = 0;

			bool is_graphical() const noexcept {
				return true;
			}

			std::unique_ptr<Graphics>& get_graphics(void) {
				assert(is_graphical());
				return graphics;
			}

			WPoint& get_position(void) { return position; }

			WDim& get_size(void) { return size; }

			void set_position(const WPoint& x) { position = x; }

			void set_size(const WDim& new_size) {

				assert(new_size.width > 0 && new_size.height > 0);

				if (size != new_size) {
					size = new_size;
					if (is_graphical()) {
						graphics->when_owner_resized(new_size);
					}
				}
			}

		private:
			WPoint position {0, 0};
			WDim size {0, 0};
			std::unique_ptr<Graphics> graphics = nullptr;
	};
}

#endif
