#include "unicode.hpp"

using Unicode::codepoint_t;


Unicode::GraphemeClusterBreak Unicode::get_grapheme_cluster_break(codepoint_t codepoint) {
	// https://www.unicode.org/reports/tr29/

	using X = Unicode::GraphemeClusterBreak;

	switch (codepoint) {
		case 0x000D: return X::CR;
		case 0x000A: return X::LF;
		case 0x200D: return X::ZWJ;

		// special inclusions for SpacingMark
		case 0x0E33: return X::SpacingMark;
		case 0x0EB3: return X::SpacingMark;
	}

	if (Unicode::is_regional_indicator(codepoint)) {
		return X::Regional_Indicator;
	}


	/* Extend */
	// U+200D (ZWJ) is specifically excluded, but we already handle that above

	if (Unicode::is_grapheme_extend(codepoint)
			|| Unicode::is_emoji_modifier(codepoint)) {
		return X::Extend;
	}
	using GC = Unicode::GeneralCategory;
	const GC gc = Unicode::get_general_category(codepoint);


	/* Control */

	if (
			(gc == GC::Line_Separator
			 || gc == GC::Paragraph_Separator
			 || gc == GC::Control
			 || (gc == GC::Unassigned && Unicode::is_default_ignorable_code_point(codepoint))
			 || gc == GC::Format)
			&& !Unicode::is_prepended_concatenation_mark(codepoint)
			// && codepoint != 0x000D // handled above (LF)
			// && codepoint != 0x000A // handled above (CR)
			// && codepoint != 0x200C // handled above (Extend)
			// && codepoint != 0x200D // handled above (ZWJ)
	   ) {
		return X::Control;
	}


	/* Prepend */

	using ISC = Unicode::IndicSyllabicCategory;
	const ISC isc = Unicode::get_indic_syllabic_category(codepoint);

	if (Unicode::is_prepended_concatenation_mark(codepoint)
			|| isc == ISC::Consonant_Preceding_Repha
			|| isc == ISC::Consonant_Prefixed) {
		return X::Prepend;
	}


	/* L V T LV LVT */

	using HST = Unicode::HangulSyllableType;
	const HST hst = Unicode::get_hangul_syllable_type(codepoint);

	switch (hst) {
		case HST::L:   return X::L;
		case HST::V:   return X::V;
		case HST::T:   return X::T;
		case HST::LV:  return X::LV;
		case HST::LVT: return X::LVT;
		default: break;
	}

	/* SpacingMark */
	// must not be Extend (handled above)

	if (gc == GC::Spacing_Mark) {
		switch(codepoint) {
			// manual exceptions
			case 0x102B:
			case 0x102C:
			case 0x1038:

			// 0x1062 .. 0x1064
			case 0x1062:
			case 0x1063:
			case 0x1064:

			// 0x1067 .. 0x106D
			case 0x1067:
			case 0x1068:
			case 0x1069:
			case 0x106A:
			case 0x106B:
			case 0x106C:
			case 0x106D:

			case 0x1083:

			// 0x1087 .. 0x108C
			case 0x1087:
			case 0x1088:
			case 0x1089:
			case 0x108A:
			case 0x108B:
			case 0x108C:

			case 0x108F:

			// 0x109A .. 0x109C
			case 0x109A:
			case 0x109B:
			case 0x109C:

			case 0x1A61:
			case 0x1A63:
			case 0x1A64:
			case 0xAA7B:
			case 0xAA7D:
			case 0x11720:
			case 0x11721:
				break;
			default:
				return X::SpacingMark;
		}
	}


	/* Other */

	return X::Other;
}


