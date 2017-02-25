#include "test5.h"
#define MODE105H 		0x105
#define DAC8BITS 		BIT(0)
#define VGA_COMPATIBLE 	BIT(1)
#define RAMDAC 			BIT(2)
unsigned int hookID_keyboard=1;

void* test_init(unsigned short mode, unsigned short delay) {
	void *FBVirtualAddress = (void*)vg_init(mode);
	timer_waitXSeconds((long)delay);
	vg_exit();
	vg_printPhysAddress();
	return FBVirtualAddress;
}


int test_square(unsigned short x, unsigned short y, unsigned short size, unsigned long color) {
	unsigned long garbage, statusByte;
	unsigned long  scancode = 0;
	int ipc_status, r;
	message msg;

	int irq_set = BIT(KEYBOARD_IRQ);

	int i,j;
	char * videoMemAddress = (char*)vg_init(MODE105H);
	if (videoMemAddress == NULL){
		return 1;
	}

	for (i = y; i < y+size;i++){
		for (j = x; j < x+size;j++){
			vg_setPixel(j,i,color,1);
		}
	}
	//subscribe the keyboard
	if (subscribeKeyboard()){
		vg_exit();
		printf("Error subscribing keyboard\n");
		return 1;
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
					readStatusRegister(&statusByte);
					printf("statusByte:0x%X",statusByte);
					if ( (statusByte & (PAR_ERR | TO_ERR)) != OK){
						printf("Invalid Data on Out_Buffer\n");
						vg_exit();
						return 1;
					}
					if (statusByte & OBF != 0) {
						readOutBuffer(&scancode);
					}
				}
				break;
			default:
				printf("Default notification\n");
				break; /* no other notifications expected: do nothing */
			}
		}

	}
	//unsubscribe the keyboard
	if (unsubscribeKeyboard() != OK){
		printf("Error unsubscribing keyboard\n");
		vg_exit();
		return 1;
	}

	vg_exit();

	return 0;
}

/*Extra Functions*/
int cena_marada(unsigned short xi,unsigned short yi, unsigned short size, unsigned short n){

	char * videoMemAddress = (char*)vg_init(MODE105H);
	if (videoMemAddress == NULL){
		return 1;
	}
	int i;
	int x = xi;
	int y = yi;
	unsigned long color;
	for (i = 0; i<n;i++){
		color = rand() %64;
		vg_drawLine(x+size/2 + size * i/(2*n),y,x+size,y+size*(i+1)/(2*n),color);
		vg_drawLine(x+size/2 - size * i/(2*n),y,x,y+size*(i+1)/(2*n),color);
		vg_drawLine(x+size/2 + size * i/(2*n),y+size,x+size,y+size-size*(i+1)/(2*n),color);
		vg_drawLine(x+size/2 - size * i/(2*n),y+size,x,y+size-size*(i+1)/(2*n),color);
	}

	timer_waitXSeconds(5);
	vg_exit();
	return 0;
}



/*End Of Extra Functions*/

int test_line(unsigned short xi, unsigned short yi, unsigned short xf, unsigned short yf, unsigned long color) {

	unsigned long garbage, statusByte;
	unsigned long  scancode = 0;
	int ipc_status, r;
	message msg;

	int irq_set = BIT(KEYBOARD_IRQ);


	char * videoMemAddress = (char*)vg_init(MODE105H);
	if (videoMemAddress == NULL){
		unsubscribeKeyboard();
		return 1;
	}

	if (vg_drawLine(xi,yi,xf,yf,color)){
		return 1;
	}

	//subscribe the keyboard
	if (subscribeKeyboard()){
		vg_exit();
		printf("Error subscribing keyboard\n");
		return 1;
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
					readStatusRegister(&statusByte);
					printf("statusByte:0x%X",statusByte);
					if ( (statusByte & (PAR_ERR | TO_ERR)) != OK){
						printf("Invalid Data on Out_Buffer\n");
						vg_exit();
						return 1;
					}
					if (statusByte & OBF != 0) {
						readOutBuffer(&scancode);
					}
				}
				break;
			default:
				printf("Default notification\n");
				break; /* no other notifications expected: do nothing */
			}
		}

	}
	//unsubscribe the keyboard
	if (unsubscribeKeyboard() != OK){
		printf("Error unsubscribing keyboard\n");
		vg_exit();
		return 1;
	}


	vg_exit();
	return 0;
}

