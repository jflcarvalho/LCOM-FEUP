#include <limits.h>
#include <string.h>
#include <errno.h>
#include "test5.h"
#include "pixmap.h"

static void print_usage(char **argv);
static int proc_args(int argc, char **argv);
static unsigned long parse_ulong(char *str, int base);

int main(int argc, char **argv){
	sef_startup();

	if (argc == 1) {					/* Prints usage of the program if no arguments are passed */
		print_usage(argv);
		return 0;
	}
	else
		return proc_args(argc, argv);
}

static void print_usage(char **argv)
{
	printf("Usage: one of the following:\n"
			"\t service run %s -args \"test_init(mode,delay)\"\n"
			"\t service run %s -args \"test_square(x, y, size, color)\"\n"
			"\t service run %s -args \"test_line(xi, yi, xf,yf, color)\"\n"
			"\t service run %s -args \"test_xpm(xi, yi, char *xpm[])\"\n"
			"\t service run %s -args \"test_move(xi,yi,*xpm[],hor,delta,time)\"\n"
			"\t service run %s -args \"test_controller()\"\n",
			argv[0], argv[0],argv[0], argv[0],argv[0], argv[0]);
}


static int proc_args(int argc, char **argv)
{
	unsigned short mode,delay;
	unsigned short x,y,size,color;
	unsigned short xi,yi,xf,yf;
	char **xpm;
	unsigned short hor,delta,time;

	/*---------------------------------------TEST INIT--------------------------------------------------*/

	if (strncmp(argv[1], "test_init", strlen("test_init")) == 0) {
		if (argc != 4) {
			printf("GraphicsLab : wrong no. of arguments for test_init\n");
			return 1;
		}
		mode = parse_ulong(argv[2], 10);
		delay = parse_ulong(argv[3],10);
		if (mode == ULONG_MAX || delay == ULONG_MAX){
			return 1;
		}
		printf("test_init(%lu,%lu)\n",mode,delay);
		return test_init(mode,delay);
	}

	/*---------------------------------------TEST SQUARE--------------------------------------------------*/

	else if (strncmp(argv[1], "test_square", strlen("test_square")) == 0) {
		if (argc != 6) {
			printf("GraphicsLab : wrong no. of arguments for test_square\n");
			return 1;
		}

		x = parse_ulong(argv[2], 10);						/* Parses string to unsigned long */
		if (x == ULONG_MAX){
			return 1;
		}
		y= parse_ulong(argv[3], 10);
		if (y == ULONG_MAX){
			return 1;
		}
		size= parse_ulong(argv[4], 10);
		if (size == ULONG_MAX){
			return 1;
		}
		color=parse_ulong(argv[5], 10);
		if (color == ULONG_MAX){
			return 1;
		}
		printf("test_square(%lu,%lu,%lu,%lu)\n", x,y,size,color);
		return test_square(x,y,size,color);
	}


	/*---------------------------------------TEST LINE--------------------------------------------------*/

	else if (strncmp(argv[1], "test_line", strlen("test_line")) == 0) {
		if (argc != 7) {
			printf("GraphicsLab : wrong no. of arguments for test_line\n");
			return 1;
		}
		xi=parse_ulong(argv[2], 10);
		yi=parse_ulong(argv[3], 10);
		xf=parse_ulong(argv[4], 10);
		yf=parse_ulong(argv[5], 10);
		color=parse_ulong(argv[6], 10);
		if (xi == ULONG_MAX || yi == ULONG_MAX || xf == ULONG_MAX || yf == ULONG_MAX || color == ULONG_MAX){
			return 1;
		}
		printf("test_line(%lu,%lu,%lu,%lu,%lu)\n", xi,yi,xf,yf,color);
		return test_line(xi,yi,xf,yf,color);

	}

	/*---------------------------------------TEST CENA MARADA--------------------------------------------------*/
	else if (strncmp(argv[1], "cena_marada", strlen("cena_marada")) == 0) {
		if (argc != 6) {
			printf("GraphicsLab : wrong no. of arguments for cena_marada\n");
			return 1;
		}
		xi=parse_ulong(argv[2], 10);
		yi=parse_ulong(argv[3], 10);
		xf=parse_ulong(argv[4], 10);
		yf=parse_ulong(argv[5], 10);
		if (xi == ULONG_MAX || yi == ULONG_MAX || xf == ULONG_MAX || yf == ULONG_MAX){
			return 1;
		}
		printf("test_line(%lu,%lu,%lu,%lu)\n", xi,yi,xf,yf);
		return cena_marada(xi,yi,xf,yf);

	}

	/*---------------------------------------TEST XPM--------------------------------------------------*/
	else if (strncmp(argv[1], "test_xpm", strlen("test_xpm")) == 0) {
		if (argc != 5) {
			printf("GraphicsLab : wrong no. of arguments for test_xpm \n");
			return 1;
		}
		xi = parse_ulong(argv[2], 10);
		yi = parse_ulong(argv[3], 10);
		if (xi == ULONG_MAX || yi == ULONG_MAX){
			return 1;
		}
		if(strncmp(argv[4], "pic1", strlen("pic1")) == 0){
			xpm = pic1;
		}else if(strncmp(argv[4], "pic2", strlen("pic2")) == 0){
			xpm = pic2;
		}else if(strncmp(argv[4], "cross", strlen("cross")) == 0){
			xpm = cross;
		}else if(strncmp(argv[4], "pic3", strlen("pic3")) == 0){
			xpm = pic3;
		}else if(strncmp(argv[4], "penguin", strlen("penguin")) == 0){
			xpm = penguin;
		}else{
			printf("\nGraphicsLab::test_xpm. Try using pic1, pic2, pic3, cross or penguin\n");
			return 1;
		}
		printf("GraphicsLab: test_xpm\n");
		if(test_xpm(xi, yi, xpm) != 0){
			printf("GraphicsLab : test_xpm error!\n");
			return 1;
		}
		return 0;
	}

	/*---------------------------------------TEST MOVE--------------------------------------------------*/

	else if (strncmp(argv[1], "test_move", strlen("test_move")) == 0) {
		if (argc != 8) {
			printf("GraphicsLab: wrong no. of arguments in test_move \n");
			return 1;
		}

		xi = parse_ulong(argv[2], 10);
		yi = parse_ulong(argv[3], 10);
		hor= parse_ulong(argv[5], 10);
		delta = parse_ulong(argv[6], 10);
		time = parse_ulong(argv[7], 10);
		if (xi == ULONG_MAX || yi == ULONG_MAX || hor == ULONG_MAX || delta == ULONG_MAX || time == ULONG_MAX){
			return 1;
		}

		if(strncmp(argv[4], "pic1", strlen("pic1")) == 0){
			xpm = pic1;
		}else if(strncmp(argv[4], "pic2", strlen("pic2")) == 0){
			xpm = pic2;
		}else if(strncmp(argv[4], "cross", strlen("cross")) == 0){
			xpm = cross;
		}else if(strncmp(argv[4], "pic3", strlen("pic3")) == 0){
			xpm = pic3;
		}else if(strncmp(argv[4], "penguin", strlen("penguin")) == 0){
			xpm = penguin;
		}else{
			printf("GraphicsLab:test_move. Try using pic1, pic2, pic3, cross or penguin\n");
			return 1;
		}

		printf("GraphicsLab:test_move\n");
		if(test_move(xi, yi, xpm,hor,delta, time) != 0){
			printf("GraphicsLab: Mistakes Happen!\n");
			return 1;
		}
		return 0;

	}

	/*-------------------------------------TEST CONTROLLER --------------------------------*/
	else if (strncmp(argv[1], "test_controller", strlen("test_controller")) == 0) {
		if (argc != 2) {
			printf("GraphicsLab : wrong no. of arguments for test_controller\n");
			return 1;
		}
		printf("test_controller(void)\n");
		return test_controller();
	}
	else {
		printf("GraphicsLab: %s - no valid function!\n", argv[1]);
		return 1;
	}
}

static unsigned long parse_ulong(char *str, int base)
{
	char *endptr;
	unsigned long val;

	/* Convert string to unsigned long */
	val = strtoul(str, &endptr, base);

	/* Check for conversion errors */
	if ((errno == ERANGE && val == ULONG_MAX) || (errno != 0 && val == 0)) {
		perror("strtoul");
		return ULONG_MAX;
	}

	if (endptr == str) {
		printf("parse_ulong: no digits were found in %s\n", str);
		return ULONG_MAX;
	}

	/* Successful conversion */
	return val;
}
