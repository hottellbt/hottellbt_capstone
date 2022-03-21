#include "twig_app.hpp"

#include <stdexcept>
#include <iostream>
#include <memory>
#include <queue>

#include <cerrno>
#include <cstring>
#include <cassert>

#include <ncurses.h>

bool in_app_mode = false;
WINDOW* stdscr;

void twig::curses::init() {
	if (in_app_mode) return;

	stdscr = initscr();

	keypad(stdscr, true);
	raw();
	noecho();
	curs_set(0);

	// this prevents the screen from being blank at launch
	wrefresh(stdscr);

	in_app_mode = true;
}

void twig::curses::exit() {
	if (!in_app_mode) return;

	keypad(stdscr, false);
	noraw();
	echo();
	curs_set(1);

	endwin();

	in_app_mode = false;
}

using twig::widget::Widget;
using twig::widget::Graphics;
using twig::widget::WInt;
using twig::widget::WDim;
using twig::widget::WPoint;

using twig::color::ColorPair;

void Graphics::delmagic(void) {
	if (magic != nullptr && magic != stdscr) delwin((WINDOW*) magic);
}

/*
WDim Graphics::get_size(void) {
	assert(magic != nullptr);
	WInt y, x;
	getmaxyx((WINDOW*) magic, y, x);
	return {x, y};
}
*/

WPoint Graphics::get_position(void) {
	assert(magic != nullptr);
	WInt y, x;
	getyx((WINDOW*) magic, y, x);
	return {x, y};
}

void Graphics::clear_fast(void) {
	assert(magic != nullptr);
	werase((WINDOW*) magic);
}

void Graphics::clear_full(void) {
	assert(magic != nullptr);
	wclear((WINDOW*) magic);
}

void Graphics::set_color_pair(const ColorPair &c) {
	// TODO this will get interesting :)
}

void Graphics::set_normal(void) {
	assert(magic != nullptr);
	wattrset((WINDOW*) magic, A_NORMAL);
}

void Graphics::mv(WInt x, WInt y) {
	assert(magic != nullptr);
	wmove((WINDOW*) magic, y, x);
}

void Graphics::add_ch(char c) {
	assert(magic != nullptr);
	waddch((WINDOW*) magic, c);
}

void Graphics::add_str_raw(const char* str, const size_t len) {
	assert(magic != nullptr);
	waddnstr((WINDOW*) magic, str, len);
}

void Graphics::add_str_raw(const char* str) {
	assert(magic != nullptr);
	waddstr((WINDOW*) magic, str);
}

void Graphics::set_standout(const bool b) {
	assert(magic != nullptr);
	if (b) wattron ((WINDOW*) magic, A_STANDOUT);
	else   wattroff((WINDOW*) magic, A_STANDOUT);
}

void Graphics::set_underline(const bool b) {
	assert(magic != nullptr);
	if (b) wattron ((WINDOW*) magic, A_UNDERLINE);
	else   wattroff((WINDOW*) magic, A_UNDERLINE);
}

void Graphics::set_reverse(const bool b) {
	assert(magic != nullptr);
	if (b) wattron ((WINDOW*) magic, A_REVERSE); 
	else   wattroff((WINDOW*) magic, A_REVERSE); 
}

void Graphics::set_blink(const bool b) {
	assert(magic != nullptr);
	if (b) wattron ((WINDOW*) magic, A_BLINK);   
	else   wattroff((WINDOW*) magic, A_STANDOUT);
}

void Graphics::set_dim(const bool b) {
	assert(magic != nullptr);
	if (b) wattron ((WINDOW*) magic, A_DIM);     
	else   wattroff((WINDOW*) magic, A_DIM);     
}

void Graphics::set_bold(const bool b) {
	assert(magic != nullptr);
	if (b) wattron ((WINDOW*) magic, A_BOLD);    
	else   wattroff((WINDOW*) magic, A_BOLD);    
}

void Graphics::set_protect(const bool b) {
	assert(magic != nullptr);
	if (b) wattron ((WINDOW*) magic, A_PROTECT); 
	else   wattroff((WINDOW*) magic, A_PROTECT); 
}

void Graphics::set_invisible(const bool b) {
	assert(magic != nullptr);
	if (b) wattron ((WINDOW*) magic, A_INVIS);   
	else   wattroff((WINDOW*) magic, A_INVIS);   
}

void Graphics::set_italic(const bool b) {
	assert(magic != nullptr);
	if (b) wattron ((WINDOW*) magic, A_ITALIC);  
	else   wattroff((WINDOW*) magic, A_ITALIC);  
}

// FIXME here we assume the terminal is in UTF-8
auto decoder = encoding::get_decoder(encoding::Encoding::UTF8);

twig::special_key get_special_key(int curses_key) {
	using X = twig::special_key;

	switch (curses_key) {
		case KEY_UP:        return X::UP;
		case KEY_DOWN:      return X::DOWN;
		case KEY_LEFT:      return X::LEFT;
		case KEY_RIGHT:     return X::RIGHT;
		case KEY_HOME:      return X::HOME;
		case KEY_END:       return X::END;
		case KEY_ENTER:     return X::ENTER;
		case KEY_NPAGE:     return X::NPAGE;
		case KEY_PPAGE:     return X::PPAGE;
		case KEY_BACKSPACE: return X::BACKSPACE;
		default: return X::OTHER;
	}
}

inline void do_repaint(Graphics& g, twig::TwigApp *app) {
	assert(app != nullptr);

	Widget* root_widget = app->get_root_widget();
	assert(root_widget != nullptr);

	WDim root_dim = root_widget->get_size();
	WPoint root_ul = root_widget->get_position();

	root_widget->paint(g);

	pnoutrefresh(
			(WINDOW*) g.magic,
			0,
			0,
			root_ul.y,
			root_ul.x,
			root_ul.y + root_dim.height,
			root_ul.x + root_dim.width);

	wrefresh(stdscr);

}

inline void do_resize(twig::TwigApp *app) {
	assert(app != nullptr);

	// this prevents strange behavior while resizing
	wrefresh(stdscr);

	WInt maxy, maxx;
	getmaxyx(stdscr, maxy, maxx);

	Widget* root_widget = app->get_root_widget();
	assert(root_widget != nullptr);

	root_widget->set_size({maxx, maxy});
}

int twig::curses::run_twig_app(TwigApp *app) {

	if (app == nullptr) {
		throw std::runtime_error("cannot give a null pointer as an application");
	}

	if (app->get_root_widget() == nullptr) {
		throw std::runtime_error("root widget cannot be null");
	}

	try {

		app->when_starting();

		twig::curses::init();

		Graphics g ( stdscr );

		do_resize(app);
		do_repaint(g, app);

		int ch;

		while (app->is_running()) {

			ch = getch();

			if (ch == -1) {
				app->when_no_event();

			} else if (ch >= 0 && ch <= 0xFF) {
				char ch_real = (char) ch;
				Unicode::string_t s = decoder->decode(&ch_real, 1);
				for(auto cp : s) {
					app->when_typed(cp);
				}

			} else if (ch == KEY_RESIZE) {
				do_resize(app);

			} else {
				app->when_typed_special(get_special_key(ch));
			}

			do_repaint(g, app);

		}

		twig::curses::exit();

		app->when_exiting();

		return 0;

	} catch (const std::exception& e) {
		twig::curses::exit();

		std::cerr << "Uncaught exception: " << e.what() << std::endl;
		if (errno != 0) {
			std::cerr << "errno " << std::to_string(errno) << ": "
				<< std::strerror(errno) << std::endl;
		}

		return 128;
	}

}
