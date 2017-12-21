/* strcpyc.c: driver for testing assembly language mystrcpy function 
	Bob Wilson
	4/28/2016
   */

extern char *mystrcpy(char *s, char *ct);

int main()
{
   char s[100];
   char *ct ="Hello World\n";

   if(s == mystrcpy(s, ct))
      printf("The strcpy function returned the correct pointer value.\n");
   else
      printf("The strcpy function returned the wrong pointer value.\n");
   if (!strcmp(s,ct))
      printf("The strcpy function copied the data correctly.\n");
   else
      printf("The strcpy function copied the data incorrectly.\n");
   printf("The contents of array s are: %s", s);
   return 0;	
}
