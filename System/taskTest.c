#include "taskTest.h"
#include "i2c.h"
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

void taskClientCSP(void *param)
{
    const unsigned long Delayms = 5000 / portTICK_RATE_MS;
//    int csp_node = *(int *)param;

    int pingResult;

    while(1)
    {
        vTaskDelay(Delayms);

        printf("[CLI] Sending ping\n");
        pingResult = csp_ping(1, 3000, 1, CSP_O_NONE);
        #if SCH_GRL_VERBOSE
            printf("Ping with payload of %d bytes, took %d ms\n", 100, pingResult);
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