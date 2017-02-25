#include "video_gr.h"

/* Constants for VBE 0x105 mode */

/* The physical address may vary from VM to VM.
 * At one time it was 0xD0000000
 *  #define VRAM_PHYS_ADDR    0xD0000000 
 * Currently on lab B107 is 0xF0000000
 * Better run my version of lab5 as follows:
 *     service run `pwd`/lab5 -args "mode 0x105"
 */
#define VRAM_PHYS_ADDR	0xE0000000
#define H_RES             1024
#define V_RES		  768
#define BITS_PER_PIXEL	  8

/* Private global variables */

static char *video_mem;		/* Process address to which VRAM is mapped */
static phys_bytes phys_mem;
static unsigned h_res;		/* Horizontal screen resolution in pixels */
static unsigned v_res;		/* Vertical screen resolution in pixels */
static unsigned bits_per_pixel; /* Number of VRAM bits per pixel */
static char *double_buf;

/* Colors*/
#define BLACK 0
#define WHITE 63

void vg_clearScreen(unsigned short writeOnVRAM){
	vg_fill(BLACK,writeOnVRAM);
}

int vg_writeToVRAM(){
	memcpy(video_mem,double_buf,h_res*v_res);
	return 0;
}

char* vg_xpmToPix(char *xpm[], int *width, int *height){

	return (char *)read_xpm(xpm, width, height,h_res,v_res);

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

void vg_fill(unsigned long color, unsigned short writeOnVRAM) {
	int i;
	char *ptr;
	if (writeOnVRAM){
		ptr = video_mem;
	}else{
		ptr = double_buf;
	}
	for( i= 0; i< h_res*v_res; i++, ptr++) /*Handle a pixel at a time*/
	{
		*ptr = color;
	}
}


int vg_DrawSprite(Sprite * sp,unsigned short writeOnVRAM){
	if (sp->x >= h_res || sp->y >= v_res){
		return 1;
	}
	unsigned short xi,yi;
	for (yi = 0; yi < sp->height;yi++){
			for (xi = 0; xi < sp->width;xi++){
				if (vg_setPixel(xi+sp->x,yi+sp->y,*(sp->map + yi*sp->width + xi),writeOnVRAM)){
					return 1;
				}
			}
		}
		return 0;
}

int vg_DrawXPM(unsigned short x, unsigned short y, char * xpm[], unsigned short writeOnVRAM){
	if (x >= h_res || y >= v_res){
		return 1;
	}
	int wd,ht;
	unsigned short xi,yi;
	char * pixelsColor = read_xpm(xpm, &wd, &ht, h_res,v_res);
	for (yi = 0; yi < ht;yi++){
		for (xi = 0; xi < wd;xi++){
			if (vg_setPixel(xi+x,yi+y,*(pixelsColor + yi*wd + xi),writeOnVRAM)){
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
	double_buf = malloc(h_res*v_res);

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

void vg_printPhysAddress(){
	printf("Physical Address %x\n",phys_mem);
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


