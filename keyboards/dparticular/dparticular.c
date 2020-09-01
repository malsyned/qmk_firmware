#include "dparticular.h"

bool numlock_state = false;

void matrix_init_kb(void) {
    matrix_init_user();
}

void led_set_kb(uint8_t usb_led)
{
    numlock_state = usb_led & (1<<USB_LED_NUM_LOCK);
}

bool process_record_kb(uint16_t keycode, keyrecord_t *record)
{
    if (!process_record_user(keycode, record))
        return false;

    if (!numlock_state) {
        SEND_STRING(SS_DOWN(X_NUMLOCK) SS_DELAY(100) SS_UP(X_NUMLOCK));
        numlock_state = true;
    }

    return true;
}
