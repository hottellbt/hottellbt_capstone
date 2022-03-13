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

namespace twig::widget {
	typedef unsigned short int WInt;

	typedef twig::geom::Point<WInt>     WPoint;
	typedef twig::geom::Dimension<WInt> WDim;
	typedef twig::geom::Rectangle<WInt> WRect;

	class Graphics {
		public:
			Graphics(void* magic) : magic(magic) {}
			~Graphics();

			WDim get_size(void);

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

			void* get_magic() { return magic; }

			void when_owner_resized(const WDim& new_size);

		private:
			void* magic;
	};

	std::unique_ptr<Graphics> request_graphics(void);

	class Widget {
		public:
			Widget() {}

			virtual ~Widget() {}

			virtual void repaint() = 0;

			void* get_graphics_magic() {
				return graphics->get_magic();
			}

			std::unique_ptr<Graphics>& get_graphics(void) {
				return graphics;
			}

			WPoint& get_position(void) { return position; }

			WDim& get_size(void) { return size; }

			void set_position(const WPoint& x) { position = x; }

			void set_size(const WDim& new_size) {
				if (new_size.width < 1 || new_size.height < 1) {
					throw std::runtime_error("cannot resize widget to zero");
				}
				if (size != new_size) {
					size = new_size;
					graphics->when_owner_resized(new_size);
				}
			}

		private:
			WPoint position {0, 0};
			WDim size {0, 0};
			std::unique_ptr<Graphics> graphics = request_graphics();
	};
}

#endif
