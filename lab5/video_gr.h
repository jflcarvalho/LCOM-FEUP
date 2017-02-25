#ifndef __VIDEO_GR_H
#define __VIDEO_GR_H

#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <string.h>
#include "vbe.h"
#include "read_xpm.h"
#include "pixmap.h"
#include "sprite.h"
#include "lmlib.h"

void vg_clearScreen(unsigned short writeOnVRAM);

int vg_writeToVRAM();

void vg_printPhysAddress();

void vg_fill(unsigned long color,unsigned short writeOnVRAM);

int vg_DrawXPM(unsigned short x, unsigned short y, char * xpm[], unsigned short writeOnVRAM);

char* vg_xpmToPix(char *xpm[], int *width, int *height);

int vg_setPixel(short x,short y, unsigned long color,unsigned short writeOnVRAM);

/** @defgroup video_gr video_gr
 * @{
 *
 * Functions for outputing data to screen in graphics mode
 */

/**
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

//int vg_map();


void* vg_init(unsigned short mode);

 /**
 * @brief Returns to default Minix 3 text mode (0x03: 25 x 80, 16 colors)
 * 
 * @return 0 upon success, non-zero upon failure
 */
int vg_exit(void);

 /** @} end of video_gr */
 
#endif /* __VIDEO_GR_H */
