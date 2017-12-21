

.data
finalstring:
     	.asciz "__________"

.text
.globl _printbin

_printbin:
	pushl %ebp
	movl $4, %ecx
//	movb $4, %cl
	movl 8(%esp), %edx
//        nop//movl %edx, %ebp
	movl $finalstring, %eax
//	movl %eax, %ebp
	andl $0x000000f0, %edx
	sarl $4, %edx
	movl %edx, %ebx
	call donibble
	movb $' ', (%eax)
	incl %eax
//	nop//	movl $4, %ecx
	movl $4, %ecx	
	movl 8(%esp), %edx
	andl $0x0000000f, %edx
	movl %edx, %ebx
	call donibble
	movb $0,(%eax)
	popl %ebp
	movl $finalstring, %eax
	ret 
	

donibble:
	decl %ecx
	sarl %cl, %edx
	andl $1, %edx
	cmpl $0, %edx
	je CaseI
	jne CaseII
//	movl %ebx, (%edx)		
//	cmpl $0, %ecx
//	jne donibble
	

CaseI:
	movb $0x30, (%eax)
	incl %eax
	movl %ebx, %edx
	cmpl $0, %ecx
	jne donibble
	ret
	

CaseII:
	movb $0x31,(%eax)
	incl %eax
	movl %ebx, %edx
	cmpl $0, %ecx
	jne donibble
	ret

.end		


	
