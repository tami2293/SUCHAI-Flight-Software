#include <csp_i2c.h>
#include <stdio.h>

int i2c_init(int handle, int mode, uint8_t addr, uint16_t speed, int queue_len_tx, int queue_len_rx, i2c_callback_t callback)
{
    return E_NO_ERR;
}

int i2c_send(int handle, i2c_frame_t * frame, uint16_t timeout)
{
    printf("I2C_SEND");
    return E_NO_ERR;
}
