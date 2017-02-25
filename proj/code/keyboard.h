#ifndef __KEYBOARD_H_
#define __KEYBOARD_H_



/*
 * @fn	unsigned int kbd_readStatusRegister(unsigned long *statusByte)
 * @brief	Reads Status Register
 * @return	Returns 0 - success, 1 unsucess;  statusByte gets actualized;
 */
unsigned int kbd_readStatusRegister(unsigned long *statusByte);



/*
 * @fn	unsigned int kbd_readOutBuffer(unsigned long *OutBufValue)
 * @brief	Reads KBC Out Buffer
 *
 *@return 	Returns 0 - success, 1 unsucess;  OutBufValue gets actualized;
 *
 */
unsigned int kbd_readOutBuffer(unsigned long *OutBufValue);

/**
 * @fn	int subscribeKeyboard()
 * @brief	Subscribes and enables keyboard interrupts
 *
 * @return	Returns bit order in interrupt mask; negative value on failure
 */
int subscribeKeyboard();

/**
 * @fn	int unsubscribeKeyboard()
 * @brief	Unsubscribes and disables keyboard interrupts
 *
 * @return	Returns 0 upon success, 1 upon failure.
 */

int unsubscribeKeyboard();

/**
 * @fn	int verifyOBF()
 * @brief	Ckecks if output buffer is full
 *
 * @return	Returns 1 if output buffer is full, 0 otherwise.
 */


int verifyOBF();

/**
 * @fn	int keyboardEnable()
 * @brief	Enable keyboard data reporting.
 *
 * @return	Returns 0 upon success, 1 upon failure.
 */

int keyboardEnable();

/**
 * @fn	int keyboardDisable()
 * @brief	Disable keyboard data reporting.
 *
 * @return	Returns 0 upon success, 1 upon failure.
 */


int keyboardDisable();

#endif
