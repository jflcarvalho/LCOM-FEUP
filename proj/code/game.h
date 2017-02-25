#ifndef _GAME_H
#define _GAME_H


/**
 * @fn	void prepareGame(int *counter, int nBalls)
 * @brief Resets Game Initial State
 *
 * @param counter Time in seconds*60
 * @param nBalls Number of balls
 */

void prepareGame(int *counter, int nBalls);

/**
 * @fn	void handleBalls(Sprite *sp)
 * @brief Calculates and sets a ball position
 *
 * @param sp Sprite of the ball to handle
 */

void handleBalls(Sprite *sp);

/**
 * @fn	void handleBH(int *counter, int nBalls)
 * @brief Handle blue hero position and check for colisions.
 *
 * @param counter Current time left in seconds * 60.
 * @param nBalls Number of balls loaded in current level (Not displayed + displayed)
 */

void handleBH(int *counter, int nBalls);

/**
 * @fn	void handleRH(int *counter, int nBalls)
 * @brief Handle red hero position and check for colisions.
 *
 * @param counter Current time left in seconds * 60.
 * @param nBalls Number of balls loaded in current level (Not displayed + displayed)
 */


void handleRH(int *counter, int nBalls);

/**
 * @fn	void keyboard_Handler(unsigned long scancode,  int *isDoubleByte, unsigned long *firstScan, unsigned long *RBR_char)
 * @brief Handles keyboard interruptions
 *
 * @param scancode	Scancode from keyboard
 * @param isDoubleByte	1 if Scancode is a double byte, 0 otherwise.
 * @param firstScan If scancode is double byte, firstScan will save the makecode;
 * @param RBR_char	Char that will be transfered through the serial port.
 */


void keyboard_Handler(unsigned long scancode,  int *isDoubleByte, unsigned long *firstScan, unsigned long *RBR_char);

/**
 * @fn	void balls_Collision_Handler(int i, int nBalls)
 * @brief Handles (splits) balls in case of collision with harpoon
 *
 * @param i	index of ball
 * @param nBalls	Number of balls loaded in current level (Not displayed + displayed)
 */

void balls_Collision_Handler(int i, int nBalls);

/**
 * @fn	int testCollision(Bitmap *obj1, int xObj1, int yObj1, Bitmap *obj2, int xObj2, int yObj2)
 * @brief Test colision between 2 Bitmaps
 *
 * @param obj1 Pointer to first Bitmap
 * @param xObj1 Coord x of first Bitmap
 * @param yObj1 Coord y of first Bitmap
 * @param obj2 Pointer to second Bitmap
 * @param xObj2 Coord x of second Bitmap
 * @param yObj2 Coord y of second Bitmap
 * @return 1 if collision detected, 0 otherwise
 */


int testCollision(Bitmap *obj1, int xObj1, int yObj1, Bitmap *obj2, int xObj2, int yObj2);

/**
 * @fn	void time_handler(int counter)
 * @brief Draws time left in a secondary buffer
 *
 * @param counter Time left in seconds * 60
 */


void time_handler(int counter);

/**
 * @fn	void handleVictory(int lives, int counter, int ballInGame)
 * @brief Checks if players won the game
 *
 * @param lives Number of Blue Hero lives + Red Hero lives
 * @param counter Time left in seconds * 60
 * @param ballInGame 1 if there are still balls on screen, 0 otherwise;
 */

void handleVictory(int lives, int counter, int ballInGame);

/**
 * @fn	int test_game()
 * @brief Main cycle, receives interruptions from Timer, Keyboard and UART port and calls respective handlers.
 *
 * @return 1 if some error happened, 0 otherwise;
 */

int test_game();


#endif
