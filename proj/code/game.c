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
#include "menu.h"
#include "uart.h"
#include "game.h"


#define v_res_playable 605
static int playerNumber = 0;
static int red_Prepared = 0;
static int blue_Prepared = 0;

void prepareGame(int *counter, int nBalls){
	*counter = 60*90;
	BH->xi = getH_RES() / 2 + BH->bmp->bitmapInfoHeader.width;
	BH->yi = v_res_playable - BH->height;
	BH_right->xi = BH->xi;
	BH_right->yi = BH->yi;
	BH_left->xi = BH->xi;
	BH_left->yi = BH->yi;
	BH->x = BH->xi;
	BH->y = BH->yi;
	BH_right->x = BH->x;
	BH_right->y = BH->y;
	BH_left->x = BH->x;
	BH_left->y = BH->y;
	BH_harpoon->ySpeed = 0;
	if(Play_Mode == MultiPlayer){
		RH->xi = getH_RES() / 2 - RH->bmp->bitmapInfoHeader.width;
		RH->yi = v_res_playable - RH->height;
		RH_right->xi = RH->xi;
		RH_right->yi = RH->yi;
		RH_left->xi = RH->xi;
		RH_left->yi = RH->yi;
		RH->x = RH->xi;
		RH->y = RH->yi;
		RH_right->x = RH->x;
		RH_right->y = RH->y;
		RH_left->x = RH->x;
		RH_left->y = RH->y;
		RH_harpoon->ySpeed = 0;
	}
	int i;
	for(i = 1; i < nBalls; i++){
		ballsBool[i] = 0;
	}
	ballsBool[0] = 1;
	balls[0]->x = balls[0]->xi;
	balls[0]->y = balls[0]->yi;
	balls[0]->ySpeed = 0;
	balls[0]->xSpeed = abs(balls[0]->xSpeed);
}

void handleBalls(Sprite *sp){
	//printf("Start handleBalls\n");
	unsigned int h_res = getH_RES();
	unsigned int v_res = v_res_playable;
	sp->x = sp->x + sp->xSpeed;
	sp->y = sp->y + sp->ySpeed * sp->t + 0.5 * sp->yAce * sp->t * sp->t;
	sp->ySpeed = sp->ySpeed + sp->yAce * sp->t;
	if (sp->x > h_res - sp->width - 10){
		sp->xSpeed = -sp->xSpeed;
		sp->x = h_res - sp->width - 10;
	}
	else if (sp->x < 10){
		sp->xSpeed = -sp->xSpeed;
		sp->x = 10;
	}
	if(sp->y < 10){
		sp->y = 10;
		sp->ySpeed = -sp->ySpeed;
	}
	else if (sp->y > v_res - sp->height){
		sp->ySpeed = -sqrt(2 * (v_res_playable-sp->yi) / sp->yAce) * sp->yAce + 3;
		sp->y = v_res - sp->height;
	}
	prepareDraw(sp->bmp, sp->x, sp->y);
	//printf("End handleBalls\n");
}

