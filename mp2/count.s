
# Anwesh Joshi

    .text
    .globl _count

_count:
    pushl %ebp            #create stack 
    movl 8(%esp), %ecx    #accessing first parameter -- string 
    movl 12(%esp), %dl	  #accessing second parameter -- character
    movl $0, %eax         #counter
while:
    cmpb $0, (%ecx)      #compare character string with null character
    je done               #if null character jump to done label
    cmpb (%ecx), %dl      #comparing charater from input string to the charater
    jne else              
    addl $1, %eax         #increase counter by 1
    incl %ecx             #go to next string character
    jmp while             #jump back to while

else:
    incl %ecx		  #advance to next character string
    jmp while		  #go back to while label

done:
    popl %ebp		  #pop %ebp register parameters
    ret
    .end



