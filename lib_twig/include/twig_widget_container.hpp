#ifndef INCLUDED_TWIG_WIDGET_CONTAINER_HPP
#define INCLUDED_TWIG_WIDGET_CONTAINER_HPP

#include <vector>

#include <cassert>

namespace twig::widget {

	class Container : public Widget {
		public:
			Container() {}
			virtual ~Container() {}

			bool contains_child(Widget* widget) {
				for (Widget* w : widgets)  {
					if (w == widget) return true;
				}
				return false;
			}

			void add_child(Widget* widget) {
				assert(!contains_child(widget));
				widgets.push_back(widget);
			}

			void add_child(Widget* widget, size_t index) {
				assert(!contains_child(widget));
				assert(index <= widgets.size());
				widgets.push_back(widget);
			}

			void remove_child(size_t index) {
				assert(index < widgets.size());
				widgets.erase(widgets.begin() + index);
			}

			void remove_child(Widget* widget) {
				for (size_t i = 0; i < widgets.size(); i++) {
					if (widgets[i] == widget) {
						remove_child(i);
						return;
					}
				}
			}

			void remove_all_children() {
				widgets.clear();
			}

			size_t get_num_children() {
				return widgets.size();
			}

		private:
			std::vector<Widget*> widgets;
	};

}

#endif
