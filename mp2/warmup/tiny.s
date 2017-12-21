# tiny.s: mp2warmup program

tiny.s:
	movl $8, %eax
	addl $3, %eax
	movl %eax, 0x200
	int $3
   	.end

