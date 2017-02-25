#ifndef __I8042
#define __I8042


//Macros for keyboard controller i8042

#define KEYBOARD_IRQ	1  /**< @brief Keyboard IRQ line */
#define MOUSE_IRQ		12  /**< @brief Mouse IRQ line */

#define OUT_BUF			0x60	/**< @brief KBC output buffer */
#define IN_BUF			0x60	/**< @brief KBC input buffer */
#define KBC_AUX_BUF		0x64 	/**< @brief KBC auxiliary buffer */
#define STATUS_REG		0x64	/**< @brief KBC status register */
#define OBF				BIT(0) 	/**< @brief KBC output buffer full */
#define IBF				BIT(1)	/**< @brief KBC input buffer full */
#define ESC_KEY			0x81	/**< @brief ESC key make code */
#define WAIT_KBC		20000	/**< @brief Sleep time */
#define SET_LEDS		0xED	/**< @brief KBC command to set leds */
#define ACK				0xFA	/**< @brief KBC acknowledge */
#define RESEND			0xFE	/**< @brief KBC resend */
#define ERROR 			0xFC	/**< @brief KBC error */
#define LED_SELECTOR(n)	BIT(n)	/**< @brief KBC led selector */
#define DOUBLEBYTE		0xE0	/**< @brief	Makecode and Scancode are double bytes */
#define PAR_ERR			BIT(7)	/**< @brief KBC parity error */
#define TO_ERR			BIT(6)	/**< @brief KBC error */
#define SEND_CMD		0xD4	/**< @brief KBC command to send another command */
#define ENABLE_MOUSE	0xF4	/**< @brief KBC enable mouse */
#define DISABLE_MOUSE	0xF5	/**< @brief KBC disable mouse */
#define ENABLE_KEYBOARD	0xAE	/**< @brief KBC enable keyboard */
#define DISABLE_KEYBOARD 0xAD	/**< @brief KBC disable keyboard */
#define RIGHT_BUTTON	BIT(0)	/**< @brief Mouse right button */
#define X_SIGN			BIT(4)	/**< @brief Mouse x sign */
#define Y_SIGN			BIT(5)	/**< @brief Mouse y sign */
#define WRITEB_MOUSE	0xD4	/**< @brief KBC send byte to mouse */
#define STATUS_REQUEST	0xE9	/**< @brief Mouse status request */
#define BIT7			BIT(7)	/**< @brief Mouse BIT7 */
#define FIRST_BYTE 		BIT(3)	/**< @brief Mouse First Byte */

#define DELAY_US 		20000	/**< @brief Sleep time */

#define LEFTMBUTTON		 0x4b	/**< @brief Keyboard left arrow make code */
#define RIGHTMBUTTON	 0x4d	/**< @brief Keyboard right arrow make code */
#define LEFTBBUTTON		 0xcb	/**< @brief Keyboard left arrow break code */
#define RIGHTBBUTTON	 0xcd	/**< @brief Keyboard right arrow break code */
#define AMBUTTON         0x1e	/**< @brief Keyboard up arrow make code */
#define DMBUTTON         0X20	/**< @brief Keyboard down arrow make code */
#define ABBUTTON         0x9e	/**< @brief Keyboard up arrow break code */
#define DBBUTTON         0xa0	/**< @brief Keyboard down arrow break code */
#define SPACEMBUTTON     0x39	/**< @brief Keyboard space make code */
#define SPACEBBUTTON     0xb9	/**< @brief Keyboard space break code */
#define SHIFTMBUTTON     0x2a	/**< @brief Keyboard shift make code */
#define SHIFTBBUTTON     0xaa	/**< @brief Keyboard shift break code */

#endif
