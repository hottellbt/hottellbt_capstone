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
#include <vector>

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
			~Graphics() { delmagic(); }

			WDim get_size(void);
			WPoint get_position(void);

			void subgraphics(
					const WPoint& on_screen,
					const std::unique_ptr<Graphics>& g);

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

			inline void add_unicode_str(const Unicode::string_t& str) {
				add_str_raw(encoding::encode(encoding::Encoding::UTF8, str));
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

			void resize(const WDim& new_size);

			void set_parent(const std::unique_ptr<Graphics>& parent);

			void set_no_parent(void);

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

		private:
			void delmagic(void);
	};

	class Widget {
		public:
			Widget() {
				graphics = std::unique_ptr<Graphics>(new Graphics());
			}

			virtual ~Widget() {}

			std::unique_ptr<Graphics>& get_graphics(void) {
				return graphics;
			}

			virtual void repaint() = 0;

			WPoint& get_position(void) { return position; }

			WDim& get_size(void) { return size; }

			void set_position(const WPoint& x) { position = x; }

			void set_size(const WDim& new_size) {

				assert(new_size.width > 0 && new_size.height > 0);

				if (size != new_size) {
					size = new_size;
					graphics->resize(new_size);
				}

				this->when_resized(new_size);
			}

		protected:

			virtual void when_resized(const WDim& new_size) {}

		private:
			WPoint position {0, 0};
			WDim size {0, 0};
			std::unique_ptr<Graphics> graphics = nullptr;
	};
}

#endif
