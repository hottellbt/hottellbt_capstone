#include "encoding.hpp"

const char* Encoding::to_string(const Encoding e) {
	switch(e) {
		case Encoding::UTF8: return "UTF-8";
		case Encoding::UCS2: return "UCS-2";
	}
	throw new std::runtime_error("unknown value: " + std::to_string((uint8_t) e));
}

