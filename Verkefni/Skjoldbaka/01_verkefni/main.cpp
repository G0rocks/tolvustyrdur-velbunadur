/* Includes */
#include "mbed.h"
#include "rtos.h"
#include "XNucleoIKS01A3.h"

// Create static function to c all uart io's 
static BufferedSerial pc(USBTX, USBRX);

// write out available commands
static void instructionMain(void)
{
    printf("[1] Start \r\n");
    printf("[2] Stop \r\n");
    printf("[3] Reset \r\n"); 
}

// main
int main()
{
 
 instructionMain();
 while(1)
 {
// do something
 }  
}