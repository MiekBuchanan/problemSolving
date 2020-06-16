// This scans the windows services file and replaces port assignments for Bobware, BobwareT5 and DSSUtils
// file to be edited is \%SystemRoot%\system32\drivers\etc\services
// BOBWARE          7004/TCP                           #BOBWARE
// BOBWARET5        7004/TCP                           #BOBWARET5
// dssutil          7005/TCP                           #DSS Utility Service
// DSSSECSRV        7006/TCP                           #DSS SECURITY SERVICE

/* Change log:

    (*) 1.0  - initial release
    (*) 1.1  - add backup of services file before changes.  because i am paranoid
               and add the ability to update the DSS security services port if passed a 4th parm
    (*) 1.2  - handle input parms better

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <ctype.h>

#define TRUE  1
#define FALSE 0

const char compiled1[]   = " Compile date is ";
const char compiled2[]   = __DATE__;
const char Author[]      = " Written by Miek Buchanan ";
const char VERSION[] = "1.2";

char strbufIO[1000] = "";       // i/o buffer
char work_str[350]  = "";      // string work buffer
char in_args[90]    = "";       // storage for input arguments

char *path;

int lcin     = 0;       // line count in
int tempx    = 0;       // temp integer, loop counter, etc.
int retCode  = 32;      // return code
int in_len   = 0;       // temp string length, etc.
int rc_check = 0;       // holder for return code

int got_bobware   = FALSE;      // found bobware?
int got_bobwareT5 = FALSE;      // found bobwareT5
int got_dssutil   = FALSE;      // found DSSUTIL?
int got_secsvcs   = FALSE;      // found DSSSECSRV?
int good_rec      = FALSE;      // means not commented and longer than 10 characters, set FALSE at EOF
int eof_in        = FALSE;      // = TRUE when EOF found
int set_DSSSECSRV = FALSE;      // need to set the DSS security services port too?



FILE *fpio;

long int fpsave   = 0L;        // temporary file pointer, for 'rewind'

// function definitions //
void getaline(void);


// start of main 
int main(int argc, char *argv[])    /* =================================================================== */

