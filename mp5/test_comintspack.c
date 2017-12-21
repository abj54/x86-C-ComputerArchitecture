/* compile with "gcc -I $pcinc ..." to make IntHandler def available */
#include <stdio.h>
#include "comintspack.h"

#define DEV COM1		/* test com port DEV */

int main(void)
{
  init_comints(DEV, 1);
  printf("Waiting for interrupts from COM1 .... \n");

  /* delay loop while things happen at interrupt level */
  getchar();
  /* when user types anything on COM2, delay is over */

  printf("All done\n");
  shutdown_comints(DEV);
  return 0;
}

