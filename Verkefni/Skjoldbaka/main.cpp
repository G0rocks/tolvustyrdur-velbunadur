// Includes
#include "mbed.h"
#include "rtos.h"
#include "XNucleoIKS01A3.h"

/* Instantiate the expansion board */
static XNucleoIKS01A3 *mems_expansion_board = XNucleoIKS01A3::instance(D14, D15, D4, D5, A3, D6, A4);
 
/* Retrieve the composing elements of the expansion board */
static LIS2MDLSensor *magnetometer = mems_expansion_board->magnetometer;
static HTS221Sensor *hum_temp = mems_expansion_board->ht_sensor;
static LPS22HHSensor *press_temp = mems_expansion_board->pt_sensor;
static LSM6DSOSensor *acc_gyro = mems_expansion_board->acc_gyro;
static LIS2DW12Sensor *accelerometer = mems_expansion_board->accelerometer;
static STTS751Sensor *temp = mems_expansion_board->t_sensor;

// State variable. ESC (27) = "Stop", 'o' = ON, 'm' = Measuring (isl. maelandi)
char state = 'o';

const int ESC = 27;    //Escape

// create static function to see all UART io's
static BufferedSerial pc(USBTX, USBRX);

// write out available commands
static void instructionMain(void) {
    printf(" ('1') ON\r\n");
    printf(" ('2') Measuring\r\n");
    printf(" (ESC) STOP\r\n");
}

// Set state to input
static void setState(char my_state) {
    state = my_state;
    switch (state) {
        case 'o':
            printf(" State: ON   ");
            break;
        case 'm':
            printf(" State: Measuring\n");
            break;
        case ESC:
            printf(" State: STOP");
            break;
    }
    printf("\r");
}

// Buffer
void dealWithBuffer(char buffer[10]) {
    do {
        if(pc.readable()) {
            pc.read(buffer, 10);
        }
    }while(*buffer != '1' && *buffer != '2' && *buffer != ESC);
    switch(*buffer) {
        case '1':
            setState('o');
            //ON();
            break;

        case '2':
            setState('m');
            break;

        case ESC:
            setState(ESC);
            break;
            
        default:
            break;
    }
    printf("\r");
}

int main(void) {
    printf("Starting\r\n");
    // Setup
    uint8_t id;
    int32_t accel[3];
    int32_t accel_last[3];
    int32_t gyro[3];
    int32_t gyro_last[3];       // XYZ
    accel_last[0] = 0;accel_last[1] = 0;accel_last[2] = 0;
    gyro_last[0] = 0;gyro_last[1] = 0;gyro_last[2] = 0;

    instructionMain();

    // Enable sensors
    acc_gyro->enable_x();
    acc_gyro->enable_g();
    
    //Test measurements
    acc_gyro->read_id(&id);
    printf("LSM6DSO accelerometer & gyroscope = 0x%X\r\n", id);
    
    // Buffer setup
    char buffer[10] = {};
    
    while(1) {
        // Skjoldbaka loop here
        dealWithBuffer(buffer);
        
        
        switch(state) {
            case 'o':
                // code block when ON state
            break;
            case 'm':
                // CHECK ACCELERATION
                //accelerometer->get_x_axes(axes);
                //printf("LIS2DW12 [acc/mg]:  %6d, %6d, %6d\r\n", axes[0], axes[1], axes[2]);
         
                acc_gyro->get_x_axes(accel);
                accel[0] = accel[0]/100;accel[1] = accel[1]/100;accel[2] = accel[2]/100;
                printf("LSM6DSO [acc/mg]:      %6d, %6d, %6d\r\n", accel[0], accel[1], accel[2]);
         
                if (abs(accel[0]) > 4 || abs(accel[1]) > 4) {
                    printf("WROOOMM!\r");
                }
                if (accel[2] < -3) {
                    printf("Hjalp! Eg er a hvolfi!!\r\n");
                }
                else if (accel[2] < 3) {
                    printf("Aaaaah! Eg er ad detta!\r\n");
                }
                
                //acc_gyro->get_g_axes(gyro);
                //printf("LSM6DSO [gyro/mdps]:   %6d, %6d, %6d\r\n", axes[0], axes[1], axes[2]);                        
                //Gyro= sækja hallatölu 
                //If gyro minna en 90 í allar áttir 
                //ekkert 
                //Ifelse gyro er minna en 170 I tvær áttier 
                //prenta ónei, ég er að velta 
                //Else 
                //prenta hjálp, ég er á hvolfix 
            break;
            case ESC:
                // code block when STOP state
            break;
            default:
                // code block
            break;
        }   // Switch

        ThisThread::sleep_for(750);
    }
    return 6;
}


