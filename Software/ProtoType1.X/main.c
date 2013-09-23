/*
 * File:   main.c
 * Author: buchio
 *
 * Created on 2013/09/23, 5:23
 */


#include "xc.h"

int gNum = 0;

int main(void) {
    int i=0, j=0;
    while( 1 ) {
        i = i + 1;
        j = i * 10;
        if( (j % 100000) == 0) {
            gNum += 1;
        }
    }
    return 0;
}
