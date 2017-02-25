#include "timer_reusable.h"

static int hookID_timer;

int timer_waitXSeconds(long secondsToWait){

	if (timer_subscribe_int() != OK){
		printf("Error subscribing timer\n");
		return 1;
	}
		message msg;
		int irq_set_timer = BIT(TIMER0_IRQ);
		int ipc_status, r;
		secondsToWait = secondsToWait * DEFAULT_FREQ;

	while (secondsToWait > 0) {
		// Get a request message.
		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d\n", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) { // received notification of kernel
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: // hardware interrupt notification
				if (msg.NOTIFY_ARG & irq_set_timer) {// subscribed interrupt
					secondsToWait--;
				}
				break;
			default:
				printf("Default notification\n");
				break; // no other notifications expected: do nothing
			}
		}
	}

	//unsubscribe timer
	if (timer_unsubscribe_int() != OK) {
		printf("Unsubscribe error\n");
		return 1;
	}
	return 0;
}

int timer_subscribe_int(void) {
	if (sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hookID_timer) != OK) { //esta kernel call vai alterar o valor de hookIDTemp
		printf("Error calling sys_irqsetpolicy\n");
		return 1;
	}
	if (sys_irqenable(&hookID_timer) != OK) {
		printf("Error calling sys_irqenable\n");
		return 1;
	}
	return 0;

}

int timer_unsubscribe_int(void) {

	if (sys_irqdisable(&hookID_timer) != OK) {
		printf("Error disabling IRQ\n");
		return 1;
	}

	if (sys_irqrmpolicy(&hookID_timer) != OK) {
		printf("Error removing policy\n");
		return 1;
	}

	return 0;
}


int timer_set_square(unsigned long timer, unsigned long freq) {
	if (freq < TIMER_MIN_FREQ){
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
