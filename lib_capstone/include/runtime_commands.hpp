#ifndef INCLUDED_RUNTIME_COMMANDS_HPP
#define INCLUDED_RUNTIME_COMMANDS_HPP

#include "unicode.hpp"

#include <cstdint>
#include <optional>
#include <vector>

namespace RuntimeCommands {

	enum class TokenType : uint8_t {
		error,
		arg_part_literal,
		arg_part_substitute,
		end_arg,
		end_line,
		end_file
	};

	struct Token {

		TokenType type;
		std::optional<Unicode::string_t> payload;

		Token(TokenType type) : type(type), payload() {}

		Token(TokenType type, const Unicode::string_t& payload) :
			type(type), payload(payload) {}

		bool operator ==(const Token &other) const {
			return type == other.type && payload == other.payload;
		}
	};

	class BufferedTokenizer {
		public:

			void decode(
					const Unicode::string_t &string,
					std::vector<Token> &tokens);

			bool require_done(void);

		private:
			bool in_comment = false;

			Unicode::codepoint_t quote;
			Unicode::string_t payload_builder;

	};

};

#endif
