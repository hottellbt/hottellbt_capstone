#ifndef INCLUDED_TWIG_GEOMETRY_HPP
#define INCLUDED_TWIG_GEOMETRY_HPP

#include <cmath>

namespace twig::geom {

	template <typename T> struct Point {

		T x, y;

		Point() {}

		Point(T x, T y) :
			x(x), y(y) {}

		bool operator==(const Point<T>& o) const {
			return x == o.x && y == o.y;
		}

		bool operator!=(const Point<T>& o) const {
			return x != o.x || y != o.y;
		}

	};

	template <typename T> struct Point3 {

		T x, y, z;

		Point3() {}

		Point3(T x, T y, T z) :
			x(x), y(y), z(z) {}

		bool operator==(const Point<T>& o) const {
			return x == o.x && y == o.y && z == o.z;
		}

		bool operator!=(const Point<T>& o) const {
			return x != o.x || y != o.y || z != o.z;
		}

	};

	template <typename T> inline T abs(T t) {
		return t > 0 ? t : -t;
	}

	template <typename T> inline T taxi_distance(
			Point3<T> p1, Point3<T> p2) {
		T x = abs(p1.x - p2.x);
		T y = abs(p1.y - p2.y);
		T z = abs(p1.z - p2.z);
		return x + y + z;
	}

	template <typename T> struct Dimension {

		T width, height;

		Dimension() {}

		Dimension(T width, T height) :
			width(width), height(height) {}

		bool operator==(const Dimension<T>& o) const {
			return width == o.width && height == o.height;
		}

		bool operator!=(const Dimension<T>& o) const {
			return width != o.width || height != o.height;
		}

	};

	template <typename T> struct Rectangle {

		T x, y, width, height;

		Rectangle() {}

		Rectangle(
				T x,
				T y,
				T width,
				T height) :
			x(x), y(y), width(width), height(height) {}

		Rectangle(
				T width,
				T height) :
			x(0), y(0), width(width), height(height) {}

		Rectangle(
				Point<T>&& point,
				Dimension<T>&& dimension) :
			x(point.x), y(point.y), width(dimension.width), height(dimension.height) {}

		bool is_zero() const {
			return width == 0 || height == 0;
		}

		bool operator==(const Rectangle<T>& o) const {
			return x == o.x
				&& y == o.y
				&& width == o.width
				&& height == o.height;
		}

		bool operator!=(const Rectangle<T>& o) const {
			return x != o.x
				|| y != o.y
				|| width != o.width
				|| height != o.height;
		}

		Rectangle<T> operator+(Dimension<T>&& o) const {
			return {
				x,
				y,
				width + o.width,
				height + o.height
			};
		}

		Rectangle<T> operator-(Dimension<T>&& o) const {
			return {
				x,
				y,
				(T) (width - o.width),
				(T) (height - o.height)
			};
		}

		void operator+=(Dimension<T>&& o) {
			width += o.width;
			height += o.height;
		}

		void operator-=(Dimension<T>&& o) {
			width -= o.width;
			height -= o.height;
		}
	};

}

#endif
