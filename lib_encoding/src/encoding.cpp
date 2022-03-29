#include "encoding.hpp"

const char* encoding::to_string(const Encoding e) {
	switch(e) {
		case encoding::Encoding::UTF8:    return "UTF-8";
		case encoding::Encoding::UCS2LE:  return "UCS-2 LE";
		case encoding::Encoding::UCS2BE:  return "UCS-2 BE";
		case encoding::Encoding::UTF16LE: return "UTF-16 LE";
		case encoding::Encoding::UTF16BE: return "UTF-16 BE";
		case encoding::Encoding::UTF32LE: return "UTF-32 LE";
		case encoding::Encoding::UTF32BE: return "UTF-32 BE";
	}
	throw new std::runtime_error("unknown value: " + std::to_string((uint8_t) e));
}

