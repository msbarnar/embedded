/*
 * The RF transmitter is very noisy, so we want some parity information
 * along with the commands to the helmet signals.
 * This file packs the signal commands into a byte and signs them with a
 * second byte. The receiver can use the function
 *  valid(short) -> bool
 * to confirm that the commands it receives are not mangled.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// The byte used to sign the commands
char salt = 'K';

// Prints binary
void printb(int s,void* p){int i,j;for(i=s-1;i>=0;i--)for(j=7;j>=0;j--)printf("%u",(*((unsigned char*)p+i)&(1<<j))>>j);puts("");}


// Signs the signal command byte and combines it with the signature
short sign(char signal) {
    return (signal << 8) | (signal ^ salt);
}

// Returns true if the given command and signature are correct.
bool valid(short recv) {
    return sign(recv >> 8) == recv;
}

// Packs the three signal commands into a single byte along with an ID
// to allow up to 4 separate helmets.
char pack_signal(char id, bool blinkLeft, bool stop, bool blinkRight) {
    return ((id & 3) << 6)
            | ((blinkLeft ? 3 : 0) << 4)
            | ((stop ? 3 : 0) << 2)
            | (blinkRight ? 3 : 0);
}

// Tests the above functions
int main(int argc, char **argv) {
    char buffer[9] = {'\0'};

    char sig;
    short ssig;

    printf("Unit tests:\n");

    printf("Iterate IDs\n");
    for (int i = 0; i < 4; i++) {
        sig = pack_signal(i, false, false, false);
        printb(1, &sig);
    }
    printf("\n");
    
    printf("Left blinker\t");
    sig = pack_signal(1, true, false, false);
    printb(1, &sig);
    printf("Signed\t\t"); 
    ssig = sign(sig);
    printb(2, &ssig);
    if (valid(ssig)) printf("Valid!\n");
    printf("\n");

    printf("Stop!\t\t");
    sig = pack_signal(1, false, true, false);
    printb(1, &sig);
    printf("Signed\t\t"); 
    ssig = sign(sig);
    printb(2, &ssig);
    if (valid(ssig)) printf("Valid!\n");
    printf("\n");

    printf("Right blinker\t");
    sig = pack_signal(1, false, false, true);
    printb(1, &sig);
    printf("Signed\t\t"); 
    ssig = sign(sig);
    printb(2, &ssig);
    if (valid(ssig)) printf("Valid!\n");
}

