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

	inline std::string to_string(GeneralCategory x) {
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

	inline std::string to_string(HangulSyllableType x) {
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

	inline std::string to_string(IndicSyllabicCategory x) {
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

	inline std::string to_string(EastAsianWidth x) {
		using X = EastAsianWidth;
		switch (x) {
			case X::A:  return "A";
			case X::F:  return "F";
			case X::H:  return "H";
			case X::N:  return "N";
			case X::Na: return "Na";
			case X::W:  return "W";
			default: return "?";
		}
	}

	inline std::string to_string(GraphemeClusterBreak x) {
		using X = GraphemeClusterBreak;
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

	inline std::string to_string(WordBreak x) {
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

	inline std::string to_string(LineBreak x) {
		using X = LineBreak;
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

	inline std::string to_string(Script x) {
		using X = Script;
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

	inline std::string to_string(SentenceBreak x) {
		using X = SentenceBreak;
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
