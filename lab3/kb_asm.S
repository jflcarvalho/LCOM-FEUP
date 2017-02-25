#include "i8042.h"
.global _keyboardHandler_ASM

.text
_keyboardHandler_ASM:
			movl $0, %eax
			inb $STATUS_REG
			rcr $1, %eax
			jnc OutBNF
			inb $OUT_BUF
			jmp endfunc
OutBNF:
			movl $1, %eax
			jmp endfunc

endfunc:	ret
