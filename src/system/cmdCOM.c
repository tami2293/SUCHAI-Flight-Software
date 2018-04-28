/*                                 SUCHAI
 *                      NANOSATELLITE FLIGHT SOFTWARE
 *
 *      Copyright 2017, Carlos Gonzalez Cortes, carlgonz@uchile.cl
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "cmdCOM.h"

static const char *tag = "cmdCOM";
static int com_send_data(char node, char *data, size_t len);

void cmd_com_init(void)
{
    cmd_add("ping", com_ping, "%d", 1);
    cmd_add("send", com_send_dbg, "%d %s", 2);
    cmd_add("send_cmd", com_send_cmd, "%d %n", 1);
}

int com_ping(char *fmt, char *params, int nparams)
{
    int node;
    if(sscanf(params, fmt, &node) == nparams)
    {
        int rc = csp_ping((uint8_t)node, 3000, 10, CSP_O_NONE);
        LOGI(tag, "Ping to %d took %d", node, rc);
        if(rc > 0)
            return CMD_OK;
    }
    return CMD_FAIL;
}

int com_send_dbg(char *fmt, char *params, int nparams)
{
    int node;
    char msg[CMD_MAX_STR_PARAMS];

    if(sscanf(params, fmt, &node, msg) == nparams)
    {
        /* Sending message to node */
        int rc = com_send_data((char)node, (void *)msg, strlen(msg));
        LOGV(tag, "Sending %s (%d) to %d. Bytes sent %d\n", msg, (int)strlen(msg), node, rc);

        if(rc >= 0)
            return CMD_OK;
    }
    return CMD_FAIL;
}

int com_send_cmd(char *fmt, char *params, int nparams)
{
    int node, next, n_args;
    char msg[CMD_MAX_STR_PARAMS];

    n_args = sscanf(params, fmt, &node, &next);

    if(n_args == nparams && next > 1)
    {
        strncpy(msg, params+next, (size_t)CMD_MAX_STR_PARAMS);
        LOGV(tag, "Parsed %d: %d, %s (%d))", n_args, node, msg, next);

        /* Sending message to node debug port */
        int rc = csp_transaction(1, (uint8_t)node, SCH_TRX_PORT_TC, 1000,
                                 (void *)msg, strlen(msg),
                                 (void *)msg, strlen(msg));

        LOGV(tag, "Sending %s to %d. Bytes read %d\n", msg, node, rc);

        if(rc >= 0)
            return CMD_OK;
    }
    return CMD_FAIL;
}

/**
 * Send data @len bytes of @data buffer to @node
 * @param node int node id
 * @param data char * data buffer
 * @param len size_t data len
 * @return
 */
static int com_send_data(char node, char *data, size_t len)
{
    int rc;
//    /* Create socket */
    assert(zmq_context != NULL);
//    void *socket = zmq_socket(zmq_context, ZMQ_PUB);
    assert(pub_socket != NULL);
//    int rc = zmq_connect(socket, SCH_COMM_ZMQ_OUT);
//    assert(rc == 0);

    /* Creating a ZMQ message [NODE(1)][      DATA (255)    ] */
    char msg[SCH_COM_MAX_LEN+1];
    assert(msg != NULL);
    memset(msg, '\0', SCH_COM_MAX_LEN+1);
    /* Copy Node and Data to message */
    assert(len <= SCH_COM_MAX_LEN);
    msg[0] = node;
    memcpy(msg+1, data, len);
    LOGV(tag, "%s", msg);
    /* Send message */
    rc = zmq_send(pub_socket, msg, SCH_COM_MAX_LEN+1, 0);
    assert(rc > 0);
//    free(msg);
}