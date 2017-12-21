
/******************************************************************
*
*   file:        testpack.c
*   author:      Ethan Bolker
*   date:        November, 1990
*
*   Driver to test the timing package.
*
*/

#include <stdio.h>
#include <timepack.h>

PROTOTYPE void delay( int ms );
PROTOTYPE char *return_msg( int rc );

int main(void) {
   int time;
   int startrc, stoprc;
   int secs, ms, precision, is_running;

   inittimer();
   querytimer( &precision, &is_running);
   if (is_running)
     printf("Problem: timer shouldn't be 'running' until starttimer is called\n");
   if (precision ==  T_DONT_KNOW)
     printf("querytimer: precision unknown\n");
   else
     printf("querytimer: precision %d us\n", precision);

   printf("---------------------------------------------------------\n");
   printf("First show that the provided timepack *can* keep time\n");
   printf("---------------------------------------------------------\n");
   for( secs = 1; secs < 4; secs++ ) {
      printf("\nwait about %d seconds (%d us)\n", secs , 1000000*secs);
      startrc = starttimer();
      delay( secs * 1000 /* ms */ );
      stoprc = stoptimer( &time );
      printf("starttimer returned %s\n", return_msg(startrc) );
      printf("timer counted %d us \n", time );
      printf("stoptimer returned %s\n", return_msg(stoprc) );
    }
   printf("\n");
   printf("---------------------------------------------------------\n");
   printf("Now do the harder test: can it measure 10ms? 20ms?\n");
   printf("Answer: only after timepack is made more accurate.\n");
   printf("---------------------------------------------------------\n");
   for( ms = 10; ms < 40; ms+=10 ) {
      printf("\nwait about %d ms (%d us)\n", ms , 1000*ms);
      startrc = starttimer();
      delay( ms );
      stoprc = stoptimer( &time );
      printf("starttimer returned %s\n", return_msg(startrc) );
      printf("timer counted %d us \n", time );
      printf("stoptimer returned %s\n", return_msg(stoprc) );
    }

   printf("\nstop stopped clock\n");
   printf("stoptimer returned %s\n", return_msg(stoptimer(&time)) );

 
   shutdowntimer();		/* undo init of timer */
   return 0;
}

/*************************************************************************
*
*  delay - without using timer, just CPU, approximate times.
*   Uses library call to make most of the execution time in the library code
*   so that it shouldn't make too much difference if *this* code is
*   is compiled with optimization flags such as -O or -O2.  Optimization
*   can affect times by a significant amount, maybe 50%.  (But note that
*   the library use disqualifies this as any kind of "benchmark".)
*/
#ifdef SAPC
/* for 400Mhz Pentium 
#define NLOOPS_PER_MSEC 3500 */
/* for higher speed SAPC VMs */
#define NLOOPS_PER_MSEC 5400
#else
/* for u13 (Sun Ultra5_10 Sparc) */
#define NLOOPS_PER_MSEC 3800
#endif

void delay( int ms ){
  char string1[] = "abcdefghij01234567890";
  char string2[sizeof(string1)];
  int i;

  for( i = NLOOPS_PER_MSEC*ms; i--; )
    strcpy(string2,string1);
  return;
}


/*************************************************************************
*
*  convert numerical return codes to strings to print
*
*/
char *return_msg( int rc ) {

    switch (rc) {
      case T_OK:        return("T_OK");
      case T_RUNNING:   return("T_RUNNING");
      case T_STOPPED:   return("T_STOPPED");
      case T_DONT_KNOW: return("T_DONT_KNOW");
      default:          return("");
    }
}

