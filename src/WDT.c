// Inkluderingsdirektiv:
#include "WDT.h"

/**
 * @brief Initiates watchdog timer with desired timeout.
 *
 * @details
 * WDTCSR = Watchdog Timer Control and Status Register
 * WDE = Watchdog System Reset Enable
 * WDCE = Watchdog Change Enable
 *   value:    0       1       2       3       4       5      6   7   32  33
 * timeout:   16ms    32ms    64ms    128ms   256ms   512ms   1s  2s  4s  8s
 *
 * @param timeout_val The value who correspond desired timeout
 */
void WDT_init(uint8_t timeout_val)
{
    WDTCSR |= (1 << WDE) | (1 << WDCE);
    WDTCSR = (1 << WDE) | timeout_val;
}

/**
 * @brief Resets the WDT.
 * @details
 *  asm("CLI");             // Disable interrupts
 *  asm("WDR");             // Resets WDT
 *  MCUSR &= ~(1 << WDRF);  // Resets Watch Dog Reset Flag
 *  asm("SEI");             // Enable interrupts
 */
void WDT_reset(void)
{
    asm("CLI");
    asm("WDR");
    MCUSR &= ~(1 << WDRF);
    asm("SEI");
}

/**
 * @brief Enables WDT in System Reset Mode.
 * @details Resets WDT to prevent the timer from expire
 * while enabling System Reset Mode (WDE)
 */
void WDT_system_reset_enable(void)
{
    WDT_reset();
    WDTCSR |= (1 << WDE);
}
