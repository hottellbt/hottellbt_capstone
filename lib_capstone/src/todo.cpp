#include "todo.hpp"

#include <stdexcept>
#include <string>

const char* todo::to_string(const todo::Priority &p) {
	using P = todo::Priority;
	switch (p) {
		case P::LOWEST:  return "Lowest";
		case P::LOWER:   return "Lower";
		case P::LOW:     return "Low";
		case P::NORMAL:  return "Normal";
		case P::HIGH:    return "High";
		case P::HIGHER:  return "Higher";
		case P::HIGHEST: return "Highest";
	}
	throw std::runtime_error(std::to_string((int8_t) p));
}

const char* todo::to_string(const Status &s) {
	using S = todo::Status;
	switch(s) {
		case S::DOING:     return "Doing";
		case S::NO_STATUS: return "None";
		case S::DONE:      return "Done";
		case S::CANCELED:  return "Canceled";
	}
	throw std::runtime_error(std::to_string((uint8_t) s));
}
