//
// Created by kaminari on 27-04-18.
//

#include "cmdWeather.h"

void cmd_weather_init(void)
{
    cmd_add("send_weather_data", cmd_send_weather, "%d", 1);
    cmd_add("receive_weather_data", cmd_receive_weather, "", 0);
//    cmd_add("send_cmd", com_send_cmd, "%d %n", 1);
}


int cmd_send_weather(char *fmt, char *params, int nparams)
{
    int node;
    if(sscanf(params, fmt, &node) == nparams)
    {
        weather_data *data;
        int rc = storage_weather_data_get(DAT_WEATHER_SYSTEM, data, atoi(params));

        if(rc > 0)
            return CMD_OK;
    }
    return CMD_FAIL;
}

int cmd_receive_weather(char *fmt, char *params, int nparams)
{

    weather_data data;
    data.temp1 = 1.0;
    data.temp2 = 1.0;
    data.press1 = 1.0;
    data.height = 1.0;
    data.imu1 = 1.0;
    data.imu2 = 1.0;
    data.imu3 = 1.0;
    data.gps_lat = 1.0;
    data.gps_lon = 1.0;
    data.gps_height = 1.0;
    data.gps_cur = 1.0;
    data.gps_v = 1.0;
    data.gps_HH = 1;
    data.gps_MM = 1;
    data.gps_SS = 1;
    data.gps_SAT = 1;
    data.rssi=1;
    int rc = storage_weather_data_set(DAT_WEATHER_SYSTEM, &data);

    if(rc > 0)
        return CMD_OK;

    return CMD_FAIL;
}
