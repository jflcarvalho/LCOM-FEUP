#include "graphics.h"
#include "rtc.h"
#include "bitmap.h"
#include "BMPLoaded.h"


void loadTimeBMP(){

	timeBitmap = loadBitmap("/home/lcom/svn/proj/res/Time/Time.bmp");

	ZERO_Bitmap = loadBitmap("/home/lcom/svn/proj/res/Time/0.bmp");

	ONE_Bitmap = loadBitmap("/home/lcom/svn/proj/res/Time/1.bmp");

	TWO_Bitmap = loadBitmap("/home/lcom/svn/proj/res/Time/2.bmp");

	THREE_Bitmap = loadBitmap("/home/lcom/svn/proj/res/Time/3.bmp");

	FOUR_Bitmap = loadBitmap("/home/lcom/svn/proj/res/Time/4.bmp");

	FIVE_Bitmap = loadBitmap("/home/lcom/svn/proj/res/Time/5.bmp");

	SIX_Bitmap = loadBitmap("/home/lcom/svn/proj/res/Time/6.bmp");

	SEVEN_Bitmap = loadBitmap("/home/lcom/svn/proj/res/Time/7.bmp");

	EIGHT_Bitmap = loadBitmap("/home/lcom/svn/proj/res/Time/8.bmp");

	NINE_Bitmap = loadBitmap("/home/lcom/svn/proj/res/Time/9.bmp");

}

void loadBall(int *nBalls){

	printf("Start Load Ball Images\n");

	*nBalls = 15;

	ballLarge = loadBitmap("/home/lcom/svn/proj/res/Balls/BallLarge.bmp");

	ballMedium = loadBitmap("/home/lcom/svn/proj/res/Balls/BallMedium.bmp");

	ballSmall = loadBitmap("/home/lcom/svn/proj/res/Balls/BallSmall.bmp");

	ballExtraSmall  = loadBitmap("/home/lcom/svn/proj/res/Balls/BallExtraSmall.bmp");

	//printf("start allocation of balls\n");

	balls = (Sprite **) malloc(sizeof(Sprite *) * (*nBalls));

	ballsBool = (int *) malloc(sizeof(int) * (*nBalls));

	/* Need be change this if u want other balls */

	balls[0] = create_sprite(ballLarge, 300, 150, 2, 0);

	balls[1] = create_sprite(ballMedium, 300, 200, -2, 0);

	balls[2] = create_sprite(ballMedium, 624, 200, 2, 0);

	balls[3] = create_sprite(ballSmall, 0, 300, -2, 0);

	balls[4] = create_sprite(ballSmall, 0, 300, 2, 0);

	balls[5] = create_sprite(ballSmall, 0, 300, -2, 0);

	balls[6] = create_sprite(ballSmall, 0, 300, 2, 0);

	balls[7] = create_sprite(ballExtraSmall, 0, 400, -2, 0);

	balls[8] = create_sprite(ballExtraSmall, 0, 400, 2, 0);

	balls[9] = create_sprite(ballExtraSmall, 0, 400, -2, 0);

	balls[10] = create_sprite(ballExtraSmall, 0, 400, 2, 0);

	balls[11] = create_sprite(ballExtraSmall, 0, 400, -2, 0);

	balls[12] = create_sprite(ballExtraSmall, 0, 400, 2, 0);

	balls[13] = create_sprite(ballExtraSmall, 0, 400, -2, 0);

	balls[14] = create_sprite(ballExtraSmall, 0, 400, 2, 0);

	//printf("end allocation of balls\n");

	printf("End Load Ball Images\n");

}

