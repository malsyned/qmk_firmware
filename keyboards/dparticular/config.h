#ifndef CONFIG_H
#define CONFIG_H

#include "config_common.h"

/* USB Device descriptor parameter */
#define VENDOR_ID       0x6666
#define PRODUCT_ID      0x6060
#define DEVICE_VER      0x0001
#define MANUFACTURER    Black Sheep
#define PRODUCT         DParticular
#define DESCRIPTION     One-Piece Split Column-Staggered Keyboard

/* key matrix size */
#define MATRIX_ROWS 7
#define MATRIX_COLS 16
#define MCU_FIRST_COL 0
#define EXPANDER_FIRST_COL 8

/* key matrix pins */
#define MATRIX_ROW_PINS { C7, C6, B7, B3, B2, B1, B0 }
#define MATRIX_COL_PINS { F0, F1, F4, F5, F6, F7, B6, B5 }

#define E_PIN(REG, PIN_NO) (((REG) << 4) | (PIN_NO))
#define E_REG(PIN) ((PIN) >> 4)
#define E_PIN_NO(PIN) ((PIN) & 0xf)

#define E_A 0
#define E_B 1

#define E_A0 E_PIN(E_A, 0)
#define E_A1 E_PIN(E_A, 1)
#define E_A2 E_PIN(E_A, 2)
#define E_A3 E_PIN(E_A, 3)
#define E_A4 E_PIN(E_A, 4)
#define E_A5 E_PIN(E_A, 5)
#define E_A6 E_PIN(E_A, 6)
#define E_A7 E_PIN(E_A, 7)
#define E_B0 E_PIN(E_B, 0)
#define E_B1 E_PIN(E_B, 1)
#define E_B2 E_PIN(E_B, 2)
#define E_B3 E_PIN(E_B, 3)
#define E_B4 E_PIN(E_B, 4)
#define E_B5 E_PIN(E_B, 5)
#define E_B6 E_PIN(E_B, 6)
#define E_B7 E_PIN(E_B, 7)

#define EXPANDER_MATRIX_ROW_PINS { E_B0, E_B1, E_B2, E_B3, E_B4, E_B5, E_B6 }
#define EXPANDER_MATRIX_COL_PINS { E_A0, E_A1, E_A2, E_A3, E_A4, E_A5, E_A6, E_A7 }

#define UNUSED_PINS

/* COL2ROW or ROW2COL */
#define DIODE_DIRECTION ROW2COL

/* number of backlight levels */

#ifdef BACKLIGHT_PIN
#define BACKLIGHT_LEVELS 0
#endif

/* Set 0 if debouncing isn't needed */
#define DEBOUNCING_DELAY 5

/* Mechanical locking support. Use KC_LCAP, KC_LNUM or KC_LSCR instead in keymap */
#define LOCKING_SUPPORT_ENABLE

/* Locking resynchronize hack */
#define LOCKING_RESYNC_ENABLE

/* key combination for command */
#define IS_COMMAND() ( \
    keyboard_report->mods == (MOD_BIT(KC_LSHIFT) | MOD_BIT(KC_RSHIFT)) \
)

/* prevent stuck modifiers */
#define PREVENT_STUCK_MODIFIERS

#define RGB_DI_PIN D2
#ifdef RGB_DI_PIN
#define RGBLED_NUM 3
#define RGBLIGHT_HUE_STEP 2
#define RGBLIGHT_SAT_STEP 1
#define RGBLIGHT_VAL_STEP 2
#define WS2812_SWAP_RG
#endif

#endif
