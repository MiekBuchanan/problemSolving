
// Head_Copy.c -- this will copy some number of lines from the beginning of a file, right now set for 1000
//
// for filtering the EAM01.CSV for the change request from YYYYY:
//Excel column CJ (AM01-ACCT-ANAL-CODE) = '1' && DZ (AM01-NSF-FEE-WAIVE-1ST) = '0' or 'A'
//

// the c compiler switches were ""$(%watcom)/h;$(%watcom)/h/nt""

/* Change log:
        (*) 1.0  - initial release


*/

#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <ctype.h>
#include <time.h>
#include <stdint.h>

#define TRUE   1
#define FALSE  0
#define __STDC_FORMAT_MACROS 

const char compiled1[]   = " Compile date is ";
const char compiled2[]   = __DATE__;
const char Author[]      = " Written by Miek Buchanan ";
const char VERSION[] = "1.2 it keeps changing";


// start of main 
int main(int argc, char *argv[])    /* =================================================================== */

{


    FILE *fpin;             // input file pointer
    FILE *fpout;            // output file pointer
//    FILE *fpout2k;          // output file pointer for first 2000
    
    int lcin  = 0;          // input count
//    int tokInLine   = 0;
//    int tempInt     = 0;
//    int colCJ       = FALSE;    // does column CJ, "AM01-ACCT-ANAL-CODE" have something interesting?
//    int colDZ       = FALSE;    // does column DZ, "AM01-NSF-FEE-WAIVE-1ST" have something interesting?
    int makeEOF    = FALSE; 
//    char *tokeL;            // pointer for locating commas/ separating values -- left side token
//    char *tokeR;            // pointer for locating commas/ separating values -- right side token

    char inFileName[1024]   = "";
    char outFileName[1024]  = "";

    char strbufIN[10240]    = "";
//    char strbufOUT[10240]   = "";       

    printf("\n File copier. Program version: %s. Compiled: %s.\n", VERSION, __DATE__);
    printf(" The first parm is the file that's read in.\n If you give it a second parm, that is the number of lines that's copued to the second file");

//    time ( &rawtime );                              // get current date
//    timeinfo = localtime ( &rawtime );

//    strcpy(strbufIN, asctime (timeinfo));           // date to header line


    if (argc > 1)    // if we have more than one parm
    {
        printf(" Rec'd input file name.\n");
        strcpy(inFileName,   argv[1]);      // input file name
        strcpy(outFileName, inFileName);
        printf(" Input name %s.\n", inFileName);
        strcat(outFileName, ".trim");
        printf("Output name %s.\n", outFileName);

        for (tempInt = 1; tempInt < argc; tempInt++)
        {
            sprintf(tempStrM, "argv # %d is <%s>\n", tempInt, argv[tempInt]);
            reptLine(tempStrM);
        }
    }

    


//    if ((fpin=fopen("EAM01.csv", "r"))==NULL) {
/*    if ((fpin=fopen("EAM01_JZ.csv", "r"))==NULL) {
        printf("Cannot open input file \n");
        perror(" -- Error is "); 
        getch();
        exit(1);
    }
*/



    if ((fpin=fopen(inFileName, "r"))==NULL) {
        printf("Cannot open input file %s\n", inFileName);
        perror(" -- Error is "); 
        getch();
        exit(1);
    }


   if ((fpout=fopen(outFileName, "w"))==NULL) {
       printf("Cannot open output file %s.\n", outFileName);
       perror(" -- Error is "); 
       getch();
       exit(1);
   }


/*
    fgets(strbufIN, (sizeof(strbufIN)), fpin);      // get a record
    fputs(strbufIN, fpout);                         // ALWAYS WRITE OUT THE HEader record
    fputs(strbufIN, fpout2k);                       // ALWAYS WRITE OUT THE HEader record    
    
    fgets(strbufIN, (sizeof(strbufIN)), fpin);      // get a record, throw away header
    fgets(strbufIN, (sizeof(strbufIN)), fpin);      // get a record, throw away header
    fgets(strbufIN, (sizeof(strbufIN)), fpin);      // get a record, throw away header
    fgets(strbufIN, (sizeof(strbufIN)), fpin);      // get a record, throw away header
    fgets(strbufIN, (sizeof(strbufIN)), fpin);      // get a record, throw away header
    fgets(strbufIN, (sizeof(strbufIN)), fpin);      // get a record, throw away header
    fgets(strbufIN, (sizeof(strbufIN)), fpin);      // get a record, throw away header
  */      

    fgets(strbufIN, (sizeof(strbufIN)), fpin);      // get a record, throw away header
    if (feof(fpin)) makeEOF = TRUE;
    
    while(!makeEOF)
    {                            // start main program loop

        fputs(strbufIN, fpout);     // then this one is interesting
        fgets(strbufIN, (sizeof(strbufIN)), fpin);    // get a record
        lcin++;             // line counter
        if (lcin > 19999) makeEOF = TRUE;
        
    }
    

    printf(" done -- closing %d files\n", fcloseall() );
    getch();
    return 0;

}


// ===== end ===== //