void loadBH(){

	printf("Start Load BH Images\n");

	Bitmap *BH_bmp = loadBitmap("/home/lcom/svn/proj/res/BH/BH_0.bmp");

	BH = create_sprite(BH_bmp, 0, 0, 0, 0);

	life_Hero = loadBitmap("/home/lcom/svn/proj/res/BH/life_Hero.bmp");

	BH_R = (Bitmap **) malloc(sizeof(Bitmap *) * 12);

	BH_L = (Bitmap **) malloc(sizeof(Bitmap *) * 12);

	BH_S = (Bitmap **) malloc(sizeof(Bitmap *) * 2);

	//printf("Start inicialization of array\n");

	BH_R[0] = loadBitmap("/home/lcom/svn/proj/res/BH/BH_1.bmp");

	BH_R[1] = loadBitmap("/home/lcom/svn/proj/res/BH/BH_2.bmp");

	BH_R[2] = loadBitmap("/home/lcom/svn/proj/res/BH/BH_3.bmp");

	BH_R[3] = loadBitmap("/home/lcom/svn/proj/res/BH/BH_4.bmp");

	BH_R[4] = loadBitmap("/home/lcom/svn/proj/res/BH/BH_5.bmp");

	BH_R[5] = loadBitmap("/home/lcom/svn/proj/res/BH/BH_6.bmp");

	BH_R[6] = loadBitmap("/home/lcom/svn/proj/res/BH/BH_7.bmp");

	BH_R[7] = loadBitmap("/home/lcom/svn/proj/res/BH/BH_8.bmp");

	BH_R[8] = loadBitmap("/home/lcom/svn/proj/res/BH/BH_9.bmp");

	BH_R[9] = loadBitmap("/home/lcom/svn/proj/res/BH/BH_10.bmp");

	BH_R[10] = loadBitmap("/home/lcom/svn/proj/res/BH/BH_11.bmp");

	BH_R[11] = loadBitmap("/home/lcom/svn/proj/res/BH/BH_12.bmp");

	//printf("End inicialization of array\n");

	//printf("Start inicialization of array\n");

	BH_L[0] = loadBitmap("/home/lcom/svn/proj/res/BH/BH_1_E.bmp");

	BH_L[1] = loadBitmap("/home/lcom/svn/proj/res/BH/BH_2_E.bmp");

	BH_L[2] = loadBitmap("/home/lcom/svn/proj/res/BH/BH_3_E.bmp");

	BH_L[3] = loadBitmap("/home/lcom/svn/proj/res/BH/BH_4_E.bmp");

	BH_L[4] = loadBitmap("/home/lcom/svn/proj/res/BH/BH_5_E.bmp");

	BH_L[5] = loadBitmap("/home/lcom/svn/proj/res/BH/BH_6_E.bmp");

	BH_L[6] = loadBitmap("/home/lcom/svn/proj/res/BH/BH_7_E.bmp");

	BH_L[7] = loadBitmap("/home/lcom/svn/proj/res/BH/BH_8_E.bmp");

	BH_L[8] = loadBitmap("/home/lcom/svn/proj/res/BH/BH_9_E.bmp");

	BH_L[9] = loadBitmap("/home/lcom/svn/proj/res/BH/BH_10_E.bmp");

	BH_L[10] = loadBitmap("/home/lcom/svn/proj/res/BH/BH_11_E.bmp");

	BH_L[11] = loadBitmap("/home/lcom/svn/proj/res/BH/BH_12_E.bmp");

	//printf("End inicialization of array\n");

	BH_S[0] = loadBitmap("/home/lcom/svn/proj/res/BH/BH_Shoot0.bmp");

	BH_S[1] = loadBitmap("/home/lcom/svn/proj/res/BH/BH_Shoot1.bmp");

	//printf("Start creat AnimatedSprites\n");

	BH_right = create_AnimatedSprite(BH_R, 12, 0, 0, 0, 0);

	BH_left = create_AnimatedSprite(BH_L, 12, 0, 0, 0, 0);

	BH_shoot = create_AnimatedSprite(BH_S, 2, 0, 0, 0, 0);

	//printf("End creat AnimatedSprites\n");

	Bitmap **harpoonB = (Bitmap **) malloc(sizeof(Bitmap *) * 2);

	harpoonB[0] = loadBitmap("/home/lcom/svn/proj/res/harpoon.bmp");

	harpoonB[1] = loadBitmap("/home/lcom/svn/proj/res/harpoon_cable.bmp");

	BH_harpoon = create_AnimatedSprite(harpoonB, 2, 0, 0, 0, 0);

	printf("End Load BH Images\n");

}

