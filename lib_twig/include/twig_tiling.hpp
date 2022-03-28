#ifndef INCLUDED_TWIG_TILING_HPP
#define INCLUDED_TWIG_TILING_HPP

#include "twig_widget.hpp"
#include "twig_geometry.hpp"

#include <vector>

namespace twig::tiling {

	class TileStrategy {
		public:
			TileStrategy() {}
			virtual ~TileStrategy() {}

			virtual void retile(
					const twig::geom::Rectangle& bounds,
					std::vector<Widget*> widgets,
					size_t focused_idx) = 0;
	};

	class FullscreenStrategy : public TileStrategy {
		public:
			FullscreenStrategy() {}
			virtual ~FullscreenStrategy() {}


			void retile(
					const twig::geom::Rectangle& bounds,
					std::vector<Widget*> widgets,
					size_t focused_idx) {

				const size_t num_widgets = widgets.size();
				size_t i = 0;

				for (; i < num_widgets; i++) {
					Widget* w = widgets[i];
					if (i == focused_idx) {
						w->set_visible(true);
						w->set_size(bounds);
					} else {
						w->set_visible(false);
					}
				}

			}
	};

}

#endif
