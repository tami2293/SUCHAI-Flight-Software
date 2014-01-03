#include "taskTest.h"
#include "i2c.h"
#include "i2c_comm.h"
#include "csp_if_lo.h"
#include "csp_thread.h"

void taskTest(void *param)
{
    const unsigned long Delayms = 500 / portTICK_RATE_MS;
    char *msg = (char *)param;

    while(1)
    {
        vTaskDelay(Delayms);

        #if SCH_GRL_VERBOSE
            printf("[taskTest] %s\n", msg);
        #endif
    }
}

int i2c__test_send()
{
//    if(handle != 0)
//        return CSP_ERR_DRIVER;
//
//#ifdef SCH_GRL_VERBOSE
//    //Some debug
//    printf("To: %d. Size: %d. Data: ", frame->dest, frame->len);
//    int i; for(i=0; i<frame->len; i++) printf("0x%X,", frame->data[i]); printf("\n");
//#endif

    char d_address[] = {I2C_EEPROM_ID, 0};
    char test[10] = {9,1,9,1,9,5,6,7,8,9};
    int d_address_len = 2; //Don't use register address, only device address

    //Send frame via I2C1
//    int total = i2c1_master_fputs((const char*)(frame->data), frame->len, d_address, d_address_len);
    taskENTER_CRITICAL();
    int total = i2c1_master_fputs(test, 10, d_address, d_address_len);
    taskEXIT_CRITICAL();
//    int total = frame->len;

    if(total == 10)
    {
        printf("Success in I2C driver (%d of %d bytes transmited)\n", total, 10);
        return E_NO_ERR;
    }
    else
    {
        printf("Error in I2C driver (%d of %d bytes transmited)\n", total, 10);
        return CSP_ERR_DRIVER;
    }
}

void taskClientCSP(void *param)
{
    const unsigned long Delayms = 000 / portTICK_RATE_MS;
//    int csp_node = *(int *)param;

    int pingResult = 0;
    int len = 10;

    while(1)
    {
        vTaskDelay(Delayms);

        printf("[CLI] Sending ping\n");
//        pingResult = i2c__test_send();
        pingResult = csp_ping(10, 3000, len, CSP_O_NONE);
        #if SCH_GRL_VERBOSE
            printf("Ping with payload of %d bytes, took %d ms\n", len, pingResult);
        #endif
    }
}

void taskServerCSP(void *param)
{
    /* Create socket without any socket options */
    csp_socket_t *sock = csp_socket(CSP_SO_NONE);
    /* Pointer to current connection and packet */
    csp_conn_t *conn;
    csp_packet_t *packet;

    /* Bind all ports to socket */
    csp_bind(sock, CSP_ANY);

    /* Create connections backlog queue */
    csp_listen(sock, 5);

    /* Process incoming connections */
    while (1)
    {
        printf("[SRV] Waiting connection\n");
        /* Wait for connection, 10000 ms timeout */
        if ((conn = csp_accept(sock, 10000)) == NULL)
            continue;

        printf("[SRV] New connection\n");
        /* Read packets. Timout is 1000 ms */
        while ((packet = csp_read(conn, 1000)) != NULL)
        {
            switch (csp_conn_dport(conn))
            {
                case 111:
                    /* Process packet here */
                default:
                    /* Let the service handler reply pings, buffer use, etc. */
                    csp_service_handler(conn, packet);
                    break;
            }
        }

        /* Close current connection, and handle next */
        csp_close(conn);
        printf("[SRV] Connection closed\n");
    }
}