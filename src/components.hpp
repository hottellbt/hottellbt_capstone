#ifndef INCLUDED_COMPONENTS_HPP
#define INCLUDED_COMPONENTS_HPP

#include "unicode.hpp"

struct Point {

	unsigned short x, y;

	Point() {}

	Point(
			unsigned short x,
			unsigned short y) :
		x(x), y(y) {}

	bool operator==(const Point& o) const {
		return x == o.x && y == o.y;
	}

	bool operator!=(const Point& o) const {
		return x != o.x || y != o.y;
	}

};

struct Dimension {

	unsigned short width, height;

	Dimension() {}

	Dimension(
			unsigned short width,
			unsigned short height) :
		width(width), height(height) {}

	bool operator==(const Dimension& o) const {
		return width == o.width && height == o.height;
	}

	bool operator!=(const Dimension& o) const {
		return width != o.width || height != o.height;
	}

};

struct Rectangle {

	unsigned short x, y, width, height;

	Rectangle() {}

	Rectangle(
			unsigned short x,
			unsigned short y,
			unsigned short width,
			unsigned short height) :
		x(x), y(y), width(width), height(height) {}

	Rectangle(
			unsigned short width,
			unsigned short height) :
		x(0), y(0), width(width), height(height) {}

	Rectangle(
			const Point& point,
			const Dimension& dimension) :
		x(point.x), y(point.y), width(dimension.width), height(dimension.height) {}

	bool is_zero() const {
		return width == 0 || height == 0;
	}

	bool operator==(const Rectangle& o) const {
		return x == o.x
			&& y == o.y
			&& width == o.width
			&& height == o.height;
	}

	bool operator!=(const Rectangle& o) const {
		return x != o.x
			|| y != o.y
			|| width != o.width
			|| height != o.height;
	}

};

#endif