{
    int parmProb = FALSE;

    printf("\n Program version %s. Compile date %s.\n", VERSION, __DATE__);

    if (argc < 2 ) {
        parmProb = TRUE;
    }
    else
    {
        tempx = strlen(argv[1]);
        }

    if (tempx < 14)  {
        parmProb = TRUE;
    }


    if (parmProb)  {        //  if there's an input parm problem
        
        printf("\n ==================== Input Parms Invalid =====================\a\n") ;
        printf("\n This will search and replace port numbers used in\n the windows services file for Bobware, BobwareT5 and DSSUtils.\n");
        printf(" Input should be the three port numbers that are to be replaced in that order\n and should look like \"Upd_Port 7001,7002,7003\"\n to make bobware=7001, bobwareT5=7002 and DSSUtils=7003\n");
        printf(" If you are using the DSS Security Services, you can give it a fourth port\n and it will replace the DSSSECSRV values\n");
        printf(" The input shoiuld be in this form: \"Upd_Port 7001,7002,7003,7004\"\n\a");
        printf("\n -=> Don't forget the commas between the port numbers! <=-\n");
        printf("\n ==================== Input Parms Invalid =====================\a\n") ;
        getch();
        return(1);
    }

    strncpy(in_args, argv[1], 19);

    if ( tempx == 19 )  {          // input parms include security services port?
        printf("DSSSECSRV port found in parm line.\n");
        set_DSSSECSRV = TRUE;
    }
    
//    printf(">%s\n",in_args);

    path = getenv( "SystemRoot" );
    printf("SystemRoot=%s\n", path);
//    getch();

    strcpy(work_str, "copy ");
    strcat(work_str, path);
    strcat(work_str, "\\system32\\drivers\\etc\\services ");    // note the extra space on the end to separate
    strcat(work_str, path);
    strcat(work_str, "\\system32\\drivers\\etc\\services.bkt /y");

//    printf("xcopy will be>%s\n", work_str);
//    getch();

    if( system(work_str))  {                   // go copy services file to a backup
        printf(" Error backing up the services file before changes.  CTRL-C to exit\n");
        getch();
    }
    else
    {
        printf(" Backed up services file as \"services.bkt\"\n");
    }

    strcpy(work_str, path);
    strcat(work_str, "\\system32\\drivers\\etc\\services");
    printf(" File path is >%s<\n", work_str);

//    if ((fpio=fopen("services", "r+t"))==NULL) {    // open file for I/O (test version)
    if ((fpio=fopen(work_str, "r+t"))==NULL) {    // open file for I/O        
         printf("\n Can't open services file for update.\a\a\n");
         getch();
         return(1);
     }

    printf(" Opened services file\n");


    getaline();                                     // get 1st valid line

    while (!eof_in)  {

        if ( !got_bobware && (!strncmp(work_str, "BOBWARE  ", 8)))  {  // is this the bobware line?

            got_bobware   = TRUE;

            for ( tempx = 8; tempx < in_len; tempx++ )  {

                if (isdigit(strbufIO[tempx]))  {
                    memcpy(strbufIO+tempx, in_args, 4);

                    fflush(fpio);                              // clear any buffers out

                    rc_check = fseek(fpio, fpsave, SEEK_SET);     // rewind one record to write over correct one.

                    if (rc_check != 0 ) {
                        printf("\n  fseek returned an error=%d\n", rc_check);
                        getch();
                        return(1);
                    }

//                    printf("\n -- ready to re-write record\n>%s\n", strbufIO);

                    fputs(strbufIO, fpio);                     // update record

                    if (rc_check < 0) {
                        printf(" fpout fseek error on bobware, rc_check=%d\n", rc_check);
                        getch();
                        return(1);                        
                    }

                    fflush(fpio);                              // clear any buffers out
//                    strbufIO[0] = '\0';
//                    getaline();                     // re-get updated record
//                    printf(">re-got same ??? >%s\n\n", strbufIO);

                    tempx = 1999;                   // loop done

                }     // end of - if this is a digit
            }         // end of - for loop
        }             // end of - if "BOBWARE"


        if ( !got_bobwareT5 && (!strncmp(work_str, "BOBWARET5 ", 10)))  {  // is this the bobwareT5 line?

            got_bobwareT5   = TRUE;

            for ( tempx = 10; tempx < in_len; tempx++ )  {

                if (isdigit(strbufIO[tempx]))  {
                    memcpy(strbufIO+tempx, in_args+5, 4);

                    fflush(fpio);                              // clear any buffers out

                    rc_check = fseek(fpio, fpsave, SEEK_SET);     // rewind one record to write over correct one.

                    if (rc_check != 0 ) {
                        printf("\n fseek returned an error on bobwareT5=%d\n\n", rc_check);
                        getch();
                        return(1);                        
                    }

//                    printf("\n -- ready to re-write record\n>%s\n", strbufIO);

                    fputs(strbufIO, fpio);                     // update record

                    if (rc_check < 0) {
                        printf(" fpout fseek error on bobwareT5\n");
                        getch();
                        return(1);                        
                    }

                    fflush(fpio);                   // clear any buffers out
                    tempx = 1999;                   // loop done

                }     // end of - if this is a digit
            }         // end of - for loop
        }             // end of - if "BOBWARET5"

        if ( !got_dssutil && (!strncmp(work_str, "DSSUTIL ", 8)))  {  // is this the dssutil line?

            got_dssutil   = TRUE;

            for ( tempx = 8; tempx < in_len; tempx++ )  {

                if (isdigit(strbufIO[tempx]))  {
                    memcpy(strbufIO+tempx, in_args+10, 4);

                    fflush(fpio);                              // clear any buffers out

                    rc_check = fseek(fpio, fpsave, SEEK_SET);     // rewind one record to write over correct one.

                    if (rc_check != 0 ) {
                        printf("\n fseek returned an error on dssutil=%d, \n\n", rc_check);
                        getch();
                        return(1);                        
                    }

                    fputs(strbufIO, fpio);                     // update record

                    if (rc_check < 0) {
                        printf(" fpout fseek error on dssutil, rc_check=%d\n", rc_check);
                        getch();
                        return(1);                        
                    }

                    fflush(fpio);                              // clear any buffers out
                    tempx = 1999;                   // loop done

                }     // end of - if this is a digit
            }         // end of - for loop
        }             // end of - if "DSSUtil"

        if ( set_DSSSECSRV && !got_secsvcs && (!strncmp(work_str, "DSSSECSRV ", 10)))  {  // is this the DSS security services line?

            got_secsvcs   = TRUE;

            for ( tempx = 10; tempx < in_len; tempx++ )  {

                if (isdigit(strbufIO[tempx]))  {
                    memcpy(strbufIO+tempx, in_args+15, 4);

                    fflush(fpio);                              // clear any buffers out

                    rc_check = fseek(fpio, fpsave, SEEK_SET);     // rewind one record to write over correct one.

                    if (rc_check != 0 ) {
                        printf("\n fseek returned an error on DSSSECSRV=%d\n\n", rc_check);
                        getch();
                        return(1);                        
                    }

//                    printf("\n -- ready to re-write record\n>%s\n", strbufIO);

                    fputs(strbufIO, fpio);                     // update record

                    if (rc_check < 0) {
                        printf(" fpout fseek error on DSSSECSRV\n");
                        getch();
                        return(1);                        
                    }

                    fflush(fpio);                   // clear any buffers out
                    tempx = 1999;                   // loop done

                }     // end of - if this is a digit
            }         // end of - for loop
        }             // end of - if "BOBWARET5"

        getaline();
    
    };                // do while not EOF
        
    fcloseall();

    printf( " Bobware   was %sfound and set.\n", ( got_bobware )   ? "" : "\a\aNOT " );
    printf( " BobwareT5 was %sfound and set.\n", ( got_bobwareT5 ) ? "" : "\a\aNOT " );
    printf( " DSSUtil   was %sfound and set.\n", ( got_dssutil )   ? "" : "\a\aNOT " );
    if (set_DSSSECSRV)  printf(" DSS Security port received.\n");
    printf( " DSSSECSRV   was %sfound and set.\n", ( got_dssutil )   ? "" : "\a\aNOT " );        
    printf( " Lines read = %d\n", lcin);

//    tempx = 8;

    if (got_bobware && got_bobwareT5 && got_dssutil)  {

        retCode = 0;

//        if ( ( got_dssutil && set_DSSSECSRV ) || ( !got_dssutil && !set_DSSSECSRV ) )  {
//        }
            
    }

    if (!tempx)  {          // If saved r/c = zero, exit ok
        printf("\n Unhappy ending.  Press space bar to exit.\n");
        getch();
        retCode = 4;
    }

    return(retCode);
}

/* ===== start of functions ===== */

// ========================================================================== 
// this gets a new record

void getaline(void)

{
    int keep_looking = TRUE;          // until a good record found

    if (!eof_in)  {
        do  {
            fpsave = ftell(fpio);           // save our current file pointer
            if (fgets(strbufIO, (sizeof(strbufIO)-1), fpio)) {

                lcin++;
                in_len = strlen(strbufIO);        // length of this input line  
    
                if ( in_len  > 14 && strbufIO[0] != '#')  {  // a decent length line and not a comment?

                    memcpy(work_str, "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0", 20);
                    strncpy(work_str, strbufIO, 11);
                    keep_looking = FALSE;

                    for (tempx=0; tempx < 10; tempx++)  work_str[tempx] = toupper(work_str[tempx]);   // temp text to upper case
                
                }       // end of -- if a decent candidate

            }               // end of -- if not EOF
            else
            {
                keep_looking = FALSE;
                eof_in       = TRUE;
                strbufIO[0]  = '\0';                
//                getch();
            }
       } while (keep_looking);     // end of -- while not a good record & not EOF

    }       // end of -- if not EOF

}

// ===== end ===== //


