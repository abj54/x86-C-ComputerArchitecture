/* comintspack.c */
/* package for using com port interrupts */

#include <stdio.h>
#include <serial.h>
#include <pic.h>
#include <cpu.h>
#include "comintspack.h"

/* ascii characters used */
#define CR '\x0d'
#define LF '\x0a'

void irq4inthandc (void);
static void (*this_callback)(char *);
static char *this_buffer;
static int this_size;
static int this_mode;
static int cursor;

/* This assembler routine is supplied in the SAPC library */
/* in irq4.s, calls irq4inthandc */
extern IntHandler irq4inthand;

void init_comints (int mode, void (*callback)(char *), char *buffer, int size)
{
   /* This function expects to be called with interrupts disabled.
    * It may be called by the background between cli() and sti() calls
    * or by an interrupt service routine (ISR) where interrupts are off.
    * That is why this code does not call cli() or sti() itself.
    */

   /* Save the inputs statically for use in the ISR */
   this_callback = callback;
   this_buffer = buffer;
   this_size = size;
   this_mode = mode;
   cursor = 0;
   
   /* COM1 uses IRQ 4 */
   /* First drain any input out of the COM1's receiver--a full FIFO */
   /* (multichar buffer in UART) causes this UART to stop interrupting!  */
   while (inpt(COM1_BASE+UART_LSR)&UART_LSR_DR)/* while one there */
      inpt(COM1_BASE+UART_RX); /* pull another char out of UART */

   /* You fill in: */
   /* arm the interrupt using irq4inthand,
   i.e., put irq4inthand in the right place in the IDT */
  set_intr_gate(COM1_IRQ + IRQ_TO_INT_N_SHIFT,&irq4inthand ); 

   /* enable interrupts for COM1's IRQ in the PIC */
     pic_enable_irq(COM1_IRQ);
   if (mode == RECEIVE) {
     /* turn on receiver interrupts in the UART's IER */
  /*   status = inpt(COM1_BASE + UART_IER);
   //  status |= UART_IER_RDI;
   //  outpt(COM1_BASE + UART_IER, status); */ 
     outpt(COM1_BASE+UART_IER,UART_IER_RDI);
   }
   else {
     /* turn on transmitter interrupts in the UART's IER */
   /* //  status = inpt(COM1_BASE + UART_IER);
//     status |= UART_IER_THRI;
  //   outpt(COM1_BASE + UART_IER, status);  */
 outpt(COM1_BASE+UART_IER,UART_IER_THRI);
   }
}

void shutdown_comints ()
{
  /* This function expects to be called with interrupts disabled.
   * It may be called by the background between cli() and sti() calls
   * or by an interrupt service routine (ISR) where interrupts are off.
   * That is why this code does not call cli() or sti() itself.
   */

  /* You fill in: */
  /* disable COM1's IRQ in the PIC */
  pic_disable_irq(COM1_IRQ);

  /* turn off transmitter and receiver interrupts in COM1's IER */
 /* //unsigned char status;
//  status = inpt(COM1_BASE+UART_IER);
//  status &= UART_IER_RDI;
//  outpt(COM1_BASE + UART_IER, status);

  //status = inpt(COM1_BASE + UART_IER);
 // status &= UART_IER_THRI;*/
  outpt(COM1_BASE + UART_IER, 0);
}

/* this is called from irq4inthand, the assembler interrupt envelope */
/* that routine saves the C compiler scratch registers */
/* calls this function to process the interrupt */
/* restores the C compiler scratch registers and executes iret */
void irq4inthandc(void)
{
  char c;
  pic_end_int();                      /* notify pic that its part is done */
   
  if (this_mode == RECEIVE) {         /* receive interrupt */
    /* you write the code here */

c = inpt(COM1_BASE + UART_RX);
this_buffer[cursor++] = c;
/*outpt(COM1_BASE+UART_TX,c);*/
if(c == CR){
  /* printf("-->%s\n",this_buffer);*/
/*        outpt(COM1_BASE+UART_TX,'\n');*/
        this_buffer[cursor]= '\0';	
	cursor = 0;

	this_callback (this_buffer);
        outpt(COM1_BASE+UART_TX,'\n');
}
outpt(COM1_BASE+UART_TX,c);
}
  else {                            /* transmit interrupt */
    /* you write the code here */
c =  this_buffer[cursor++];
outpt(COM1_BASE+UART_IER,0);
outpt(COM1_BASE+UART_IER, UART_IER_THRI); /* kickstart TX interrupt */    
if(c != '\0' ){
   outpt(COM1_BASE + UART_TX,c);	}
else {
   outpt(COM1_BASE + UART_TX, '\n'); /*}*/
   cursor = 0;
  
   this_callback(this_buffer);
} }
} /*200920*/
