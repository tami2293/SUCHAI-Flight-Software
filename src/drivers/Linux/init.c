/*                                 SUCHAI
 *                      NANOSATELLITE FLIGHT SOFTWARE
 *
 *      Copyright 2018, Matias Ramirez Martinez, nicoram.mt@gmail.com
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

#include "init.h"

static const char *tag = "Initializer";

void on_close(int signal)
{
    dat_repo_close();

    zmq_close(pub_socket);
    zmq_close(sub_socket);
    zmq_ctx_destroy(zmq_context);

    LOGI(tag, "Exit system!");
    exit(signal);
}

void on_reset(void)
{
    /* Register INT/TERM signals */
    struct sigaction act;
    act.sa_handler = on_close;
    sigaction(SIGINT, &act, NULL);  // Register CTR+C signal handler
    sigaction(SIGTERM, &act, NULL);

#if SCH_COMM_ENABLE
    int rc;
    /* Init communications */
    zmq_context = zmq_ctx_new();  // Initialize ZMQ context

    // Create in socket
    assert(zmq_context != NULL);
    sub_socket = zmq_socket(zmq_context, ZMQ_SUB);
    assert(sub_socket != NULL);
    rc = zmq_connect(sub_socket, SCH_COMM_ZMQ_IN);
    assert (rc == 0);
    char addr = (char) SCH_COMM_ADDRESS;
    rc = zmq_setsockopt(sub_socket, ZMQ_SUBSCRIBE, &addr, 1);
    assert (rc == 0);

    /* Create out socket */
    assert(zmq_context != NULL);
    pub_socket = zmq_socket(zmq_context, ZMQ_PUB);
    assert(pub_socket != NULL);
    rc = zmq_connect(pub_socket, SCH_COMM_ZMQ_OUT);
    assert(rc == 0);
#endif
}
