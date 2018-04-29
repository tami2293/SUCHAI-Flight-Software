//
// Created by kaminari on 27-04-18.
//

#ifndef SUCHAI_FLIGHT_SOFTWARE_CMDWEATHER_H
#define SUCHAI_FLIGHT_SOFTWARE_CMDWEATHER_H

#include "data_storage.h"
#include "repoData.h"
#include "repoCommand.h"

void cmd_weather_init(void);
int cmd_send_weather(char *fmt, char *params, int nparams);
int cmd_receive_weather(char *fmt, char *params, int nparams);
int cmd_get_weather(char *fmt, char *params, int nparams);
int cmd_send_iridium(char *fmt, char *params, int nparams);

#endif //SUCHAI_FLIGHT_SOFTWARE_CMDWEATHER_H
