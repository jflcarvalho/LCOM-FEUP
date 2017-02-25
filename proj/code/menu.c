#include <minix/syslib.h>
#include <minix/drivers.h>
#include "keyboard.h"
#include "mouse.h"
#include "timer.h"
#include "bitmap.h"
#include "i8042.h"
#include "i8254.h"
#include "math.h"
#include "graphics.h"
#include "BMPLoaded.h"
#include "game.h"
#include "menu.h"

static short xMouse = 150;
static short yMouse = 150;
static short mouse1Click = 0;

static unsigned int hookID_keyboard = KEYBOARD_IRQ;


void updateCoord(unsigned short *packet){
	unsigned int h_res = getH_RES();
	unsigned int v_res = getV_RES();
	//printf("Delta x = %d", (char)packet[1]);
	//printf("Delta y = %d", (char)packet[2]);
	xMouse += (char) packet[1];
	yMouse -= (char) packet[2];
	if (xMouse < 0){
		xMouse = 0;
	}
	if (yMouse < 0){
		yMouse = 0;
	}
	if (xMouse > h_res - 1){
		xMouse = h_res - 1;
	}
	if (yMouse > v_res - 1){
		yMouse = v_res - 1;
	}
}


int mouse_handler(unsigned short *packet){
	//printf("Inside mouse_handler\n");

	unsigned int h_res = getH_RES();
	unsigned int v_res = getV_RES();
	unsigned int x, y;
	mouse1Click = packet[0] & MOUSECLICK;
	updateCoord(packet);
	//printf("Actual x =%d",xMouse);
	//printf("Actual y =%d",yMouse);

	if (xMouse > (h_res - localN->bitmapInfoHeader.width) / 2 && xMouse < (h_res + localN->bitmapInfoHeader.width) / 2 && yMouse > 375 && yMouse < 375 + localN->bitmapInfoHeader.height ){
		if(current_State == InitialMenu){
			current_Event = LocalHilight;
		}
		else if(current_State == LocalGameMenu){
			current_Event = SingleHilight;
		}
		if (mouse1Click){
			if(current_State == InitialMenu){
				current_Event = LocalPressed;
			}
			else if(current_State == LocalGameMenu){
				current_Event = SinglePressed;
			}
			return 0;
		}
	}
	else if (xMouse > (h_res - twoPcN->bitmapInfoHeader.width) / 2 && xMouse < (h_res + twoPcN->bitmapInfoHeader.width) / 2 && yMouse > 475 && yMouse < 475 + twoPcN->bitmapInfoHeader.height){
		if(current_State == InitialMenu){
			current_Event = TwoPcHilight;
		}
		else if(current_State == LocalGameMenu){
			current_Event = MultiHilight;
		}
		if (mouse1Click){
			if(current_State == InitialMenu){
				current_Event = TwoPcPressed;
			}
			else if(current_State == LocalGameMenu){
				current_Event = MultiPressed;
			}
			return 0;
		}
	}
	else if (xMouse > (h_res - exitN->bitmapInfoHeader.width) / 2 && xMouse < (h_res + exitN->bitmapInfoHeader.width) / 2 && yMouse > 575 && yMouse < 575 + exitN->bitmapInfoHeader.height){
		if(current_State == InitialMenu){
			current_Event = ExitHilight;
		}
		else if(current_State == LocalGameMenu){
			current_Event = BackHilight;
		}
		if (mouse1Click){
			if(current_State == InitialMenu){
				current_Event = ExitPressed;
			}
			else if(current_State == LocalGameMenu){
				current_Event = BackPressed;
			}
			return 0;
		}
	}
	else{
		current_Event = Nothing;
	}
	//printf("Mouse has been draw\n");
	return 0;
}