void handleBH(int *counter, int nBalls){
	//printf("Start handlerBH\n");
	int i, j, collision = 0;
	unsigned int hor_res = getH_RES();
	if (*counter == 0){
		//it means there is no time remaining
		prepareDraw(BH->bmp, BH->x, BH->y);
		return;
	}

	if(BH_harpoon->ySpeed != 0){
		prepareDraw(BH_harpoon->bmps[0], BH_harpoon->x , BH_harpoon->y - BH_harpoon->bmps[0]->bitmapInfoHeader.height);
		for(i = 0; i < nBalls; i++){
			if(ballsBool[i]){
				if(testCollision(BH_harpoon->bmps[0], BH_harpoon->x , BH_harpoon->y - BH_harpoon->bmps[0]->bitmapInfoHeader.height, balls[i]->bmp, balls[i]->x, balls[i]->y)){
					collision = 1;
					//printf("Harpoon & Ball\n");
					BH_harpoon->ySpeed = 0;
					balls_Collision_Handler(i, nBalls);
					break;
				}
			}
		}
		if(BH_harpoon->y != BH_harpoon->yi){
			for(i = BH_harpoon->y; i < BH_harpoon->yi - BH_harpoon->bmps[1]->bitmapInfoHeader.height; i += BH_harpoon->bmps[1]->bitmapInfoHeader.height){
				prepareDraw(BH_harpoon->bmps[1], BH_harpoon->x , i);
				if(collision == 0){
					for(j = 0; j < nBalls; j++){
						if(ballsBool[j]){
							if(testCollision(BH_harpoon->bmps[1], BH_harpoon->x , i, balls[j]->bmp, balls[j]->x, balls[j]->y)){
								collision = 1;
								//printf("Harpoon & Ball\n");
								BH_harpoon->ySpeed = 0;
								balls_Collision_Handler(j, nBalls);
								break;
							}
						}
					}
				}
			}
		}
		BH_harpoon->y = BH_harpoon->y - BH_harpoon->ySpeed;
		if(BH_harpoon->y - BH_harpoon->bmps[0]->bitmapInfoHeader.height < 5){
			BH_harpoon->ySpeed = 0;
		}
	}
	if(*counter % 2 == 0){
		if (BH_right->xSpeed != 0){
			if(BH_right->x + BH_right->xSpeed < hor_res - 5 - BH_right->bmps[BH_right->nextBmp]->bitmapInfoHeader.width){
				BH_right->x = BH_right->x + BH_right->xSpeed;
				BH->x = BH_right->x;
				BH_left->x = BH_right->x;
			}
			BH_right->nextBmp++;
			if(BH_right->nextBmp == BH_right->nBMP){
				BH_right->nextBmp = 0;
			}
			for(j = 0; j < nBalls; j++){
				if(ballsBool[j]){
					if(testCollision(BH_right->bmps[BH_right->nextBmp], BH_right->x, BH_right->y, balls[j]->bmp, balls[j]->x, balls[j]->y)){
						*counter = 0;
						printf("BH & Ball\n");
					}
				}
			}
		}
		else if (BH_left->xSpeed != 0){
			if(BH_left->x - BH_left->xSpeed > 6){
				BH_left->x = BH_left->x - BH_left->xSpeed;
				BH_right->x = BH_left->x;
				BH->x = BH_left->x;
			}
			BH_left->nextBmp++;
			if(BH_left->nextBmp == BH_left->nBMP){
				BH_left->nextBmp = 0;
			}
			for(j = 0; j < nBalls; j++){
				if(ballsBool[j]){
					if(testCollision(BH_left->bmps[BH_left->nextBmp], BH_left->x, BH_left->y, balls[j]->bmp, balls[j]->x, balls[j]->y)){
						*counter = 0;
						printf("BH & Ball\n");
					}
				}
			}
		}
		else{
			if(BH_harpoon->ySpeed != 0 && BH_shoot->xi == BH->x){
				for(j = 0; j < nBalls; j++){
					if(ballsBool[j]){
						if(testCollision(BH_shoot->bmps[0], BH_shoot->x, BH_shoot->y, balls[j]->bmp, balls[j]->x, balls[j]->y)){
							*counter = 0;
							printf("BH & Ball\n");
						}
					}
				}
			}
			else{
				for(j = 0; j < nBalls; j++){
					if(ballsBool[j]){
						if(testCollision(BH->bmp, BH->x, BH->y, balls[j]->bmp, balls[j]->x, balls[j]->y)){
							*counter = 0;
							printf("BH & Ball\n");
						}
					}
				}
			}
		}
	}
	if (BH_right->xSpeed != 0){
		prepareDraw(BH_right->bmps[BH_right->nextBmp], BH_right->x, BH_right->y);
	}
	else if (BH_left->xSpeed != 0){
		prepareDraw(BH_left->bmps[BH_left->nextBmp], BH_left->x, BH_left->y);
	}
	else{
		if (BH_harpoon->ySpeed != 0 && BH_shoot->xi == BH->x){
			prepareDraw(BH_shoot->bmps[0], BH_shoot->xi, BH_shoot->yi);
		}
		else{
			prepareDraw(BH->bmp, BH->x, BH->y);
		}
	}
	//printf("End handlerBH\n");
}

