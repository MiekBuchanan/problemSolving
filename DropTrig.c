
//  This creates a trigger / flag file based on the parameter that it is passed as a command
// the text in the trigger file is the flag name that is created

/*
    Change log: 1.00    - original release

*/

//
// error codes:
//  0   = normal exit no problems
// xxxx 8   = no parameter passed for trigger name
//  12  = could not open our trigger to write run time
//  16  =* could not open named trigger to create
//  

#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <time.h>
#include <ctype.h>
// #include <dos.h>

#define TRUE   1
#define FALSE  0

const char VERSION[] = "1.0 ";
const char author[] = " Written by Miek Buchanan , compile date is ";
const char compiled2[]   = __DATE__;

int main(int argc, char *argv[])    /* =================================================================== */

{

    FILE *fpinTrig;         // file pointer to our trigger file, contains the name of the trigger to be dropped
    FILE *fpoutFlag;        // file pointer to flag to be created
    FILE *fpLog;            // log file pointer

//    int loopCnt;            // lop counter

    char trigPath[500] = "Temp\\";      // path to where the trigger will be opened    
    char dateBuff[250] = "";            // buffer for the date info
    char strbufIN[250] = "";            // buffer from value in trigger file

    time_t time_of_day; 
    time_of_day = time( NULL );         // get time of day

    strftime(dateBuff, sizeof(dateBuff), "Trigger created %c.", localtime( &time_of_day ));    

    if ((fpLog=fopen("temp\\DropTrig.LOG", "wt"))==NULL) {   
        printf("\n Cannot open output log file DropTrig.log.\a\a\n The Trigger has NOT been dropped.\n");
        return(16);
    }

    fprintf(fpLog, " Starting DropTrig, %s\n", dateBuff);

    if ((fpinTrig=fopen("temp\\DropTrig.exj", "r"))==NULL) {   
        printf("\n Cannot open input trigger DropTrig.EXJ.\a\a\n The Trigger has NOT been dropped.\n");
        fprintf(fpLog, " Cannot open input trigger DropTrig.EXJ. The Trigger has NOT been dropped.\n");
        getch();
        return(16);
    }

    fprintf(fpLog, " EXJ trigger opened for file name\n");

    memset(strbufIN, '\0', sizeof(strbufIN));    // make sure all nulls
    fgets(strbufIN, sizeof(strbufIN), fpinTrig);

    fprintf(fpLog, " Read \"%s\" from trigger\n", strbufIN);

//    for (loopCnt=0; loopCnt<strlen(strbufIN); loopCnt++)    {
//        if( !isprint(strbufIN[loopCnt]))    {
//            printf(" Bad char of file name, parm, disp=%d.\a\n", loopCnt);
//            getch();
//            strbufIN[loopCnt] = '_';
//        }
//    }

    strcat(trigPath, strbufIN);      // add the passed parameter / file name

    remove("temp\\DropTrig.EOF");   //  remove our last EOF trigger if it's there
    remove(trigPath);               //  remove our new flag to be dropped if it's still there

//    sleep(5);                       // sleep 5 seconds for any other processes to complete, caches to be flushed, etc.


    if ((fpoutFlag=fopen(trigPath, "wt"))==NULL) {   
        printf("\n Cannot open output trigger name %c.\a\a\n The Trigger has NOT been dropped.\a\a\n", trigPath);
        getch();
        return(16);
    }

    fprintf(fpoutFlag, dateBuff);
    fprintf(fpLog, " Done, closing all files\n");

    fcloseall();
//    getch();

    return(0);

}

