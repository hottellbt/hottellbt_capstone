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