void handleRH(int *counter, int nBalls){
	//printf("Start handlerRH\n");
	int i, j, collision = 0;
	unsigned int hor_res = getH_RES();
	if (*counter == 0){
		//it means there is no time remaining
		prepareDraw(RH->bmp, RH->x, RH->y);
		return;
	}

	if(RH_harpoon->ySpeed != 0){
		prepareDraw(RH_harpoon->bmps[0], RH_harpoon->x , RH_harpoon->y - RH_harpoon->bmps[0]->bitmapInfoHeader.height);
		for(i = 0; i < nBalls; i++){
			if(ballsBool[i]){
				if(testCollision(RH_harpoon->bmps[0], RH_harpoon->x , RH_harpoon->y - RH_harpoon->bmps[0]->bitmapInfoHeader.height, balls[i]->bmp, balls[i]->x, balls[i]->y)){
					collision = 1;
					//printf("Harpoon & Ball\n");
					RH_harpoon->ySpeed = 0;
					balls_Collision_Handler(i, nBalls);
					break;
				}
			}
		}
		if(RH_harpoon->y != RH_harpoon->yi){
			for(i = RH_harpoon->y; i < RH_harpoon->yi - RH_harpoon->bmps[1]->bitmapInfoHeader.height; i += RH_harpoon->bmps[1]->bitmapInfoHeader.height){
				prepareDraw(RH_harpoon->bmps[1], RH_harpoon->x , i);
				if(collision == 0){
					for(j = 0; j < nBalls; j++){
						if(ballsBool[j]){
							if(testCollision(RH_harpoon->bmps[1], RH_harpoon->x , i, balls[j]->bmp, balls[j]->x, balls[j]->y)){
								collision = 1;
								//printf("Harpoon & Ball\n");
								RH_harpoon->ySpeed = 0;
								balls_Collision_Handler(j, nBalls);
								break;
							}
						}
					}
				}
			}
		}
		RH_harpoon->y = RH_harpoon->y - RH_harpoon->ySpeed;
		if(RH_harpoon->y - RH_harpoon->bmps[0]->bitmapInfoHeader.height < 5){
			RH_harpoon->ySpeed = 0;
		}
	}
	if(*counter % 2 == 0){
		if (RH_right->xSpeed != 0){
			if(RH_right->x + RH_right->xSpeed < hor_res - 5 - RH_right->bmps[RH_right->nextBmp]->bitmapInfoHeader.width){
				RH_right->x = RH_right->x + RH_right->xSpeed;
				RH->x = RH_right->x;
				RH_left->x = RH_right->x;
			}
			RH_right->nextBmp++;
			if(RH_right->nextBmp == RH_right->nBMP){
				RH_right->nextBmp = 0;
			}
			for(j = 0; j < nBalls; j++){
				if(ballsBool[j]){
					if(testCollision(RH_right->bmps[RH_right->nextBmp], RH_right->x, RH_right->y, balls[j]->bmp, balls[j]->x, balls[j]->y)){
						*counter = 0;
						printf("RH & Ball\n");
					}
				}
			}
		}
		else if (RH_left->xSpeed != 0){
			if(RH_left->x - RH_left->xSpeed > 6){
				RH_left->x = RH_left->x - RH_left->xSpeed;
				RH_right->x = RH_left->x;
				RH->x = RH_left->x;
			}
			RH_left->nextBmp++;
			if(RH_left->nextBmp == RH_left->nBMP){
				RH_left->nextBmp = 0;
			}
			for(j = 0; j < nBalls; j++){
				if(ballsBool[j]){
					if(testCollision(RH_left->bmps[RH_left->nextBmp], RH_left->x, RH_left->y, balls[j]->bmp, balls[j]->x, balls[j]->y)){
						*counter = 0;
						printf("RH & Ball\n");
					}
				}
			}
		}
		else{
			if(RH_harpoon->ySpeed != 0 && RH_shoot->xi == RH->x){
				for(j = 0; j < nBalls; j++){
					if(ballsBool[j]){
						if(testCollision(RH_shoot->bmps[0], RH_shoot->x, RH_shoot->y, balls[j]->bmp, balls[j]->x, balls[j]->y)){
							*counter = 0;
							printf("RH & Ball\n");
						}
					}
				}
			}
			else{
				for(j = 0; j < nBalls; j++){
					if(ballsBool[j]){
						if(testCollision(RH->bmp, RH->x, RH->y, balls[j]->bmp, balls[j]->x, balls[j]->y)){
							*counter = 0;
							printf("RH & Ball\n");
						}
					}
				}
			}
		}
	}
	if (RH_right->xSpeed != 0){
		prepareDraw(RH_right->bmps[RH_right->nextBmp], RH_right->x, RH_right->y);
	}
	else if (RH_left->xSpeed != 0){
		prepareDraw(RH_left->bmps[RH_left->nextBmp], RH_left->x, RH_left->y);
	}
	else{
		if (RH_harpoon->ySpeed != 0 && RH_shoot->xi == RH->x){
			prepareDraw(RH_shoot->bmps[0], RH_shoot->xi, RH_shoot->yi);
		}
		else{
			prepareDraw(RH->bmp, RH->x, RH->y);
		}
	}
	//printf("End handlerRH\n");
}

