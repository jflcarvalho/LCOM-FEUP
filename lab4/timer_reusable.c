#include "timer_reusable.h"


int timer_subscribe_int(unsigned int *hookID_timer) {
	if (sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, hookID_timer) != OK) { //esta kernel call vai alterar o valor de hookIDTemp
		printf("Error calling sys_irqsetpolicy\n");
		return 1;
	}
	if (sys_irqenable(hookID_timer) != OK) {
		printf("Error calling sys_irqenable\n");
		return 1;
	}
	return 0;

}

int timer_unsubscribe_int(unsigned int *hookID_timer) {

	if (sys_irqdisable(hookID_timer) != OK) {
		printf("Error disabling IRQ\n");
		return 1;
	}

	if (sys_irqrmpolicy(hookID_timer) != OK) {
		printf("Error removing policy\n");
		return 1;
	}

	return 0;
}


int timer_set_square(unsigned long timer, unsigned long freq) {
	if (freq < 19){
		printf("Impossible to set frequencies lower than 19");
		return 1;
	}

	if (timer < TIMER_0 || timer > TIMER_2) {
		printf("Timer 0x%x does not exist\n", timer);
		return 1;
	}
	unsigned long programmingTimerCMD = (TIMER_SEL0 | TIMER_LSB_MSB
			| TIMER_SQR_WAVE | TIMER_BIN);
	int valueToInsertInTimer = TIMER_FREQ / freq; //realFrequency = clock/ valor inserido;
	char LSB = (char) valueToInsertInTimer;
	char MSB = (char) (valueToInsertInTimer >> 8);
	if (sys_outb(TIMER_CTRL, programmingTimerCMD) != OK) {
		printf("Error writing on Control Register\n");
		return 1;
	}
	if (sys_outb(timer, LSB) != OK) {
		printf("Error loading timer with LSB\n");
		return 1;
	}

	if (sys_outb(timer, MSB) != OK) {
		printf("Error loading timer with MSB\n");
		return 1;
	}
	return 0;
}
