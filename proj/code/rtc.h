#ifndef _RTC_H
#define _RTC_H



#define RTC_ADDR_REG 0x70 /**< @brief must be loaded with
the address of the RTC register to be accessed */
#define RTC_DATA_REG 0x71 /**< @brief used to transfer
the data to/from the RTC’s register accessed */

#define RTC_REG_A	10 /**< @brief RTC A Register */
#define RTC_REG_B	11 /**< @brief RTC B Register */
#define RTC_REG_C	12 /**< @brief RTC C Register */
#define RTC_REG_D	13 /**< @brief RTC D Register */

#define RTC_UIP BIT(7) /**< @brief Update in progress bit (Register A) */
#define RTC_SET	BIT(7)	/**< @brief When the SET bit is a 0, the update transfer functions normally by advancing
the counts once per second.  When the SET bit is written to a 1, any update transfer is inhibited
and the program can initialize the time and calendar bytes without an update occurring in  the  midst  of  initializing. */
#define RTC_24H	BIT(1)	/**< @brief Format of the hours byte. 0-12h mode 1-24h mode (Register B) */

#define RTC_DM BIT(2) /**< @brief Data mode bit. 1-Binary 0-BCD (Register B) */

#define RTC_HOURS	4	/**< @brief Hours address location */
#define RTC_MINUTES	2	/**< @brief Minutes address location */

#define RTC_IRQ 8 	/**< @brief RTC interrupt line */

/**
 * @fn void rtc_getTime(long unsigned time[2])
 * @brief Loads the time array with the current hours and minutes
 * When this function is called: time[0] = hours, time[1] = minutes;
 * This functions call an assembly function which reads the time from the RTC
 *
 * @param time Array of size 2 which will be modified.
 */

void rtc_getTime(long unsigned time[2]);

#endif
