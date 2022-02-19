#ifndef INCLUDED_CODING_ERROR_HPP
#define INCLUDED_CODING_ERROR_HPP

#include <stdexcept>

class encoding_error : public std::runtime_error {
	public:
		encoding_error(const std::string &msg) : runtime_error(msg) {}
};

class decoding_error : public std::runtime_error {
	public:
		decoding_error(const std::string &msg) : runtime_error(msg) {}
};

#endif
