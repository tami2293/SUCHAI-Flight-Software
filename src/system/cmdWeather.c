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
    data.Temp1 = 1.0;
    data.Temp2 = 1.0;
    data.Pressure = 1.0;
    data.Alt = 1.0;
    data.Humidity = 1.0;
    data.IMU1 = 1.0;
    data.IMU2 = 1.0;
    data.IMU3 = 1.0;
    data.GPS_Lat = 1.0;
    data.GPS_Lng = 1.0;
    data.GPS_Alt = 1.0;
    data.GPS_Crse = 1.0;
    data.GPS_Speed = 1.0;
    data.GPS_HH = 1;
    data.GPS_MM = 1;
    data.GPS_SS = 1;
    data.GPS_Sat = 1;


    int rc = storage_weather_data_set(DAT_WEATHER_SYSTEM, &data);

    if(rc > 0)
        return CMD_OK;

    return CMD_FAIL;
}

//int cmd_send_photo(char *fmt, char *params, int nparams){
//    const char* path = "./26_07_-_20_46_45_image14_1500.jpg";
//
//}
