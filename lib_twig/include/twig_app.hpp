#ifndef INCLUDED_TWIG_APP_HPP
#define INCLUDED_TWIG_APP_HPP

#include "twig_constants.hpp"
#include "twig_widget.hpp"

namespace twig {

	class TwigApp {
		public:
			virtual bool is_running() = 0;

			virtual twig::widget::Widget* get_root_widget() = 0;

			virtual void when_typed(const Unicode::codepoint_t& input) {}
			virtual void when_typed_special(const special_key& key) {}

			virtual void when_no_event() {}

			virtual void when_starting() {}
			virtual void when_exiting() {}
	};

	namespace curses {

		void init();
		void exit();

		int run_twig_app(TwigApp *app);

	}

};

#endif
