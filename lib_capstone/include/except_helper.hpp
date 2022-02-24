#ifndef INCLUDED_EXCEPT_HELPER_HPP
#define INCLUDED_EXCEPT_HELPER_HPP

#include <exception>
#include <cerrno>
#include <cstring>

inline std::string comment_with_errno(const std::string &comment, int errorno) {
	std::string ret = comment;

	if (errno != 0) {
		ret += ", errno=";
		ret += std::to_string(errno);
		ret += " ";
		ret += std::strerror(errno);
	}

	return ret;
}

inline std::string comment_with_auto_errno(const std::string &comment) {
	int my_errno = errno;
	errno = 0;
	return comment_with_errno(comment, my_errno);
}

#endif