Unicode::WordBreak Unicode::get_word_break(codepoint_t codepoint) {
	// https://www.unicode.org/reports/tr29/

	using X = Unicode::WordBreak;

	switch (codepoint) {
		case 0x000D: return X::CR;
		case 0x000A: return X::LF;
		case 0x200D: return X::ZWJ;
		case 0x0027: return X::Single_Quote;
		case 0x0022: return X::Double_Quote;
		case 0x202F: return X::ExtendNumLet;

		case 0x000B:
		case 0x000C:
		case 0x0085:
		case 0x2028:
		case 0x2029: return X::Newline;

		case 0x002E:
		case 0x2018:
		case 0x2019:
		case 0x2024:
		case 0xFE52:
		case 0xFF07:
		case 0xFF0E: return X::MidNumLet;

		case 0x003A:
		case 0x00B7:
		case 0x0387:
		case 0x055F:
		case 0x05F4:
		case 0x2027:
		case 0xFE13:
		case 0xFE55:
		case 0xFF1A: return X::MidLetter;

		case 0x066C:
		case 0xFE50:
		case 0xFE54:
		case 0xFF0C:
		case 0xFF1B: return X::MidNum;

		case 0x3031:
		case 0x3032:
		case 0x3033:
		case 0x3034:
		case 0x3035:
		case 0x309B:
		case 0x309C:
		case 0x30A0:
		case 0x30FC:
		case 0xFF70: return X::Katakana;

		case 0xFF10:
		case 0xFF11:
		case 0xFF12:
		case 0xFF13:
		case 0xFF14:
		case 0xFF15:
		case 0xFF16:
		case 0xFF17:
		case 0xFF18:
		case 0xFF19: return X::Numeric;

		case 0x02C2:
		case 0x02C3:
		case 0x02C4:
		case 0x02C5:
		case 0x02D2:
		case 0x02D3:
		case 0x02D4:
		case 0x02D5:
		case 0x02D6:
		case 0x02D7:
		case 0x02DE:
		case 0x02DF:
		case 0x02E5:
		case 0x02E6:
		case 0x02E7:
		case 0x02E8:
		case 0x02E9:
		case 0x02EA:
		case 0x02EB:
		case 0x02ED:
		case 0x02EF:
		case 0x02F0:
		case 0x02F1:
		case 0x02F2:
		case 0x02F3:
		case 0x02F4:
		case 0x02F5:
		case 0x02F6:
		case 0x02F7:
		case 0x02F8:
		case 0x02F9:
		case 0x02FA:
		case 0x02FB:
		case 0x02FC:
		case 0x02FD:
		case 0x02FE:
		case 0x02FF:
		case 0x055A:
		case 0x055B:
		case 0x055C:
		case 0x055E:
		case 0x058A:
		case 0x05F3:
		case 0xA708:
		case 0xA709:
		case 0xA70A:
		case 0xA70B:
		case 0xA70C:
		case 0xA70D:
		case 0xA70E:
		case 0xA70F:
		case 0xA710:
		case 0xA711:
		case 0xA712:
		case 0xA713:
		case 0xA714:
		case 0xA715:
		case 0xA716:
		case 0xA720:
		case 0xA721:
		case 0xA789:
		case 0xA78A:
		case 0xAB5B: return X::ALetter;
	}

	if (Unicode::is_regional_indicator(codepoint)) {
		return X::Regional_Indicator;
	}

	using GC = Unicode::GeneralCategory;
	const GC gc = Unicode::get_general_category(codepoint);

	if (gc == GC::Zs) {
		return X::WSegSpace;
	}

	if (is_grapheme_extend(codepoint)
			|| gc == GC::Spacing_Mark
			|| is_emoji_modifier(codepoint)) {
		return X::Extend;
	}

	if (gc == GC::Connector_Punctuation) {
		return X::ExtendNumLet;
	}

	const auto script = Unicode::get_script(codepoint);

	if (script == Unicode::Script::Katakana) {
		return X::Katakana;
	}

	if (script == Unicode::Script::Hebrew
			&& gc == GC::Other_Letter) {
		return X::Hebrew_Letter;
	}

	const auto line_break = Unicode::get_line_break(codepoint);

	if (line_break == Unicode::LineBreak::Infix_Numeric) {
		return X::MidNum;
	}

	if (gc == GC::Zs && line_break != Unicode::LineBreak::Glue) {
		return X::WSegSpace;
	}

	if (is_alphabetic(codepoint) 
			&& !is_ideographic(codepoint)
			&& script != Unicode::Script::Hiragana
			&& line_break != Unicode::LineBreak::SA) {
		return X::ALetter;
	}

	return X::Other;
}