void loadRH(){

	printf("Start creat AnimatedSprites\n");

	Bitmap *RH_bmp = loadBitmap("/home/lcom/svn/proj/res/RH/RH_0.bmp");

	RH = create_sprite(RH_bmp, 0, 0, 0, 0);



	life_RH = loadBitmap("/home/lcom/svn/proj/res/RH/life_RH.bmp");



	RH_R = (Bitmap **) malloc(sizeof(Bitmap *) * 12);

	RH_L = (Bitmap **) malloc(sizeof(Bitmap *) * 12);

	RH_S = (Bitmap **) malloc(sizeof(Bitmap *) * 2);

	//printf("Start inicialization of array\n");

	RH_R[0] = loadBitmap("/home/lcom/svn/proj/res/RH/RH_1.bmp");

	RH_R[1] = loadBitmap("/home/lcom/svn/proj/res/RH/RH_2.bmp");

	RH_R[2] = loadBitmap("/home/lcom/svn/proj/res/RH/RH_3.bmp");

	RH_R[3] = loadBitmap("/home/lcom/svn/proj/res/RH/RH_4.bmp");

	RH_R[4] = loadBitmap("/home/lcom/svn/proj/res/RH/RH_5.bmp");

	RH_R[5] = loadBitmap("/home/lcom/svn/proj/res/RH/RH_6.bmp");

	RH_R[6] = loadBitmap("/home/lcom/svn/proj/res/RH/RH_7.bmp");

	RH_R[7] = loadBitmap("/home/lcom/svn/proj/res/RH/RH_8.bmp");

	RH_R[8] = loadBitmap("/home/lcom/svn/proj/res/RH/RH_9.bmp");

	RH_R[9] = loadBitmap("/home/lcom/svn/proj/res/RH/RH_10.bmp");

	RH_R[10] = loadBitmap("/home/lcom/svn/proj/res/RH/RH_11.bmp");

	RH_R[11] = loadBitmap("/home/lcom/svn/proj/res/RH/RH_12.bmp");

	//printf("End inicialization of array\n");

	//printf("Start inicialization of array\n");

	RH_L[0] = loadBitmap("/home/lcom/svn/proj/res/RH/RH_1_E.bmp");

	RH_L[1] = loadBitmap("/home/lcom/svn/proj/res/RH/RH_2_E.bmp");

	RH_L[2] = loadBitmap("/home/lcom/svn/proj/res/RH/RH_3_E.bmp");

	RH_L[3] = loadBitmap("/home/lcom/svn/proj/res/RH/RH_4_E.bmp");

	RH_L[4] = loadBitmap("/home/lcom/svn/proj/res/RH/RH_5_E.bmp");

	RH_L[5] = loadBitmap("/home/lcom/svn/proj/res/RH/RH_6_E.bmp");

	RH_L[6] = loadBitmap("/home/lcom/svn/proj/res/RH/RH_7_E.bmp");

	RH_L[7] = loadBitmap("/home/lcom/svn/proj/res/RH/RH_8_E.bmp");

	RH_L[8] = loadBitmap("/home/lcom/svn/proj/res/RH/RH_9_E.bmp");

	RH_L[9] = loadBitmap("/home/lcom/svn/proj/res/RH/RH_10_E.bmp");

	RH_L[10] = loadBitmap("/home/lcom/svn/proj/res/RH/RH_11_E.bmp");

	RH_L[11] = loadBitmap("/home/lcom/svn/proj/res/RH/RH_12_E.bmp");

	//printf("End inicialization of array\n");

	RH_S[0] = loadBitmap("/home/lcom/svn/proj/res/RH/RH_Shoot0.bmp");

	RH_S[1] = loadBitmap("/home/lcom/svn/proj/res/RH/RH_Shoot1.bmp");



	//printf("Start creat AnimatedSprites\n");

	RH_right = create_AnimatedSprite(RH_R, 12, 0, 0, 0, 0);

	RH_left = create_AnimatedSprite(RH_L, 12, 0, 0, 0, 0);

	RH_shoot = create_AnimatedSprite(RH_S, 2, 0, 0, 0, 0);

	//printf("End creat AnimatedSprites\n");

	Bitmap **harpoonB = (Bitmap **) malloc(sizeof(Bitmap *) * 2);

	harpoonB[0] = loadBitmap("/home/lcom/svn/proj/res/harpoon.bmp");

	harpoonB[1] = loadBitmap("/home/lcom/svn/proj/res/harpoon_cable.bmp");

	RH_harpoon = create_AnimatedSprite(harpoonB, 2, 0, 0, 0, 0);

	printf("End Load BH Images\n");

}

