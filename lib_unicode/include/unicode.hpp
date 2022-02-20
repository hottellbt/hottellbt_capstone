#ifndef INCLUDED_UNICODE_HPP
#define INCLUDED_UNICODE_HPP

#include <cstdint>
#include <string>

namespace Unicode {

	// an integer type large enough to hold all possible codepoints
	using codepoint_t = char32_t;
	using string_t = std::u32string;

	const codepoint_t MAX_CODEPOINT = 0x10FFFF;


	/***** General Categories *****/

	enum class GeneralCategory : uint8_t {
		Cc, Cf, Cn, Co, Cs,         // other
		Ll, Lm, Lo, Lt, Lu,         // letter
		Mc, Me, Mn,                 // mark
		Nd, Nl, No,                 // number
		Pc, Pd, Pe, Pf, Pi, Po, Ps, // punctuation
		Sc, Sk, Sm, So,             // symbol
		Zl, Zp, Zs,                 // separator

		Control     = Cc,
		Format      = Cf,
		Unassigned  = Cn,
		Private_Use = Co,
		Surrogate   = Cs,

		Lowercase_Letter = Ll,
		Modifier_Letter  = Lm,
		Other_Letter     = Lo,
		Titlecase_Letter = Lt,
		Uppercase_Letter = Lu,

		Spacing_Mark    = Mc,
		Enclosing_Mark  = Me,
		Nonspacing_Mark = Mn,

		Decimal_Number = Nd,
		Letter_Number  = Nl,
		Other_Number   = No,

		Connector_Punctuation = Pc,
		Dash_Punctuation      = Pd,
		Close_Punctuation     = Pe,
		Final_Punctuation     = Pf,
		Initial_Punctuation   = Pi,
		Other_Punctuation     = Po,
		Open_Punctuation      = Ps,

		Currency_Symbol = Sc,
		Modifier_Symbol = Sk,
		Math_Symbol     = Sm,
		Other_Symbol    = So,

		Line_Separator      = Zl,
		Paragraph_Separator = Zp,
		Space_Separator     = Zs,
	};

	inline bool is_other(GeneralCategory x) {
		return x == GeneralCategory::Cc
			|| x == GeneralCategory::Cf
			|| x == GeneralCategory::Cs
			|| x == GeneralCategory::Co
			|| x == GeneralCategory::Cn;
	}

	inline bool is_letter(GeneralCategory x) {
		return x == GeneralCategory::Ll
			|| x == GeneralCategory::Lm
			|| x == GeneralCategory::Lo
			|| x == GeneralCategory::Lt
			|| x == GeneralCategory::Lu;
	}

	inline bool is_cased_letter(GeneralCategory x) {
		return x == GeneralCategory::Ll
			|| x == GeneralCategory::Lt
			|| x == GeneralCategory::Lu;
	}

	// AKA is_combining_mark
	// AKA is_combining_character
	// Per D52, its all the same
	inline bool is_mark(GeneralCategory x) {
		return x == GeneralCategory::Mc
			|| x == GeneralCategory::Me
			|| x == GeneralCategory::Mn;
	}

	inline bool is_number(GeneralCategory x) {
		return x == GeneralCategory::Nd
			|| x == GeneralCategory::Nl
			|| x == GeneralCategory::No;
	}

	inline bool is_punctuation(GeneralCategory x) {
		return x == GeneralCategory::Pc
			|| x == GeneralCategory::Pd
			|| x == GeneralCategory::Pe
			|| x == GeneralCategory::Pf
			|| x == GeneralCategory::Pi
			|| x == GeneralCategory::Po
			|| x == GeneralCategory::Ps;
	}

	inline bool is_symbol(GeneralCategory x) {
		return x == GeneralCategory::Sc
			|| x == GeneralCategory::Sk
			|| x == GeneralCategory::Sm
			|| x == GeneralCategory::So;
	}

	inline bool is_separator(GeneralCategory x) {
		return x == GeneralCategory::Zl
			|| x == GeneralCategory::Zp
			|| x == GeneralCategory::Zs;
	}

	GeneralCategory get_general_category(codepoint_t codepoint);

