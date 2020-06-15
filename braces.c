//  This counts the number of left and right braces in an RTF document for debugging

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include <ctype.h>

#define TRUE   1
#define FALSE  0


// version reference
//      1.0  = original
//      1.1  = fix strHold clear, variables to local, improve VERBOSE reporting

int main(int argc, char *argv[])    /* =================================================================== */

{
    
    char VERSION[] = "1.1";
    int lines   = 0;
    int braces  = 0;
    int llength = 0;
    int x       = 0;
//    char *lplace;

    int notEOF  = TRUE;
    int verbose = FALSE;

    char strbufIN[2096];
    char strbufOUT[1024];
    char strlinePM[1024];
    char strHold[41];           // short version for screen, for really long lines from input

    FILE *fpin;
    FILE *fpout;

    memset(strHold, '\0', sizeof(strHold));
printf("a1\n");
    if (argc < 2)
    {
        printf("\n Version: %s\n You must enter an input file name.\n This program will keep track of the nesting of the braces.\n", VERSION); 
        printf(" If there is a space \"v\" after the input name\n it will give verbose information about nesting in \'braces.txt\'. \n\n");
        printf(" Exiting now, press a key.\n");
        getch();
        exit(2);
    }
printf("a2\n");
    if ((fpin=fopen(argv[1], "rt"))==NULL) {
       printf("Can not open input file \"%s\".\a\a\n", argv[1]);
       getch();
       exit(3);
    }

    if ((fpout=fopen("braces.txt", "wt"))==NULL) {             /* open as read binary */
        printf("Cannot open output file \"braces.txt\".\a\a\n");
        perror(" Error is ");                  
        getch();
        exit(4);
    }

    if (argc == 3 )
    {
        if (!strncmp(argv[2], "v", 1))
        {
            verbose = TRUE;
        }
    }

    sprintf(strbufOUT, "Input file name = %s", argv[1]);
    if (verbose) strcat(strbufOUT, " verbose is turned on");
    strcat(strbufOUT, "\n");
    printf("%s", strbufOUT);
    fputs(strbufOUT, fpout);

    do {                        /* do while not EOF and page not full */
        fgets(strbufIN, (sizeof(strbufIN)), fpin);
        strlinePM[0] = '\0';
        if  (feof(fpin))    {
             notEOF = FALSE;
             printf("===EOF===");
        }
        else
        {
            
            lines++;
            llength = strlen(strbufIN);
            if (llength >510) printf ("warning, line %i length > 510 chars\n", lines);

//            lplace = strpbrk(strbufIN, "{}");

            for (x = 0; x <=llength; x++) {
                if ( strbufIN[x] == '{') {
                    if (verbose) printf(" +1 @ %d\n", x+1);
                    braces++;
                    strcat(strlinePM, " + ");
                }
                if (strbufIN[x] == '}') {
                    if (verbose) printf(" -1 @ %d\n", x+1);
                    braces--;
                    strcat(strlinePM, " - ");

                }
            }       /* end of for x= */


//            printf("%s\nLine %i, depth after=%i\n", strbufOUT, lines, braces);
            strncpy(strHold, strbufIN, (sizeof(strHold) - 1));
            sprintf(strbufOUT, "%s\n\"%s\", Line = %i, depth after = %i\n", strlinePM, strHold, lines, braces);
            printf("%s", strbufOUT);
            fputs(strbufOUT, fpout);
            strcpy(strbufOUT, "\0");
            memset(strHold, '\0', sizeof(strHold));
        }           /* end of else not EOF */

    } while (notEOF);

    printf(" Ending brace depth = %i.", braces);
    return 0;

}

/* === end === */
