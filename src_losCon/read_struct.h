/*
     ## .h
 Loskutnikov sergey example read and write 07.06.2017
*/

// #ifndef READ_STR
// # define READ_STR

#pragma once

//put your function headers here
 
// #ifdef __cplusplus
// extern "C" {
// #endif

/* Function declaration */
//void foo(int);


#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct customer {
   char  fname[20],lname[20];
   int   acct_num;
   float acct_balance;
};


 struct LosBestMeshNew
 {
 

   int tangentCount;
   int biNormalCount;

  const float*  lmNormal;
  const float*  lmUV;
  const float*  lmTangents; 
  const float*  lmBiNormal;

   int vertexPolygonCount; 
   int normalCount;
   int uvCount;


   char* ls_name_mesh;
  
  
      // LosBestMeshNew(char* name) 
      // {
      //     ls_name_mesh = name;
      // }


  //   LosBestMeshNew(float* norm, float* uv, float* tant, 
   //    float* biNorm) 
   //    { 
   // lmNormal = norm;
   // lmUV = uv;
   // lmTangents = tant;
   // lmBiNormal = biNorm;
   //    } 
 
 

  //  LosBestMeshNew() = default;
  //  LosBestMeshNew(LosBestMeshNew &&t_other) = default;
   // LosBestMeshNew(const LosBestMeshNew &) = default;
  //  LosBestMeshNew &operator=(LosBestMeshNew &&t_other) = default;
  //  LosBestMeshNew &operator=(const LosBestMeshNew &) = default;


 };




int always();
// {
// 	 int uu = 3;
// 	return uu;
// }
  
void callToWrite(const char* nameString);
// {
// 	 FILE *outfile;
//    struct customer input;

//    // open Accounts file for writing
//    outfile = fopen (nameString,"w");
//    if (outfile == NULL)
//      {
//       fprintf(stderr, "\nError opening accounts.dat\n\n");
//       exit (1);
//      }

//    // instructions to user
//    printf("Enter \"stop\" for First Name to end program.");

//    // endlessly read from keyboard and write to file
  
//       // prompt user
//       printf("\nFirst Name: \n");
//       //scanf ("%s", input.fname);
  
//      // strcpy(input.fname,"Sergey_");
//      // strcpy(input.lname,"Loskutnikov");
//      // input.acct_num = 20;
//      // input.acct_balance = 34.55;
 
    
//    struct LosBestMeshNew  *newLos;
//  printf(" ll fin  1 \n");
//          newLos->vertexPolygonCount = 409;
//       //   newLos.vertexPolygonCount = 405;
//           printf(" ll fin 2 \n");
//          newLos->uvCount = 30;
//           printf(" ll fin  3\n");
//          strcpy(newLos->ls_name_mesh, " Loskutnikov games ");

//     printf(" ll fin \n");

//       // write entire structure to Accounts file
//       fwrite (&newLos, sizeof(struct customer), 1, outfile);
// }



void readMyStruct(const char* inputFile);
// {
//    FILE *infile;
//    struct customer input;

//    /*** open the accounts file ***/
//    infile = fopen (inputFile,"r");
//    if (infile == NULL)
//      {
//       fprintf(stderr, "\nError opening accounts.dat\n\n");
//       exit (1);
//      }

//    while (fread (&input, sizeof(struct customer), 1, infile))
//       printf ("Name = %10s %10s   Acct Num = %8d   Balance = %8.2f\n",
//               input.fname, input.lname, input.acct_num, input.acct_balance);
// }

// #ifdef __cplusplus
// }
// #endif

//#endif