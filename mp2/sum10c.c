/* sum10c.c: driver for sum10.s: print out return value from sum10 code */
extern int sum10(void);
void main()
{
  printf("sum of 10 ints is %d\n",sum10());
}