int test_xpm(unsigned short xi, unsigned short yi, char *xpm[]) {
	unsigned long garbage, statusByte;
	unsigned long  scancode = 0;
	int ipc_status, r;
	message msg;

	int irq_set = BIT(KEYBOARD_IRQ);


	char * videoMemAddress = (char*)vg_init(MODE105H);
	if (videoMemAddress == NULL){
		return 1;
	}
	if (vg_DrawXPM(xi,yi,xpm,1)){
		vg_exit();
		return 1;
	}
	//subscribe the keyboard
	if (subscribeKeyboard()){
		vg_exit();
		printf("Error subscribing keyboard\n");
		return 1;
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
					readStatusRegister(&statusByte);
					if ( (statusByte & (PAR_ERR | TO_ERR)) != OK){
						vg_exit();
						printf("Invalid Data on Out_Buffer\n");
						return 1;
					}
					if (statusByte & OBF != 0) {
						readOutBuffer(&scancode);
					}
				}
				break;
			default:
				printf("Default notification\n");
				break; /* no other notifications expected: do nothing */
			}
		}

	}
	//unsubscribe the keyboard
	if (unsubscribeKeyboard() != OK){
		printf("Error unsubscribing keyboard\n");
		vg_exit();
		return 1;
	}


	vg_exit();
	return 0;
}	

int test_movePrev(unsigned short xi, unsigned short yi, char *xpm[], unsigned short hor, short delta, unsigned short time) {
	//WITHOUT USING SPRITE
	//makes use of double buffering
	char * videoMemAddress = (char*)vg_init(MODE105H);
	if (videoMemAddress == NULL){
		return 1;
	}

	if (timer_subscribe_int() != OK){
		printf("Error subscribing timer\n");
		return 1;
	}
	message msg;
	int irq_set_timer = BIT(TIMER0_IRQ);
	int ipc_status, r;
	int y = yi;
	int x = xi;
	int xf = xi + delta;
	int yf = yi + delta;
	int timerInterruptionsCounter = 0;
	vg_clearScreen(0); // cleans the double buffer

	while (x != xf && y != yf) {
		if (hor){
			x = xi + (int)((xf-xi)/time *timerInterruptionsCounter/DEFAULT_FREQ);
		}
		else {
			y = yi + (int)((yf-yi)/time * timerInterruptionsCounter/DEFAULT_FREQ);
		}
		if (vg_DrawXPM(x,y,xpm,0)){
			vg_exit();
			if (timer_unsubscribe_int() != OK) {
				printf("Unsubscribe timer error\n");
			}

			return 1;
		}

		// Get a request message.
		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d\n", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) { // received notification of kernel
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: // hardware interrupt notification
				if (msg.NOTIFY_ARG & irq_set_timer) {// subscribed interrupt
					timerInterruptionsCounter++;
					vg_writeToVRAM();
					vg_clearScreen(0);
				}
				break;
			default:
				printf("Default notification\n");
				break; // no other notifications expected: do nothing
			}
		}
	}

	//exit from graphics mode
	vg_exit();
	printf("Exit from graphics mode, time out...\n");

	//unsubscribe timer
	if (timer_unsubscribe_int() != OK) {
		printf("Unsubscribe timer error\n");
		return 1;
	}
	return 0;

}