int machine_Handler(){
	prepareDraw(menuBackground, 0, 0);
	if (current_State == InitialMenu && current_Event == Nothing){
		drawLocal(0);
		drawTwoPc(0);
		drawExit(0);
	}
	else if(current_State == InitialMenu && (current_Event == LocalHilight || current_Event == LocalPressed)){
		drawLocal(1);
		drawTwoPc(0);
		drawExit(0);
	}
	else if(current_State == InitialMenu && (current_Event == TwoPcHilight || current_Event == TwoPcPressed)){
		drawLocal(0);
		drawTwoPc(1);
		drawExit(0);
	}
	else if(current_State == InitialMenu && (current_Event == ExitHilight || current_Event == ExitPressed)){
		drawLocal(0);
		drawTwoPc(0);
		drawExit(1);
	}
	else if(current_State == LocalGameMenu && current_Event == Nothing){
		drawSingle(0);
		drawMulti(0);
		drawBack(0);
	}
	else if(current_State == LocalGameMenu && (current_Event == SingleHilight || current_Event == SinglePressed)){
		drawSingle(1);
		drawMulti(0);
		drawBack(0);
	}
	else if(current_State == LocalGameMenu && (current_Event == MultiHilight || current_Event == MultiPressed)){
		drawSingle(0);
		drawMulti(1);
		drawBack(0);
	}
	else if(current_State == LocalGameMenu && (current_Event == BackHilight || current_Event == BackPressed)){
		drawSingle(0);
		drawMulti(0);
		drawBack(1);
	}
	drawMouse(mouse1Click,xMouse,yMouse);
	mouse1Click = 0;
	draw();

	if(current_State == InitialMenu && current_Event == LocalPressed){
		current_State = LocalGameMenu;
	}
	else if(current_State == InitialMenu && current_Event == TwoPcPressed){
		if(mouseDisable() != OK){
			printf("Error Disable Mouse\n");
			return 1;
		}
		Play_Mode = MultiPlayer;
		current_State = TwoPCMode;
        test_game();
        current_State = InitialMenu;
		current_Event = Nothing;

		if (mouseEnable() != OK){
			printf("Error Enable Mouse\n");
			return 1;
		}
	}
	else if(current_State == InitialMenu && current_Event == ExitPressed){
		current_State = Exit;
	}
	else if(current_State == LocalGameMenu && current_Event == SinglePressed){
		current_State = InitialMenu;
		if(mouseDisable() != OK){
			printf("Error Disable Mouse\n");
			return 1;
		}
		Play_Mode = SinglePlayer;
		test_game();
		current_Event = Nothing;
		if (mouseEnable() != OK){
			printf("Error Enable Mouse\n");
			return 1;
		}
	}
	else if(current_State == LocalGameMenu && current_Event == MultiPressed){
		current_State = InitialMenu;
		if(mouseDisable() != OK){
			printf("Error Disable Mouse\n");
			return 1;
		}
		Play_Mode = MultiPlayer;
		test_game();
		current_Event = Nothing;
		if (mouseEnable() != OK){
			printf("Error Enable Mouse\n");
			return 1;
		}
	}
	else if(current_State == LocalGameMenu && current_Event == BackPressed){
		current_State = InitialMenu;
	}
	return 0;
}

int start_menu(){
	unsigned short packet[3];
	unsigned long outBuffer;
	message msg;
	int counterError = 0;
	int packetCounter = 0;
	int ipc_status, r;
	int irq_set_timer = BIT(TIMER0_IRQ);
	int irq_set_keyboard = BIT(KEYBOARD_IRQ);
	int counter = 0;
    current_State = InitialMenu;
    current_Event = Nothing;

	if (timer_subscribe_int() != OK) {
		printf("Subscribe error\n");
		return 1;
	}
	if (subscribeMouse() != OK){
		printf("Error Subscribing Mouse\n");
		return 1;
	}
	while (current_State != Exit) {
		/* Get a request message. */
		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d\n", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) { /* received notification of kernel*/
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: /* hardware interrupt notification */
				if (msg.NOTIFY_ARG & irq_set_mouse) {/* subscribed interrupt */
					//printf("Mouse Interrupt\n");
					readOutBuffer(&outBuffer);
					if (((outBuffer & FIRST_BYTE) && packetCounter == 0) || packetCounter > 0 ){
						packet[packetCounter] = outBuffer;
						packetCounter++;
					}
					if (packetCounter == 3){
						packetCounter = 0;
						//printf("Calling mouse handler\n");
						mouse_handler(packet);
					}

				}
				else if (msg.NOTIFY_ARG & irq_set_timer) {/* subscribed interrupt */
					//printf ("Interrupcao timer\n");
					counter++;
					if(counter % 3 == 0){
						machine_Handler();
						counter = 0;
					}
				}
				break;
			default:
				printf("Default notification\n");
				break; /* no other notifications expected: do nothing */
			}
		}
	}
	if(unsubscribeMouse() != OK){
		printf("Error Unsubscribing Mouse\n");
		return 1;
	}
	if(timer_unsubscribe_int() != OK){
		printf("Error Unsubscribing Timer\n");
		return 1;
	}
	return 0;
}
