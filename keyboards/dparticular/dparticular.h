#ifndef KB_H
#define KB_H

#include "quantum.h"

#define ___ KC_NO

#define KEYMAP( \
	                         K05, K06,              K09, K0A, \
	K10, K11, K12, K13, K14, K15, K16,              K19, K1A, K1B, K1C, K1D, K1E, K1F, \
	K20, K21, K22, K23, K24, K25, K26, K27,    K28, K29, K2A, K2B, K2C, K2D, K2E, K2F, \
	     K31, K32, K33, K34, K35, K36, K37,    K38, K39, K3A, K3B, K3C, K3D, K3E, K3F, \
	     K41, K42, K43, K44, K45, K46, K47,    K48, K49, K4A, K4B, K4C, K4D,      K4F, \
	          K52, K53, K54, K55, K56, K57,    K58, K59, K5A, K5B, K5C, K5D, \
	          K62, K63, K64, K65,      K67,    K68,      K6A, K6B, K6C, K6D  \
) { \
	{ ___, ___, ___, ___, ___, K05, K06, ___, ___, K09, K0A, ___, ___, ___, ___, ___ }, \
	{ K10, K11, K12, K13, K14, K15, K16, ___, ___, K19, K1A, K1B, K1C, K1D, K1E, K1F }, \
	{ K20, K21, K22, K23, K24, K25, K26, K27, K28, K29, K2A, K2B, K2C, K2D, K2E, K2F }, \
	{ ___, K31, K32, K33, K34, K35, K36, K37, K38, K39, K3A, K3B, K3C, K3D, K3E, K3F }, \
	{ ___, K41, K42, K43, K44, K45, K46, K47, K48, K49, K4A, K4B, K4C, K4D, ___, K4F }, \
	{ ___, ___, K52, K53, K54, K55, K56, K57, K58, K59, K5A, K5B, K5C, K5D, ___, ___ }, \
	{ ___, ___, K62, K63, K64, K65, ___, K67, K68, ___, K6A, K6B, K6C, K6D, ___, ___ }  \
}

#endif
