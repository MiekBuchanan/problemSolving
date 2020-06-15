
// This will read in lines of a file passed on the command line
// If there is a double quote, it will verify that there an even number of double quotes on the line
// 

/* Change log:

    (*) 1.0   - initial release
    (*) 2.0   - if passed a second parameter, fix (three chars lower case),
                it will append and additional double quote to the line
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
//#include <time.h>
#include <conio.h>
#include <errno.h>
#include <math.h>

#define TRUE   1
#define FALSE  0

const char compiled1[]      = " Compile date is ";
const char compiled2[]      = __DATE__;
const char Author[]         = " Written by Miek Buchanan ";
const char VERSION[]        = "2.0";




// function list ======

//void fixThisLine(void);        // get a line from the site.ini

//

// start of main                    // ===============================================================================



void main(int argc, char *argv[])

{

    char strbufIN[2048] = "";    

    FILE *fpin;             // input file
    FILE *fpout;            // output file

//    char *findA_DQ;   // pointer to the double quote
    char *findDot;  // find the last dot in the file name
    
    int x           = 0;
    int lineCount   = 0;        // input line count
    int unBalanced  = 0;        // count of lines that had unmatched quotes
    int qol         = 0;        // number of quotes found on line
    int fixLine     = FALSE;    // should we fix line?
    int sbIN_len    = 0;        // input line length   
    double isItOdd  = 0;        // should have 1 if odd, 0 if even



    printf("\n Checks if there is an even number of double quotes in a CSV file.\n Will concatenate a '\"' to the end of you add parameter 'fix'\n");
    printf(" Program version %s,Compiled %s\n", VERSION, __DATE__);


    if (argc < 2)  {
        printf("\n You must enter an input file name to chach for balanced double qoutes.\n");
        getch();
        exit(2);        
    }
    else
    if (argc = 2 && (strncmp(argv[1], "fix", 3) ))
    {
        printf("\n Received parameter 'fix', will add a double quote to odd lines.\n");
        fixLine  = TRUE;
    }


//    if ((fpin=fopen(argv[1], "rt"))==NULL) {
    if ((fpin=fopen("STRATA_EOD.CSV", "rt"))==NULL) {
        printf(" Can not open input file \"%s\".\a\n", argv[1]);
        perror(" Error is ");
        getch();
        exit(3); 
    }

    if (fixLine)        // if we are supposed to fix uneven quotes
    {
        memset(strbufIN, '\0', sizeof(strbufIN));
        strcpy(strbufIN, argv[1]);
//        strcpy(strbufIN, "STRATA_EOD.CSV");        

        findDot = strrchr(strbufIN, '.');   // find right-most dot in file name

        if (findDot)
        {
            memmove(findDot+4, findDot, 4);     // move four places
            memcpy(findDot+1, "fix", 3);          // add "fix in file name"            
        }
        else{
            strcat(strbufIN, ".fix");
        }
        if ((fpout=fopen(strbufIN, "wt"))==NULL) {
            printf(" Can not open output file \"%s\".\a\n", strbufIN);
            perror(" Error is ");
            getch();
            exit(5);
        }
    }

// ===== files open, get started =====

    do {

        memset(strbufIN, '\0', sizeof(strbufIN));       // make sure buffer /0 filled
        fgets (strbufIN, sizeof(strbufIN), fpin);       // get input record
        lineCount++;
        qol = 0;

//        findA_DQ = strbufIN;              // set string pointer at beginning of the storage after read
        sbIN_len = strlen(strbufIN);        // length of this line

        if (!feof(fpin) )                   // if not EOF
        {
            for (x = 0; x < (sbIN_len); x++)
            {
                if (strbufIN[x] == '"') qol++;      // if this is a '"', add one to count
            }

            isItOdd = fmod( qol , 2);                      // divide count by 2, is it odd?

            if (isItOdd)
            {
                memmove(strbufIN+sbIN_len-1, "\"\n\0\0", 3);    // add a quote, new line, and end of string, -1 for cr/lf
                unBalanced++;
            }
        }
        
        if (fixLine)   fputs(strbufIN, fpout);      // if we are supposed to fix uneven quotes write it out

    }   while (!feof(fpin));

    printf("\n Closing %d files.\n Found %d line(s) with unbalanced quotes.\n Total lines are %d\n", fcloseall(), unBalanced, lineCount);

    if (!fixLine && (unBalanced > 0)) printf("\n\n Warning -- unbalanced lines found but not fixed!\n\n");

    getch();
    exit(0);

}

// ===== end of main =====


// ===== end of file =====