	inline bool is_other        (codepoint_t x) { return is_other        (get_general_category(x)); }
	inline bool is_letter       (codepoint_t x) { return is_letter       (get_general_category(x)); }
	inline bool is_cased_letter (codepoint_t x) { return is_cased_letter (get_general_category(x)); }
	inline bool is_mark         (codepoint_t x) { return is_mark         (get_general_category(x)); }
	inline bool is_number       (codepoint_t x) { return is_number       (get_general_category(x)); }
	inline bool is_punctuation  (codepoint_t x) { return is_punctuation  (get_general_category(x)); }
	inline bool is_symbol       (codepoint_t x) { return is_symbol       (get_general_category(x)); }
	inline bool is_separator    (codepoint_t x) { return is_separator    (get_general_category(x)); }


	/***** Hangul *****/

	enum class HangulSyllableType : uint8_t {
		NA,
		L,
		V,
		T,
		LV,
		LVT
	};

	HangulSyllableType get_hangul_syllable_type(codepoint_t codepoint);


	/***** Indic Syllabic Categories *****/

	enum class IndicSyllabicCategory : uint8_t {
		NA,
		Avagraha,
		Bindu,
		Brahmi_Joining_Number,
		Cantillation_Mark,
		Consonant,
		Consonant_Dead,
		Consonant_Final,
		Consonant_Head_Letter,
		Consonant_Initial_Postfixed,
		Consonant_Killer,
		Consonant_Medial,
		Consonant_Placeholder,
		Consonant_Preceding_Repha,
		Consonant_Prefixed,
		Consonant_Subjoined,
		Consonant_Succeeding_Repha,
		Consonant_With_Stacker,
		Gemination_Mark,
		Invisible_Stacker,
		Joiner,
		Modifying_Letter,
		Non_Joiner,
		Nukta,
		Number,
		Number_Joiner,
		Other,
		Pure_Killer,
		Register_Shifter,
		Syllable_Modifier,
		Tone_Letter,
		Tone_Mark,
		Virama,
		Visarga,
		Vowel,
		Vowel_Dependent,
		Vowel_Independent
	};

	IndicSyllabicCategory get_indic_syllabic_category(codepoint_t codepoint);


	/***** Grapheme (user-perceived character) *****/

	enum class GraphemeClusterBreak : uint8_t {
		Other, // placeholder for "no property"
		CR,
		LF,
		Control,
		Extend,
		ZWJ,
		Regional_Indicator,
		Prepend,
		SpacingMark,
		L,
		V,
		T,
		LV,
		LVT
	};

	GraphemeClusterBreak get_grapheme_cluster_break(codepoint_t codepoint);


	/***** Word_Break *****/

	// https://www.unicode.org/reports/tr29/tr29-37.html#Word_Boundaries

	enum class WordBreak : uint8_t {
		Other, // placeholder for "no property"
		CR,
		LF,
		Newline,
		Extend,
		ZWJ,
		Regional_Indicator,
		Format,
		Katakana,
		Hebrew_Letter,
		ALetter,
		Single_Quote,
		Double_Quote,
		MidNumLet,
		MidLetter,
		MidNum,
		Numeric,
		ExtendNumLet,
		WSegSpace
	};

	WordBreak get_word_break(codepoint_t codepoint);


	/***** Sentence Breaking *****/

	enum class SentenceBreak : uint8_t {
		Other, // placeholder
		CR,
		LF,
		Extend,
		Sep,
		Format,
		Sp,
		Lower,
		Upper,
		OLetter,
		Numeric,
		ATerm,
		SContinue,
		STerm,
		Close
	};

	SentenceBreak get_sentence_break(codepoint_t codepoint);


	/***** Line Breaking *****/

	// https://www.unicode.org/reports/tr14/#Table1
	enum class LineBreak : uint8_t {
		XX, // Unknown, unassigned

		// non-tailorable line breaking classes
		BK, CR, LF, CM, NL, SG, WJ, ZW, GL, SP, ZWJ,

		// break opportunities
		B2, BA, BB, HY, CB,

		// characters prohibiting certain breaks
		CL, CP, EX, IN, NS, OP, QU,

		// numeric context
		IS, NU, PO, PR, SY,

