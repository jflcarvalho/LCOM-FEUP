#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vbe.h"
#include "lmlib.h"
#include "graphics.h"

/* The physical address may vary from VM to VM.
 * At one time it was 0xD0000000
 *  #define VRAM_PHYS_ADDR    0xD0000000
 * Currently on lab B107 is 0xF0000000
 */


/* Private global variables */

static char *video_mem;		/* Process address to which VRAM is mapped */
static phys_bytes phys_mem; /* VRAM address */
static unsigned h_res;		/* Horizontal screen resolution in pixels */
static unsigned v_res;		/* Vertical screen resolution in pixels */
static unsigned bits_per_pixel; /* Number of VRAM bits per pixel */
static char *double_buf; /* Secondary buffer address */


AnimatedSprite* create_AnimatedSprite(Bitmap **bmp, int nBMP, int x, int y,int xspeed, int yspeed){
    AnimatedSprite *aniSprite = (AnimatedSprite*) malloc (sizeof(AnimatedSprite));
    if( aniSprite == NULL )
		return NULL;
    aniSprite->bmps = bmp;
    if( aniSprite->bmps == NULL ) {
		free(aniSprite);
		return NULL;
	}
    aniSprite->nextBmp = 0;
    aniSprite->nBMP = nBMP;
    aniSprite->xi = x;
    aniSprite->yi = y;
    aniSprite->xSpeed = xspeed;
    aniSprite->ySpeed = yspeed;
    return aniSprite;
}

void destroy_AnimatedSprite(AnimatedSprite *aniSprite){
    if( aniSprite == NULL )
		return;
    int i;
    for(i = 0; i < aniSprite->nBMP; i++){
        deleteBitmap(aniSprite->bmps[i]);
    }
	free(aniSprite);
	aniSprite = NULL;
}

Sprite *create_sprite(Bitmap *pic, int x, int y,int xspeed, int yspeed) {
	Sprite *sp = (Sprite*) malloc (sizeof(Sprite));
	if( sp == NULL )
		return NULL;
	// read the sprite pixmap
	sp->bmp = pic;
	if( sp->bmp == NULL ) {
		free(sp);
		return NULL;
	}
	sp->width = pic->bitmapInfoHeader.width;
	sp->height = pic->bitmapInfoHeader.height;
	sp->x = x;
	sp->y = y;
    sp->t = 0.5;
	sp->yAce = 0.5;
	sp->xi = x;
	sp->yi = y;
	sp->xSpeed = xspeed;
	sp->ySpeed = yspeed;
	return sp;
}

void destroy_sprite(Sprite*sp) {
	if( sp == NULL )
		return;
	deleteBitmap(sp->bmp);
	free(sp);
	sp = NULL;
}



unsigned getH_RES(){
	return h_res;
}

unsigned getV_RES(){
	return v_res;
}

char * getFrame_Buffer(){
	return video_mem;
}

char * getDouble_Buffer(){
	return double_buf;
}


int vg_writeToVRAM(){
	memcpy(video_mem,double_buf,h_res*v_res);
	return 0;
}

void vg_printPhysAddress(){
	printf("Physical Address %x\n",phys_mem);
}

void vg_fill(unsigned long color, unsigned short writeOnVRAM) {
	int i;
	char *ptr;
	if (writeOnVRAM){
		ptr = video_mem;
	}else{
		ptr = double_buf;
	}
	for( i= 0; i< h_res*v_res*2; i++, ptr++) /*Handle a pixel at a time*/
	{
		*ptr = color;
	}
}

int vg_setPixel(short x, short y, unsigned long color,unsigned short writeOnVRAM){
	if (x >= h_res || y >= v_res || x < 0 || y < 0){
		return 1;
	}

	char *pointerToVideoMem;
	if (writeOnVRAM){
		pointerToVideoMem = video_mem + y*h_res + x;

	}else{
		pointerToVideoMem = double_buf +y*h_res + x;
	}
	*pointerToVideoMem = color;
	return 0;
}

int vg_drawLine(unsigned short xi, unsigned short yi, unsigned short xf, unsigned short yf, unsigned long color){
	float dx = xf-xi;
	float dy = yf-yi;
	short x;
	short y;
	if ((dx > 0 ? dx:-dx) >= (dy > 0 ? dy:-dy) && dx!= 0){
		for (x = xi; x <= xf; x++){
			y = (yi + dy*(x-xi)/dx);
			if (vg_setPixel(x,y,color,1)){
				vg_exit();
				return 1;
			}
		}
		for (x = xi; x >= xf; x--){
			y = (yi + dy*(x-xi)/dx);
			if (vg_setPixel(x,y,color,1)){
				vg_exit();
				return 1;
			}
		}
	}
	else{
		for (y = yi; y <= yf; y++){
			x = (xi + dx*(y-yi)/dy);
			if (vg_setPixel(x,y,color,1)){
				vg_exit();
				return 1;
			}
		}
		for (y = yi; y >= yf; y--){
			x = (xi + dx*(y-yi)/dy);
			if (vg_setPixel(x,y,color,1)){
				vg_exit();
				return 1;
			}
		}
	}
	return 0;
}

