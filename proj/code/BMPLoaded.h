#ifndef _BMP_LOAD_H
#define _BMP_LOAD_H

#include "graphics.h"
#include "bitmap.h"

extern char hoursFromUser; /** @brief Hours to Change to Night Mode inputed by user */
extern char minutesFromUser;	/** @brief Minutes to Change to Night Mode inputed by user */

/* MISCELLANEOUS */

Bitmap  *dayBackground;
Bitmap  *nightBackground;
Bitmap 	*menuBackground;
Bitmap	*twoPcN;
Bitmap	*twoPcP;
Bitmap	*localP;
Bitmap	*localN;
Bitmap	*exitBitmap;
Bitmap	*mouseN;
Bitmap	*mouseP;
Bitmap	*exitN;
Bitmap	*exitP;
Bitmap	*singleN;
Bitmap	*singleP;
Bitmap	*multiN;
Bitmap	*multiP;
Bitmap	*backN;
Bitmap	*backP;
Bitmap	*victory;
Bitmap	*defeat;
Bitmap	*esc;
Bitmap 	*backN;
Bitmap	*backP;
/* TIME */
Bitmap	*timeBitmap;
Bitmap	*ZERO_Bitmap;
Bitmap	*ONE_Bitmap;
Bitmap	*TWO_Bitmap;
Bitmap	*THREE_Bitmap;
Bitmap	*FOUR_Bitmap;
Bitmap	*FIVE_Bitmap;
Bitmap	*SIX_Bitmap;
Bitmap	*SEVEN_Bitmap;
Bitmap	*EIGHT_Bitmap;
Bitmap	*NINE_Bitmap;
/* BALLS */
Bitmap *ballExtraSmall;
Bitmap *ballSmall;
Bitmap *ballMedium;
Bitmap *ballLarge;
Sprite **balls;
int *ballsBool;
/* HEROS */
Bitmap **BH_R;
Bitmap **BH_L;
Bitmap **BH_S;
Bitmap **RH_R;
Bitmap **RH_L;
Bitmap **RH_S;
Sprite *BH;
Sprite *RH;
AnimatedSprite *BH_harpoon;
AnimatedSprite *BH_right;
AnimatedSprite *BH_left;
AnimatedSprite *BH_shoot;
AnimatedSprite *RH_harpoon;
AnimatedSprite *RH_right;
AnimatedSprite *RH_left;
AnimatedSprite *RH_shoot;
Bitmap	*lifesBitmap;
Bitmap	*life_Hero;
Bitmap	*life_RH;


/**
 * @fn void loadTimeBMP()
 * @brief Loads all bmps that are used to represent time
 *
 */

void loadTimeBMP();

/**
 * @fn void loadBall(int *nBalls)
 * @brief Loads all bmps that are used to represent balls
 *
 */

void loadBall(int *nBalls);

/**
 * @fn	void loadBH()
 * @brief Loads all bmps that are used to represent the blue hero
 *
 */

void loadBH();

/**
 * @fn void loadRH()
 * @brief Loads all bmps that are used to represent the red hero
 */

void loadRH();

/**
 * @fn void loadImages()
 * @brief Loads some bmps that are used on the main menu and some others
 *
 */

void loadImages();

/**
 * @fn	void drawEsc()
 * @brief Writes on a seconday buffer the "Press Esc to go Back" bmp
 *
 */

void drawEsc();

/**
 * @fn	void drawVictory()
 * @brief Writes on a seconday buffer a "Victory" bmp
 *
 */

void drawVictory();

/**
 * @fn	void drawDefeat()
 * @brief Writes on a seconday buffer a "Defeat" bmp
 *
 */

void drawDefeat();

/**
 * @fn	void drawFundo()
 * @brief Writes on a seconday buffer a "Background" bmp that depends on the local time
 *
 */


void drawFundo();

/**
 * @fn	void drawLifes(int lifes)
 * @brief Writes on a seconday buffer the lifes bmp´s of the Blue Hero
 *
 * @param lifes Number of lifes of the Blue Hero
 */


void drawLifes(int lifes);

/**
 * @fn	void drawLifes_RH(int lifes)
 * @brief Writes on a seconday buffer the lifes bmp´s of the Red Hero
 *
 * @param lifes Number of lifes of the Red Hero
 */


void drawLifes_RH(int lifes);

/**
 * @fn	void drawLocal(int pressed)
 * @brief Writes on a seconday buffer the Local Game bmp
 *
 * @param pressed 1-If mouse over, else 0;
 */

void drawLocal(int pressed);

/**
 * @fn	void drawTwoPc(int pressed)
 * @brief Writes on a seconday buffer the Two Pc Game bmp
 *
 * @param pressed 1-If mouse over, else 0;
 */


void drawTwoPc(int pressed);

/**
 * @fn	void drawExit(int pressed)
 * @brief Writes on a seconday buffer the Exit bmp
 *
 * @param pressed 1-If mouse over, else 0;
 */


void drawExit(int pressed);

/**
 * @fn	void drawSingle(int pressed)
 * @brief Writes on a seconday buffer the Single Local Game bmp
 *
 * @param pressed 1-If mouse over, else 0;
 */


void drawSingle(int pressed);

/**
 * @fn	void drawMulti(int pressed)
 * @brief Writes on a seconday buffer the Multiplayer Local Game bmp
 *
 * @param pressed 1-If mouse over, else 0;
 */


void drawMulti(int pressed);

/**
 * @fn	void drawBack(int pressed)
 * @brief Writes on a seconday buffer the Back bmp.
 *
 * @param pressed 1-If mouse over, else 0;
 */


void drawBack(int pressed);

/**
 * @fn	void drawMouse(int pressed,int x,int y)
 * @brief Writes on a seconday buffer the Mouse bmp.
 *
 * @param pressed 1-If mouse pressed, else 0;
 * @param x Mouse X coordinate
 * @param y Mouse Y coordinate
 */


void drawMouse(int pressed,int x,int y);

/**
 * @fn	void draw_time(int counter, unsigned int xTime, unsigned int yTime)
 * @brief Writes on a seconday buffer the Time bmp.
 *
 * @param counter Number to be drawn.
 * @param xTime Time X coordinate
 * @param yTime Time Y coordinate
 */
void draw_time(int counter, unsigned int xTime, unsigned int yTime);


#endif
