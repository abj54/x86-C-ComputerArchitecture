

.text
.globl _mystrcpy

_mystrcpy:
	pushl %ebp
	movl 8(%esp),%eax
	movl 12(%esp),%edx
	movl %eax, %ebx
copy:
	cmpb $0, (%edx)
	je done
	movb (%edx), %cl
	movb %cl, (%eax)
	incl %eax
	incl %edx
	jmp copy

done:
//	popl %ebp
	movb $0, (%eax)
	movl %ebx, %eax
	popl %ebp
	ret

.end