void * vg_init(unsigned short mode){ // mode could be unsigned char{

	struct reg86u r;
	r.u.w.ax = 0x4F02; // VBE call, function 02 -- set VBE mode
	r.u.w.bx = 1<<14|mode; // set bit 14: linear framebuffer; if bit 14 is off : Use windowed frame buffer model
	r.u.b.intno = 0x10;
	if( sys_int86(&r) != OK ) {
		printf("set_vbe_mode: sys_int86() failed \n");
		return NULL;
	}

	if (r.u.b.ah != 0){
		return NULL;
	}
	vbe_mode_info_t vbe_info;
	vbe_get_mode_info(mode,&vbe_info);

	bits_per_pixel=vbe_info.BitsPerPixel;
	v_res = vbe_info.YResolution;
	h_res = vbe_info.XResolution;
	double_buf = malloc(h_res*v_res*bits_per_pixel/8);

	int m;
	struct mem_range mr;

	phys_mem=(phys_bytes)vbe_info.PhysBasePtr; // Habilitar mapeamento de memoria
	mr.mr_base = (phys_bytes)vbe_info.PhysBasePtr;
	mr.mr_limit = mr.mr_base + h_res * v_res *bits_per_pixel/8;

	if( OK != (m = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr))){
		panic("video_txt: sys_privctl (ADD_MEM) failed: %d\n", m);
	}

	video_mem = vm_map_phys(SELF, (void *)mr.mr_base, h_res*v_res*bits_per_pixel/8); //Map Memory

	if(video_mem == MAP_FAILED){
		panic("video_txt couldn't map video memory");
	}
	return video_mem;
}

int vg_exit() {
	struct reg86u reg86;

	reg86.u.b.intno = 0x10; /* BIOS video services */

	reg86.u.b.ah = 0x00;    /* Set Video Mode function */
	reg86.u.b.al = 0x03;    /* 80x25 text mode*/

	if( sys_int86(&reg86) != OK ) {
		printf("\tvg_exit(): sys_int86() failed \n");
		return 1;
	} else
	{
		return 0;
	}
}



void prepareDraw(Bitmap* bmp, int x, int y) {
    if (bmp == NULL)
        return;

    int width = bmp->bitmapInfoHeader.width;
    int drawWidth = width;
    int height = bmp->bitmapInfoHeader.height;

    if (x + width < 0 || x > getH_RES() || y + height < 0
            || y > getV_RES())
        return;

    int xCorrection = 0;
    if (x < 0) {
        xCorrection = -x;
        drawWidth -= xCorrection;
        x = 0;

        if (drawWidth > getH_RES())
            drawWidth = getH_RES();
    } else if (x + drawWidth > getH_RES()) {
        drawWidth = getH_RES() - x;
    }

    char* bufferStartPos;
    char* imgStartPos;

    int i;
    for (i = 0; i < height ; i++) {
        int pos = y + height - 1 - i;

        if (pos < 0 || pos >= getV_RES())
            continue;

        bufferStartPos = getDouble_Buffer();
        bufferStartPos += x * 2 + pos * getH_RES() * 2;

        imgStartPos = bmp->bitmapData + xCorrection * 2 + i * width * 2;
        //memcpy(bufferStartPos, imgStartPos, drawWidth * 2);
        int j;
        for(j = 0; j < drawWidth * 2; j += 2){
            if(imgStartPos[j]!=0x1f && imgStartPos[j+1] != 0xfffffff8){
                bufferStartPos[j] = imgStartPos[j];
                bufferStartPos[j+1] = imgStartPos[j+1];
            }
        }
    }
}


void draw(){
    int width = getH_RES();
    int height = getV_RES();
    char* bufferStartPos = getFrame_Buffer();
    char* imgStartPos = getDouble_Buffer();
    memcpy(bufferStartPos, imgStartPos, height * width * 2);
}

void drawBitmap(Bitmap* bmp, int x, int y) {
    if (bmp == NULL)
        return;

    int width = bmp->bitmapInfoHeader.width;
    int drawWidth = width;
    int height = bmp->bitmapInfoHeader.height;

    if (x + width < 0 || x > getH_RES() || y + height < 0
            || y > getV_RES())
        return;

    int xCorrection = 0;
    if (x < 0) {
        xCorrection = -x;
        drawWidth -= xCorrection;
        x = 0;

        if (drawWidth > getH_RES())
            drawWidth = getH_RES();
    } else if (x + drawWidth > getH_RES()) {
        drawWidth = getH_RES() - x;
    }

    char* bufferStartPos;
    char* imgStartPos;

    int i;
    for (i = 0; i < height ; i++) {
        int pos = y + height - 1 - i;

        if (pos < 0 || pos >= getV_RES())
            continue;

        bufferStartPos = getFrame_Buffer();
        bufferStartPos += x * 2 + pos * getH_RES() * 2;

        imgStartPos = bmp->bitmapData + xCorrection * 2 + i * width * 2;
        //memcpy(bufferStartPos, imgStartPos, drawWidth * 2);
        int j;
        for(j = 0; j < drawWidth * 2; j += 2){
            if(imgStartPos[j]!=0x1f && imgStartPos[j+1] != 0xfffffff8){
                bufferStartPos[j] = imgStartPos[j];
                bufferStartPos[j+1] = imgStartPos[j+1];
            }
        }
    }
}


