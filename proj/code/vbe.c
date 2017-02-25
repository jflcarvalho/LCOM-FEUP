#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>
#include <stdint.h>
#include "lmlib.h"
#include "vbe.h"


#define LINEAR_MODEL_BIT 14

#define PB2BASE(x) (((x) >> 4) & 0x0F000)
#define PB2OFF(x) ((x) & 0x0FFFF)


int vbe_get_mode_info(unsigned short mode, vbe_mode_info_t *vmi_p) {
	mmap_t buf;
	lm_init();
	lm_alloc(sizeof(vbe_mode_info_t), &buf);
	struct reg86u r;
	r.u.w.ax = 0x4F01; // VBE call, function 02 -- set VBE mode
	r.u.w.cx = 1<<14|mode; // set bit 14: linear framebuffer; if bit 14 is off : Use windowed frame buffer model
	r.u.b.intno = 0x10;
	r.u.w.es = PB2BASE(buf.phys);
	r.u.w.di = PB2OFF(buf.phys);

	if( sys_int86(&r) != OK ) {
		printf("return_vbe_mode information: sys_int86() failed \n");
		lm_free(&buf);
		return 1;
	}
	*vmi_p = *((vbe_mode_info_t *) buf.virtual);
	return 0;
}

/*int vbe_get_controller_info(VbeInfoBlock *vmi) {
	mmap_t bufMap;
	lm_init();
	lm_alloc(sizeof(VbeInfoBlock), &bufMap);
	struct reg86u r;
	r.u.w.ax = 0x4F00;
	r.u.w.es = PB2BASE(bufMap.phys);
	r.u.w.di = PB2OFF(bufMap.phys);
	r.u.b.intno = 0x10;

	if (sys_int86(&r) != OK) { // call BIOS
		printf("return_vbe_get_controller_info information: sys_int86() failed \n");
		lm_free(&bufMap);
		return 1;
	}

	*vmi = *(VbeInfoBlock *) bufMap.virtual;
	lm_free(&bufMap);
	return 0;
}*/




