
// This will rename a file
// Currently will remove the last seven characters from the file name
// this is used to remove the time stamp from the end of the file name on files

/* Change log:

    (*) 1.0   - initial release
    (*) 2.0   - add code to build a directory listing and search it for file names that end in ".######"
                only these are renamed
    (*) 2.1   - use a unique temp filename
    (*) 2.2   - minor change - verify 1st time digit to make sure they are "0", "1" or "2", the only appropriate hour digits
    (*) 2.3   - does not try to rename the temporary file
    (*) 2.4   - minor, add ".TMP" to end of the file name
    (*) 2.4.1 - even more minor, write quick description and version number out
    (*) 3.0   - will find a ".done" on the end of a file name and remove that too
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <conio.h>
#include <ctype.h>
//#include <time.h>
#include <conio.h>
#include <errno.h>

#define TRUE   1
#define FALSE  0

const char compiled1[]      = " Compile date is ";
const char compiled2[]      = __DATE__;
const char Author[]         = " Written by Miek Buchanan ";
const char VERSION[]        = "3.0";


FILE *fpin;             // input dir list


// function list ======
//
//void getNextLine(void);         // go read the next input line
//void unformatAmt(void);         // remove commas from amount

// start of main                    // ===============================================================================

void main(int argc, char *argv[])

{



//    char strbufIN[512]  = "";
    char strbufOrg[512] = "";
    char strbufRen[512] = "";
    char dirString[512] = "dir /b > ";
        
    int retCode     = 0;
    int strLenName  = 0;
    int renCount    = 0;

    char filename[ 512 ] = "";      // temporary file name storage

    tmpnam( filename );             // create a temporary file nam

    strcat(filename, ".TMP");       // add extension
    strcat(dirString, filename);    // add file name to the end of the DIR command
    printf(" Rename / remove the SDX time stamp from the end of file names.\n Version %s\n", VERSION);
    printf(" Starting with version 3.x,\n it will also remove a '.done' from the end of a file name.\n");


//    printf("string is >%s<\n", dirString);

//    if( system("dir /b > ___tempDir.txt"))  {                   // xcopy file from server, check for error
    if( system(dirString))  {                   // execute the DIR command
        printf("\a\a\nError generating the directory listing.\n");
        perror(" Error is ");
        getch();
        exit(6);
    }

//    fpin = fopen( filename, "r" ); 
    fpin = fopen( filename, "r" ); 

    if( fpin == NULL ) {
        printf("\n Can not open the directory listing.\n>%s<\n", filename);
        perror(" Error is ");
        getch();
        exit(8);
    }
    

    while( fgets( strbufOrg, sizeof(strbufOrg), fpin ) != NULL )
    {
//        printf(" working with >%s<\n", strbufOrg);
//        getch();
        strLenName = strlen(strbufOrg) - 1;     // - 1 for the LF at the end
        strbufOrg[strLenName] = '\0';           // get rid of the LF on the right
        
//        if (strcmp(filename, strbufOrg))
//        {
        
//            printf("strLenName=%d,chars are >%s<\n", strLenName, strbufOrg+strLenName-7);
               
            if (strLenName > 7 && strbufOrg[strLenName-7] == '.' && isdigit(strbufOrg[strLenName-6]) && isdigit(strbufOrg[strLenName-5]) && isdigit(strbufOrg[strLenName-4]) )
            {
                if (isdigit(strbufOrg[strLenName-3]) && isdigit(strbufOrg[strLenName-2]) && isdigit(strbufOrg[strLenName-1]) && strcmp(filename, strbufOrg))  {
                    printf("\n Found ZZZ time stamp\n>%s<\n", strbufOrg);        // file name ends in ".######"

                    if (strbufOrg[strLenName-6] == '0' || strbufOrg[strLenName-6] == '1' || strbufOrg[strLenName-6] == '2' )   // is 1st digit of time appropriate?
                    {
                        strcpy(strbufRen, strbufOrg);
                        strbufRen[(strLenName-7)] = '\0';        // end the file name 7 characters sooner    
                        retCode = rename(strbufOrg, strbufRen);
                        if (retCode != 0)   {
                            printf(" Error code from the rename=%d\n", retCode);
                            perror(" Error is ");
                            printf(" old=%s, new=%s\n",strbufOrg, strbufRen);   
                            getch();
                        }
                        else
                        {
                            renCount++;
                            printf(" Found ZZZ, old file name >%s<\n new file name >%s<\n\n", strbufOrg, strbufRen);
                        }
                    }       // end of "if 1st time digit is = 0, 1 or 2"
                }
            }          // end of 1st part of this extension numeric?
            else
            if ((strLenName > 7) && (!strnicmp(strbufOrg+strLenName-5, ".done", 5)))        // check for '.done'
            {
                strcpy(strbufRen, strbufOrg);
                strbufRen[(strLenName-5)] = '\0';        // end the file name 5 characters sooner    
                retCode = rename(strbufOrg, strbufRen);
                if (retCode != 0)   {
                    printf(" Error code from the rename=%d\n", retCode);
                    perror(" Error is ");
                    printf(" old=%s, new=%s\n",strbufOrg, strbufRen);   
                    getch();
                }
                else
                {
                    renCount++;
                    printf(" Found a 'done', old file name >%s<\n New file name >%s<\n\n", strbufOrg, strbufRen);
                }
            }
                
//        }           // end if not temp file name
    }               // end while

    printf(" Closing %d file.\n", fcloseall());
    printf(" Deleting temp file, name %s\n", filename);
    remove(filename);
    printf("\n Renamed %d files.\n", renCount);
    getch();
    exit(retCode);

}

