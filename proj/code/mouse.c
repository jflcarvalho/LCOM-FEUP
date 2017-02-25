#include <minix/syslib.h>
#include <minix/drivers.h>
#include "i8042.h"
#include "i8254.h"
#include "mouse.h"

static int hookID_mouse = MOUSE_IRQ;

unsigned int readStatusRegister(unsigned long *statusByte){
	if (sys_inb(STATUS_REG,statusByte) != OK){
		printf("Error reading Status Byte\n");
		return 1;
	}
	return 0;
}

unsigned int readOutBuffer(unsigned long *outBufValue){
	if (sys_inb(OUT_BUF,outBufValue)!= OK){
		printf("Error reading output buffer\n");
		return 1;
	}
	return 0;
}

int subscribeMouse(){
    hookID_mouse = MOUSE_IRQ;
	if (sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE|IRQ_EXCLUSIVE , &hookID_mouse) != OK) { //esta kernel call vai alterar o valor de hookID_mouseTemp
		printf("Error calling sys_irqsetpolicy\n");
		return 1;
	}
	if (sys_irqenable(&hookID_mouse) != OK) {
		printf("Error calling sys_irqenable\n");
		return 1;
	}
    if (mouseEnable() != OK){
		printf("Can't Enable Data Reporting\n");
		return 1;
	}
	return 0;
}

int unsubscribeMouse() {
    if (mouseDisable() != OK){
		printf("Can't Disanable Data Reporting\n");
		return 1;
	}
	if (sys_irqdisable(&hookID_mouse) != OK) {
		printf("Error disabling mouse IRQ\n");
		return 1;
	}

	if (sys_irqrmpolicy(&hookID_mouse) != OK) {
		printf("Error removing mouse policy\n");
		return 1;
	}
	return 0;
}

int mouseEnable(){
	int counterError = 0;
    unsigned long garbage, statusByte;
	while((sendCommandToKBC(SEND_CMD)!=OK || sendCommandToKBC(ENABLE_MOUSE)!=OK) && counterError < 5){
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

int mouseDisable(){
	int counterError = 0;
	unsigned long garbage, statusByte;

    /* Clean OutBuffer */
	readStatusRegister(&statusByte);
	while (statusByte & OBF){
		sys_inb(OUT_BUF,&garbage);
		printf("Garbage cleared\n");
        readStatusRegister(&statusByte);
	}
	while((sendCommandToKBC(SEND_CMD)!=OK || sendCommandToKBC(DISABLE_MOUSE)!=OK) && counterError < 5){
		counterError++;
		if (counterError == 4){
			return 1;
		}
		tickdelay(micros_to_ticks(DELAY_US));
	}
	return 0;
}

int sendCommandToKBC(unsigned long cmd){
	int repeat = 1;
	int counterError = 0;
	while (repeat == 1 && counterError < 4){
		unsigned long deviceResponse;
		sys_outb(KBC_AUX_BUF,WRITEB_MOUSE);
		sys_outb(OUT_BUF,cmd);
		tickdelay(micros_to_ticks(DELAY_US));
		sys_inb(0x60,&deviceResponse);
		if (deviceResponse != RESEND || deviceResponse != ERROR){
			repeat = 0;
		}
		else counterError++;

		tickdelay(micros_to_ticks(DELAY_US));
	}
	if (counterError == 5){
		return 1;
	}
	return 0;
}
