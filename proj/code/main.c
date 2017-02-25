#include <minix/syslib.h> //message
#include <minix/drivers.h> //OK
#include "menu.h"
#include "BMPLoaded.h"
#include "uart.h"
#include "rtc.h"

#define GRAPHICS_MODE 0x117

static int proc_args(int argc, char **argv);

char hoursFromUser = 0x19; /** @brief Hours to Change to Night Mode inputed by user */
char minutesFromUser = 0;	/** @brief Minutes to Change to Night Mode inputed by user */

int main(int argc,char **argv){
	sef_startup();
	sys_enable_iop(SELF);
	if (argc == 1) {
		vg_init(GRAPHICS_MODE);
		loadImages();
		start_menu();
		vg_exit();
	}
	else
		proc_args(argc, argv);

	return 0;
}

static int proc_args(int argc, char **argv){
	char* ptr1;
	char* ptr2;
	if (strncmp(argv[1], "test_uart", strlen("test_uart")) == 0) {
		if (argc != 2) {
			printf("PANG: wrong no. of arguments for test_uart\n");
			return 1;
		}
		printf("test_uart(void)\n");
		return UART_Synchornize_Game();
	}
	else if (strncmp(argv[1], "rtc", strlen("rtc")) == 0){
		if (argc != 4){
			printf("RTC: format : 'rtc hours minutes'\n");
		}
		hoursFromUser = strtol(argv[2],&ptr1,16);
		minutesFromUser = strtol(argv[3],&ptr2,16);
		printf("Time read %x:%x",hoursFromUser,minutesFromUser);
		vg_init(GRAPHICS_MODE);
		loadImages();
		start_menu();
		vg_exit();
		return 0;
	}
	else {
		printf("PANG: %s - no valid function!\n", argv[1]);
		return 1;
	}
	return 0;
}
