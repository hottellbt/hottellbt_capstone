#include "runtime_commands.hpp"

void RuntimeCommands::BufferedTokenizer::decode(
		const Unicode::string_t &s,
		std::vector<Token> &tokens) {

	size_t i = 0;
	const size_t len = s.size();

	for (; i < len; i++) {

		// while in comment, skip until a newline	
		if (this->in_comment) {
			for (; i < len && s[i] != '\n'; i++);

			if (i < len && s[i] == '\n') {
				this->in_comment = false;
				i++;
			}

			if (i == len) return;
		}

		// check for newline
		if (s[i] == '\n') {
			tokens.push_back(TokenType::end_line);
			if (i++ == len) return;
		}

		// jump over leading whitespace
		for (; i < len && Unicode::is_white_space(s[i]); i++);
		if (i == len) return;

		// enter a comment
		if (s[i] == '#') {
			this->in_comment = true;
			i++;
			continue;
		}

	}

}

bool RuntimeCommands::BufferedTokenizer::require_done(void) {

}