void loadImages(){

	printf("Start Load Images\n");

	dayBackground = loadBitmap("/home/lcom/svn/proj/res/fundoDay.bmp");

	nightBackground = loadBitmap("/home/lcom/svn/proj/res/fundoNight.bmp");

	twoPcN = loadBitmap("/home/lcom/svn/proj/res/Menus/2pcN.bmp");

	twoPcP = loadBitmap("/home/lcom/svn/proj/res/Menus/2pcP.bmp");

	localN = loadBitmap("/home/lcom/svn/proj/res/Menus/localN.bmp");

	localP = loadBitmap("/home/lcom/svn/proj/res/Menus/localP.bmp");

	exitN = loadBitmap("/home/lcom/svn/proj/res/Menus/exitN.bmp");

	exitP = loadBitmap("/home/lcom/svn/proj/res/Menus/exitP.bmp");

	mouseN = loadBitmap("/home/lcom/svn/proj/res/mouseN.bmp");

	mouseP = loadBitmap("/home/lcom/svn/proj/res/mouseP.bmp");

	singleN = loadBitmap("/home/lcom/svn/proj/res/Menus/singleN.bmp");

	singleP = loadBitmap("/home/lcom/svn/proj/res/Menus/singleP.bmp");

	multiN = loadBitmap("/home/lcom/svn/proj/res/Menus/multiN.bmp");

	multiP = loadBitmap("/home/lcom/svn/proj/res/Menus/multiP.bmp");

	backN = loadBitmap("/home/lcom/svn/proj/res/Menus/backP.bmp");

	backP = loadBitmap("/home/lcom/svn/proj/res/Menus/backN.bmp");

	victory = loadBitmap("/home/lcom/svn/proj/res/victory.bmp");

	defeat = loadBitmap("/home/lcom/svn/proj/res/defeat.bmp");

	esc = loadBitmap("/home/lcom/svn/proj/res/esc.bmp");

	lifesBitmap = loadBitmap("/home/lcom/svn/proj/res/BH/lifes.bmp");

	menuBackground = loadBitmap("/home/lcom/svn/proj/res/Menu.bmp");

	loadTimeBMP();

	printf("End Load Images\n");

}

void drawEsc(){

	prepareDraw(esc,(getH_RES() - esc->bitmapInfoHeader.width) / 2,200);

}

void drawVictory(){

	prepareDraw(victory,(getH_RES() - victory->bitmapInfoHeader.width) / 2,100);

}

void drawDefeat(){

	prepareDraw(defeat,(getH_RES() - defeat->bitmapInfoHeader.width) / 2,100);

}


void drawFundo(){
	long unsigned time[2];
	rtc_getTime(time);
	if (time[0] >= hoursFromUser && time[1] >= minutesFromUser){
		prepareDraw(nightBackground,0,0);
	}
	else
	{
		prepareDraw(dayBackground,0,0);
	}
}

