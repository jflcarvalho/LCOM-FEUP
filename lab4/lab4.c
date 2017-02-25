#include "test4.h"
#include <limits.h>
#include <string.h>
#include <errno.h>


static void print_usage(char **argv);
static int proc_args(int argc, char **argv);
static unsigned long parse_ulong(char *str, int base);

int main(int argc, char **argv){
	sef_startup();

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
			"\t service run %s -args \"test_packet <decimal no.Packets_To_Read>\"\n"
			"\t service run %s -args \"test_async <decimal Idle_Time>\"\n"
			"\t service run %s -args \"test_config>\"\n"
			"\t service run %s -args \"test_gesture <decimal length>\"\n",
			argv[0], argv[0]);
}


static int proc_args(int argc, char **argv)
{
	unsigned short numPackets,idleTime,length;
	int i;
	if (strncmp(argv[1], "test_packet", strlen("test_packet")) == 0) {
		if (argc != 3) {
			printf("MouseLab : wrong no. of arguments for test_packet\n");
			return 1;
		}
		numPackets = parse_ulong(argv[2], 10);						/* Parses string to unsigned long */
		if (numPackets == ULONG_MAX)
			return 1;
		printf("test_packet(%lu)\n", numPackets);
		return test_packet(numPackets);
	}
	else if (strncmp(argv[1], "test_async", strlen("test_async")) == 0) {
		if (argc != 3) {
			printf("MouseLab : wrong no. of arguments for test_async\n");
			return 1;
		}

		idleTime = parse_ulong(argv[2], 10);						/* Parses string to unsigned long */
		if (idleTime == ULONG_MAX){
			return 1;
		}
		printf("test_async(%lu)\n", idleTime);
		return test_async(idleTime);
	}
	else if (strncmp(argv[1], "test_config", strlen("test_config")) == 0) {
		if (argc != 2) {
			printf("MouseLab : wrong no. of arguments for test_config\n");
			return 1;
		}
		printf("test_config(void)\n");
		return test_config();
	}
	else if (strncmp(argv[1], "test_gesture", strlen("test_gesture")) == 0) {
			if (argc != 3) {
				printf("MouseLab : wrong no. of arguments for test_gesture\n");
				return 1;
			}

			length = parse_ulong(argv[2], 10);						/* Parses string to unsigned long */
			if (length == ULONG_MAX){
				return 1;
			}
			printf("test_gesture(%lu)\n", length);
			return test_gesture(length);
		}

	else {
		printf("mouse: %s - no valid function!\n", argv[1]);
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