Unicode::SentenceBreak Unicode::get_sentence_break(codepoint_t codepoint) {
	// https://www.unicode.org/reports/tr29/

	using X = Unicode::SentenceBreak;

	switch (codepoint) {
		case 0x000D: return X::CR;
		case 0x000A: return X::LF;
		case 0x200D: return X::Extend;

		case 0x0085:
		case 0x2028:
		case 0x2029: return X::Sep;

		// according to tr29, U+00A0 (NBSP) should be OLetter, but the
		// UCD claims it should be space. I will go with UCD for now.
		//case 0x00A0:
		case 0x05F3: return X::OLetter;

		case 0xFF10:
		case 0xFF11:
		case 0xFF12:
		case 0xFF13:
		case 0xFF14:
		case 0xFF15:
		case 0xFF16:
		case 0xFF17:
		case 0xFF18:
		case 0xFF19: return X::Numeric;

		case 0x002E:
		case 0x2024:
		case 0xFE52:
		case 0xFF0E: return X::ATerm;

		case 0x002C:
		case 0x002D:
		case 0x003A:
		case 0x055D:
		case 0x060C:
		case 0x060D:
		case 0x07F8:
		case 0x1802:
		case 0x1808:
		case 0x2013:
		case 0x2014:
		case 0x3001:
		case 0xFE10:
		case 0xFE11:
		case 0xFE13:
		case 0xFE31:
		case 0xFE32:
		case 0xFE50:
		case 0xFE51:
		case 0xFE55:
		case 0xFE58:
		case 0xFE63:
		case 0xFF0C:
		case 0xFF0D:
		case 0xFF1A:
		case 0xFF64: return X::SContinue;
	}

	// special exception for these georgian letters
	if ((codepoint >= 0x10D0 && codepoint <= 0x10FA)
			|| (codepoint >= 0x10FD && codepoint <= 0x10FF)
			|| (codepoint >= 0x1C90 && codepoint <= 0x1CBA)
			|| (codepoint >= 0x1CBD && codepoint <= 0x1CBF)) {
		return X::OLetter;
	}

	if (Unicode::is_sentence_terminal(codepoint)) {
		return X::STerm;
	}

	if (Unicode::is_white_space(codepoint)) {
		return X::Sp;
	}

	const bool grapheme_extend = Unicode::is_grapheme_extend(codepoint);

	if (Unicode::is_lowercase(codepoint) && !grapheme_extend) {
		return X::Lower;
	}

	using GC = Unicode::GeneralCategory;
	const auto gc = Unicode::get_general_category(codepoint);

	if (gc == GC::Titlecase_Letter
			|| Unicode::is_uppercase(codepoint)) {
		return X::Upper;
	}

	// includes 0x200C
	if (grapheme_extend || gc == GC::Spacing_Mark) {
		return X::Extend;
	}

	// must come after Extend, Upper, Lower
	if (Unicode::is_alphabetic(codepoint)) {
		return X::OLetter;
	}

	// excludes 0x200C, 0x200D
	if (gc == GC::Format) {
		return X::Format;
	}

	const auto line_break = Unicode::get_line_break(codepoint);

	// must come after ATerm, STerm
	if (
			(gc == GC::Open_Punctuation
			|| gc == GC::Close_Punctuation
			|| line_break == Unicode::LineBreak::Quotation)
			&& codepoint != 0x05F3) {
		return X::Close;
	}

	if (line_break == Unicode::LineBreak::Numeric) {
		return X::Numeric;
	}

	return X::Other;
}

const char* Unicode::to_string(Unicode::GeneralCategory x) {
	using X = GeneralCategory;
	switch (x) {
		case X::Cc: return "Cc";
		case X::Cf: return "Cf";
		case X::Cn: return "Cn";
		case X::Co: return "Co";
		case X::Cs: return "Cs";
		case X::Ll: return "Ll";
		case X::Lm: return "Lm";
		case X::Lo: return "Lo";
		case X::Lt: return "Lt";
		case X::Lu: return "Lu";
		case X::Mc: return "Mc";
		case X::Me: return "Me";
		case X::Mn: return "Mn";
		case X::Nd: return "Nd";
		case X::Nl: return "Nl";
		case X::No: return "No";
		case X::Pc: return "Pc";
		case X::Pd: return "Pd";
		case X::Pe: return "Pe";
		case X::Pf: return "Pf";
		case X::Pi: return "Pi";
		case X::Po: return "Po";
		case X::Ps: return "Ps";
		case X::Sc: return "Sc";
		case X::Sk: return "Sk";
		case X::Sm: return "Sm";
		case X::So: return "So";
		case X::Zl: return "Zl";
		case X::Zp: return "Zp";
		case X::Zs: return "Zs";
		default: return "??";
	}
}

