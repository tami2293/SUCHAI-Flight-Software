#include "taskTest.h"
#include "i2c.h"
#include "csp_if_lo.h"

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

void taskTestCSP(void *param)
{
    const unsigned long Delayms = 5000 / portTICK_RATE_MS;
    int csp_node = *(int *)param;

    /* Init buffer system with 3 packets of maximum 256 bytes each */
    csp_buffer_init(3, 256);

    /* Init CSP with address MY_ADDRESS */
    csp_init(csp_node);

    i2c_init(0, I2C_MASTER, 0xAA, 400, 2, 2, NULL);
    csp_i2c_init(0xAA, 0, 400);

    csp_route_set(CSP_DEFAULT_ROUTE, &csp_if_i2c, CSP_NODE_MAC);
    csp_route_start_task(512, 2);

    //DEBUG
    printf("Conn table\r\n");
    csp_conn_print_table();
    printf("Route table\r\n");
    csp_route_print_table();
    printf("Interfaces\r\n");
    csp_route_print_interfaces();

    int pingResult;

    while(1)
    {
        vTaskDelay(Delayms);

        pingResult = csp_ping(1, 3000, 100, CSP_O_NONE);
        #if SCH_GRL_VERBOSE
            printf("Ping with payload of %d bytes, took %d ms\n", 100, pingResult);
        #endif
    }
}
