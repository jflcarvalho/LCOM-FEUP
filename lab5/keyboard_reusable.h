#ifndef __KEYBOARD
#define __KEYBOARD

#include <minix/syslib.h>
#include "i8254.h"
#include "i8042.h"
#include <minix/drivers.h>


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
int subscribeKeyboard();

int unsubscribeKeyboard();

int verifyOBF();

#endif