void keyboard_Handler(unsigned long scancode, int *isDoubleByte, unsigned long *firstScan, unsigned long *RBR_char){
	if (scancode == DOUBLEBYTE){
		*isDoubleByte = 1;
		*firstScan = scancode;
	}
	else if (*isDoubleByte == 1){
		if (scancode & BIT7){
			//printf("Break Code: 0x%x%x ; \n",*firstScan,scancode);
		}
		else {
			//printf("Make Code: 0x%x%x ; \n",*firstScan,scancode);
		}
		*isDoubleByte = 0;
	}
	else if (*isDoubleByte == 0){
		if (scancode & BIT7) { // break code
			//printf("Break code: 0x%x ; \n", scancode);
		}
		else { // make code
			//printf("Make code : 0x%x ; \n", scancode);
		}
	}
	if(scancode == LEFTMBUTTON || *RBR_char == 'j'){ //Make <-
        if(scancode == LEFTMBUTTON){
		    *RBR_char = 'j';
            if(current_State == TwoPCMode)
                UART_writeTHR(*RBR_char);
        }
		BH_left->xSpeed = 13;
		BH_right->xSpeed = 0;
		BH_right->nextBmp = 0;
	}
	else if(scancode == RIGHTMBUTTON || *RBR_char == 'l'){ //Make ->
        if(scancode == RIGHTMBUTTON){
		    *RBR_char = 'l';
            if(current_State == TwoPCMode)
                UART_writeTHR(*RBR_char);
        }
		BH_right->xSpeed = 13;
		BH_left->xSpeed = 0;
		BH_left->nextBmp = 0;
	}
	else if(scancode == LEFTBBUTTON || *RBR_char == 'i'){ //Break <-
        if(scancode == LEFTBBUTTON){
		    *RBR_char = 'i';
            if(current_State == TwoPCMode)
                UART_writeTHR(*RBR_char);
        }
		BH_left->xSpeed = 0;
		BH_left->nextBmp = 0;
	}
	else if(scancode == RIGHTBBUTTON || *RBR_char == 'k'){ //Break ->
        if(scancode == RIGHTBBUTTON){
		    *RBR_char = 'k';
            if(current_State == TwoPCMode)
                UART_writeTHR(*RBR_char);
        }
		BH_right->xSpeed = 0;
		BH_right->nextBmp = 0;
	}
	else if(scancode == SPACEMBUTTON || *RBR_char == ' '){
        if(scancode == SPACEMBUTTON){
		    *RBR_char = ' ';
            if(current_State == TwoPCMode)
                UART_writeTHR(*RBR_char);
        }
		if(BH_harpoon->ySpeed == 0){
			BH_shoot->xi = BH->x;
			BH_shoot->yi = BH->y;
			BH_harpoon->xi = BH_shoot->xi + BH_shoot->bmps[0]->bitmapInfoHeader.width / 2 - BH_harpoon->bmps[0]->bitmapInfoHeader.width / 2;
			BH_harpoon->yi = v_res_playable;
			BH_harpoon->x = BH_harpoon->xi;
			BH_harpoon->y = BH->y + BH_harpoon->bmps[1]->bitmapInfoHeader.height;
			BH_harpoon->ySpeed = 7;
		}
        if(blue_Prepared == 0){
            blue_Prepared = 1;
        }
	}
	if(Play_Mode == MultiPlayer){
		if(scancode == AMBUTTON || *RBR_char == 'a'){
            if(scancode == AMBUTTON){
                *RBR_char = 'a';
                if(current_State == TwoPCMode)
                    UART_writeTHR(*RBR_char);
            }
			RH_left->xSpeed = 13;
			RH_right->xSpeed = 0;
			RH_right->nextBmp = 0;
		}
		else if(scancode == DMBUTTON || *RBR_char == 'd'){
            if(scancode == DMBUTTON){
                *RBR_char = 'd';
                if(current_State == TwoPCMode)
                    UART_writeTHR(*RBR_char);
            }
			RH_right->xSpeed = 13;
			RH_left->xSpeed = 0;
			RH_left->nextBmp = 0;
		}
		else if(scancode == ABBUTTON || *RBR_char == 'w'){
            if(scancode == ABBUTTON){
                *RBR_char = 'w';
                if(current_State == TwoPCMode)
                    UART_writeTHR(*RBR_char);
            }
			RH_left->xSpeed = 0;
			RH_left->nextBmp = 0;
		}
		else if(scancode == DBBUTTON || *RBR_char == 's'){
            if(scancode == DBBUTTON){
                *RBR_char = 's';
                if(current_State == TwoPCMode)
                    UART_writeTHR(*RBR_char);
            }
			RH_right->xSpeed = 0;
			RH_right->nextBmp = 0;
		}
		else if(scancode == SHIFTMBUTTON || *RBR_char == 'z'){
            if(scancode == SHIFTMBUTTON){
                *RBR_char = 'z';
                if(current_State == TwoPCMode)
                    UART_writeTHR(*RBR_char);
            }
			if(RH_harpoon->ySpeed == 0){
				RH_shoot->xi = RH->x;
				RH_shoot->yi = RH->y;
				RH_harpoon->xi = RH_shoot->xi + RH_shoot->bmps[0]->bitmapInfoHeader.width / 2 - RH_harpoon->bmps[0]->bitmapInfoHeader.width / 2;
				RH_harpoon->yi = v_res_playable;
				RH_harpoon->x = RH_harpoon->xi;
				RH_harpoon->y = RH->y + RH_harpoon->bmps[1]->bitmapInfoHeader.height;
				RH_harpoon->ySpeed = 7;
			}
            if(red_Prepared == 0){
                red_Prepared = 1;
            }
		}
	}
}

