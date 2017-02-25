#include "Keyboard.h"
#include "test3.h"
#include "i8042.h"
#include <limits.h>
#include <string.h>
#include <errno.h>


static void print_usage(char **argv);
static int proc_args(int argc, char **argv);
static unsigned long parse_ulong(char *str, int base);

int main(int argc, char **argv){
	sef_startup();
	sys_enable_iop(SELF);

	/* Enable IO-sensitive operations for ourselves */
								sys_enable_iop(SELF);

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
			"\t service run %s -args \"test_scan <deciman no. 0=C or 1=Assembly>\"\n"
			"\t service run %s -args \"leds <decimal no. list_size listOfLeds>\"\n",
			argv[0], argv[0]);
}


static int proc_args(int argc, char **argv)
{
	unsigned short ass, ledsSize, time;
	int i;
	if (strncmp(argv[1], "test_scan", strlen("test_scan")) == 0) {
		if (argc != 3) {
			printf("test_scan : wrong no. of arguments\n");
			return 1;
		}
		ass = parse_ulong(argv[2], 10);						/* Parses string to unsigned long */
		if (ass == ULONG_MAX)
			return 1;
		printf("kbd::kbd_test_scan(%lu)\n", ass);
		return kbd_test_scan(ass);
	}
	else if (strncmp(argv[1], "test_leds", strlen("test_leds")) == 0) {
		if (argc < 4) {
			printf("kbd: wrong no. of arguments for kbd_test_leds()\n");
			return 1;
		}

		ledsSize = parse_ulong(argv[2], 10);						/* Parses string to unsigned long */
		printf("Debugger: Leds Size = %d\n", ledsSize);
		if (ledsSize == ULONG_MAX){
			return 1;
		}
		unsigned short *ledsPointer = malloc (sizeof(unsigned short) * (argc - 2));
		for (i = 0; i < ledsSize; i++){
			ledsPointer[i] = parse_ulong(argv[i+3],10);
			printf("Debugger: ledsPointer[%d] = %d\n", i ,ledsPointer[i]);
		}
		printf("kbd::kbd_test_leds(%lu)\n", ledsSize);
		return kbd_test_leds(ledsSize, ledsPointer);
	}
	else if (strncmp(argv[1], "timed_scan", strlen("timed_scan")) == 0) {
		if (argc != 3) {
			printf("kbd: wrong no of arguments for kbd_test_timed_scan(unsigned short)\n");
			return 1;
		}
		time = parse_ulong(argv[2], 10);						// Parses string to unsigned long
		if (time == ULONG_MAX)
			return 1;
		printf("kbd::kbd_test_timed_scan(%lu)\n", time);
		return kbd_test_timed_scan(time);
	}
	else {
		printf("keyboard: %s - no valid function!\n", argv[1]);
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
		printf("keyboard: parse_ulong: no digits were found in %s\n", str);
		return ULONG_MAX;
	}

	/* Successful conversion */
	return val;
}