const char* Unicode::to_string(Unicode::HangulSyllableType x) {
	using X = HangulSyllableType;
	switch(x) {
		case X::NA:  return "N/A";
		case X::L:   return "L";
		case X::V:   return "V";
		case X::T:   return "T";
		case X::LV:  return "LV";
		case X::LVT: return "LVT";
		default: return "?";
	}
}

const char* Unicode::to_string(Unicode::IndicSyllabicCategory x) {
	using X = IndicSyllabicCategory;
	switch(x) {
		case X::NA:                          return "NA";
		case X::Avagraha:                    return "Avagraha";
		case X::Bindu:                       return "Bindu";
		case X::Brahmi_Joining_Number:       return "Brahmi_Joining_Number";
		case X::Cantillation_Mark:           return "Cantillation_Mark";
		case X::Consonant:                   return "Consonant";
		case X::Consonant_Dead:              return "Consonant_Dead";
		case X::Consonant_Final:             return "Consonant_Final";
		case X::Consonant_Head_Letter:       return "Consonant";
		case X::Consonant_Initial_Postfixed: return "Consonant_Initial_Postfixed";
		case X::Consonant_Killer:            return "Consonant_Killer";
		case X::Consonant_Medial:            return "Consonant_Medial";
		case X::Consonant_Placeholder:       return "Consonant_Placeholder";
		case X::Consonant_Preceding_Repha:   return "Consonant_Preceding_Repha";
		case X::Consonant_Prefixed:          return "Consonant_Prefixed";
		case X::Consonant_Subjoined:         return "Consonant_Subjoined";
		case X::Consonant_Succeeding_Repha:  return "Consonant_Succeeding_Repha";
		case X::Consonant_With_Stacker:      return "Consonant_With_Stacker";
		case X::Gemination_Mark:             return "Gemination_Mark";
		case X::Invisible_Stacker:           return "Invisible_Stacker";
		case X::Joiner:                      return "Joiner";
		case X::Modifying_Letter:            return "Modifying_Letter";
		case X::Non_Joiner:                  return "Non_Joiner";
		case X::Nukta:                       return "Nukta";
		case X::Number:                      return "Number";
		case X::Number_Joiner:               return "Number_Joiner";
		case X::Other:                       return "Other";
		case X::Pure_Killer:                 return "Pure_Killer";
		case X::Register_Shifter:            return "Register_Shifter";
		case X::Syllable_Modifier:           return "Syllable_Modifier";
		case X::Tone_Letter:                 return "Tone_Letter";
		case X::Tone_Mark:                   return "Tone_Mark";
		case X::Virama:                      return "Virama";
		case X::Visarga:                     return "Visarga";
		case X::Vowel:                       return "Vowel";
		case X::Vowel_Dependent:             return "Vowel_Dependent";
		case X::Vowel_Independent:           return "Vowel_Independent";
		default: return "???";
	}
};

const char* Unicode::to_string(Unicode::EastAsianWidth x) {
	using X = Unicode::EastAsianWidth;
	switch (x) {
		case X::A:  return "A";
		case X::F:  return "F";
		case X::H:  return "H";
		case X::N:  return "N";
		case X::Na: return "Na";
		case X::W:  return "W";
		default: return "?";
	}
};

const char* Unicode::to_string(Unicode::GraphemeClusterBreak x) {
	using X = Unicode::GraphemeClusterBreak;
	switch (x) {
		case X::Other:       return "Other";
		case X::CR:          return "CR";
		case X::LF:          return "LF";
		case X::Control:     return "Control";
		case X::Extend:      return "Extend";
		case X::ZWJ:         return "ZWJ";
		case X::Regional_Indicator: return "Regional_Indicator";
		case X::Prepend:     return "Prepend";
		case X::SpacingMark: return "SpacingMark";
		case X::L:           return "L";
		case X::V:           return "V";
		case X::T:           return "T";
		case X::LV:          return "LV";
		case X::LVT:         return "LVT";
		default: return "???";
	}
}

