/* compile with "gcc -I $pcinc ..." to make IntHandler def available */
#include <stdio.h>
#include "tickpack.h"

IntHandler printstar;
void fake_init_ticks(int interval, IntHandler *app_tick_callback);
/* We want a delay of over 10 secs, to see at least 2 stars */
/* Assuming about 4 instructions/loop, this will be about 12 secs */
#define CPU_MHZ 400000000
#define LOOPCOUNT (3*CPU_MHZ)

int main(void)
{
  int i;

  fake_init_ticks(5000000,&printstar); /* print one star via callback */

  init_ticks(5000000,&printstar); /* print star every 5 secs */
  for (i=0;i<LOOPCOUNT;i++)
    ;
  shutdown_ticks();
  return 0;
}

void printstar()
{
  putchar('*');
}

/* show a call of callback */
void fake_init_ticks(int interval, IntHandler *app_tick_callback)
{
  IntHandler *cb = app_tick_callback; /* show we can save it */

  printf("in fake init_ticks, calling callback only once...\n");
  (*cb)();
}
