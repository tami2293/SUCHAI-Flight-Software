//
// Created by kaminari on 27-04-18.
//

#include "cmdWeather.h"

void cmd_weather_init(void)
{
    cmd_add("send_weather_data", cmd_send_weather, "%d", 1);
    cmd_add("receive_weather_data", cmd_receive_weather, "%s", 1);
//    cmd_add("send_cmd", com_send_cmd, "%d %n", 1);
}


int cmd_send_weather(char *fmt, char *params, int nparams)
{
    int n_data;
    if(sscanf(params, fmt, &n_data) == nparams)
    {
        weather_data data[n_data];
        int rc = storage_weather_data_get(DAT_WEATHER_SYSTEM, &data, n_data);

        if(rc > 0)
            return CMD_OK;
    }
    return CMD_FAIL;
}

int cmd_receive_weather(char *fmt, char *params, int nparams)
{
    weather_data data;
    memcpy((void *)&data, (void *)params, sizeof(data));
    int rc = storage_weather_data_set(DAT_WEATHER_SYSTEM, &data);

    if(rc >= 0)
        return CMD_OK;
    return CMD_FAIL;
}

//int cmd_send_photo(char *fmt, char *params, int nparams){
//    const char* path = "./26_07_-_20_46_45_image14_1500.jpg";
//
//}
