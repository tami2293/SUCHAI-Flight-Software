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

#include "taskCommunications.h"

static const char *tag = "Communications";

static void com_receive_tc(csp_packet_t *packet);
static void com_receive_cmd(char *buff, size_t len);

void taskCommunications(void *param)
{
    LOGD(tag, "Started");
    int rc;

    assert(zmq_context != NULL);
    assert(sub_socket != NULL);

    char buff[SCH_BUFF_MAX_LEN];
    memset(buff, '\0', SCH_BUFF_MAX_LEN);

    while(1)
    {
        /* ZMQ SERVER */
        if((rc = zmq_recv(sub_socket, buff, SCH_BUFF_MAX_LEN, 0)) != -1)
        {
            /* Parse commands [Node(1)][Port(2)][Data(254)] */
            if(rc > 0)
            {
                int port = (int) buff[1];
                cmd_t *new_tm;
                switch(port)
                {
                    case SCH_TRX_PORT_TC:
                        com_receive_cmd(buff + 2, (size_t) (rc - 2));
                        break;
                    case SCH_TRX_PORT_WT:
                        new_tm = cmd_get_str("receive_weather_data");
                        cmd_add_params_raw(new_tm, buff+2, rc-2);
                        cmd_send(new_tm);
                        break;
                    default:
                        LOGD(tag, "TC: %s", buff+1);
                }
            }



            /* Clear buffer */
            memset(buff, '\0', SCH_BUFF_MAX_LEN);
        }
    }
}

/**
 * Parse TC frames and generates corresponding commands
 *
 * @param packet TC Buffer
 */
static void com_receive_tc(csp_packet_t *packet)
{
    int next;
    int n_args;
    static cmd_t *new_cmd;
    char tmp_cmd[packet->length];
    char tmp_arg[packet->length];
    char buffer[packet->length];
    strncpy(buffer, (char *)(packet->data), (size_t)packet->length);
    LOGI(tag, "New TC: %s (%d)", buffer, packet->length);

    n_args = sscanf(buffer, "%s %n", tmp_cmd, &next);
    LOGV(tag, "Parsed %d: %s (%d))", n_args, tmp_cmd, next);
    strncpy(tmp_arg, buffer+next, (size_t)(packet->length));
    LOGV(tag, "Parsed args: %s", tmp_arg);

    if (n_args == 1)
    {
        new_cmd = cmd_get_str(tmp_cmd);
        if(new_cmd)
        {
            if(next > 1)
            {
                cmd_add_params_str(new_cmd, tmp_arg);
            }
            else
            {
                cmd_add_params_str(new_cmd, "");
            }

            cmd_send(new_cmd);
        }
    }
    else
    {
        LOGE(tag, "Invalid number of arguments (%d)", n_args)
    }
}

/**
 * Parse tc frame as console commands
 * @param buff command string buffer
 * @param len string length
 */
static void com_receive_cmd(char *buff, size_t len)
{
    int next;
    int n_args;
    cmd_t *new_cmd;
    char tmp_cmd[len];
    char tmp_arg[len];
    memset(tmp_cmd, '\0', len);
    memset(tmp_arg, '\0', len);
    LOGI(tag, "New TC: %s (%d)", buff, (int)len);

    n_args = sscanf(buff, "%s %n", tmp_cmd, &next);
    LOGV(tag, "Parsed %d: %s (%d)", n_args, tmp_cmd, next);

    if(n_args)
    {
        new_cmd = cmd_get_str(tmp_cmd);
        if (new_cmd)
        {
            n_args = next < len ? 1 : 0;
            if(n_args)
            {
                strncpy(tmp_arg, buff + next, len - next);
                LOGV(tag, "Parsed args: %s", tmp_arg);
                cmd_add_params_str(new_cmd, tmp_arg);
            }
            else
            {
                cmd_add_params_str(new_cmd, "");
            }

            cmd_send(new_cmd);
        }
    }
}
