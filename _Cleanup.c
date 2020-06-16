// do what you like -- like what you do

// This deletes older files off of the sorter in the "c:\seqcap" directory
// it will delete the future 25 calendar days.

/* Change log:
        (*) 1.0   - initial release
        (*) 1.1   - more messages with a better explanation of how to use the "run now" and "don't run now" options.
        (*) 1.2   - change to delete all days but the current one, roll up code
        (*) 1.3   - added drive and path to the DEL statements, addded the six file types rather than just ???
        (*) 2.0   - welcome to symcor.  
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
//#include <ctype.h>
#include <time.h>

#define TRUE   1
#define FALSE  0

int inrec   =  0;
int outrec  =  0;
int curDate =  0;   // current day of month
int wrkDate =  0;   // working date to delete
int rc      =  0;   // function return code

int runIt   = FALSE;

const char ME[]      = " Written by Miek Buchanan ";
const char VERSION[] = "2.0";
const char spaces[]  = "                   ";

time_t time_of_day;              // for current date

char date_buff[16];              // save current date, format is mm/dd/yy
char inpt_date[9];              // save incoming date report
char wrkDateChr[3]="99";        // character form of the work date

//                     0         1          2         3          4        5         6         7         8
//                     0123456789012345678590123456789012345678901234567890123456789012345678901234567890123456789

char strbuf_exj[]  = "del c:\\seqcap\\??????##.exj \n";
char strbuf_fi2[]  = "del c:\\seqcap\\??????##.fi2 \n";
char strbuf_fim[]  = "del c:\\seqcap\\??????##.fim \n";
char strbuf_idx[]  = "del c:\\seqcap\\??????##.idx \n";
char strbuf_rim[]  = "del c:\\seqcap\\??????##.rim \n";
char strbuf_trx[]  = "del c:\\seqcap\\??????##.trx \n";

char strbufPA[]  = " pause -----        \n";
char strbufW1[]  = "REM - This batch file will clean up all BUT today's check images.\n";
char strbufW2[]  = "\nREM - Do you want to continue?  -- OR -- Ctrl-C to exit.\n\n";

//char strbufDT[]  = "REM - Created on        0. Program version ";
char strbufOUT[180] = "";        // output buffer

//                                                                        ";

//int  inputEOF      = FALSE;             // EOF on input yet?

//FILE *fpin;                         /* input file pointer */
FILE *fpout;                        /* output file pointer */


/* function definitions */

void buildDate(void);                    // build the credit line


/* start of main */
void main(int argc, char *argv[])    /* =================================================================== */

{

    printf("\n To have the created batch file run automatically,\n  give the program a start paramter of \"r\".\n");

//    strncpy(strbufDT+43, VERSION, sizeof(VERSION));     // copy version # into message

    if (argc < 2)  {
        printf("\n\n No parm received.\n Batch file \"CLEANUP.BAT\" will have to be run manually.\n\n");
    }
    else
    {
        if (!strncmp(argv[1], "r", 1)) {
            runIt = TRUE;
            printf(" Automatic run set.\n");
        }
        else {
            printf(" Manual run set.\n You must manually run run the BAT file.\n");
        }
    }
    if ((fpout=fopen("CleanUp.BAT", "wt"))==NULL) {
        printf("Cannot open output file \"CleanUp.BAT\".\a\a\n");
        getch();
        getch();        
        exit(1);

    }

    time_of_day = time( NULL );
    strftime(date_buff, sizeof(date_buff), "%m/%d/%y", localtime( &time_of_day ));

    sprintf(strbufOUT, "REM - Created on %s from version %s.\n", date_buff, VERSION);

    printf("\n Current date is %s,\n", date_buff);

//    printf("\n argc =%d, argv1=%s, argv2=%s\n", argc,argv[1], argv[2]);
//    getch();

//    strncpy(strbufDT+17, date_buff, 8);

    curDate  = atoi(date_buff+3);     // fill our date fields

    fputs(strbufW1, fpout);
//    fputs(strbufDT, fpout);     // "date created"
    fputs(strbufOUT, fpout);     // "date created", etc.


    if (!runIt)  {
        fputs(strbufW2, fpout);     // if to be manually run, put in "are you sure?"
        fputs(strbufPA, fpout);     // pause
    }


    for (wrkDate = 1; wrkDate < 32;     wrkDate++)  {

        if (wrkDate != curDate)  {
            buildDate();
        }
    }

    fclose (fpout);

    if (runIt)  {
        printf(" Running BAT file now.\n");
        system("CleanUp.BAT");      // run this batch file
        printf("\n Run cpmpleted, deleting BAt file.\n");
        remove("CleanUp.BAT");      // delete it after done
    }
    else
    {
        printf("\n  Clean up not done yet.\n Run the \"CleanUp.BAT\" file to clean up.\n\n");
    }

    printf("\n Complete. Version %s, Compiled on %s.\n Press ENTER to exit.\n", VERSION, __DATE__);
    getch();
    exit(0);
}

//  ===== beginning of functions =====

void buildDate(void)

{

    itoa(wrkDate, wrkDateChr, 10);

    if (wrkDateChr[1] < ' ')  {            // zero out 1st char if needed
        memcpy(wrkDateChr+1 , wrkDateChr, 1);
        memcpy(wrkDateChr   , "0", 1);
    }

//    memcpy(strbufDE+20, wrkDateChr, 2);
//    fputs(strbufDE, fpout);

    memcpy(strbuf_exj+20, wrkDateChr, 2);
    memcpy(strbuf_fi2+20, wrkDateChr, 2);
    memcpy(strbuf_fim+20, wrkDateChr, 2);
    memcpy(strbuf_idx+20, wrkDateChr, 2);
    memcpy(strbuf_rim+20, wrkDateChr, 2);
    memcpy(strbuf_trx+20, wrkDateChr, 2);
        
    fputs(strbuf_exj, fpout);
    fputs(strbuf_fi2, fpout);
    fputs(strbuf_fim, fpout);
    fputs(strbuf_idx, fpout);
    fputs(strbuf_rim, fpout);
    fputs(strbuf_trx, fpout);

}




