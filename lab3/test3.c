#include "test3.h"

unsigned int hookID_keyboard=1;
unsigned int hookID_timer= 0;

int kbd_test_scan(unsigned short ass) {
	//Should clear the kbc out_buf before subscribing the keyboard;
	unsigned long garbage, statusByte;
	unsigned long  scancode = 0;
	unsigned long firstScan;
	int isDoubleByte = 0;
	int ipc_status, r;
	message msg;
	int irq_set = BIT(hookID_keyboard);

	//subscribe the keyboard
	if (subscribeKeyboard(&hookID_keyboard)){
		printf("Error subscribing keyboard\n");
	}
	readStatusRegister(&statusByte);
	if (statusByte & OBF){
		sys_inb(OUT_BUF,&garbage);
		printf("Garbage cleared\n");
	}

	while (scancode != ESC_KEY) { /* key pressed != ESC */
		/* Get a request message. */
		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d\n", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) { /* received notification of kernel*/
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: /* hardware interrupt notification */
				if (msg.NOTIFY_ARG & irq_set) {/* subscribed interrupt */
					if(ass == 0){
						readStatusRegister(&statusByte);
						if ( (statusByte & (PAR_ERR | TO_ERR)) != OK){
							printf("Invalid Data on Out_Buffer\n");
							return -1;
						}
						if (statusByte & OBF != 0) {//&& (statusByte & (PAR_ERR | TO_ERR) == 0))
							readOutBuffer(&scancode);
							if (scancode == DOUBLEBYTE){
								isDoubleByte = 1;
								firstScan = scancode;
							}
							else if (isDoubleByte == 1){
								if (scancode & BIT7){
									printf("Break Code: 0x%x%x ; \n",firstScan,scancode);
								}
								else {
									printf("Make Code: 0x%x%x ; \n",firstScan,scancode);
								}
								isDoubleByte = 0;
							}
							else if (isDoubleByte == 0){
								if (scancode & BIT7) { // break code
									printf("Break code: 0x%x ; \n", scancode);
								}
								else { // break code
									printf("Make code : 0x%x ; \n", scancode);

								}
							}
						}

					}
					else if (ass == 1){
						scancode = keyboardHandler_ASM();
						if (scancode & BIT7 == 0){
							printf("Make code : %x", scancode);
						}
						if (scancode & BIT7){
							printf("Break code : %x\n", scancode);
						}

					}
					tickdelay(micros_to_ticks(WAIT_KBC));
				}
				break;
			default:
				printf("Default notification\n");
				break; /* no other notifications expected: do nothing */
			}
		}
	}
	if (unsubscribeKeyboard(&hookID_keyboard) != OK){
		printf("Error unsubscribing keyboard\n");
		return 0;
	}

	//clear garbage
	readStatusRegister(&statusByte);
	if (statusByte & OBF){
		sys_inb(OUT_BUF,&garbage);
		printf("Garbage cleared\n");
	}

	return 0;
}



