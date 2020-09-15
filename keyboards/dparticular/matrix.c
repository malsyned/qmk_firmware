#include "matrix.h"
#include "debounce.h"
#include "i2c_master.h"
#include "quantum.h"
#include <print.h>

#define MATRIX_IO_DELAY 30

#define EXP_I2C_TIMEOUT 100
#define EXP_ADDR        0b0100000
#define EXP_ADDR_WRITE  ( (EXP_ADDR<<1) | I2C_WRITE )
#define EXP_ADDR_READ   ( (EXP_ADDR<<1) | I2C_READ  )
#define IODIRA          0x00            // i/o direction register
#define IODIRB          0x01
#define GPPUA           0x0C            // GPIO pull-up resistor register
#define GPPUB           0x0D
#define GPIOA           0x12            // general purpose i/o port register (write modifies OLAT)
#define GPIOB           0x13
#define OLATA           0x14            // output latch register
#define OLATB           0x15

static matrix_row_t raw_matrix[MATRIX_ROWS];  // raw values
static matrix_row_t matrix[MATRIX_ROWS];      // debounced values

static bool i2c_initialized = 0;
static i2c_status_t mcp23018_status = 0x20;

__attribute__((weak)) void matrix_init_kb(void) { matrix_init_user(); }

__attribute__((weak)) void matrix_scan_kb(void) { matrix_scan_user(); }

__attribute__((weak)) void matrix_init_user(void) {}

__attribute__((weak)) void matrix_scan_user(void) {}

static pin_t row_pins[] = MATRIX_ROW_PINS;
static pin_t col_pins[] = MATRIX_COL_PINS;
static uint8_t exp_row_pins[] = EXPANDER_MATRIX_ROW_PINS;
static uint8_t exp_col_pins[] = EXPANDER_MATRIX_COL_PINS;

#define MCU_COLS (sizeof(col_pins) / sizeof(uint8_t))
#define EXP_COLS (sizeof(exp_col_pins) / sizeof(uint8_t))

void matrix_io_delay(void) { wait_us(MATRIX_IO_DELAY); }

static i2c_status_t mcp23018_init(void)
{
    uint8_t iodir[2], gppu[2], olat[2];
    mcp23018_status = I2C_STATUS_ERROR;

    if (i2c_initialized == 0) {
        i2c_init();
        i2c_initialized = true;
        wait_ms(1000);
    }

    /* Defaults for inputs and unused pins */
    iodir[0] = iodir[1] = 0xff;
    gppu[0]  = gppu[1]  = 0xff;
    olat[0]  = olat[1]  = 0xff;

    /* Overrides for outputs */
    for (int8_t c = 0; c < EXP_COLS; c++) {
        uint8_t e_pin = exp_col_pins[c];
        uint8_t e_reg = E_REG(e_pin);
        uint8_t e_pin_no = E_PIN_NO(e_pin);
        uint8_t mask = 1 << e_pin_no;
        iodir[e_reg] &= ~mask;
        /* gppu[e_reg] &= ~mask; */
    }
    
    mcp23018_status = i2c_writeReg(EXP_ADDR_WRITE, IODIRA, iodir, 2, EXP_I2C_TIMEOUT);
    if (mcp23018_status != I2C_STATUS_SUCCESS)
        goto out;
    
    mcp23018_status = i2c_writeReg(EXP_ADDR_WRITE, GPPUA, gppu, 2, EXP_I2C_TIMEOUT);
    if (mcp23018_status != I2C_STATUS_SUCCESS)
        goto out;
    
    mcp23018_status = i2c_writeReg(EXP_ADDR_WRITE, OLATA, olat, 2, EXP_I2C_TIMEOUT);
    if (mcp23018_status != I2C_STATUS_SUCCESS)
        goto out;

out:
    return mcp23018_status;
}

static void select_col(uint8_t col) {
    setPinOutput(col_pins[col]);
    writePinLow(col_pins[col]);
}

static void unselect_col(uint8_t col) { setPinInputHigh(col_pins[col]); }

static void unselect_cols(void) {
    for (uint8_t x = 0; x < MCU_COLS; x++) {
        setPinInputHigh(col_pins[x]);
    }
}

static void init_pins(void) {
    unselect_cols();
    for (uint8_t x = 0; x < MATRIX_ROWS; x++) {
        setPinInputHigh(row_pins[x]);
    }
}