		// other characters
		AI, AL, CJ, EB, EM, H2, H3, HL, ID, JL, JV, JT, RI, SA,

		// aliases
		Mandatory_Break              = BK,
		Carriage_Return              = CR,
		Line_Feed                    = LF,
		Combining_Mark               = CM,
		Next_Line                    = NL,
		Surrogate                    = SG,
		Word_Joiner                  = WJ,
		Zero_Width_Space             = ZW,
		Glue                         = GL, // aka "Non-breaking"
		Zero_Width_Joiner            = ZWJ,
		Break_Opportunity_Before_and_After = B2,
		Break_After                  = BA,
		Break_Before                 = BB,
		Hyphen                       = HY,
		Contingent_Break_Opportunity = CB,
		Close_Punctuation            = CL,
		Close_Parenthesis            = CP,
		Exclamation_Interrogation    = EX,
		Inseperable                  = IN,
		Nonstarter                   = NS,
		Open_Punctuation             = OP,
		Quotation                    = QU,
		Infix_Numeric_Separator      = IS,
		Infix_Numeric                = IS, // unicode is inconsistent
		Numeric                      = NU,
		Postfix_Numeric              = PO,
		Prefix_Numeric               = PR,
		Symbols_Allowing_Break_After = SY,
		Ambiguous                    = AI,
		Alphabetic                   = AL,
		Conditional_Japanese_Starter = CJ,
		Emoji_Base                   = EB,
		Emoji_Modifier               = EM,
		Hangul_LV_Syllable           = H2,
		Hangul_LVT_Syllable          = H3,
		Hebrew_Letter                = HL,
		Ideographic                  = ID,
		Hangul_L_Jamo                = JL,
		Hangul_V_Jamo                = JV,
		Hangul_T_Jamo                = JT,
		Regional_Indicator           = RI,
		Complex_Content_Dependent    = SA,
		Unknown                      = XX
	};

	LineBreak get_line_break(codepoint_t codepoint);


	/***** Properties *****/

	// these are listed in no particular order
	bool is_white_space                        (codepoint_t x);
	bool is_bidi_control                       (codepoint_t x);
	bool is_join_control                       (codepoint_t x);
	bool is_dash                               (codepoint_t x);
	bool is_hyphen                             (codepoint_t x);
	bool is_quotation_mark                     (codepoint_t x);
	bool is_terminal_punctuation               (codepoint_t x);
	bool is_other_math                         (codepoint_t x);
	bool is_hex_digit                          (codepoint_t x);
	bool is_other_alphabetic                   (codepoint_t x);
	bool is_ascii_hex_digit                    (codepoint_t x);
	bool is_ideographic                        (codepoint_t x);
	bool is_diacritic                          (codepoint_t x);
	bool is_extender                           (codepoint_t x);
	bool is_other_lowercase                    (codepoint_t x);
	bool is_other_uppercase                    (codepoint_t x);
	bool is_noncharacter_code_point            (codepoint_t x);
	bool is_other_grapheme_extend              (codepoint_t x);
	bool is_ids_binary_operator                (codepoint_t x);
	bool is_ids_trinary_operator               (codepoint_t x);
	bool is_radical                            (codepoint_t x);
	bool is_unified_ideograph                  (codepoint_t x);
	bool is_deprecated                         (codepoint_t x);
	bool is_soft_dotted                        (codepoint_t x);
	bool is_logical_order_exception            (codepoint_t x);
	bool is_other_id_start                     (codepoint_t x);
	bool is_other_id_continue                  (codepoint_t x);
	bool is_sentence_terminal                  (codepoint_t x);
	bool is_variation_selector                 (codepoint_t x);
	bool is_pattern_white_space                (codepoint_t x);
	bool is_pattern_syntax                     (codepoint_t x);
	bool is_prepended_concatenation_mark       (codepoint_t x);
	bool is_other_default_ignorable_code_point (codepoint_t x);
	bool is_regional_indicator                 (codepoint_t x);

	bool is_emoji                 (codepoint_t x);
	bool is_emoji_presentation    (codepoint_t x);
	bool is_emoji_modifier        (codepoint_t x);
	bool is_emoji_modifier_base   (codepoint_t x);
	bool is_emoji_component       (codepoint_t x);
	bool is_extended_pictographic (codepoint_t x);