void drawLifes(int lifes){
	int x = 728, y = 700;
	prepareDraw(lifesBitmap,x,y);
	x += lifesBitmap->bitmapInfoHeader.width + 5;
	y -= 4;
	int i;
	for (i = 0; i < lifes; i++){
		prepareDraw(life_Hero, x, y);
		x += life_Hero->bitmapInfoHeader.width + 5;
	}
}

void drawLifes_RH(int lifes){
	int x = 50, y = 700;
	prepareDraw(lifesBitmap,x,y);
	x += lifesBitmap->bitmapInfoHeader.width + 5;
	y -= 4;
	int i;
	for (i = 0; i < lifes; i++){
		prepareDraw(life_RH, x, y);
		x += life_RH->bitmapInfoHeader.width + 5;
	}
}

void drawLocal(int pressed){
	if (pressed){
		prepareDraw(localP,(getH_RES() - localP->bitmapInfoHeader.width) / 2,375);
	}
	else
		prepareDraw(localN,(getH_RES() - localN->bitmapInfoHeader.width) / 2,375);
}

void drawTwoPc(int pressed){
	if (pressed){
		prepareDraw(twoPcP,(getH_RES() - twoPcP->bitmapInfoHeader.width) / 2,475);
	}
	else {
		prepareDraw(twoPcN,(getH_RES() - twoPcN->bitmapInfoHeader.width) / 2,475);
	}
}

void drawExit(int pressed){
	if (pressed){
		prepareDraw(exitP,(getH_RES() - exitP->bitmapInfoHeader.width) / 2,575);
	}
	else {
		prepareDraw(exitN,(getH_RES() - exitN->bitmapInfoHeader.width) / 2,575);
	}
}

void drawSingle(int pressed){
	if (pressed){
		prepareDraw(singleP,(getH_RES() - singleP->bitmapInfoHeader.width) / 2,375);
	}
	else {
		prepareDraw(singleN,(getH_RES() - singleN->bitmapInfoHeader.width) / 2,375);
	}
}

void drawMulti(int pressed){
	if (pressed){
		prepareDraw(multiP,(getH_RES() - multiP->bitmapInfoHeader.width) / 2,475);
	}
	else {
		prepareDraw(multiN,(getH_RES() - multiN->bitmapInfoHeader.width) / 2,475);
	}
}

void drawBack(int pressed){
	if (pressed){
		prepareDraw(backN,(getH_RES() - backN->bitmapInfoHeader.width) / 2,575);
	}
	else {
		prepareDraw(backP,(getH_RES() - backP->bitmapInfoHeader.width) / 2,575);
	}
}


void drawMouse(int pressed,int x,int y){
	if (pressed){
		prepareDraw(mouseP,x,y);
	}
	else {
		prepareDraw(mouseN,x,y);
	}
}

void draw_time(int counter, unsigned int xTime, unsigned int yTime){
	switch (counter)
	{
	case 0:
		prepareDraw(ZERO_Bitmap,xTime, yTime);
		break;
	case 1:
		prepareDraw(ONE_Bitmap,xTime, yTime);
		break;
	case 2:
		prepareDraw(TWO_Bitmap,xTime, yTime);
		break;
	case 3:
		prepareDraw(THREE_Bitmap,xTime, yTime);
		break;
	case 4:
		prepareDraw(FOUR_Bitmap,xTime, yTime);
		break;
	case 5:
		prepareDraw(FIVE_Bitmap,xTime, yTime);
		break;
	case 6:
		prepareDraw(SIX_Bitmap,xTime, yTime);
		break;
	case 7:
		prepareDraw(SEVEN_Bitmap,xTime, yTime);
		break;
	case 8:
		prepareDraw(EIGHT_Bitmap,xTime, yTime);
		break;
	case 9:
		prepareDraw(NINE_Bitmap,xTime, yTime);
		break;
	default:
		break;
	}
}
