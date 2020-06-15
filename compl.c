/* This code started out to perform a one's complement on a file.  All I/O is binary.  */
/* Usage - "compl  fromfile tofile */
/* It's morphed into a 'sealth' program to hide EXE files from email scanners. */

/*  version history:
    1.0 - Original, flips the byte from big-endian to small and or-s the bits.
    1.1 - Writes the bytes back-to front.
    1.2 - More bit play. 0123-4567 becomes 7653-4210 then one's compliment
    1.3 - More bit play. the bits flip as => 0123-4567 becomes 7653-4210 then one's compliment
            - except the 0000-1000 bit which is not moved, just NOT-ed twice so it gets the original value
            so 00- => F7, 08 => FF, 10 => E7, FF => 08
            - note that the 0001-0000 bit is left in it's place to be NOT-ed once

    */


#include <stdio.h>
#include <stdlib.h>
// #include <string.h>
#include <conio.h>
// #include <ctype.h>
// #include <time.h>


const char ME[]      = "   Written by Miek Buchanan   ";
const char VERSION[] = "1.3";

FILE *fpin;
FILE *fpout;

unsigned char theByte; 
unsigned char Byte0;
unsigned char Byte1;
unsigned char Byte2;
unsigned char Byte3;
unsigned char Byte4;
unsigned char Byte5;
unsigned char Byte6;
unsigned char Byte7;

unsigned char bytetest = 8;

unsigned char Work1;

long int thisByte( FILE *fp );
long int thisByte2;
long int save_pos, size_of_file;
long int xl = 0;
int x        = 0;
int y        = 1;

int finished;

int rcval = 1;


/* ======================================== */

long int filesize( FILE *fpin )
  {
    long int save_pos, size_of_file;

    save_pos = ftell( fpin );
    fseek( fpin, 0L, SEEK_END );
    size_of_file = ftell( fpin );
    fseek( fpin, save_pos, SEEK_SET );
    return( size_of_file );
  }


/* start of main */
int main(int argc, char *argv[])    /* =================================================================== */

{
    /* */


    if (argc != 3)  {
        printf("\n Version: %s\n You must enter an input and an output file name.\n This program minimally \"encodes\" a file.\n To \"decode\" the file, run it again with a new output file name.\n\n Warning!! The output file name will be overwritten!\n\n", VERSION);
    }
    else
    {

        if ((fpin=fopen(argv[1], "rb"))==NULL) {
//        if ((fpin=fopen("a.txt", "rb"))==NULL) {

            printf("Can not open input file \"%s\".\a\a\n", argv[1]);
            getch();
        return 1;
        }

        if ((fpout=fopen(argv[2], "wb"))==NULL) { 
//        if ((fpout=fopen("b.txt", "wb"))==NULL) { 
            printf("Can not open output file \"%s\".\a\a\n", argv[2]);
            getch();
            return 1;
        }



        if( fpin != NULL ) {
            thisByte2 = filesize(fpin );
//            printf( "\nInput file size=%ld\n     ", filesize(fpin ) );
            printf( "\nInput file size=%ld\n     ", thisByte2 );            
        }
        

        do {

            x++;

            if (x > 500 ) {    /* ok, this is just pretty stuff */

                x = 0;
                y++;


                switch(y) {
                    case 1:
                        printf("\b\\");
                    break;
                    case 2:
                        printf("\b~");
                    break;
                    case 3:
                        printf("\b/");
                    break;
                    default:
                        printf("\b\_");
                        y = 0;
                }
            }

                
    
        thisByte2--;

         fseek(fpin, thisByte2, SEEK_SET);
         theByte = getc(fpin);
         if (!feof(fpin)) {
//                Byte0 = Byte1 = Byte2  = Byte3 = Byte4 = Byte5 = Byte6 = Byte7 = theByte;

//  >>> this high-low's the bits, yep it's slow

             Byte0   = ((theByte       ) >> 7);
             Byte1   = ((theByte & 0x40) >> 5);
             Byte2   = ((theByte & 0x20) >> 3);

                // bit three is left unchanged except for the byte NOT below
//                Byte3   = ((Byte3 & 0x10) >> 1);

// play with bit four -- NOT the value, // this is to attempt to flip just this bit

             Byte4   = ((~theByte) & 0x08);  // NOT the byte and strip out all bits but that one
             Byte5   = ((theByte & 0x04) << 3);
             Byte6   = ((theByte & 0x02) << 5);
             Byte7   = ((theByte       ) << 7);
                
                
//                theByte = theByte & 0x18;  // this is used when not changing the 0x08 bit
             theByte = theByte & 0x10;       // leave only the 0x10 bit from the original

                
//                theByte = Byte0;
             theByte = theByte | Byte0;
             theByte = theByte | Byte1;
             theByte = theByte | Byte2;
//                theByte = theByte | Byte3;
              theByte = theByte | Byte4;      // <--
                theByte = theByte | Byte5;
                theByte = theByte | Byte6;
                theByte = theByte | Byte7;


//                theByte = Byte0 + Byte1 + Byte2 + Byte3 + Byte4 + Byte5 + Byte6 + Byte7;


/* this flips the low and high nibble in each byte */
/*              Byte1   = Byte1 >> 4;                
                Byte2   = Byte2 << 4;

                theByte  = (~(Byte1 | Byte2));   */ /* put it back the other way around and compliment it */


                putc(~(theByte), fpout);

            }
        } while ( thisByte2 > 0);
    }
    printf("\n\nDone - Version: %s\n", VERSION);
    return 0;
    
}