void matrix_init(void) {
    mcp23018_status = mcp23018_init();

    init_pins();

    // initialize matrix state: all keys off
    for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
        matrix[i] = 0;
        raw_matrix[i] = 0;
    }

    debounce_init(MATRIX_ROWS);
    matrix_init_quantum();
}

void matrix_power_up(void) {
    mcp23018_status = mcp23018_init();

    init_pins();

    // initialize matrix state: all keys off
    for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
        matrix[i] = 0;
    }
}

matrix_row_t matrix_get_row(uint8_t row) {
    return matrix[row];
}

static bool read_rows_on_col(matrix_row_t current_matrix[], uint8_t current_col) {
    bool matrix_changed = false;

    // Select col and wait for col selecton to stabilize
    select_col(current_col);
    matrix_io_delay();

    // For each row...
    for (uint8_t row_index = 0; row_index < MATRIX_ROWS; row_index++) {
        // Store last value of row prior to reading
        matrix_row_t current_row_value = current_matrix[row_index];
        matrix_row_t mask = MATRIX_ROW_SHIFTER << (current_col + MCU_FIRST_COL);

        // Check row pin state
        if (readPin(row_pins[row_index]) == 0)
            current_row_value |= mask;
        else
            current_row_value &= ~mask;

        // Determine if the matrix changed state
        if ((current_matrix[row_index] != current_row_value)) {
            matrix_changed |= true;
            current_matrix[row_index] = current_row_value;
        }
    }

    // Unselect col
    unselect_col(current_col);

    return matrix_changed;
}

static void exp_select_col(uint8_t col_index)
{
    uint8_t data[2] = {0xff, 0xff};

    uint8_t e_pin = exp_col_pins[col_index];
    data[E_REG(e_pin)] &= ~(1 << E_PIN_NO(e_pin));
    mcp23018_status = i2c_writeReg(EXP_ADDR_WRITE, OLATA, data, 2, EXP_I2C_TIMEOUT);
}

static void exp_unselect_col(uint8_t col)
{
    uint8_t data[2] = {0xff, 0xff};
    mcp23018_status = i2c_writeReg(EXP_ADDR_WRITE, OLATA, data, 2, EXP_I2C_TIMEOUT);
}

static bool exp_read_rows_on_col(matrix_row_t current_matrix[], uint8_t current_col) {
    bool matrix_changed = false;
    uint8_t data[2];

    // Select col and wait for col selecton to stabilize
    exp_select_col(current_col);
    matrix_io_delay();
    mcp23018_status = i2c_readReg(EXP_ADDR_WRITE, GPIOA, data, 2, EXP_I2C_TIMEOUT);
    if (mcp23018_status != I2C_STATUS_SUCCESS)
        /* Assume no keys are pressed */
        data[0] = data[1] = 0xff;

    // For each row...
    for (uint8_t row_index = 0; row_index < MATRIX_ROWS; row_index++) {
        // Store last value of row prior to reading
        matrix_row_t current_row_value = current_matrix[row_index];
        matrix_row_t mask = MATRIX_ROW_SHIFTER << (current_col + EXPANDER_FIRST_COL);

        // Check row pin state
        uint8_t e_pin = exp_row_pins[row_index];
        uint8_t v = data[E_REG(e_pin)] & (1 << E_PIN_NO(e_pin));

        if (v == 0)
            current_row_value |= mask;
        else
            current_row_value &= ~mask;

        // Determine if the matrix changed state
        if ((current_matrix[row_index] != current_row_value)) {
            matrix_changed |= true;
            current_matrix[row_index] = current_row_value;
        }
    }

    // Unselect col
    exp_unselect_col(current_col);

    return matrix_changed;
}

uint8_t matrix_scan(void)
{
    bool changed = false;

    for (uint8_t current_col = 0; current_col < MCU_COLS; current_col++) {
        changed |= read_rows_on_col(raw_matrix, current_col);
    }
    for (uint8_t current_col = 0; current_col < EXP_COLS; current_col++) {
        changed |= exp_read_rows_on_col(raw_matrix, current_col);
    }

    debounce(raw_matrix, matrix, MATRIX_ROWS, changed);
    matrix_scan_quantum();

    return changed;
}

void matrix_print(void) {
    // TODO: use print() to dump the current matrix state to console
}
