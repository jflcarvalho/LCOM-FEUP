#include "keyboard_reusable.h"

static unsigned int hookID_keyboard = KEYBOARD_IRQ;

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

int subscribeKeyboard(){
	if (sys_irqsetpolicy(KEYBOARD_IRQ, IRQ_REENABLE|IRQ_EXCLUSIVE , &hookID_keyboard) != OK) { //esta kernel call vai alterar o valor de hookID_keyboardTemp
		printf("Error calling sys_irqsetpolicy\n");
		return 1;
	}
	if (sys_irqenable(&hookID_keyboard) != OK) {
		printf("Error calling sys_irqenable\n");
		return 1;
	}
	return 0;
}

int unsubscribeKeyboard() {

	if (sys_irqdisable(&hookID_keyboard) != OK) {
		printf("Error disabling keyboard IRQ\n");
		return 1;
	}

	if (sys_irqrmpolicy(&hookID_keyboard) != OK) {
		printf("Error removing keyboard policy\n");
		return 1;
	}

	return 0;
}

int verifyOBF(){
	unsigned long statusByte, garbage;
	readStatusRegister(&statusByte);
	if (statusByte & OBF){
		return 1; //exist data to read
	}
	return 0;//not exist data to read
}
