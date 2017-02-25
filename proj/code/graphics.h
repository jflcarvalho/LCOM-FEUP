#ifndef __GRAPHICS_H
#define __GRAPHICS_H

#include "bitmap.h"


/*----------------------------------------SPRITE----------------------------------------*/

/**
 * @struct	AnimatedSprite
 *
 * @brief	An animated sprite that holds an array of pointers to Bitmaps
 */

typedef struct {
	int xi,yi; /**< @brief Initial Position */
	int x, y;  /**< @brief Current Position */
	int xSpeed, ySpeed; /**< @brief Current Speed */
	int nextBmp, nBMP; /**< @brief Next Bitmap index */
	Bitmap **bmps; /**< @brief Array of pointers to Bitmaps */
} AnimatedSprite;

/**
 * @struct	Sprite
 *
 * @brief	A Sprite that holds a Bitmap and his current State.
 */
typedef struct {
	int xi,yi; /**< @brief Initial Position */
	float t,yAce; /**< @brief Auxiliary variables to calculation next position */
	int x, y; /**< @brief Current Position */
	int width, height; /**< @brief Bitmap dimensions */
	float xSpeed, ySpeed; /**< @brief Current speed  */
	Bitmap *bmp;   /**< @brief Pointer to the bitmap */
} Sprite;


/**
 * @fn	AnimatedSprite* create_AnimatedSprite(Bitmap *bmp[], int nBMP, int x, int y,int xspeed, int yspeed)
 * @brief Creates an Animated Sprite
 *
 * @param bmp Array of pointers to bitmaps
 * @param nBMP Number of bitmaps in the array
 * @param x initial x coordinate
 * @param y initial y coordinate
 * @param xspeed initial speed in x axis
 * @param yspeed initial speed in y axis
 *
 * @return Pointer to an Animated Sprite
 */


AnimatedSprite* create_AnimatedSprite(Bitmap *bmp[], int nBMP, int x, int y,int xspeed, int yspeed);


/**
 * @fn	void destroy_AnimatedSprite(AnimatedSprite *aniSprite)
 * @brief Destroys an Animated Sprite
 *
 * @param aniSprite	Pointer to the animated sprite to destroy.
 */

void destroy_AnimatedSprite(AnimatedSprite *aniSprite);

/**
 * @fn	Sprite* create_sprite(Bitmap *pic, int x, int y,int xspeed, int yspeed)
 * @brief Creates a non-animated Sprite
 *
 * @param pic Pointer to the bitmap
 * @param x initial x coordinate
 * @param y initial y coordinate
 * @param xspeed initial speed in x axis
 * @param yspeed initial speed in y axis
 *
 * @return Pointer to a non-animated Sprite
 */


Sprite* create_sprite(Bitmap *pic, int x, int y,int xspeed, int yspeed);

/**
 * @fn	void destroy_sprite(Sprite*sp)
 * @brief Destroys a non-animated Sprite
 *
 * @param sp	Pointer to the non-animated sprite to destroy.
 */


void destroy_sprite(Sprite*sp);



/*---------------------------------------GET_INFO----------------------------------------*/

/**
 * @fn	unsigned getH_RES()
 * @brief Gets Screen Horizontal Resolution
 *
 * @return Screen Horizontal Resolution
 */


unsigned getH_RES();

/**
 * @fn	unsigned getV_RES()
 * @brief Gets Screen Vertical Resolution
 *
 * @return Screen Vertical Resolution
 */

unsigned getV_RES();

/**
 * @fn	char * getFrame_Buffer()
 * @brief Gets Pointer to frame buffer
 *
 * @return pointer to frame buffer
 */

char * getFrame_Buffer();

/**
 * @fn	char * get_Buffer()
 * @brief Gets Pointer to secondary frame buffer
 *
 * @return pointer to secondary frame buffer
 */

char * get_Buffer();

/*----------------------------------------VIDEO------------------------------------------*/

/** @defgroup graphics graphics
 * @{
 *
 * Functions for outputing data to screen in graphics mode
 */



/**
 * @fn	int vg_writeToVRAM()
 * @brief Copies secondary buffer to main buffer
 *
 * @return 0
 */
int vg_writeToVRAM();

/**
 * @fn	void vg_printPhysAddress()
 * @brief Prints main buffer physical address
 *
 */

void vg_printPhysAddress();

/**
 * @fn	void vg_fill(unsigned long color,unsigned short writeOnVRAM)
 * @brief Fills a buffer with the desired color.
 *
 * @param color Desired color to fill the screen with.
 * @param writeOnVRAM 1 to Write on main bufffer, 0 to write on seconday buffer.
 *
 */

void vg_fill(unsigned long color,unsigned short writeOnVRAM);

/**
 * @fn	int vg_setPixel(short x,short y, unsigned long color,unsigned short writeOnVRAM)
 * @brief Changes the color of a pixel based on is coordinates
 *
 * @param x Coordinate x of the pixel to be modified
 * @param y Coordinate y of the pixel to be modified
 * @param color New color of the pixel
 * @param writeOnVRAM 1 to Write on main buffer, 0 to write on seconday buffer.
 *
 * @return 0 if sucess, 1 otherwise.
 *
 */

int vg_setPixel(short x,short y, unsigned long color,unsigned short writeOnVRAM);

/**
 * @fn	int vg_drawLine(unsigned short xi, unsigned short yi, unsigned short xf, unsigned short yf, unsigned long color)
 * @brief Draw a line between two points with a choosen color.
 *
 * @param xi First point's x coordinate
 * @param yi Frist point's y coordinate
 * @param xf Last point's x coordinate
 * @param yf Last point's y coordinate
 * @param color Color of the line
 *
 * @return 0 upon sucess, 1 upon failure.
 */

int vg_drawLine(unsigned short xi, unsigned short yi, unsigned short xf, unsigned short yf, unsigned long color);

/**
 * @fn	void* vg_init(unsigned short mode)
 * @brief Initializes the video module in graphics mode
 *
 * Uses the VBE INT 0x10 interface to set the desired
 *  graphics mode, maps VRAM to the process' address space and
 *  initializes static global variables with the resolution of the screen,
 *  and the number of colors
 *
 * @param mode 16-bit VBE mode to set
 * @return Virtual address VRAM was mapped to. NULL, upon failure.
 */

void* vg_init(unsigned short mode);

/**
 * @fn	int vg_exit(void)
 * @brief Returns to default Minix 3 text mode (0x03: 25 x 80, 16 colors)
 *
 * @return 0 upon success, non-zero upon failure
 */

int vg_exit(void);

/**
 * @fn	void drawBitmap(Bitmap* bmp, int x, int y)
 * @brief Draw a bitmap in a given position on the screen.
 *
 * @param bmp Pointer to the bitmap to draw.
 * @param x Coordinate x of the bitmap.
 * @param y Coordinate y of the bitmap.
 *
 */

void drawBitmap(Bitmap* bmp, int x, int y);

/**
 * @fn	void prepareDraw(Bitmap* bmp, int x, int y)
 * @brief Writes a bitmap in a given position on the screen, but on the secondary buffer.
 *
 * After calling this function, one should call the draw() function in order to display the bitmap on the screen.
 *
 * @param bmp Pointer to the bitmap to draw.
 * @param x Coordinate x of the bitmap.
 * @param y Coordinate y of the bitmap.
 *
 */

void prepareDraw(Bitmap* bmp, int x, int y);

/**
 * @fn	void draw()
 * @brief Copies secondary buffer to main buffer
 */

void draw();

/** @} end of video_gr */

#endif /* __VIDEO_GR_H */
