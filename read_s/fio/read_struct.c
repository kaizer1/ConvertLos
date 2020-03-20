/*
   ## .c
 Loskutnikov sergey example read and write 07.06.2017
*/

#include "read_struct.h"

int always()
{
	 int uu = 3;
	return uu;
}


void callToWrite(const char* nameString)
{
    FILE *outfile;
   struct customer input;

   // open Accounts file for writing
   outfile = fopen (nameString,"w");
   if (outfile == NULL)
     {
      fprintf(stderr, "\nError opening accounts.dat\n\n");
      exit (1);
     }

   // instructions to user
   printf("Enter \"stop\" for First Name to end program.");

   // endlessly read from keyboard and write to file
  
      // prompt user
      printf("\nFirst Name: \n");
      //scanf ("%s", input.fname);
  
     // strcpy(input.fname,"Sergey_");
     // strcpy(input.lname,"Loskutnikov");
     // input.acct_num = 20;
     // input.acct_balance = 34.55;
 
    
   struct LosBestMeshNew  *newLos;
 printf(" ll fin  1 \n");
         newLos->vertexPolygonCount = 409;
      //   newLos.vertexPolygonCount = 405;
          printf(" ll fin 2 \n");
         newLos->uvCount = 30;
          printf(" ll fin  3\n");
         strcpy(newLos->ls_name_mesh, " Loskutnikov games ");

    printf(" ll fin \n");

      // write entire structure to Accounts file
      fwrite (&newLos, sizeof(struct customer), 1, outfile);
}



void readMyStruct(const char* inputFile)
{
   FILE *infile;
   struct customer input;

   /*** open the accounts file ***/
   infile = fopen (inputFile,"r");
   if (infile == NULL)
     {
      fprintf(stderr, "\nError opening accounts.dat\n\n");
      exit (1);
     }

   while (fread (&input, sizeof(struct customer), 1, infile))
      printf ("Name = %10s %10s   Acct Num = %8d   Balance = %8.2f\n",
              input.fname, input.lname, input.acct_num, input.acct_balance);
}