int test_move(unsigned short xi, unsigned short yi, char *xpm[], unsigned short hor, short delta, unsigned short time) {
	//Using Sprite and double buffering
	char * videoMemAddress = (char*)vg_init(MODE105H);
	if (videoMemAddress == NULL){
		return 1;
	}

	if (timer_subscribe_int() != OK){
		printf("Error subscribing timer\n");
		return 1;
	}
	unsigned long garbage, statusByte;
	unsigned long  scancode = 0;
	int irq_set_keyboard = BIT(KEYBOARD_IRQ);
	message msg;
	int irq_set_timer = BIT(TIMER0_IRQ);
	int ipc_status, r;

	//subscribe the keyboard
	if (subscribeKeyboard() != OK){
		vg_exit();
		timer_unsubscribe_int();
		printf("Error subscribing keyboard\n");
		return 1;
	}
	readStatusRegister(&statusByte);
	if (statusByte & OBF){
		sys_inb(OUT_BUF,&garbage);
		printf("Garbage cleared\n");
	}
	int xf = xi + delta;
	int yf = yi + delta;
	int xspeed = (int)(xf-xi)/time;
	int yspeed = (int)(yf-yi)/time;
	Sprite * mySprite = create_sprite(xpm, xi , yi , xspeed , yspeed);
	if (mySprite == NULL){
		timer_unsubscribe_int();
		unsubscribeKeyboard();
		vg_exit();
		return 1;
	}
	int timerInterruptionsCounter = 0;
	vg_clearScreen(0); // cleans the double buffer

	while (mySprite->x != xf && mySprite->y != yf && scancode != ESC_KEY) {
		if (hor){
			mySprite->x = xi + (mySprite->xSpeed) * timerInterruptionsCounter/DEFAULT_FREQ;
		}
		else {
			mySprite->y = yi + (mySprite->ySpeed) * timerInterruptionsCounter/DEFAULT_FREQ;
		}
		if (vg_DrawSprite(mySprite,0)){
			vg_exit();
			if (timer_unsubscribe_int() != OK) {
				printf("Unsubscribe timer error\n");
			}
			if (unsubscribeKeyboard() != OK){
				printf("Error unsubscribing keyboard interrupts\n");
			}

			return 1;
		}

		// Get a request message.
		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d\n", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) { // received notification of kernel
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: // hardware interrupt notification
				if (msg.NOTIFY_ARG & irq_set_timer) {// subscribed interrupt
					timerInterruptionsCounter++;
					vg_writeToVRAM();
					vg_clearScreen(0);
				}
				if (msg.NOTIFY_ARG & irq_set_keyboard){
					readStatusRegister(&statusByte);
					if ( (statusByte & (PAR_ERR | TO_ERR)) != OK){
						vg_exit();
						unsubscribeKeyboard();
						timer_unsubscribe_int();
						return 1;
					}
					if (statusByte & OBF != 0) {
						readOutBuffer(&scancode);
					}
				}
				break;
			default:
				break; // no other notifications expected: do nothing
			}
		}
	}
	destroy_sprite(mySprite);
	vg_exit();
	//unsubscribe timer
	if (timer_unsubscribe_int() != OK) {
		printf("Unsubscribe timer error\n");
		return 1;
	}
	if (unsubscribeKeyboard() != OK){
		printf("Error unsubscribing keyboard interrupts\n");
		return 1;
	}
	return 0;

}

void printModes(phys_bytes ptrOfModes, phys_bytes ini){
	phys_bytes ptrTemp = ptrOfModes;

	short * modes = (short *)((ptrTemp >> 12 & 0xFFFF0000 | ini) | ptrOfModes & 0x0000FFFF);
	int counter = 0;
	while (*modes != -1){
		if(counter % 7 == 0){
			printf("\n - ");
		}
		printf("0x%03X, ", *modes);
		modes++;
		counter++;
	}

}

#define PB2BASE(x) (((x) >> 4) & 0x0F000)
#define PB2OFF(x) ((x) & 0x0FFFF)

int test_controller() {
	VbeInfoBlock controller_inf;
	short *ptrOfModes;

	mmap_t bufMap;
	phys_bytes ini = lm_init();
	lm_alloc(sizeof(VbeInfoBlock), &bufMap);
	struct reg86u r;
	r.u.w.ax = 0x4F00;
	r.u.w.es = PB2BASE(bufMap.phys);
	r.u.w.di = PB2OFF(bufMap.phys);
	r.u.b.intno = 0x10;
	if (sys_int86(&r) != OK) { /* call BIOS */
		printf("return_vbe_get_controller_info information: sys_int86() failed \n");
		lm_free(&bufMap);
		return 1;
	}
	controller_inf = *(VbeInfoBlock *) bufMap.virtual;

	/* CAPABILITIES */
	printf("\nCapabilities : \n");

	/*DAC*/
	if(controller_inf.Capabilities[0] && DAC8BITS)
		printf(" - DAC width is switchable to 8 bits per primary color\n");
	else printf(" - DAC is fixed width, with 6 bits per primary color\n");

	/*VGA Compatible*/
	if(controller_inf.Capabilities[0] && VGA_COMPATIBLE)
		printf(" - Controller is VGA compatible\n");
	else printf(" - Controller is not VGA compatible\n");

	/*RAMDAC*/
	if(controller_inf.Capabilities[0] && RAMDAC)
		printf(" - Normal RAMDAC operation\n");
	else printf(" - When programming large blocks of information to the RAMDAC,use the blank bit in Function 09h.\n");

	/* MEMORY SIZE */
	printf("Size Of VRam Mem: %d KB\n" ,controller_inf.TotalMemory*64);

	/* MODES */
	printf("SUPPORTED MODES:");
	printModes(controller_inf.VideoModePtr, ini);
	printf("\n");

	lm_free(&bufMap);

	return 0;

}