int kbd_test_leds(unsigned short n, unsigned short *leds) {
	if (n < 1){
		printf("Expecified length of array is invalid\n");
		return 1;
	}

	//Declaration of variables
	int ipc_status;
	int r;
	int time = n * 60;
	int counter = 0;
	int nLed = 0;
	int irq_set = BIT(hookID_timer);
	unsigned int ledMask;
	message msg;

	//verify if time is correct
	if (timer_set_square(TIMER_0, DEFAULT_FREQ) != OK) {
		printf("Error setting frequency of timer %x\n", TIMER_0);
		return 1;
	}
	printf("Debugger: Timer freq set to default!\n");

	//subscribe timer
	if (timer_subscribe_int(&hookID_timer) != OK) {
		printf("Subscribe error\n");
		return 1;
	}

	printf("Debugger: Timer IRQ subscribed!\n");


	while (time > 0) { /* You may want to use a different condition */
		/* Get a request message. */
		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d\n", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) /* received notification of kernel*/
		{
			switch (_ENDPOINT_P(msg.m_source))
			{
			case HARDWARE: /* hardware interrupt notification */
				if (msg.NOTIFY_ARG & irq_set) {/* subscribed interrupt */
					//printf ("Interrupcao\n");
					counter++;
					if (counter % 60 == 0) {
						ledMask ^= LED_SELECTOR(leds[nLed]);
						if (kbd_set_led(ledMask) != OK){
							printf("Some error ocurred in kbd_set_led\n");
						}
						nLed++;
					}
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

	//unsubscribe timer
	if (timer_unsubscribe_int(&hookID_timer) != OK) {
		printf("Unsubscribe error\n");
		return 1;
	}

	return 0;
}
int kbd_test_timed_scan(unsigned short n) {
	if (n < 1){
		printf("Expecified time is invalid\n");
		return 1;
	}

	//Declaration of variables
	//Should clear the kbc out_buf before subscribing the keyboard;
	unsigned long garbage, statusByte;
	unsigned long  scancode = 0;
	unsigned long firstScan;
	int isDoubleByte = 0;
	int ipc_status;
	int r;
	int time = n * 60;
	int counter = 0;
	int irq_set_timer = BIT(hookID_timer);
	int irq_set_keyboard = BIT(hookID_keyboard);
	message msg;

	//verefy if time is correct
	if (timer_set_square(TIMER_0, DEFAULT_FREQ) != OK) {
		printf("Error setting frequency of timer %x\n", TIMER_0);
		return 1;
	}
	printf("Debugger: Timer freq set to default!\n");

	//subscribe timer
	if (timer_subscribe_int(&hookID_timer) != OK) {
		printf("Subscribe error\n");
		return 1;
	}

	printf("Debugger: Timer IRQ subscribed!\n");

	//subscribe the keyboard
	if (subscribeKeyboard(&hookID_keyboard)){
		printf("Error subscribing keyboard\n");
	}
	readStatusRegister(&statusByte);
	if (statusByte & OBF){
		sys_inb(OUT_BUF,&garbage);
		printf("Garbage cleared\n");
	}

	while (scancode != ESC_KEY) { /* You may want to use a different condition */
		/* Get a request message. */
		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d\n", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) /* received notification of kernel*/
		{
			switch (_ENDPOINT_P(msg.m_source))
			{
			case HARDWARE: /* hardware interrupt notification */
				if (msg.NOTIFY_ARG & irq_set_timer) {/* subscribed interrupt */
					//printf ("Interrupcao\n");
					counter++;
					if (counter == time) {
						printf("\nTime Out ...\n");
						return 0;
					}
				}
				if (msg.NOTIFY_ARG & irq_set_keyboard) {/* subscribed interrupt */
					readStatusRegister(&statusByte);
					counter = 0;
					if (statusByte & OBF){
						readOutBuffer(&scancode);
						if (scancode == DOUBLEBYTE){
							isDoubleByte = 1;
							firstScan = scancode;
						}
						else if (isDoubleByte == 1){
							if (scancode & BIT7){
								printf("Break Code: 0x%x%x ; \n",firstScan,scancode);
							}
							else {
								printf("Make Code: 0x%x%x ; \n",firstScan,scancode);
							}
							isDoubleByte = 0;
						}
						else if (isDoubleByte == 0){
							if (scancode & BIT7) { // break code
								printf("Break code: 0x%x ; \n", scancode);
							}
							else { // break code
								printf("Make code : 0x%x ; \n", scancode);

							}
						}

					}
					tickdelay(micros_to_ticks(WAIT_KBC));
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

	//unsubscribe timer
	if (timer_unsubscribe_int(&hookID_timer) != OK) {
		printf("Unsubscribe error\n");
		return 1;
	}

	//unsubscribe keyboard
	if (unsubscribeKeyboard(&hookID_keyboard) != OK){
		printf("Error unsubscribing keyboard\n");
		return 0;
	}

	return 0;
}
