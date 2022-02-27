#include <cxxtest/TestSuite.h>

#include "runtime_commands.hpp"

class RuntimeCommandTestSuite : public CxxTest::TestSuite {
	public:
		template <typename T>
		bool vec_eq(const std::vector<T> v1, const std::vector<T> v2) {
			const size_t size = v1.size();
			if (v2.size() != size) return false;
			for (size_t i = 0; i < size; i++) {
				if (v1[i] != v2[i]) {
					return false;
				}
			}
			return true;
		}

		void check_token_types(
				const std::string &string,
				const std::vector<RuntimeCommands::Token> &tokens) {
			check_token_types(UTF8::decode(string), tokens);
		}

		void check_token_types(
				const Unicode::string_t &string,
				const std::vector<RuntimeCommands::Token> &tokens) {

			std::vector<RuntimeCommands::Token> actual_tokens;
			RuntimeCommands::BufferedTokenizer bt;
			bt.decode(string, actual_tokens);

			if (actual_tokens != tokens) {
				TS_FAIL("token mismatch for input " + UTF8::encode(string));
			}
		}

		void test_token_types(void) {
			check_token_types("", {});
			check_token_types("   ", {});
			check_token_types("\n", {RuntimeCommands::TokenType::end_line});
		}
};

