#include "test4.h"


unsigned int hookID_timer= 0;

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

int subscribeMouse(unsigned int *hookID_mouse){
	if (sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE|IRQ_EXCLUSIVE , hookID_mouse) != OK) { //esta kernel call vai alterar o valor de hookID_mouseTemp
		printf("Error calling sys_irqsetpolicy\n");
		return 1;
	}
	if (sys_irqenable(hookID_mouse) != OK) {
		printf("Error calling sys_irqenable\n");
		return 1;
	}
	return 0;
}

int unsubscribeMouse(unsigned int *hookID_mouse) {

	if (sys_irqdisable(hookID_mouse) != OK) {
		printf("Error disabling mouse IRQ\n");
		return 1;
	}

	if (sys_irqrmpolicy(hookID_mouse) != OK) {
		printf("Error removing mouse policy\n");
		return 1;
	}

	return 0;
}

int mouseEnable(){
	int counterError = 0;
	while((sendCommandToKBC(SEND_CMD)!=OK || sendCommandToKBC(ENABLE_MOUSE)!=OK) && counterError < 5){
		counterError++;
		if (counterError == 4){
			return -1;
		}
		tickdelay(micros_to_ticks(DELAY_US));
	}
	return 0;
}

int mouseDisable(){
	int counterError = 0;
	while((sendCommandToKBC(SEND_CMD)!=OK || sendCommandToKBC(DISABLE_MOUSE)!=OK) && counterError < 5){
		counterError++;
		if (counterError == 4){
			return -1;
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
void displayPacket(unsigned long *packet){
	signed char xDelta = packet[1];
	signed char yDelta = packet[2];
	unsigned char x = 0;
	unsigned char y = 0;
	if (!(packet[0] & BIT(4)) && (xDelta & BIT(7))){
		printf ("invoked x ");
		x = (unsigned char) xDelta;
	}
	if (!(packet[0] & BIT(5)) && (yDelta & BIT(7))){
		printf ("invoked y ");
		y = (unsigned char) yDelta;
	}
	printf("B1=0x%02X ",packet[0]);
	printf("B2=0x%02X ",packet[1]);
	printf("B3=0x%02X ",packet[2]);
	printf("LB=%u ",(packet[0] & BIT(1))>>1);
	printf("MB=%u ", (packet[0] & BIT(2))>>2);
	printf("RB=%u ",packet[0] & BIT(0));
	printf("XOV=%u ",(packet[0] & BIT(6))>>6);
	printf("YOV=%u ",(packet[0] & BIT(7))>>7);
	if (x != 0){
		printf("X=%d ",x);
	}
	else{
		printf("X=%d ",xDelta);
	}
	if (y != 0){
		printf("Y=%d\n",y);
	}else{
		printf("Y=%d\n",yDelta);
	}

}


int test_packet(unsigned short cnt){
	int hookID_mouse = 12;
	unsigned long packet[3];
	unsigned long garbage, outBuffer, statusByte;
	message msg;
	int irq_set_mouse = BIT(hookID_mouse);
	int counterError = 0;
	int packetRead = 0;
	int packetCounter = 0;
	int ipc_status, r;

	if (cnt < 1){
		printf("Error - Cnt should be bigger than 0\n");
		return 1;
	}

	//subscribe the mouse
	if (subscribeMouse(&hookID_mouse)){
		printf("Error subscribing mouse\n");
	}

	//enable mouse
	if (mouseEnable() != OK){
		printf("Can't Enable Data Reporting\n");
		return 1;
	}


	readStatusRegister(&statusByte);
	if (statusByte & OBF){
		sys_inb(OUT_BUF,&garbage);
		printf("Garbage cleared\n");
	}


	while (cnt > 0) {
		/* Get a request message. */
		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d\n", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) { /* received notification of kernel*/
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: /* hardware interrupt notification */
				if (msg.NOTIFY_ARG & irq_set_mouse) {/* subscribed interrupt */
					readOutBuffer(&outBuffer);
					if (((outBuffer & FIRST_BYTE) && packetCounter == 0) || packetCounter > 0 ){
						packet[packetCounter] = outBuffer;
						packetCounter++;
					}
					if (packetCounter == 3){
						packetCounter = 0;
						packetRead++;
					}
					if (packetRead == 1){
						displayPacket(packet);
						packetRead--;
						cnt--;
					}

				}
				break;
			default:
				printf("Default notification\n");
				break; /* no other notifications expected: do nothing */
			}
		}
	}

	if (mouseDisable() != OK){
		printf("Error disabling stream mode\n");
	}
	if (unsubscribeMouse(&hookID_mouse)){
		printf("Error subscribing mouse\n");
	}
	readStatusRegister(&statusByte);
	if (statusByte & OBF){
		sys_inb(OUT_BUF,&garbage);
	}
	printf("\nAll Packets have been read, press Enter to Continue ...\n");

	return 0;
}	

int test_async(unsigned short idle_time) {

	int hookID_mouse = 12;
	unsigned long packet[3];
	unsigned long garbage, outBuffer, statusByte;
	message msg;
	int irq_set_mouse = BIT(hookID_mouse);
	int irq_set_timer = BIT(hookID_timer);
	int counterError = 0;
	int packetRead = 0;
	int packetCounter = 0;
	int ipc_status, r;
	int time = idle_time * 60;
	int counter = 0;

	if (idle_time < 1){
		printf("Error - idle_time should be bigger than 0\n");
		return 1;
	}

	//verefy if time is correct
	if (timer_set_square(TIMER_0, DEFAULT_FREQ) != OK) {
		printf("Error setting frequency of timer %x\n", TIMER_0);
		return 1;
	}

	//subscribe timer
	if (timer_subscribe_int(&hookID_timer) != OK) {
		printf("Subscribe error\n");
		return 1;
	}

	//subscribe the mouse
	if (subscribeMouse(&hookID_mouse)){
		printf("Error subscribing mouse\n");
	}

	//enable mouse
	if (mouseEnable() != OK){
		printf("Can't Enable Data Reporting\n");
		return 1;
	}


	readStatusRegister(&statusByte);
	if (statusByte & OBF){
		sys_inb(OUT_BUF,&garbage);
		printf("Garbage cleared\n");
	}


	while (counter < time) {
		// Get a request message.
		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d\n", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) { // received notification of kernel
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: // hardware interrupt notification
				if (msg.NOTIFY_ARG & irq_set_mouse) {// subscribed mouse interrupt
					readOutBuffer(&outBuffer);
					if (((outBuffer & FIRST_BYTE) && packetCounter == 0) || packetCounter > 0 ){
						packet[packetCounter] = outBuffer;
						packetCounter++;
					}
					if (packetCounter == 3){
						packetCounter = 0;
						packetRead++;
					}
					if (packetRead == 1){
						displayPacket(packet);
						packetRead--;
					}
					counter = 0;
				}
				if (msg.NOTIFY_ARG & irq_set_timer) {// subscribed interrupt
					//printf ("Interrupcao\n");
					counter++;
				}
				break;
			default:
				printf("Default notification\n");
				break; // no other notifications expected: do nothing
			}
		}
	}

	//unsubscribe timer
	if (timer_unsubscribe_int(&hookID_timer) != OK) {
		printf("Unsubscribe error\n");
		return 1;
	}
	if (mouseDisable() != OK){
		printf("Error disabling stream mode\n");
	}
	//unsubcribe mouse
	if (unsubscribeMouse(&hookID_mouse)){
		printf("Error subscribing mouse\n");
	}
	readStatusRegister(&statusByte);
	if (statusByte & OBF){
		sys_inb(OUT_BUF,&garbage);
	}
	printf("\nTime Out. Press Enter to Continue ...\n");

	return 0;
}

