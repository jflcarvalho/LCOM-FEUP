#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/com.h>
#include "minix/sysutil.h"
#include "uart.h"

static unsigned int uart_COM1_hookID = uart_COM1_IRQ;

int subscribeUART(){
	//printf("Start subscribeUART()\n");
	uart_COM1_hookID = uart_COM1_IRQ;
	if (sys_irqsetpolicy(uart_COM1_IRQ, IRQ_REENABLE|IRQ_EXCLUSIVE , &uart_COM1_hookID) != OK) { //esta kernel call vai alterar o valor de hookID_mouseTemp
		printf("Error calling sys_irqsetpolicy\n");
		return 1;
	}
	if (sys_irqenable(&uart_COM1_hookID) != OK) {
		printf("Error calling sys_irqenable\n");
		return 1;
	}
	if (sys_outb(IER_ADDR_COM1, UART_IER_RD|UART_IER_RLS) != OK) { //|UART_IER_TE
		printf("Error calling sys_outb\n");
		return 1;
	}
	//printf("End subscribeUART()\n");
	return 0;
}

int unsubscribeUART(){
	//printf("Start unsubscribeUART()\n");
	if (sys_irqdisable(&uart_COM1_hookID) != OK) {
		printf("Error disabling mouse IRQ\n");
		return 1;
	}

	if (sys_irqrmpolicy(&uart_COM1_hookID) != OK) {
		printf("Error removing mouse policy\n");
		return 1;
	}
	//printf("End unsubscribeUART()\n");
	return 0;
}

int UART_readRBR(unsigned long *RBR){ //Receiver Buffer Register
	//printf("Start UART_readRBR()\n");
	unsigned long LSR;
	int tries = 5;

	while(tries > 0){
			if(UART_readLSR(&LSR) != OK){
				printf("Error readind LSB\n");
			}
			else if(LSR & UART_LSR_RR){
				sys_inb(RBR_ADDR_COM1, RBR);
				printf("Char Received: %c \n",(char) *RBR);
                tickdelay(micros_to_ticks(UART_DELAY));
				//printf("End UART_readRBR\n");
				return 0;
			}
			else{
				tries--;
			}
	}
	//printf("End UART_readRBR()\n");
	return 1;
} 

int UART_writeTHR(unsigned long data_char){ //Transmitter Holding Register
	//printf("Start UART_writeTHR()\n");
	unsigned long LSR;
	int tries = 5;

	while(tries > 0){
			if(UART_readLSR(&LSR) != OK){
				printf("Error readind LSB\n");
			}
			else if(LSR & UART_LSR_THRE){
				sys_outb(THR_ADDR_COM1, data_char);
				printf("Char Transmitted: %c \n",(char)data_char);
				//printf("End UART_writeTHR()\n");
				return 0;
			}
			else{
				tries--;
			}
		}
	//printf("End UART_writeTHR()\n");
	return 1;
}

inline int UART_readIIR(unsigned long *IIR){ //Interrupt Identification Register
	if(sys_inb(IIR_ADDR_COM1, IIR) != OK){
		printf("Error calling sys_inb\n");
		return 1;
	}
	/* DEBUG 
	unsigned long IIR_Debug = *IIR;
	int i;
	printf("IIR: ");
	for(i=0; i <= 7 ; i++){
		printf("%d" ,(IIR_Debug >> (7-i))&1);
	}
	printf("\n");
    */
	return 0;
}

inline int UART_readLSR(unsigned long *LSR){ //Line Status Register
    if(sys_inb(LSR_ADDR_COM1, LSR) != OK){
		printf("Error calling sys_inb\n");
		return 1;
	}
	/* DEBUG 
	unsigned long LSR_Debug = *LSR;
	int i;
	printf("LSR: ");
	for(i=0; i <= 6 ; i++){
		printf("%d" ,(LSR_Debug >> (6-i))&1);
	}
	printf("\n");
    */
	return 0;
}

char UART_Transmitter_Empty(){
	unsigned long status = 0;
	if(UART_readLSR(&status) != OK){
		printf("Error reading LSR\n");
		return 0;
	}
	return (status & UART_LSR_THRE);
}

int UART_Interrupt_Handler(unsigned long *RBR){
	unsigned long IIR = 0;
	unsigned long LSR = 0;
	if(UART_readIIR(&IIR) != OK){
		printf("Error reading IIR\n");
		return 1;
	}
    ((IIR << 4) >> 4);
    if(IIR == UART_IIR_RD){
		//printf("Received Data Available Interrupt\n");
        if(UART_readRBR(RBR) != OK){
            return 1;
        }
	}
	else if (IIR == UART_IIR_RLS){
		printf("Line Status Interrupt\n");
        if(UART_readLSR(&LSR) != OK){
            return 1;
        }
	}
	/*else if (IIR == UART_IIR_TE){
		printf("Transmitter Empty Interrupt\n");
		UART_writeTHR(*dataToSynchronize);
	}*/
	return 0;
}

int UART_Synchornize_Game(){
	printf("Start uart_test()\n");
	int ipc_status, r;
	message msg;
	int irq_set_uart_com1 = BIT(uart_COM1_IRQ);
    char *dataToSynchronize = "pang#";
    unsigned long RBR_char = 0;

	int exit = 0;
	if(subscribeUART() != OK){
		printf("Error subscribing UART\n");
		return 1;
	}
	while (!exit) { /* key pressed != ESC */
		/* Get a request message. */
		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d\n", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) { /* received notification of kernel*/
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: /* hardware interrupt notification */
				if(msg.NOTIFY_ARG & irq_set_uart_com1){
					printf("UART COM1 Interrupt\n");
                    UART_Interrupt_Handler(&RBR_char);
                    if (RBR_char == '#'){
                        exit = 1;
                    }
                    else if(RBR_char == *dataToSynchronize){
                        dataToSynchronize++;
                        UART_writeTHR(*dataToSynchronize);
                    }
				}
				break;
			default:
				printf("Default notification\n");
				break; /* no other notifications expected: do nothing */
			}
		}
	}
	if(unsubscribeUART() != OK){
		printf("Error unsubscribing UART\n");
		return 1;
	}
	printf("End uart_test()\n");
	return 0;
}