	// derived properties
	// they can be derived from the properties above
	bool is_math                         (codepoint_t x);
	bool is_alphabetic                   (codepoint_t x);
	bool is_lowercase                    (codepoint_t x);
	bool is_uppercase                    (codepoint_t x);
	bool is_cased                        (codepoint_t x);
	bool is_case_ignorable               (codepoint_t x);
	bool changes_when_lowercased         (codepoint_t x);
	bool changes_when_uppercased         (codepoint_t x);
	bool changes_when_titlecased         (codepoint_t x);
	bool changes_when_casefolded         (codepoint_t x);
	bool changes_when_casemapped         (codepoint_t x);
	bool is_id_start                     (codepoint_t x);
	bool is_id_continue                  (codepoint_t x);
	bool is_xid_start                    (codepoint_t x);
	bool is_xid_continue                 (codepoint_t x);
	bool is_default_ignorable_code_point (codepoint_t x);
	bool is_grapheme_extended            (codepoint_t x);
	bool is_grapheme_base                (codepoint_t x);
	bool is_grapheme_link                (codepoint_t x);

	// implementations of derived properties

	inline bool is_math(codepoint_t x) {
		const auto g = get_general_category(x);
		return g == GeneralCategory::Sm
			|| is_other_math(x);
	}

	inline bool is_alphabetic(codepoint_t x) {
		const auto g = get_general_category(x);
		return is_uppercase(x)
			|| is_lowercase(x)
			|| g == GeneralCategory::Lt
			|| g == GeneralCategory::Lm
			|| g == GeneralCategory::Lo
			|| g == GeneralCategory::Nl
			|| is_other_alphabetic(x);
	}

	inline bool is_lowercase(codepoint_t x) {
		const auto g = get_general_category(x);
		return g == GeneralCategory::Ll
			|| is_other_lowercase(x);
	}

	inline bool is_uppercase(codepoint_t x) {
		const auto g = get_general_category(x);
		return g == GeneralCategory::Lu
			|| is_other_uppercase(x);
	}

	// D135
	inline bool is_cased(codepoint_t x) {
		const auto g = get_general_category(x);
		return is_lowercase(x)
			|| is_uppercase(x)
			|| g == GeneralCategory::Titlecase_Letter;
	}

	// D136
	inline bool is_case_ignorable(codepoint_t x) {
		const auto g = get_general_category(x);
		if (g == GeneralCategory::Mn
				|| g == GeneralCategory::Me
				|| g == GeneralCategory::Cf
				|| g == GeneralCategory::Lm
				|| g == GeneralCategory::Sk) {
			return true;
		}

		const auto wb = get_word_break(x);
		return wb == WordBreak::MidLetter
			|| wb == WordBreak::MidNumLet
			|| wb == WordBreak::Single_Quote;
	}

	// D139
	inline bool changes_when_lowercased(codepoint_t x) {
		// TODO true when toLowercase(toNFD(x)) != toNFD(x)
		return false;
	}

	// D140
	inline bool changes_when_uppercased(codepoint_t x) {
		// TODO true when toUpperCase(toNFD(x)) != toNFD(x)
		return false;
	}

	// D141
	inline bool changes_when_titlecased(codepoint_t x) {
		// TODO true when toTitlecase(toNFD(x)) != toNFD(x)
		return false;
	}

	// D142
	inline bool changes_when_casefolded(codepoint_t x) {
		// TODO true when toCasefold(toNFD(x)) != toNFD(x)
		return false;
	}

	// D143
	inline bool changes_when_casemapped(codepoint_t x) {
		return changes_when_lowercased(x)
			|| changes_when_uppercased(x)
			|| changes_when_titlecased(x);
	}

	// UAX #31
	inline bool is_id_start(codepoint_t x) {
		const auto g = get_general_category(x);
		return (g == GeneralCategory::Lu
				|| g == GeneralCategory::Ll
				|| g == GeneralCategory::Lt
				|| g == GeneralCategory::Lm
				|| g == GeneralCategory::Lo
				|| g == GeneralCategory::Nl
				|| is_other_id_start(x))
			&& !is_pattern_syntax(x)
			&& !is_pattern_white_space(x);
	}

