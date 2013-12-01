/*
 * Packs the signal controls into a byte and signs it.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

char salt = 'K';

void printb(int s,void* p){int i,j;for(i=s-1;i>=0;i--)for(j=7;j>=0;j--)printf("%u",(*((unsigned char*)p+i)&(1<<j))>>j);puts("");}


short sign(char signal) {
    return (signal << 8) | (signal ^ salt);
}

bool valid(short recv) {
    return sign(recv >> 8) == recv;
}

char pack_signal(char id, bool blinkLeft, bool stop, bool blinkRight) {
    return ((id & 3) << 6)
            | ((blinkLeft ? 3 : 0) << 4)
            | ((stop ? 3 : 0) << 2)
            | (blinkRight ? 3 : 0);
}

int main(int argc, char **argv) {
    char buffer[9] = {'\0'};

    char sig;
    short ssig;

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