void balls_Collision_Handler(int i, int nBalls){
	//printf("Start balls_Collision_Handler\n");
	ballsBool[i] = 0;
	if (i < (nBalls - 1) / 2){
		ballsBool[2*i + 1] = 1;
		balls[2*i + 1]->x = balls[i]->x;
		balls[2*i + 1]->y = balls[i]->y + balls[i]->bmp->bitmapInfoHeader.height / 4;
		balls[2*i + 1]->xSpeed = -abs(balls[i]->xSpeed);
		balls[2*i + 1]->ySpeed = -abs(balls[i]->ySpeed);
		ballsBool[2*i + 2] = 1;
		balls[2*i + 2]->x = balls[i]->x + balls[i]->bmp->bitmapInfoHeader.width / 2;
		balls[2*i + 2]->y = balls[i]->y + balls[i]->bmp->bitmapInfoHeader.height / 4;
		balls[2*i + 2]->xSpeed = abs(balls[i]->xSpeed);
		balls[2*i + 2]->ySpeed = -abs(balls[i]->ySpeed);
	}
	//printf("End balls_Collision_Handler\n");
}

int testCollision(Bitmap *obj1, int xObj1, int yObj1, Bitmap *obj2, int xObj2, int yObj2){
	if (obj1 == NULL)
		return 0;

	if (obj2 == NULL)
		return 0;

	int widthObj1 = obj1->bitmapInfoHeader.width;
	int heightObj1 = obj1->bitmapInfoHeader.height;
	int widthObj2 = obj2->bitmapInfoHeader.width;
	int heightObj2 = obj2->bitmapInfoHeader.height;

	//printf("xObj1: %d, widthObj1: %d, xObj2: %d, widthObj2: %d\n", xObj1, widthObj1, xObj2, widthObj2);
	//printf("yObj1: %d, heightObj1: %d, yObj2: %d, heightObj1: %d\n", yObj1, heightObj1, yObj2, heightObj2);

	if(xObj1 + widthObj1 < xObj2 || xObj2 + widthObj2 < xObj1){
		return 0;
	}
	if(yObj1 + heightObj1 < yObj2 || yObj2 + heightObj2 < yObj1){
		return 0;
	}

	//printf("test with more accuracy collision\n");

	int drawWidthObj1 = widthObj1;
	int drawWidthObj2 = widthObj2;
	if (xObj1 + widthObj1 < 0 || xObj1 > getH_RES() || yObj1 + heightObj1 < 0 || yObj1 > getV_RES())
		return 0;
	if (xObj2 + widthObj2 < 0 || xObj2 > getH_RES() || yObj2 + heightObj2 < 0 || yObj2 > getV_RES())
		return 0;

	int xCorrectionObj1 = 0;
	int xCorrectionObj2 = 0;
	if (xObj1 < 0) {
		xCorrectionObj1 = -xObj1;
		drawWidthObj1 -= xCorrectionObj1;
		xObj1 = 0;

		if (drawWidthObj1 > getH_RES())
			drawWidthObj1 = getH_RES();
	} else if (xObj1 + drawWidthObj1 > getH_RES()) {
		drawWidthObj1 = getH_RES() - xObj1;
	}
	if (xObj2 < 0) {
		xCorrectionObj2 = -xObj2;
		drawWidthObj2 -= xCorrectionObj2;
		xObj2 = 0;

		if (drawWidthObj2 > getH_RES())
			drawWidthObj2 = getH_RES();
	} else if (xObj2 + drawWidthObj2 > getH_RES()) {
		drawWidthObj2 = getH_RES() - xObj2;
	}

	char* bufferStartPos = malloc(sizeof(char) * 2 * getH_RES() * getV_RES());
	char* bufferPos;
	char* imgStartPos;

	int i;
	int j;
	for (i = 0; i < heightObj1 ; i++) {
		int pos = yObj1 + heightObj1 - 1 - i;

		if (pos < 0 || pos >= getV_RES())
			continue;
		bufferPos = bufferStartPos;
		bufferPos += xObj1 * 2 + pos * getH_RES() * 2;
		imgStartPos = obj1->bitmapData + xCorrectionObj1 * 2 + i * widthObj1 * 2;

		for(j = 0; j < drawWidthObj1 * 2; j += 2){
			if(imgStartPos[j] != 0x1f && imgStartPos[j+1] != 0xfffffff8){
				bufferPos[j] = imgStartPos[j];
				bufferPos[j+1] = imgStartPos[j+1];
			}
		}
	}
	for (i = 0; i < heightObj2 ; i++) {
		int pos = yObj2 + heightObj2 - 2 - i;

		if (pos < 0 || pos >= getV_RES())
			continue;
		bufferPos = bufferStartPos;
		bufferPos += xObj2 * 2 + pos * getH_RES() * 2;
		imgStartPos = obj2->bitmapData + xCorrectionObj2 * 2 + i * widthObj2 * 2;

		for(j = 0; j < drawWidthObj2 * 2; j += 2){
			if(imgStartPos[j] != 0x1f && imgStartPos[j+1] != 0xfffffff8){
				if(bufferPos[j] != 0x0 || bufferPos[j+1] != 0x0){
					printf("Collision Detected\n");
					return 1;
				}
			}
		}
	}
	return 0;
}