const char* Unicode::to_string(Unicode::WordBreak x) {
	using X = WordBreak;
	switch (x) {
		case X::Other:              return "Other";
		case X::CR:                 return "CR";
		case X::LF:                 return "LF";
		case X::Newline:            return "Newline";
		case X::Extend:             return "Extend";
		case X::ZWJ:                return "ZWJ";
		case X::Regional_Indicator: return "Regional_Indicator";
		case X::Format:             return "Format";
		case X::Katakana:           return "Katakana";
		case X::Hebrew_Letter:      return "Hebrew_Letter";
		case X::ALetter:            return "ALetter";
		case X::Single_Quote:       return "Single_Quote";
		case X::Double_Quote:       return "Double_Quote";
		case X::MidNumLet:          return "MidNumLet";
		case X::MidLetter:          return "MidLetter";
		case X::MidNum:             return "MidNum";
		case X::Numeric:            return "Numeric";
		case X::ExtendNumLet:       return "ExtendNumLet";
		case X::WSegSpace:          return "WSegSpace";
		default: return "???";
	}
}

const char* Unicode::to_string(Unicode::LineBreak x) {
	using X = Unicode::LineBreak;
	switch (x) {
		case X::XX:  return "XX";
		case X::BK:  return "BK";
		case X::CR:  return "CR";
		case X::LF:  return "LF";
		case X::CM:  return "CM";
		case X::NL:  return "NL";
		case X::SG:  return "SG";
		case X::WJ:  return "WJ";
		case X::ZW:  return "ZW";
		case X::GL:  return "GL";
		case X::SP:  return "SP";
		case X::ZWJ: return "ZWJ";
		case X::B2:  return "B2"; 
		case X::BA:  return "BA";
		case X::BB:  return "BB";
		case X::HY:  return "HY";
		case X::CB:  return "CB";
		case X::CL:  return "CL";
		case X::CP:  return "CP";
		case X::EX:  return "EX";
		case X::IN:  return "IN";
		case X::NS:  return "NS";
		case X::OP:  return "OP";
		case X::QU:  return "QU";
		case X::IS:  return "IS";
		case X::NU:  return "NU";
		case X::PO:  return "PO";
		case X::PR:  return "PR";
		case X::SY:  return "SY";
		case X::AI:  return "AI";
		case X::AL:  return "AL";
		case X::CJ:  return "CJ";
		case X::EB:  return "EB";
		case X::EM:  return "EM";
		case X::H2:  return "H2";
		case X::H3:  return "H3";
		case X::HL:  return "HL";
		case X::ID:  return "ID";
		case X::JL:  return "JL";
		case X::JV:  return "JV";
		case X::JT:  return "JT";
		case X::RI:  return "RI";
		case X::SA:  return "SA";
		default: return "??";
	}
}

