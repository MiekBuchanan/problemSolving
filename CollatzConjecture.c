
/*      do what you like
        like what you do
*/

// This is a program to play with the Collatz Conjecture,
// see http://en.wikipedia.org/wiki/Collatz_conjecture

// The sequence of numbers involved is referred to as the hailstone sequence or hailstone numbers,[2] or as wondrous numbers.

// Take any natural number n.
// If n is even, divide it by 2 to get n / 2.
// If n is odd, multiply it by 3 and add 1 to obtain 3n + 1.
// Repeat the process (which has been called "Half Or Triple Plus One", or HOTPO[4]) indefinitely.
// The conjecture is that no matter what number you start with, you will always eventually reach 1. The property has also been called oneness



/* Change log:
  (*) x.1.0  hey, would I need more than one?


* * * * * * * * * * * * * * */

#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
#include <conio.h>
#include <dos.h> 
//#include <ctype.h>

#define TRUE   1
#define FALSE  0

const char ME[]      = " Written by Miek Buchanan ";
const char VERSION[] = "My 1.0";

// function definitions
//void getDetail(void);       // read through the input report to find next detail line


// start of main
int main(int argc, char *argv[])    // =============================================================================
{
    FILE *fp_log;          // file pointer log of what it's done and the results

    long int theNum     = 1;    // the number we are working on now
    long int theCount   = 0;    // how many times have we 'worked' this number?
    int loopCount       = 1;    // this is how many numbers we have processed so far
    int maxLoop         = 99999999;  // to keep it from running away

    char strBuffer[2048]    = "";   // string buffer for screen & file output


    if ((fp_log=fopen("CC.log", "wt"))==NULL)
    {
        printf(" Unable to open output log file 'CC.log'.\n");
        perror(" -- Error is ");
        printf(" Press a key to exit.\n\n");
        getch();
        return(1);
    }



    printf(" This is a program to test out the Collatz Conjecture.\n See http://en.wikipedia.org/wiki/Collatz_conjecture\n");
//    printf(" Starting with one for 100 numbers...\n");

    for (loopCount = 1001; loopCount < 9999999; loopCount++)      // this is the start and end numbers, range
    {
//        printf("\n Starting with %d.\n", loopCount);
        theNum = loopCount;

        do
        {

            if (!(theNum % 2))     // is this an evan number?
            {
//                printf(" Was even ");
                theNum = theNum / 2;
            }
            else                // else it's odd
            {
//                printf(" Was odd  ");
                theNum = theNum * 3 + 1;
            }

            theCount++;
//            printf(" num = %d, count = %d.\n", theNum, theCount);

            if (theNum == 1)
            {
                sprintf(strBuffer, " Starting with %8d, made it to one after %8d iterations.\n", loopCount, theCount);
                printf("%s", strBuffer);
                fputs(strBuffer, fp_log);
                theCount = 0;
                sleep(1);           // pause for a bit
                //                getch();
            }

            if (loopCount > maxLoop)
            {
                printf(" ---- loopCount exceeds maxLoop, %d > %d on number %d, resulting number is now %d.\n ctrl-C to exit\n", loopCount, maxLoop, theNum);
                getch();
                getch();
            }
        } while (theNum != 1);
        
    }

    fclose(fp_log);

    printf(" Complete for %d numbers.\n", loopCount-1);
    return(0);

}

// === end ===

