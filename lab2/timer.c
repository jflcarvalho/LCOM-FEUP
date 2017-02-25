#include <minix/syslib.h>
#include <minix/drivers.h>
#include "i8254.h"
#include <minix/com.h>

//Global Variables declaration
int hookID = 0;
int counter;

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

int timer_subscribe_int(void) {
	if (sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hookID) != OK) { //esta kernel call vai alterar o valor de hookIDTemp
		printf("Error calling sys_irqsetpolicy\n");
		return 1;
	}
	if (sys_irqenable(&hookID) != OK) {
		printf("Error calling sys_irqenable\n");
		return 1;
	}
	return 0;

}

int timer_unsubscribe_int() {

	if (sys_irqdisable(&hookID) != OK) {
		printf("Error disabling IRQ\n");
		return 1;
	}

	if (sys_irqrmpolicy(&hookID) != OK) {
		printf("Error removing policy\n");
		return 1;
	}

	return 0;
}

void timer_int_handler() {
	counter++;
	if (counter % 60 == 0) {
		printf("1 Second Went By\n");
	}

}

int timer_get_conf(unsigned long timer, unsigned char *st) {
	if (timer < 0 || timer > 2) //verify if timer is correct
	{
		printf("Timer %u does not exist\n", timer);
		return 1;
	}
	char RBCMD = TIMER_RB_CMD | TIMER_RB_SEL(timer); // CMD = 1100XXX0 ->X depend on TIMER_RB_SEL(timer)
	if (sys_outb(TIMER_CTRL, RBCMD) != OK) {
		printf("Error writing read back command on control register\n");
		return 1;
	}
	unsigned long stLong = (long) st;
	unsigned long TIMER_ADRESS = TIMER_0 + timer;
	if (sys_inb(TIMER_ADRESS, &stLong) != OK) {
		printf("Error reading from timer %b\n", timer);
		return 1;
	}
	*st = (char) stLong;
	return 0;
}

int timer_display_conf(unsigned char conf) {
	unsigned int outputCMD = conf & TIMER_OUT;
	unsigned int nullCount = conf & TIMER_NC;
	unsigned int typeOfAccess = conf & TIMER_LSB_MSB;
	unsigned int operatingMode = conf & TIMER_MODE;
	unsigned int BCD = conf & TIMER_BCD;
	//timer status
	printf("Timer Status byte: 0x%x\n", conf);
	//output command
	printf("Output Value: ");
	if (outputCMD) {
		printf("%d\n", 1);
	} else {
		printf("%d\n", 0);
	}

	//null count
	printf("Null Count: ");
	if (nullCount) {
		printf("%d\n", 1);
	} else {
		printf("%d\n", 0);
	}

	//type of access
	printf("Type of Access: ");
	if (typeOfAccess == TIMER_LSB) {
		printf("LSB\n");
	} else if (typeOfAccess == TIMER_MSB) {
		printf("MSB\n");
	} else if (typeOfAccess == TIMER_LSB_MSB) {
		printf("LSB + MSB\n");
	}

	//operating mode
	printf("Operating Mode: ");
	switch (operatingMode) {
	case TIMER_INTERRUPT_ON_TERMINAL_COUNT:
		printf("Timer Interrupt On Terminal Count (0)\n");
		break;
	case TIMER_ONE_SHOT:
		printf("Hardware Retriggerable One-Shot (1)\n");
		break;
	case TIMER_RATE_GEN:
		printf("Rate Generator (2)\n");
		break;
	case TIMER_SQR_WAVE:
		printf("Square Wave (3)\n");
		break;
	case TIMER_SOFT_STROBE:
		printf("Software Triggered Strobe (4)\n");
		break;
	case TIMER_HARD_STROBE:
		printf("Hardware Triggered Strobe (5)\n");
		break;
	default:
		printf("Operating Mode not recognized\n");
		return 1;
		break;
	}
	printf("BCD: ");
	if (BCD) {
		printf("BCD(%d)\n", 1);
	} else {
		printf("Binario(%d)\n", 0);
	}

	return 0;
}

int timer_test_square(unsigned long freq) {
	if (timer_set_square(TIMER_0, freq) != OK) {
		printf("Error setting frequency of timer %x\n", TIMER_0);
		return 1;
	}
	return 0;
}

int timer_test_int(unsigned long time) {

	//verify args
	if (time < 1) {
		printf("Expecified time is invalid\n");
		return 1;
	}

	//initialization of global variable
	counter = 0;

	time = time * 60;
	int ipc_status, r;
	message msg;
	int irq_set = BIT(hookID);

	if (timer_test_square(DEFAULT_FREQ) != OK) {
		printf("Error testing timer frequency");
		return 1;
	}

	if (timer_subscribe_int() != OK) {
		printf("Subscribe error\n");
		return 1;
	}

	while (time > 0) { /* You may want to use a different condition */
		/* Get a request message. */
		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d\n", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) { /* received notification of kernel*/
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: /* hardware interrupt notification */
				if (msg.NOTIFY_ARG & irq_set) {/* subscribed interrupt */
					//printf ("Interrupcao\n");
					timer_int_handler();
					time--;
				}
				break;
			default:
				printf("Default notification\n");
				break; /* no other notifications expected: do nothing */
			}
		} else { /* received a standard message, not a notification */
			/* no standard messages expected: do nothing */
		}
	}

	if (timer_unsubscribe_int() != OK) {
		printf("Unsubscribe error\n");
		return 1;
	}

	return 0;
}

int timer_test_config(unsigned long timer) {
	if (timer < 0 || timer > 2) //verify if timer is correct
		{
			printf("Timer %u does not exist\n", timer);
			return 1;
		}

	unsigned char conf;

	if (timer_get_conf(timer, &conf) != OK) {
		return 1;
	}

	timer_display_conf(conf);
	return 0;
}
