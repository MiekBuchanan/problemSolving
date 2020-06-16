
/*      do what you like
        like what you do
*/

// this is my version of the FizzBuzz programmer test

/* Change log:
  (*) x.1.0  hey, would I need more than one?


* * * * * * * * * * * * * * */

#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
#include <conio.h>
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

    int loopCount;
    int gotSomething;

    printf(" Write a program that prints the numbers from 1 to 100.\n But for multiples of three print \"Fizz\" instead of the number\n");
    printf(" and for the multiples of five print \"Buzz\".\n For numbers which are multiples of both three and five print \"FizzBuzz\".\n\n");

    for (loopCount = 1; loopCount < 101; loopCount++)
    {
        gotSomething = FALSE;
//        printf(" on number %d, 3 gives %d, 5 gives %d\n", loopCount, (loopCount % 3), (loopCount % 5));
        
        if ( loopCount % 3 == 0)            // is it evenly divisible by three?
        {
            printf("Fizz");
            gotSomething = TRUE;
        }
        if ( loopCount % 5 == 0)            // is it evenly divisible by five?
        {
            printf("Buzz");
            gotSomething = TRUE;
        }
        if (!gotSomething) printf("%d", loopCount);      // if DIDN'T get either of the above then print the number

        printf("\n");       // to send whatever you want out onto the screen on a line
    }

    printf("\n Complete.\n");
    getch(); 
    return(0);

}

// === end ===
