#include "Keyboard.h"


unsigned int readStatusRegister(unsigned long *statusByte){
	if (sys_inb(STATUS_REG,statusByte) != OK){
		printf("Error reading Status Byte\n");
		return 1;
	}
	return 0;
}

unsigned int readOutBuffer(unsigned long *OutBufValue){
	if (sys_inb(OUT_BUF,OutBufValue)!= OK){
		printf("Error reading output buffer\n");
		return 1;
	}
	return 0;
}

int subscribeKeyboard(unsigned int *hookID_keyboard){
	if (sys_irqsetpolicy(KEYBOARD_IRQ, IRQ_REENABLE|IRQ_EXCLUSIVE , hookID_keyboard) != OK) { //esta kernel call vai alterar o valor de hookID_keyboardTemp
		printf("Error calling sys_irqsetpolicy\n");
		return 1;
	}
	if (sys_irqenable(hookID_keyboard) != OK) {
		printf("Error calling sys_irqenable\n");
		return 1;
	}
	return 0;
}

int unsubscribeKeyboard(unsigned int *hookID_keyboard) {

	if (sys_irqdisable(hookID_keyboard) != OK) {
		printf("Error disabling keyboard IRQ\n");
		return 1;
	}

	if (sys_irqrmpolicy(hookID_keyboard) != OK) {
		printf("Error removing keyboard policy\n");
		return 1;
	}

	return 0;
}


int kbc_send_cmd(unsigned long reg, unsigned long cmd){
	unsigned long status;
	unsigned int counter = 0;

	printf("Debugger: kbc_send_cmd has been called\n");

	//check if IN_BUF is Empty
	if (readStatusRegister(&status) != OK){
		printf("Error reading status register.\n");
		return 1;
	}
	printf("Debugger: status register has been read\n");

	while (status & IBF != 0){
		printf("Debugger: Looping on While because IBF is ON\n");
		tickdelay(micros_to_ticks(WAIT_KBC));
		counter++;
		if (counter == 5){
			printf("Input buffer is full ! Time out.\n");
			return 1;
		}
	}
	printf("Debugger: IBF is OFF\n");
	//escrever o commando no IN_BUF

	if (sys_outb(reg, cmd) != OK){
		printf("Error inserting command on register 0x%x\n",reg);
		return 1;
	}
	printf("Debugger: cmd sent to reg!\n");

	//ler o valor no OUT_BUF
	if (readStatusRegister(&status) != OK){
		printf("Error reading status register.\n");
		return 1;
	}
	if ( (status & (PAR_ERR | TO_ERR)) != OK){
		printf("Invalid Data on Out_Buffer\n");
		return -1;
	}
	unsigned long outBuffer;
	if(readOutBuffer(&outBuffer) != OK){
		printf("Error reading OUT_BUF\n");
		return 1;
	}
	printf("Debugger: OUT BUFFER HAS BEEN READ and has value 0x%x\n",outBuffer);

	while (outBuffer != KBD_ACK){
		printf("Debugger: Out Buffer != KBD_ACK\n");
		if(outBuffer == RESEND){
			printf("Debugger: Resending command;\n");
			if (sys_outb(reg, cmd) != OK){
				printf("Error inserting command on register 0x%x\n",reg);
				return 1;
			}
		}
		else if(outBuffer == ERROR){
			printf("KBD sent ERROR CMD\n");
			return 1;
		}
	}


	return 0;
}

int kbd_set_led( unsigned short mask) {
	printf("Debugger: kbd_set_led called!\n");
	if(mask > 7){
		printf("Leds indexs should be only in range [0-2].\n");
		return 1;
	}
	if(kbc_send_cmd(IN_BUF, SET_LEDS) != OK){
		printf("Error sending set_leds to port 0x64\n");
		return 1;
	}

	if(kbc_send_cmd(IN_BUF, mask) != OK){
		printf("Error loading parameters to port 0x60\n");
		return 1;
	}
	printf("Debugger: Led has been set with success\n");

	return 0;
}
