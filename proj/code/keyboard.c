#include <minix/syslib.h>
#include <minix/drivers.h>
#include "i8254.h"
#include "i8042.h"
#include "keyboard.h"

static unsigned int hookID_keyboard = KEYBOARD_IRQ;

unsigned int kbd_readStatusRegister(unsigned long *statusByte){
	if (sys_inb(STATUS_REG,statusByte) != OK){
		printf("Error reading Status Byte\n");
		return 1;
	}
	return 0;
}

unsigned int kbd_readOutBuffer(unsigned long *OutBufValue){
	if (sys_inb(OUT_BUF,OutBufValue)!= OK){
		printf("Error reading output buffer\n");
		return 1;
	}
	return 0;
}

int subscribeKeyboard(){
	unsigned long garbage, statusByte;
	hookID_keyboard = KEYBOARD_IRQ;
	if (sys_irqsetpolicy(KEYBOARD_IRQ, IRQ_REENABLE|IRQ_EXCLUSIVE , &hookID_keyboard) != OK) { //esta kernel call vai alterar o valor de hookID_keyboardTemp
		printf("Error calling sys_irqsetpolicy\n");
		return 1;
	}
	if (sys_irqenable(&hookID_keyboard) != OK) {
		printf("Error calling sys_irqenable\n");
		return 1;
	}
    /* Clean OutBuffer */
	kbd_readStatusRegister(&statusByte);
	while (statusByte & OBF){
		sys_inb(OUT_BUF,&garbage);
		printf("Garbage cleared\n");
		kbd_readStatusRegister(&statusByte);
	}
	return 0;
}

int unsubscribeKeyboard() {
	unsigned long garbage, statusByte;
    /* Clean OutBuffer */
	kbd_readStatusRegister(&statusByte);
	while (statusByte & OBF){
		sys_inb(OUT_BUF,&garbage);
		printf("Garbage cleared\n");
		kbd_readStatusRegister(&statusByte);
	}

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

int keyboardEnable(){
	int counterError = 0;
    unsigned long garbage, statusByte;
	while((sendCommandToKBC(SEND_CMD)!=OK || sendCommandToKBC(ENABLE_KEYBOARD)!=OK) && counterError < 5){
		counterError++;
		if (counterError == 4){
			return 1;
		}
		tickdelay(micros_to_ticks(DELAY_US));
	}
    /* Clean OutBuffer */
	readStatusRegister(&statusByte);
	while (statusByte & OBF){
		sys_inb(OUT_BUF,&garbage);
		printf("Garbage cleared\n");
        readStatusRegister(&statusByte);
	}
	return 0;
}

int keyboardDisable(){
	int counterError = 0;
	unsigned long garbage, statusByte;

    /* Clean OutBuffer */
	readStatusRegister(&statusByte);
	while (statusByte & OBF){
		sys_inb(OUT_BUF,&garbage);
		printf("Garbage cleared\n");
        readStatusRegister(&statusByte);
	}
	while((sendCommandToKBC(SEND_CMD)!=OK || sendCommandToKBC(DISABLE_KEYBOARD)!=OK) && counterError < 5){
		counterError++;
		if (counterError == 4){
			return 1;
		}
		tickdelay(micros_to_ticks(DELAY_US));
	}
	return 0;
}