void display_mouse_config(unsigned long *mouseResponse){
	printf("Byte1: 0x%02X ",mouseResponse[0]);
	printf("Byte2: 0x%02X ",mouseResponse[1]);
	printf("Byte3: 0x%02X\n",mouseResponse[2]);
	printf("Remote: %d ", (mouseResponse[0]&BIT(6))>>6);
	printf("Enable: %d ", (mouseResponse[0]&BIT(5))>>5);
	printf("Scaling: %d ", (mouseResponse[0]&BIT(4))>>4);
	printf("LB: %d ", (mouseResponse[0]&BIT(2))>>2);
	printf("MB: %d ", (mouseResponse[0]&BIT(1))>>1);
	printf("RB: %d ", (mouseResponse[0]&BIT(0)));
	printf("Res: 0x%01X ", mouseResponse[1]);
	printf("Samp.Rate: 0x%02X\n", mouseResponse[2]);
}

int test_config(void) {

	int hookID_mouse = 12;
	int irq_set_mouse = BIT(hookID_mouse);
	unsigned long mouseResponse[3];
	unsigned long outBuffer,statusByte,garbage;
	char isFinished = 0;
	char count = 0;

	if (subscribeMouse(&hookID_mouse)){
		printf("Error subscribing mouse\n");
	}
	readStatusRegister(&statusByte);
	if (statusByte & OBF){
		sys_inb(OUT_BUF,&garbage);
		printf("Garbage cleared\n");
	}

	if (sendCommandToKBC(STATUS_REQUEST) != OK){
		printf("Error...\n");

		if (unsubscribeMouse(&hookID_mouse)){
			printf("Error subscribing mouse\n");
		}
		readStatusRegister(&statusByte);
		if (statusByte & OBF){
			sys_inb(OUT_BUF,&garbage);
		}
		return 1;
	}
	while (count < 3){
		readStatusRegister(&statusByte);
		if (statusByte & OBF){
			sys_inb(OUT_BUF,&mouseResponse[count++]);
		}

	}

	printf("\nDisplaying Config ...\n");

	display_mouse_config(mouseResponse);

	if (unsubscribeMouse(&hookID_mouse)){
		printf("Error subscribing mouse\n");
	}
	readStatusRegister(&statusByte);
	if (statusByte & OBF){
		sys_inb(OUT_BUF,&garbage);
	}

	return 0;
}


