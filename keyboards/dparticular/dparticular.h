#ifndef KB_H
#define KB_H

#include "quantum.h"

#define KEYMAP( \
	                         K05, K06,      \
	K10, K11, K12, K13, K14, K15, K16,      \
	K20, K21, K22, K23, K24, K25, K26, K27, \
	     K31, K32, K33, K34, K35, K36, K37, \
	     K41, K42, K43, K44, K45, K46, K47, \
	          K52, K53, K54, K55, K56, K57, \
	          K62, K63, K64, K65,      K67  \
) { \
	{ KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, K05,   K06,   KC_NO }, \
	{ K10,   K11,   K12,   K13,   K14,   K15,   K16,   KC_NO }, \
	{ K20,   K21,   K22,   K23,   K24,   K25,   K26,   K27 }, \
	{ KC_NO, K31,   K32,   K33,   K34,   K35,   K36,   K37 }, \
	{ KC_NO, K41,   K42,   K43,   K44,   K45,   K46,   K47 }, \
	{ KC_NO, KC_NO, K52,   K53,   K54,   K55,   K56,   K57 }, \
	{ KC_NO, KC_NO, K62,   K63,   K64,   K65,   KC_NO, K67 }  \
}

#endif
