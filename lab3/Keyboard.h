#ifndef __KEYBOARD
#define __KEYBOARD

#include <minix/drivers.h>
#include "i8042.h"
#include "test3.h"

/*@brief Reads Status Register
 *
 *@return Returns 0 - success, 1 unsucess;  statusByte gets actualized;
 *
 */
unsigned int readStatusRegister(unsigned long *statusByte);



/*@brief Reads KBC Out Buffer
 *
 *@return Returns 0 - success, 1 unsucess;  OutBufValue gets actualized;
 *
 */
unsigned int readOutBuffer(unsigned long *OutBufValue);

/**
 * @brief Subscribes and enables KBC interrupts
 *
 * @return Returns bit order in interrupt mask; negative value on failure
 */
int subscribeKeyboard(unsigned int *hookID_keyboard);

int unsubscribeKeyboard(unsigned int *hookID_keyboard);

/**
 * @brief Set led keyboard command
 * Toggles LED indicator, according to the LED expecified
 * @param unsigned int with the LED to toggle (Ranges from 0 t0 2)
 * 0 - scrlock; 1 - numlock; 2 - capslock
 * @return Return 0 upon success and non-zero otherwise
 */
int kbc_send_cmd(unsigned long reg, unsigned long cmd);
int kbd_set_led(unsigned short led);

#endif