const char* Unicode::to_string(Unicode::Script x) {
	using X = Unicode::Script;
	switch(x) {
		case X::Unknown:                return "Unknown";
		case X::Common:                 return "Common";
		case X::Latin:                  return "Latin";
		case X::Greek:                  return "Greek";
		case X::Cyrillic:               return "Cyrillic";
		case X::Armenian:               return "Armenian";
		case X::Hebrew:                 return "Hebrew";
		case X::Arabic:                 return "Arabic";
		case X::Syriac:                 return "Syriac";
		case X::Thaana:                 return "Thaana";
		case X::Devanagari:             return "Devanagari";
		case X::Bengali:                return "Bengali";
		case X::Gurmukhi:               return "Gurmukhi";
		case X::Gujarati:               return "Gujarati";
		case X::Oriya:                  return "Oriya";
		case X::Tamil:                  return "Tamil";
		case X::Telugu:                 return "Telugu";
		case X::Kannada:                return "Kannada";
		case X::Malayalam:              return "Malayalam";
		case X::Sinhala:                return "Sinhala";
		case X::Thai:                   return "Thai";
		case X::Lao:                    return "Lao";
		case X::Tibetan:                return "Tibetan";
		case X::Myanmar:                return "Myanmar";
		case X::Georgian:               return "Georgian";
		case X::Hangul:                 return "Hangul";
		case X::Ethiopic:               return "Ethiopic";
		case X::Cherokee:               return "Cherokee";
		case X::Canadian_Aboriginal:    return "Canadian_Aboriginal";
		case X::Ogham:                  return "Ogham";
		case X::Runic:                  return "Runic";
		case X::Khmer:                  return "Khmer";
		case X::Mongolian:              return "Mongolian";
		case X::Hiragana:               return "Hiragana";
		case X::Katakana:               return "Katakana";
		case X::Bopomofo:               return "Bopomofo";
		case X::Han:                    return "Han";
		case X::Yi:                     return "Yi";
		case X::Old_Italic:             return "Old_Italic";
		case X::Gothic:                 return "Gothic";
		case X::Deseret:                return "Deseret";
		case X::Inherited:              return "Inherited";
		case X::Tagalog:                return "Tagalog";
		case X::Hanunoo:                return "Hanunoo";
		case X::Buhid:                  return "Buhid";
		case X::Tagbanwa:               return "Tagbanwa";
		case X::Limbu:                  return "Limbu";
		case X::Tai_Le:                 return "Tai_Le";
		case X::Linear_B:               return "Linear_B";
		case X::Ugaritic:               return "Ugaritic";
		case X::Shavian:                return "Shavian";
		case X::Osmanya:                return "Osmanya";
		case X::Cypriot:                return "Cypriot";
		case X::Braille:                return "Braille";
		case X::Buginese:               return "Buginese";
		case X::Coptic:                 return "Coptic";
		case X::New_Tai_Lue:            return "New_Tai_Lue";
		case X::Glagolitic:             return "Glagolitic";
		case X::Tifinagh:               return "Tifinagh";
		case X::Syloti_Nagri:           return "Syloti_Nagri";
		case X::Old_Persian:            return "Old_Persian";
		case X::Kharoshthi:             return "Kharoshthi";
		case X::Balinese:               return "Balinese";
		case X::Cuneiform:              return "Cuneiform";
		case X::Phoenician:             return "Phoenician";
		case X::Phags_Pa:               return "Phags_Pa";
		case X::Nko:                    return "Nko";
		case X::Sundanese:              return "Sundanese";
		case X::Lepcha:                 return "Lepcha";
		case X::Ol_Chiki:               return "Ol_Chiki";
		case X::Vai:                    return "Vai";
		case X::Saurashtra:             return "Saurashtra";
		case X::Kayah_Li:               return "Kayah_Li";
		case X::Rejang:                 return "Rejang";
		case X::Lycian:                 return "Lycian";
		case X::Carian:                 return "Carian";
		case X::Lydian:                 return "Lydian";
		case X::Cham:                   return "Cham";
		case X::Tai_Tham:               return "Tai_Tham";
		case X::Tai_Viet:               return "Tai_Viet";
		case X::Avestan:                return "Avestan";
		case X::Egyptian_Hieroglyphs:   return "Egyptian_Hieroglyphs";
		case X::Samaritan:              return "Samaritan";
		case X::Lisu:                   return "Lisu";
		case X::Bamum:                  return "Bamum";
		case X::Javanese:               return "Javanese";
		case X::Meetei_Mayek:           return "Meetei_Mayek";
		case X::Imperial_Aramaic:       return "Imperial_Aramaic";
		case X::Old_South_Arabian:      return "Old_South_Arabian";
		case X::Inscriptional_Parthian: return "Inscriptional_Parthian";
		case X::Inscriptional_Pahlavi:  return "Inscriptional_Pahlavi";
		case X::Old_Turkic:             return "Old_Turkic";
		case X::Kaithi:                 return "Kaithi";
		case X::Batak:                  return "Batak";
		case X::Brahmi:                 return "Brahmi";
		case X::Mandaic:                return "Mandaic";
		case X::Chakma:                 return "Chakma";
		case X::Meroitic_Cursive:       return "Meroitic_Cursive";
		case X::Meroitic_Hieroglyphs:   return "Meroitic_Hieroglyphs";
		case X::Miao:                   return "Miao";
		case X::Sharada:                return "Sharada";
		case X::Sora_Sompeng:           return "Sora_Sompeng";
		case X::Takri:                  return "Takri";
		case X::Caucasian_Albanian:     return "Caucasian_Albanian";
		case X::Bassa_Vah:              return "Bassa_Vah";
		case X::Duployan:               return "Duployan";
		case X::Elbasan:                return "Elbasan";
		case X::Grantha:                return "Grantha";
		case X::Pahawh_Hmong:           return "Pahawh_Hmong";
		case X::Khojki:                 return "Khojki";
		case X::Linear_A:               return "Linear_A";
		case X::Mahajani:               return "Mahajani";
		case X::Manichaean:             return "Manichaean";
		case X::Mende_Kikakui:          return "Mende_Kikakui";
		case X::Modi:                   return "Modi";
		case X::Mro:                    return "Mro";
		case X::Old_North_Arabian:      return "Old_North_Arabian";
		case X::Nabataean:              return "Nabataean";
		case X::Palmyrene:              return "Palmyrene";
		case X::Pau_Cin_Hau:            return "Pau_Cin_Hau";
		case X::Old_Permic:             return "Old_Permic";
		case X::Psalter_Pahlavi:        return "Psalter_Pahlavi";
		case X::Siddham:                return "Siddham";
		case X::Khudawadi:              return "Khudawadi";
		case X::Tirhuta:                return "Tirhuta";
		case X::Warang_Citi:            return "Warang_Citi";
		case X::Ahom:                   return "Ahom";
		case X::Anatolian_Hieroglyphs:  return "Anatolian_Hieroglyphs";
		case X::Hatran:                 return "Hatran";
		case X::Multani:                return "Multani";
		case X::Old_Hungarian:          return "Old_Hungarian";
		case X::SignWriting:            return "SignWriting";
		case X::Adlam:                  return "Adlam";
		case X::Bhaiksuki:              return "Bhaiksuki";
		case X::Marchen:                return "Marchen";
		case X::Newa:                   return "Newa";
		case X::Osage:                  return "Osage";
		case X::Tangut:                 return "Tangut";
		case X::Masaram_Gondi:          return "Masaram_Gondi";
		case X::Nushu:                  return "Nushu";
		case X::Soyombo:                return "Soyombo";
		case X::Zanabazar_Square:       return "Zanabazar_Square";
		case X::Dogra:                  return "Dogra";
		case X::Gunjala_Gondi:          return "Gunjala_Gondi";
		case X::Makasar:                return "Makasar";
		case X::Medefaidrin:            return "Medefaidrin";
		case X::Hanifi_Rohingya:        return "Hanifi_Rohingya";
		case X::Sogdian:                return "Sogdian";
		case X::Old_Sogdian:            return "Old_Sogdian";
		case X::Elymaic:                return "Elymaic";
		case X::Nandinagari:            return "Nandinagari";
		case X::Nyiakeng_Puachue_Hmong: return "Nyiakeng_Puachue_Hmong";
		case X::Wancho:                 return "Wancho";
		case X::Chorasmian:             return "Chorasmian";
		case X::Dives_Akuru:            return "Dives_Akuru";
		case X::Khitan_Small_Script:    return "Khitan_Small_Script";
		case X::Yezidi:                 return "Yezidi";
		case X::Cypro_Minoan:           return "Cypro_Minoan";
		case X::Old_Uyghur:             return "Old_Uyghur";
		case X::Tangsa:                 return "Tangsa";
		case X::Toto:                   return "Toto";
		case X::Vithkuqi:               return "Vithkuqi";
		default: return "???";
	}
}

const char* Unicode::to_string(Unicode::SentenceBreak x) {
	using X = Unicode::SentenceBreak;
	switch(x) {
		case X::Other:     return "Other";
		case X::CR:        return "CR";
		case X::LF:        return "LF";
		case X::Extend:    return "Extend";
		case X::Sep:       return "Sep";
		case X::Format:    return "Format";
		case X::Sp:        return "Sp";
		case X::Lower:     return "Lower";
		case X::Upper:     return "Upper";
		case X::OLetter:   return "OLetter";
		case X::Numeric:   return "Numeric";
		case X::ATerm:     return "ATerm";
		case X::SContinue: return "SContinue";
		case X::STerm:     return "STerm";
		case X::Close:     return "Close";
		default: return "???";
	}
};

