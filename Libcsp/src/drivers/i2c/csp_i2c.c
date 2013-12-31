#include <csp_i2c.h>
#include <stdio.h>
#include "i2c_comm.h"
#include "csp_error.h"
#include "SUCHAI_config.h"

int i2c_init(int handle, int mode, uint8_t addr, uint16_t speed, int queue_len_tx, int queue_len_rx, i2c_callback_t callback)
{
    if(handle != 0)
        return CSP_ERR_DRIVER;

    unsigned int BRG = 0;
    switch(speed)
    {
        case 100:
            BRG = 157;
            break;
        case 400:
            BRG = 37;
            break;
        default:
            BRG = 0;
            break;
    }

    if(BRG == 0)
        return CSP_ERR_DRIVER;

    i2c1_open(BRG, (char)addr);
    return E_NO_ERR;
}

int i2c_send(int handle, i2c_frame_t * frame, uint16_t timeout)
{
    if(handle != 0)
        return CSP_ERR_DRIVER;
    
#ifdef SCH_GRL_VERBOSE
    //Some debug
    printf("To: %d. Size: %d. Data: ", frame->dest, frame->len);
    int i; for(i=0; i<frame->len; i++) printf("0x%X,", frame->data[i]); printf("\n");
#endif

    char d_address[] = {I2C_EEPROM_ID, 0};
    int d_address_len = 2; //Don't use register address, only device address

    //Send frame via I2C1
    int total = i2c1_master_fputs((const char*)(frame->data), frame->len, d_address, d_address_len);
    
    if(total == frame->len)
    {
        printf("Success in I2C driver (%d of %d bytes transmited)", total, frame->len);
        return E_NO_ERR;
    }
    else
    {
        printf("Error in I2C driver (%d of %d bytes transmited)", total, frame->len);
        return CSP_ERR_DRIVER;
    }
}
