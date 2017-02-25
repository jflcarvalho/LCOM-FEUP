#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/com.h>
#include "minix/sysutil.h"
#include "i8254.h"
#include "rtc.h"

char hours1;
char minutes1;

void rtc_getTime(long unsigned time[2]){
	/*sys_outb(RTC_ADDR_REG,RTC_MINUTES);
	sys_inb(RTC_DATA_REG,&time[1]);
	sys_outb(RTC_ADDR_REG,RTC_HOURS);
	sys_inb(RTC_DATA_REG,&time[0]);*/
	printf("Calling RTC Functions\n");
	rtc_get_time_asm();
	time[0] = hours1;
	time[1] = minutes1;
	//printf("Time : %x : %x", time[0],time[1]);
}