typedef enum {INIT, DRAW, COMP} state_t;
typedef enum {RDOW, RUP} ev_type_t;
typedef enum {PSLOPE,NSLOPE} slope_t;

state_t state = INIT; // initial state; keep state
struct event_t{
	slope_t slope;
	ev_type_t type;
	signed short x_delta;
	signed short y_delta;
	signed short x_delta_last;
	signed short y_delta_last;
	short length;
	int n_packet;
};


void handle_event(struct event_t *evt) {
	float tolerance = 0.125;
	float prevMeanSlope = (float)(evt->y_delta_last/ evt->x_delta_last);
	float actualMeanSlope = (float)(evt->y_delta/ evt->x_delta);


	switch (state) {
	case INIT:
		if( evt->type == RDOW )
			state = DRAW;
		break;
	case DRAW:
		if( evt->type == RDOW ) {
			evt->n_packet++;
			if (evt->n_packet == 1){
				if ((abs(evt->y_delta) > evt->length) && evt->slope == PSLOPE){
					state = COMP;
				}
			}
			else{
				if ((abs(actualMeanSlope - prevMeanSlope) < tolerance) && evt->slope == PSLOPE){
					if (abs(evt->y_delta) > evt->length){
						state = COMP;

					}
					else{
						evt->x_delta_last = evt->x_delta;
						evt->y_delta_last = evt->y_delta;
					}
				}
			}

		} else if( evt->type == RUP )
			state = INIT;
			evt->n_packet = 0;
			evt->x_delta = 0;
			evt->y_delta = 0;
			evt->x_delta_last = 0;
			evt->y_delta_last = 0;

		break;
	default:
		break;
	}
}

int test_gesture(short length) {
	//verify args
	if (length < 1){
		printf("Error - Length should be bigger than 0\n");
		return 1;
	}

	int hookID_mouse = 12;
	unsigned long packet[3];
	unsigned long garbage, outBuffer, statusByte;
	message msg;
	int irq_set_mouse = BIT(hookID_mouse);
	int counterError = 0;
	int packetRead = 0;
	int packetCounter = 0;
	short fivePacketsCounter = 0;
	int ipc_status, r;
	int cnt, m = 0;
	printf ("Make a positive slope while pressing the right button to exit!\n");
	struct event_t mouse_event;
	mouse_event.x_delta = 0;
	mouse_event.y_delta = 0;
	mouse_event.x_delta_last = 0;
	mouse_event.y_delta_last = 0;
	mouse_event.length = length;
	mouse_event.n_packet = 0;
	mouse_event.slope = NSLOPE;
	signed char xAndYValues[2];

	printf("Debug step: 2\n");
	//subscribe the mouse
	if (subscribeMouse(&hookID_mouse)){
		printf("Error subscribing mouse\n");
	}

	//enable mouse
	if (mouseEnable() != OK){
		printf("Can't Enable Data Reporting\n");
		return 1;
	}

	readStatusRegister(&statusByte);
	if (statusByte & OBF){
		sys_inb(OUT_BUF,&garbage);
		printf("Garbage cleared\n");
	}

	while (state != COMP) {
		/* Get a request message. */
		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d\n", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) { /* received notification of kernel*/
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: /* hardware interrupt notification */
				if (msg.NOTIFY_ARG & irq_set_mouse) {/* subscribed interrupt */
					readOutBuffer(&outBuffer);
					if (((outBuffer & FIRST_BYTE) && packetCounter == 0) || packetCounter > 0 ){
						packet[packetCounter] = outBuffer;
						packetCounter++;
					}
					if (packetCounter == 3){
						packetCounter = 0;
						packetRead++;
					}
					if (packetRead == 1){
						displayPacket(packet);
						packetRead--;
						if (packet[0] & RIGHT_BUTTON){
							fivePacketsCounter++;
							xAndYValues[0] = packet[1];
							xAndYValues[1] = packet[2];
							mouse_event.x_delta += xAndYValues[0];
							mouse_event.y_delta += xAndYValues[1];
							if(fivePacketsCounter == 5){
								fivePacketsCounter = 0;
								if ((mouse_event.y_delta / (float)mouse_event.x_delta)>0){
									mouse_event.type = RDOW;
									mouse_event.slope = PSLOPE;
									handle_event(&mouse_event);
								}
								else {
									mouse_event.type = RDOW;
									mouse_event.slope = NSLOPE;
									handle_event(&mouse_event);

								}
							}
						}
						else{
								//printf("Right Button Not Pressed");
								mouse_event.type = RUP;
								fivePacketsCounter = 0;
								handle_event(&mouse_event);
							}

					}
				}
				break;
			default:
				printf("Default notification\n");
				break; /* no other notifications expected: do nothing */
			}
		}

	}

	printf("\n Congratulations, you are good at making straight lines\n");

	if (mouseDisable() != OK){
			printf("Error disabling stream mode\n");
		}
	if (unsubscribeMouse(&hookID_mouse)){
		printf("Error subscribing mouse\n");
	}
	readStatusRegister(&statusByte);
	if (statusByte & OBF){
		sys_inb(OUT_BUF,&garbage);
	}

	return 0;
}
