#ifndef INCLUDED_ENCODING_HPP
#define INCLUDED_ENCODING_HPP

#include "unicode.hpp"

#include <memory>
#include <stdexcept>
#include <cstdint>
#include <string>

namespace Encoding {

	class encoding_error : public std::runtime_error {
		public:
			encoding_error(const std::string &msg) : runtime_error(msg) {}
	};

	class decoding_error : public std::runtime_error {
		public:
			decoding_error(const std::string &msg) : runtime_error(msg) {}
	};

	enum class Encoding : uint8_t {
		UTF8,
		UCS2
	};

	const char* to_string(const Encoding e);

	class BufferedDecoder {
		public:

			virtual Encoding get_encoding() const noexcept = 0;

			virtual Unicode::string_t decode(const char *bytes, const size_t bytes_len) = 0;

			virtual Unicode::string_t decode(char byte) { return decode(&byte, 1); }

			virtual bool can_end() { return true; }

			virtual void end() {
				if (!can_end()) {
					throw decoding_error((std::string) to_string(this->get_encoding()) 
							+ ": Incomplete byte sequence");
				}
			}
	};

	namespace UTF8 {

		class UTF8BufferedDecoder : public BufferedDecoder {
			public:
				Encoding get_encoding() const noexcept override { return Encoding::UTF8; }

				Unicode::string_t decode(const char *bytes, const size_t bytes_len) override;

				bool can_end() override { return codepoint_pos == 0; }

			private:
				Unicode::codepoint_t codepoint;
				uint_fast8_t codepoint_pos = 0;
				uint_fast8_t codepoint_len = 0;
		};

		inline Unicode::string_t decode(const std::string &s) {
			UTF8BufferedDecoder bd;
			auto ret = bd.decode(s.c_str(), s.size());
			return ret;
		}

		std::string encode(const Unicode::string_t &s);

	};

	inline std::unique_ptr<BufferedDecoder> get_decoder(Encoding e) {
		switch(e) {

			case Encoding::UTF8: return std::make_unique<UTF8::UTF8BufferedDecoder>();

			default: break;
		}
		throw std::runtime_error((std::string) "Not supported: " + to_string(e));
	}
}

#endif
