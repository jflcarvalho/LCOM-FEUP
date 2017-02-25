#define RTC_ADDR_REG 0x70
#define RTC_DATA_REG 0x71
#define RTC_HOURS	4
#define RTC_MINUTES	2

.intel_syntax

.global _rtc_get_time_asm
.global _hours1
.global _minutes1

.text

_rtc_get_time_asm:
	xor eax,eax
	movb al, RTC_HOURS
	out RTC_ADDR_REG, al
	in  al, RTC_DATA_REG	//al contains hours
	movb _hours1, al	//Assigns al to global variable hours
	movb al, RTC_MINUTES
	out RTC_ADDR_REG, al
	in al, RTC_DATA_REG	// al contains minutes
	movb _minutes1,al	//Assigns al to global variable minutes
	ret

