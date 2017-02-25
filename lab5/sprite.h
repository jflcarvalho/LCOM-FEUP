#ifndef SPRITE
#define SPRITE

#include "video_gr.h"

typedef struct {
int x, y; // current position
int width, height;  // dimensions
int xSpeed, ySpeed; // current speed
char*map;          // the pixmap
} Sprite;


Sprite *create_sprite(char*pic[], int x, int y,int xspeed, int yspeed);

void destroy_sprite(Sprite*sp);

#endif
