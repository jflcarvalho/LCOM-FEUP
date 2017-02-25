#ifndef __I8042
#define __I8042


//Macros for keyboard controller i8042

#define KEYBOARD_IRQ	1

#define OUT_BUF			0x60
#define IN_BUF			0x60
#define KBC_AUX_BUF		0x64
#define STATUS_REG		0x64
#define OBF				BIT(0) //output buffer full
#define IBF				BIT(1)
#define ESC_KEY			0x81
#define WAIT_KBC		2000
#define SET_LEDS		0xED
#define KBD_ACK			0xFA
#define RESEND			0xFE
#define ERROR 			0xFC
#define LED_SELECTOR(n)	BIT(n)
#define DOUBLEBYTE		0xE0
#define PAR_ERR			BIT(7)
#define TO_ERR			BIT(6)

#define BIT7			BIT(7)





#endif
