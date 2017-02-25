#ifndef __TIMER_REUSABLE_H
#define __TIMER_REUSABLE_H
#include <minix/syslib.h>
#include "i8254.h"
#include "i8042.h"
#include <minix/drivers.h>


/** @defgroup timer timer
 * @{
 *
 * Functions for using the i8254 timers
 */

/**
 * @brief Subscribes and enables Timer 0 interrupts
 *
 * @return Returns bit order in interrupt mask; negative value on failure
 */
int timer_subscribe_int(unsigned int *hookID_timer);

/**
 * @brief Unsubscribes Timer 0 interrupts
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int timer_unsubscribe_int(unsigned int *hookID_timer);

/**
 * @brief Configures a timer to generate a square wave
 *
 * Does not change the LSB (BCD/binary) of the timer's control word.
 *
 * @param timer Timer to configure. (Ranges from 0 to 2)
 * @param freq Frequency of the square wave to generate
 * @return Return 0 upon success and non-zero otherwise
 */
int timer_set_square(unsigned long timer, unsigned long freq);

/**
 * @brief Timer 0 interrupt handler
 *
 * Increments counter
 */
void timer_int_handler();

#endif /* __TIMER_REUSABLE_H */
