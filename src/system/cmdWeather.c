//
// Created by kaminari on 27-04-18.
//

#include "cmdWeather.h"

void cmd_weather_init(void)
{
    cmd_add("send_weather_data", cmd_send_weather, "", 0);
    cmd_add("receive_weather_data", cmd_receive_weather, "%s", 1);
    cmd_add("get_weather", cmd_get_weather, "", 0);
}


int cmd_send_weather(char *fmt, char *params, int nparams)
{
    int n_data = 3;
    weather_data data[n_data];
    int rc = storage_weather_data_get(DAT_WEATHER_SYSTEM, data, n_data);
    uint8_t node = 2;

    char msg[SCH_COM_MAX_LEN];
    msg[0] = (uint8_t) 10;
    memcpy(msg+1,(char*)&data, sizeof(weather_data)*2);
    com_send_data(node, msg, (sizeof(weather_data)*2)+1);
    if(rc > 0)
        return CMD_OK;

    return CMD_FAIL;
}

int cmd_get_weather(char *fmt, char *params, int nparams)
{
    uint8_t node = 2;
    uint8_t port = 10;
    char msg[SCH_COM_MAX_LEN];
    msg[0] = port;
    com_send_data(node, msg, 1);
}

int cmd_receive_weather(char *fmt, char *params, int nparams)
{
    weather_data data;
    memcpy((void *)&data, (void *)params, sizeof(data));

#if (LOG_LEVEL >= LOG_LVL_DEBUG)
    print_weather_data(&data);
#endif

    int rc = storage_weather_data_set(DAT_WEATHER_SYSTEM, &data);

    if(rc >= 0)
        return CMD_OK;
    return CMD_FAIL;
}

//int cmd_send_photo(char *fmt, char *params, int nparams){
//    const char* path = "./26_07_-_20_46_45_image14_1500.jpg";
//
//}
