// Includes
#include "mbed.h"
#include "rtos.h"
#include "XNucleoIKS01A3.h"

/*
    desc: void main(void)
    input: none
    output: none
*/

// create static function to see all UART io's
static BufferedSerial pc(USBTX, USBRX);

// write out available commands
static void instructionMain(void) {
    printf("(1) Start \r\n");
    printf("(2) Stop \r\n");
    printf("(3) Reset \r\n");
}

// Buffer
char dealWithBuffer(void) {
    do {
        do {
            if(pc.readable()) {
                pc.read(buffer, 10)
            }
        }while((*buffer != '1') && (*buffer != '2') && (*buffer != '3'));
        
        switch(*buffer) {
            case '1':
                printf("Entering Start\r\n");
                //start();
                break;

            case '2':
                printf("Entering Stop\r\n");
                //stop();
                break;

            case '3':
                printf("Entering Reset\r\n");
                //reset();
                break;                
                
            default:
                break;
        }
        *buffer = 0;
        printf("\r\n");
    }while(1);
}



int main(void) {
    // Setup
    instructionMain();
    
    // Buffer setup
    char buffer[10] = {};
    
    while(1) {
        // Skjoldbaka loop here
        dealWithBuffer();
    }
    return 6;
}

