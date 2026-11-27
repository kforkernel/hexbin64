/*
Copyright (c) 2026 @kforkernel
All rights reserved.

Licensed under the kforkernel Use License (KUL) v1.0.

See LICENSE.md for terms and conditions.
*/




#include <stdio.h>
#include <stdint.h>
#include <inttypes.h> 
#include <stdlib.h>
#include <errno.h>


void print_binary(int64_t num) {
    // 1. cast to UNSIGNED 64-bit so right shifts are safe, some systems may break bit shifting for signed ints, so we
    // make a copy of num as unsigned 64 bit int, this doesnt alter anything and keeps num the same but protects possible errors
    // caused by systems or compilers (unsigned are handled same across so now need to worry)
    uint64_t bits = (uint64_t)num;

    // print twos complement
    printf("Two's complement\n");
    for (int i = 63; i >= 0; i--){
        printf("%d", (int)((bits >> i) & 1ULL)); // 1ULL just means 1 as unsigned 64 bit int
        if (i % 8 == 0 && i != 0) printf(" "); // space between every byte
    }
    printf("\nHex: 0x%016" PRIX64, bits);
    printf("\n------------------------------------------------------\n\n");



    // does same technique as twos if greater than or equal to 0
    if (num >= 0){
        // print ones complement
        printf("One's complement\n");
        for (int i = 63; i >= 0; i--){
            printf("%d", (int)((bits >> i) & 1ULL));
            if (i % 8 == 0 && i != 0) printf(" ");
        }
        printf("\nHex: 0x%016" PRIX64, bits);
        printf("\n------------------------------------------------------\n\n");




        // print sign magnitude
        printf("Sign magnitude\n");
        for (int i = 63; i >= 0; i--){
            printf("%d", (int)((bits >> i) & 1ULL));
            if (i % 8 == 0 && i != 0) printf(" "); 
        }
        printf("\nHex: 0x%016" PRIX64, bits);
        printf("\n------------------------------------------------------\n\n");
    }

    else{
        if (num == INT64_MIN){
            fprintf(stderr, "Error number too small, buffer overflow\nMIN: -9,223,372,036,854,775,807 -(2⁶³ - 1)\nMAX: 9,223,372,036,854,775,807 (2⁶³ - 1)\n");
            exit(1);
        }
        // print ones complement
        printf("One's complement\n");
        uint64_t ones = bits - 1ULL;
        for (int i = 63; i >= 0; i--){
            printf("%d", (int)((ones >> i) & 1ULL));
            if (i % 8 == 0 && i != 0) printf(" ");
        }
        printf("\nHex: 0x%016" PRIX64, ones);
        printf("\n------------------------------------------------------\n\n");




        // print sign magnitude
        printf("Sign magnitude\n");
        // note when i write 0010 or other binary this is shortened version of 64 bit for ease
        // below mag means (~bits + 1ULL) invert bits val (~ means NOT aka invert bits) e.g lets say bits = -5 (1011), ~bits = 4 (0100)
        // the + 1ULL then adds unsigned 1 to it so its 0100 (4) + 0001 (1) which equals 5 (0101) aka opposite of -5
        // now do (1ULL << 63) which is move 0001 (1ULL) 63 spaces left aka bit shift 1 63 spaces left, so now 1ULL aka 1 is in the
        // left most index aka index 63 (2⁶³ or 9,223,372,036,854,775,808). now we run OR (|) on 00000...0101 (5) | 100000...0 
        // this means if one or the other bit equals 1 then turn on (aka 1) e.g from right to left 000...0101 | 1000...00 the rightmost
        // bit of 5 is 1 and right most of 2⁶³ is 0, 1 | 0 equals 1 so on, then next 5 2nd binary num (index 1) is 0 and so is 2⁶³
        // so that part equals 0. so now mag equals unsigned 10000000...0101 (9,223,372,036,854,775,813)
        uint64_t mag = (~bits + 1ULL) | (1ULL << 63);
        // now when we print mag, left most bit is 1 and we & it against 1 so it prints 1 then next (which is 0) so print 0 until end
        for (int i = 63; i >= 0; i--){
            printf("%d", (int)((mag >> i) & 1ULL));
            if (i % 8 == 0 && i != 0) printf(" "); 
        }
        printf("\nHex: 0x%016" PRIX64, mag);
        printf("\n------------------------------------------------------\n\n");
    }
}

int main(void){

    char input_buf[64] = {0};
    char *user_input;
    errno = 0;

    printf("Enter a 64-bit number: ");
    // input_buf = where to store, sizeof(input_buf) = how much to store,  stdin = from where to get
    if ((user_input = fgets(input_buf, sizeof(input_buf), stdin)) != NULL){
        
        // this for the error checking of strtoimax, it points to to first char it couldnt read
        char *endptr;
        // must set errno to 0 before calling strtoimax because c never resets
        // errno back to 0 (we use the errno for e.g ERANGE aka out of range for a huge num like 99999999999999999999999999999999999999999999)
        errno = 0;
        // strtoimax returns intmax_t so to convert that to int64_t (signed 64 bit int)
        // we put (int64_t) before it to convert it
        intmax_t number = strtoimax(input_buf, &endptr, 10);        
        
        if (endptr == input_buf){
            // pointer didn't move -> User typed no digits (e.g., "abc")
            fprintf(stderr, "Entered invalid character(s)\nEnter only numbers (0-9)\nMIN: -9,223,372,036,854,775,807 -(2⁶³ - 1)\nMAX: 9,223,372,036,854,775,807 (2⁶³ - 1)\n");
            return 1;
        } 
        else if (*endptr != '\n' && *endptr != '\0') {
            // stopped early -> trailing junk after digits (e.g., "123abc")
            fprintf(stderr, "Entered invalid character(s)\nEnter only numbers (0-9)\nMIN: -9,223,372,036,854,775,807 -(2⁶³ - 1)\nMAX: 9,223,372,036,854,775,807 (2⁶³ - 1)\n");
            return 1;
        } 
        else if (errno == ERANGE || number < INT64_MIN || number > INT64_MAX){
            // number too big/small for 64 bits
            fprintf(stderr, "Entered out of range number\nMIN: -9,223,372,036,854,775,807 -(2⁶³ - 1)\nMAX: 9,223,372,036,854,775,807(2⁶³ - 1)\n");
            return 1;
        } 
        else{
            int64_t num = (int64_t)number; // safe to cast and use
            print_binary(num);
        }
    }

    return 0;
}