	// UAX #31
	inline bool is_id_continue(codepoint_t x) {
		const auto g = get_general_category(x);
		return (g == GeneralCategory::Mn
				|| g == GeneralCategory::Mc
				|| g == GeneralCategory::Nd
				|| g == GeneralCategory::Pc
				|| is_other_id_continue(x))
			&& !is_pattern_syntax(x)
			&& !is_pattern_white_space(x);
	}

	// UAX #15
	inline bool is_xid_start(codepoint_t x) {
		return false; // TODO
	}

	// UAX #15
	inline bool is_xid_continue(codepoint_t x) {
		return false; // TODO
	}

	inline bool is_default_ignorable_code_point(codepoint_t x) {
		const auto g = get_general_category(x);
		return (is_other_default_ignorable_code_point(x)
				|| g == GeneralCategory::Cf
				|| is_variation_selector(x))
			&& !is_white_space(x)
			&& !(x >= 0xFFF9 && x <= 0xFFFB) // interlinear annotation format
			&& !(x >= 0x13430 && x <= 0x13438) // egyptian hieroglyph format
			&& !is_prepended_concatenation_mark(x);
	}

	inline bool is_grapheme_extend(codepoint_t x) {
		const auto g = get_general_category(x);
		return g == GeneralCategory::Me
			|| g == GeneralCategory::Mn
			|| is_other_grapheme_extend(x);
	}

	inline bool is_grapheme_base(codepoint_t x) {
		const auto g = get_general_category(x);
		return (x <= 0x10FFFF)
			&& g != GeneralCategory::Cc
			&& g != GeneralCategory::Cf
			&& g != GeneralCategory::Cs
			&& g != GeneralCategory::Co
			&& g != GeneralCategory::Cn
			&& g != GeneralCategory::Zl
			&& g != GeneralCategory::Zp
			&& !is_grapheme_extend(x);
	}

	inline bool is_grapheme_link(codepoint_t x) {
		// TODO return Canonical_Combining_Class=Virama
		return false;
	}

	// D49
	inline bool is_private_use(codepoint_t x) {
		return (x >= 0xE000 && x <= 0xF8FF)
			|| (x >= 0xF0000 && x <= 0xFFFFD)
			|| (x >= 0x100000 && x <= 0x10FFFD);
	}

	// D50
	// private-use characters (Co) are not included
	inline bool is_graphic_character(GeneralCategory x) {
		return is_letter(x)
			|| is_mark(x)
			|| is_number(x)
			|| is_punctuation(x)
			|| is_symbol(x)
			|| x == GeneralCategory::Zs;
	}
	inline bool is_graphic_character(codepoint_t x) {
		return is_graphic_character(get_general_category(x));
	}

	// D51
	inline bool is_base_character(GeneralCategory x) {
		return is_graphic_character(x) && !is_mark(x);
	}
	inline bool is_base_character(codepoint_t x) {
		return is_base_character(get_general_category(x));
	}


	/***** East Asian Width *****/

	// contrary to the name, all characters have an East_Asian_Width property

	enum class EastAsianWidth : uint8_t {
		A,
		F,
		H,
		N,
		Na,
		W,

		Ambiguous = A,
		Fullwidth = F,
		Halfwidth = H,
		Neutral   = N,
		Narrow    = Na,
		Wide      = W,
	};

	EastAsianWidth get_east_asian_width(codepoint_t x);


	/***** Scripts *****/

