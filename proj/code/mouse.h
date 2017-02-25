#ifndef _MOUSE_
#define _MOUSE_





#define irq_set_mouse	BIT(MOUSE_IRQ) //MOUSE_IRQ is defined in i8042.h
#define PlayXMin	412	/**< @brief Coordinate x of the left border of the play BMP */
#define PlayXMax	612	/**< @brief Coordinate x of the right border of the play BMP */
#define PlayYMin	359	/**< @brief Coordinate y of the top border of the play BMP */
#define PlayYMax	409	/**< @brief Coordinate y of the bottom border of the play BMP */
#define MOUSECLICK	BIT(0)	/**< @brief Mouse left button click */


/**
 * @fn	int unsubscribeMouse()
 * @brief Unsubscribes Mouse
 *
 * @return 0 upon sucess, 1 upon failure.
 */


int unsubscribeMouse();

/**
 * @fn	int subscribeMouse()
 * @brief Subscribes Mouse
 *
 * @return 0 upon sucess, 1 upon failure.
 */

int subscribeMouse();

/**
 * @fn	unsigned int readStatusRegister(unsigned long *statusByte)
 * @brief Reads Mouse Status Register
 *
 * @return 0 upon success, 1 upon failure. Updates statusByte with the value read.
 */

unsigned int readStatusRegister(unsigned long *statusByte);

/**
 * @fn	unsigned int readOutBuffer(unsigned long *outBufValue)
 * @brief Reads Output buffer
 *
 * @return 0 upon success, 1 upon failure. Updates outBufValue with the value read.
 */

unsigned int readOutBuffer(unsigned long *outBufValue);

/**
 * @fn	int mouseEnable()
 * @brief Enables Mouse data reporting
 *
 * @return 0 upon success, 1 upon failure.
 */

int mouseEnable();

/**
 * @fn	int mouseDisable()
 * @brief Disables Mouse data reporting
 *
 * @return 0 upon success, 1 upon failure.
 */

int mouseDisable();

/**
 * @fn	int sendCommandToKBC(unsigned long cmd)
 * @brief Calls function 'write byte to mouse' and sends command 'cmd' to mouse,
 *
 * @return 0 upon success, 1 upon failure.
 */

int sendCommandToKBC(unsigned long cmd);

#endif
