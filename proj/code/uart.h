#ifndef _UART_H
#define _UART_H



#define BIT(n) (0x01<<(n))
#define COM1_N 1

#define uart_COM1_IRQ 4	/**< @brief COM1 IRQ */

#define UART_ADDR_COM1 0x3F8 /**< @brief UART-COM1 Base Address */
#define IER_ADDR_COM1 0x3F9  /**< @brief Interrupt Enable Register */
#define RBR_ADDR_COM1 0x3F8	/**< @brief Receiver Buffer Register */
#define THR_ADDR_COM1 0x3F8	/**< @brief Transmitter Holding Register */
#define IIR_ADDR_COM1 0x3FA	/**< @brief Interrupt Identification Register */
#define LSR_ADDR_COM1 0x3FD	/**< @brief Line Status Register */

#define UART_LSR_RR BIT(0)	/**< @brief Receiver Data (Set to 1 when there is data for receiving) */
#define UART_LSR_OE BIT(1) /**< @brief  Overrun Error (Set to 1 when a characters received is overwritten by another one) */
#define UART_LSR_PE BIT(2) /**< @brief Parity Error*/
#define UART_LSR_FE BIT(3) /**< @brief Framing Error */
#define UART_LSR_THRE BIT(5) /**< @brief  Transmitter Holding Register Empty*/

#define UART_IER_RD BIT(0) /**< @brief Enables the Received Data Available Interrupt */
#define UART_IER_TE BIT(1) /**< @brief Enables the Transmitter Holding Register Empty Interrupt */
#define UART_IER_RLS BIT(2) /**< @brief Enables the Receiver Line Status Interrupt */

#define UART_IIR_RD 0x4 /**< @brief  Received Data Available */
#define UART_IIR_TE 0x2 /**< @brief  Transmitter Holding Register Empty */
#define UART_IIR_RLS 0x6 /**< @brief  Receiver Line Status  (FIFO)*/

#define UART_DELAY 10000

/**
 * @fn	int subscribeUART()
 * @brief Subscribes UART
 *
 * @return 0 upon success, 1 upon failure.
 */

int subscribeUART();

/**
 * @fn	int unsubscribeUART()
 * @brief Unsubscribes UART
 *
 * @return 0 upon success, 1 upon failure.
 */

int unsubscribeUART();

/**
 * @fn	int UART_readRBR(unsigned long *RBR)
 * @brief Read Receiver Buffer Register
 * @param RBR Pointer to a variable which will be updated with the value read.
 * @return 0 upon success, 1 upon failure. RBR is updated with the value read.
 */


int UART_readRBR(unsigned long *RBR); //Receive Buffer Register

/**
 * @fn	int UART_writeTHR(unsigned long data_char)
 * @brief Write a char to the Transmitter Holding Register.
 * @param data_char Char to be transmitted.
 * @return 0 upon success, 1 upon failure.
 */

int UART_writeTHR(unsigned long data_char); //Transmitter Holding Register

/**
 * @fn	int UART_readIIR(unsigned long *IIR)
 * @brief Read Interrupt Identification Register.
 * @param IIR Pointer to a variable that will be updated with the value contained by the IIR.
 * @return 0 upon success, 1 upon failure.
 */


int UART_readIIR(unsigned long *IIR); //Interrupt Identification Register

/**
 * @fn	int UART_readLSR(unsigned long *LSR)
 * @brief Read Line Status Register.
 * @param LSR Pointer to a variable that will be updated with the value contained by the LSR.
 * @return 0 upon success, 1 upon failure.
 */


int UART_readLSR(unsigned long *LSR); //Line Status Register

/**
 * @fn	char UART_Transmitter_Empty()
 * @brief Check  if UART is ready to accept a new character for transmitting.
 * @return 0 upon success, 1 upon failure.
 */


char UART_Transmitter_Empty();

/**
 * @fn	int UART_Interrupt_Handler(unsigned long *RBR)
 * @brief Handles UART interruptions.
 * @param RBR Pointer to a variable that will be updated with the value contained by the Receive Buffer Register.
 * @return 0 upon success, 1 upon failure.
 */

int UART_Interrupt_Handler(unsigned long *RBR);

/**
 * @fn	int UART_Synchornize_Game()
 * @brief Subscribes UART, handles the interruptions cycle and in the end unsubscribes UART.
 * @return 0 upon success, 1 upon failure.
 */

int UART_Synchornize_Game();

#endif
