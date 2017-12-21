
//Anwesh Joshi

    .text
    .globl _echo
_echo:
    pushl %ebp            #pushing parameters to the stack
    movl %esp, %ebp       #saving %esp to %ebp
    movw 8(%ebp), %dx    #accessing first parameter 
    movb 12(%ebp), %cl	  #accessing second parameter
loop:
    movw 8(%ebp), %dx 	#get comport value and put in %dx
    addw $5, %dx	#add 5 to get to lsr bit
    inb (%dx), %al	#read from comport and put in %al
    andb $0x1, %al	#mask the result with 0x1
    jz loop		#check if receiver is ready or not
    movw 8(%ebp), %dx	#get comport value
    inb (%dx), %al	#read from comport and put in %al
    movb %al, %ah	#save %al value to %ah
    cmpb %al, %cl	#check the value of al with escape character
    je done		#if matches go to done
xmit:
    movw 8(%ebp), %dx	#get comport value
    addw $5, %dx	#add 5 to get to lsr bit
    inb (%dx), %al	#read from comport + lsr lsr bit
    andb $0x20, %al	#maske with 0x20 to %al
    jz xmit		#jump back to xmit if the xmit bit is not ready
    movw 8(%ebp), %dx	#get comport value
    movb %ah, %al	#move %ah value to %al
    outb %al, (%dx)	#output %al value to comport + lsrbit
    jmp loop		#go back to loop again to start
done:
    popl %ebp		  #pop %ebp register parameters
    ret
    .end

