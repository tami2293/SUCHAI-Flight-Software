/**
 * @file  cmdConsole.h
 * @author Carlos Gonzalez C - carlgonz@uchile.cl
 * @author Tomas Opazo T - tomas.opazo.t@gmail.com
 * @date 2017
 * @copyright GNU Public License.
 *
 * This header contains command related with serial console functions
 */

#ifndef CMD_CONSOLE_H
#define CMD_CONSOLE_H

#include "csp/csp.h"

#include "repoCommand.h"

/**
 * Registers the console commands in the system
 */
void cmd_console_init(void);

/**
 * Display a debug message
 *
 * @param fparams Str. Parameters format "%s"
 * @param params Str. Parameters as string "test"
 * @param nparam Int. Number of parameters 1
 * @return  CMD_OK if executed correctly or CMD_FAIL in case of errors
 */
int con_debug_msg(char *fmt, char *params, int nparams);

/**
 * Show the list of available commands, id and parameters format
 *
 * @param fparams Str. Parameters format ""
 * @param params Str. Parameters as string ""
 * @param nparam Int. Number of parameters 0
 * @return  CMD_OK if executed correctly or CMD_FAIL in case of errors
 */
int con_help(char *fmt, char *params, int nparams);

/**
 * Send ping to node
 * @param fmt Str. Parameters format "%d"
 * @param param Str. Parameters as string Ex: "10"
 * @param nparams Int. Number of parameters 1
 * @return CMD_OK if executed correctly or CMD_FAIL in case of errors
 */
int con_ping(char *fmt, char *param, int nparams);

#endif /* CMD_CONSOLE_H */