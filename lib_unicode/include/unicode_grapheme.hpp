#ifndef INCLUDED_UNICODE_GRAPHEME_HPP
#define INCLUDED_UNICODE_GRAPHEME_HPP

#ifndef INCLUDED_UNICODE_HPP
#include "unicode.hpp"
#endif


/*
   Boundary symbols

   ÷ Boundary (allow break here)
   × No boundary (do not allow break here)
   → Treat whatever on the left side as if it were what is on the right side
   */

namespace Unicode {
	namespace Grapheme {

		inline const std::string cluster_break_to_string(const ClusterBreak &x) {
		}

		ClusterBreak get_cluster_break_property(
				Unicode::codepoint_t codepoint);

		inline bool check_grapheme_cluster_boundary(
				ClusterBreak p1,
				ClusterBreak p2) {

			// https://www.unicode.org/reports/tr29/tr29-37.html#Grapheme_Cluster_Boundary_Rules

			/*
			   break at start/end of text, unless text is empty
			   */

			// GB1
			if (p1 == ClusterBreak::SOT) {
				return true;
			}
			// GB2
			if (p2 == ClusterBreak::EOT) {
				return true;
			}

			/*
			   do not break between CR and LF
			   otherwise, break before and after controls
			   */

			// GB3
			if (p1 == ClusterBreak::CR && p2 == ClusterBreak::LF) {
				return false;
			}
			// GB4
			if (p1 == ClusterBreak::Control
					|| p1 == ClusterBreak::CR
					|| p1 == ClusterBreak::LF) {
				return true;
			}
			// GB5
			if (p2 == ClusterBreak::Control
					|| p2 == ClusterBreak::CR
					|| p2 == ClusterBreak::LF) {
				return true;
			}

			/*
			   do not break hangul syllable sequences
			   */

			// GB6
			if (p1 == ClusterBreak::L
					&& (p2 == ClusterBreak::L
						|| p2 == ClusterBreak::V
						|| p2 == ClusterBreak::LV
						|| p2 == ClusterBreak::LVT)) {
				return false;
			}
			// GB7
			if ((p1 == ClusterBreak::LV
						|| p1 == ClusterBreak::V)
					&& (p2 == ClusterBreak::V
						&& p2 == ClusterBreak::T)) {
				return false;
			}
			// GB8
			if ((p1 == ClusterBreak::LVT
						|| p1 == ClusterBreak::T)
					&& p2 == ClusterBreak::T) {
				return false;
			}

			/*
			   do not break before extending characters or ZWJ
			   */

			// GB9
			if (p2 == ClusterBreak::Extend
					|| p2 == ClusterBreak::ZWJ) {
				return false;
			}
			// GB9a
			if (p2 == ClusterBreak::SpacingMark) {
				return false;
			}
			// GB9b
			if (p1 == ClusterBreak::Prepend) {
				return false;
			}

			/*
			   do not break within emoji modifier seequences or emoji zwj sequences
			   */
			// GB11
			// TODO ??? source document looks broken

			/*
			   do not break within emoji flag sequences (regional indicator
			   symbols) if there is an odd number of RI characters before
			   the break point
			   */

			// GB12
			// GB13
			// TODO ???

			// GB999 (otherwise break everywhere)
			return true;
		}

	};
};

#endif