	enum class Script : uint8_t {
		Unknown,
		Common,
		Latin,
		Greek,
		Cyrillic,
		Armenian,
		Hebrew,
		Arabic,
		Syriac,
		Thaana,
		Devanagari,
		Bengali,
		Gurmukhi,
		Gujarati,
		Oriya,
		Tamil,
		Telugu,
		Kannada,
		Malayalam,
		Sinhala,
		Thai,
		Lao,
		Tibetan,
		Myanmar,
		Georgian,
		Hangul,
		Ethiopic,
		Cherokee,
		Canadian_Aboriginal,
		Ogham,
		Runic,
		Khmer,
		Mongolian,
		Hiragana,
		Katakana,
		Bopomofo,
		Han,
		Yi,
		Old_Italic,
		Gothic,
		Deseret,
		Inherited,
		Tagalog,
		Hanunoo,
		Buhid,
		Tagbanwa,
		Limbu,
		Tai_Le,
		Linear_B,
		Ugaritic,
		Shavian,
		Osmanya,
		Cypriot,
		Braille,
		Buginese,
		Coptic,
		New_Tai_Lue,
		Glagolitic,
		Tifinagh,
		Syloti_Nagri,
		Old_Persian,
		Kharoshthi,
		Balinese,
		Cuneiform,
		Phoenician,
		Phags_Pa,
		Nko,
		Sundanese,
		Lepcha,
		Ol_Chiki,
		Vai,
		Saurashtra,
		Kayah_Li,
		Rejang,
		Lycian,
		Carian,
		Lydian,
		Cham,
		Tai_Tham,
		Tai_Viet,
		Avestan,
		Egyptian_Hieroglyphs,
		Samaritan,
		Lisu,
		Bamum,
		Javanese,
		Meetei_Mayek,
		Imperial_Aramaic,
		Old_South_Arabian,
		Inscriptional_Parthian,
		Inscriptional_Pahlavi,
		Old_Turkic,
		Kaithi,
		Batak,
		Brahmi,
		Mandaic,
		Chakma,
		Meroitic_Cursive,
		Meroitic_Hieroglyphs,
		Miao,
		Sharada,
		Sora_Sompeng,
		Takri,
		Caucasian_Albanian,
		Bassa_Vah,
		Duployan,
		Elbasan,
		Grantha,
		Pahawh_Hmong,
		Khojki,
		Linear_A,
		Mahajani,
		Manichaean,
		Mende_Kikakui,
		Modi,
		Mro,
		Old_North_Arabian,
		Nabataean,
		Palmyrene,
		Pau_Cin_Hau,
		Old_Permic,
		Psalter_Pahlavi,
		Siddham,
		Khudawadi,
		Tirhuta,
		Warang_Citi,
		Ahom,
		Anatolian_Hieroglyphs,
		Hatran,
		Multani,
		Old_Hungarian,
		SignWriting,
		Adlam,
		Bhaiksuki,
		Marchen,
		Newa,
		Osage,
		Tangut,
		Masaram_Gondi,
		Nushu,
		Soyombo,
		Zanabazar_Square,
		Dogra,
		Gunjala_Gondi,
		Makasar,
		Medefaidrin,
		Hanifi_Rohingya,
		Sogdian,
		Old_Sogdian,
		Elymaic,
		Nandinagari,
		Nyiakeng_Puachue_Hmong,
		Wancho,
		Chorasmian,
		Dives_Akuru,
		Khitan_Small_Script,
		Yezidi,
		Cypro_Minoan,
		Old_Uyghur,
		Tangsa,
		Toto,
		Vithkuqi,
	};

	Script get_script(codepoint_t x);


	/***** data types to strings *****/

	std::string to_string(GeneralCategory x);
	std::string to_string(HangulSyllableType x);
	std::string to_string(IndicSyllabicCategory x);
	std::string to_string(EastAsianWidth x);
	std::string to_string(GraphemeClusterBreak x);
	std::string to_string(WordBreak x);
	std::string to_string(LineBreak x);
	std::string to_string(SentenceBreak x);
	std::string to_string(Script x);

	inline std::string to_string(codepoint_t x) {
		static const char HEX_CHARS[] = {
			'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
			'A', 'B', 'C', 'D', 'E', 'F' };

		std::string ret;
		ret.reserve(7); // "U+?????"
		ret += "U+";

		codepoint_t mask = 0xF0000000;

		// jump over leading zeroes
		int i = 7;
		for (; i >= 4 && ((x & mask) == 0); i--) {
			mask >>= 4;
		}

		for (; i >= 0; i--) {
			ret += HEX_CHARS[(x & mask) >> (i * 4)];
			mask >>= 4;
		}

		return ret;
	}
};

#endif