void time_handler(int counter){
	counter = counter / 60;
	unsigned int xTime = getH_RES() / 2 - (timeBitmap->bitmapInfoHeader.width + 5 * 3 + ZERO_Bitmap->bitmapInfoHeader.width * 3) / 2;
	unsigned int yTime = v_res_playable + 30;
	prepareDraw(timeBitmap, xTime, yTime);
	xTime += timeBitmap->bitmapInfoHeader.width + 5;
	draw_time(counter / 100, xTime, yTime);
	xTime += ZERO_Bitmap->bitmapInfoHeader.width + 5;
	counter -= counter / 100 * 100;
	draw_time(counter / 10, xTime, yTime);
	xTime += ZERO_Bitmap->bitmapInfoHeader.width + 5;
	counter -= counter / 10 * 10;
	draw_time(counter, xTime, yTime);
}



void handleVictory(int lives, int counter, int ballInGame){
	if (ballInGame == 0){
		drawVictory();
		drawEsc();
	}
	else if(lives <= 0 || counter <= 0){
		drawDefeat();
		drawEsc();
	}
}



int test_game(){
	int nBalls;
	int canExit = 0, ballsInGame, firstStart = 0;
	int counter;
	int lifes_BH = 3;
	int lifes_RH = 0;
    red_Prepared = 1;
    blue_Prepared = 1;
	loadBH();
	if(Play_Mode == MultiPlayer){
		lifes_RH = 3;
		loadRH();
	}
	loadBall(&nBalls);
	prepareGame(&counter, nBalls);
	unsigned long garbage, statusByte;
	unsigned long  scancode = 0;
	unsigned long firstScan;
	int isDoubleByte = 0;
	int ipc_status, r;
	message msg;
	int irq_set_keyboard = BIT(KEYBOARD_IRQ);
	int irq_set_timer = BIT(TIMER0_IRQ);
	int irq_set_uart_com1 = BIT(uart_COM1_IRQ);
	unsigned long RBR = 0;

	if (subscribeKeyboard() != OK){
		printf("Error subscribing keyboard\n");
	}
	if(current_State == TwoPCMode){
        red_Prepared = 0;
        blue_Prepared = 0;
		if(subscribeUART() != OK){
			printf("Error subscribing UART\n");
			return 1;
		}
	}
    while (scancode != ESC_KEY || (canExit != 1 && firstStart == 1)) { /* key pressed != ESC */
		/* RESET FLAGS */
		ballsInGame = 0;
		RBR = 0;
		scancode = 0;

		/* Get a request message. */
		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d\n", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) { /* received notification of kernel*/
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: /* hardware interrupt notification */
				if (msg.NOTIFY_ARG & irq_set_timer) {/* subscribed interrupt */
					//printf ("Interrupcao timer\n");
                    if(firstStart == 0 && red_Prepared && blue_Prepared){
                        prepareGame(&counter, nBalls);
                        firstStart = 1;
                    }
					drawFundo();
					drawLifes(lifes_BH);
					time_handler(counter);
					int i;
					for (i = 0; i < nBalls; i++){
						if(ballsBool[i]){
							handleBalls(balls[i]);
							ballsInGame++;
						}
					}
					if(lifes_BH > 0){
						handleBH(&counter, nBalls);
					}
					if(counter <= 0){
                        if(current_State == TwoPCMode){
                            RBR = 'b';
                            UART_writeTHR(RBR);
                        }
                        if(firstStart == 1 && lifes_BH > 0){
                            lifes_BH--;
                        }
						prepareGame(&counter, nBalls);
					}
					if(Play_Mode == MultiPlayer){
						drawLifes_RH(lifes_RH);
						if(lifes_RH > 0){
							handleRH(&counter, nBalls);
						}
						if(counter <= 0){
                            if(current_State == TwoPCMode){
                                RBR = 'r';
                                UART_writeTHR(RBR);
                            }
                            if(firstStart == 1 && lifes_RH > 0){
                                lifes_RH--;
                            }
							prepareGame(&counter, nBalls);
						}
					}
					if (lifes_BH + lifes_RH <= 0 || counter <= 0 || ballsInGame == 0){
						canExit = 1;
						handleVictory(lifes_BH + lifes_RH, counter, ballsInGame);
					}
					draw();
					if(counter > 0)
						counter--;
				}
				if (msg.NOTIFY_ARG & irq_set_keyboard) {
					//printf ("Interrupcao teclado\n");
					kbd_readStatusRegister(&statusByte);
					if ( (statusByte & (PAR_ERR | TO_ERR)) != OK){
						printf("Invalid Data on Out_Buffer\n");
						return -1;
					}
					if (statusByte & OBF != 0) {//&& (statusByte & (PAR_ERR | TO_ERR) == 0))
						kbd_readOutBuffer(&scancode);
						if(counter > 0){
							keyboard_Handler(scancode, &isDoubleByte, &firstScan, &RBR);
						}
					}
					tickdelay(micros_to_ticks(WAIT_KBC));
				}
				if(msg.NOTIFY_ARG & irq_set_uart_com1){
					//printf("UART COM1 Interrupt\n");
					UART_Interrupt_Handler(&RBR);
					if(counter > 0){
                        if(RBR == 'r'){
                            if(red_Prepared == 1){
                                if (lifes_RH > 0) lifes_RH--;
                                prepareGame(&counter, nBalls);
                            }
                            else{
                                prepareGame(&counter, nBalls);
                            }
                        }
                        else if(RBR == 'b'){
                            if(blue_Prepared == 1){
                                if (lifes_BH > 0) lifes_BH--;
                                prepareGame(&counter, nBalls);
                            }
                            else {
                                prepareGame(&counter, nBalls);
                            }
                        }
						else{
                            keyboard_Handler(scancode, &isDoubleByte, &firstScan, &RBR);
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
	if(current_State == TwoPCMode){
		if(unsubscribeUART() != OK){
			printf("Error unsubscribing UART\n");
			return 1;
		}
	}
	if (unsubscribeKeyboard() != OK){
		printf("Error unsubscribing keyboard\n");
		return 1;
	}
	return 0;
}
