/* tickpack.c 
 * Bob Wilson
 * 05/11/2003
 * As suggested in mp5 assignment, much code borrowed from mp4 timepack_sapc.c
*/
#include <tickpack.h>
#include <timer.h>
#include <pic.h>
#include <cpu.h>

extern IntHandler irq0inthand;

#define MICROSECS_PER_SEC 1000000
#define TICKS_PER_SEC  18.2

/* internal function prototypes */
void set_timer_count(int);
void smalldelay(void);
void irq0inthandc(void);

static int tickcount;
static int end_count;
static IntHandler *callback;

void init_ticks(int interval, IntHandler *app_tick_callback)
{
  tickcount = 0;
  end_count = interval * TICKS_PER_SEC / MICROSECS_PER_SEC;
  callback = app_tick_callback;
#ifdef DEBUG
  printf("Disabling interrupts in CPU while setting them up\n");
#endif
  cli();
#ifdef DEBUG  
  printf("Setting interrupt gate for timer, irq 0\n");
#endif
  /* irq 0 maps to slot n = 0x20 in IDT for linux setup */
  set_intr_gate(TIMER0_IRQ+IRQ_TO_INT_N_SHIFT, &irq0inthand);
#ifdef DEBUG
  printf("Commanding PIC to interrupt CPU for irq 0\n");
#endif
  pic_enable_irq(TIMER0_IRQ);
#ifdef DEBUG
  printf("Commanding timer to generate a pulse train using max count\n");
#endif
  set_timer_count(0);   
#ifdef DEBUG
  printf("Enabling interrupts in CPU\n");
#endif
  sti();
}
void shutdown_ticks()
{
  cli();
#ifdef DEBUG
  printf("Commanding PIC to shut off irq 0 to CPU\n");
#endif
  pic_disable_irq(TIMER0_IRQ);  /* disallow irq 0 ints to get to CPU */
  sti();
}

/* about 10 us on a SAPC (400Mhz Pentium) */
void smalldelay(void)
{
  int i;
    
  for (i=0;i<1000;i++)
     ;
}

/* Set up timer to count down from given count, then send a tick interrupt, */
/* over and over. A count of 0 sets max count, 65536 = 2**16 */
void set_timer_count(int count)
{
  outpt(TIMER_CNTRL_PORT, TIMER0|TIMER_SET_ALL|TIMER_MODE_RATEGEN);
  outpt(TIMER0_COUNT_PORT,count&0xff); /* set LSB here */
  outpt(TIMER0_COUNT_PORT,count>>8); /* and MSB here */
  smalldelay();                 /* give the timer a moment to init. */
}

/* timer interrupt handler */
void irq0inthandc(void)
{
  pic_end_int();                /* notify PIC that its part is done */
  tickcount++;                  /* count the tick in global var */
  if (tickcount >= end_count) {
    tickcount = 0;              /* reset tickcount */
    (*callback) ();		/* call application callback function */
  }  
}

