#ifndef __TIMER_H
#define __TIMER_H



/** @defgroup timer timer
 * @{
 *
 * Functions for using the i8254 timers
 */

/**
 * @fn	int timer_waitXSeconds(long secondsToWait)
 * @brief Sleeps for a given time
 * Attention : this function subscribes and unsubscribes the timer interruptions.
 * @param secondsToWait Amount of time in seconds to sleep.
 * @return 0 upon success, 1 upon failure.
 */

int timer_waitXSeconds(long secondsToWait);


/**
 * @fn	int timer_subscribe_int()
 * @brief Subscribes and enables Timer 0 interrupts
 *
 * @return Returns bit order in interrupt mask; negative value on failure
 */
int timer_subscribe_int();

/**
 * @fn	int timer_unsubscribe_int()
 * @brief Unsubscribes Timer 0 interrupts
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int timer_unsubscribe_int();

/**
 * @fn	int timer_set_square(unsigned long timer, unsigned long freq)
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
 * @fn	void timer_int_handler()
 * @brief Timer 0 interrupt handler
 *
 * Increments counter
 */
void timer_int_handler();

/** @} end of timer */

#endif /* __TIMER_H */
