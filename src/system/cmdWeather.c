//
// Created by kaminari on 27-04-18.
//

#include "cmdWeather.h"

void cmd_weather_init(void)
{
    cmd_add("send_weather_data", cmd_send_weather, "", 0);
    cmd_add("receive_weather_data", cmd_receive_weather, "%s", 1);
    cmd_add("send_iridium", cmd_send_iridium, "%d", 1);
    cmd_add("get_weather", cmd_get_weather, "", 0);
}


int cmd_send_weather(char *fmt, char *params, int nparams)
{
    int n_data = 1;
    weather_data data[n_data];
    int rc = storage_weather_data_get(DAT_WEATHER_SYSTEM, data, n_data);
    uint8_t node = SCH_NODE_ARDUINO ;

    char msg[SCH_COM_MAX_LEN];
    msg[0] = (uint8_t) SCH_TRX_PORT_RS ;
    assert(sizeof(weather_data)*n_data <= SCH_COM_MAX_LEN-1);
    memcpy(msg+1,(char*)&data, sizeof(weather_data)*n_data);
    com_send_data(node, msg, (sizeof(weather_data)*n_data)+1);
    if(rc > 0)
        return CMD_OK;

    return CMD_FAIL;
}

int cmd_get_weather(char *fmt, char *params, int nparams)
{
    uint8_t node = SCH_NODE_ARDUINO;
    uint8_t port = SCH_TRX_PORT_WT;
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

int cmd_send_iridium(char *fmt, char *params, int nparams)
{
    int n_data;
    if(sscanf(params, fmt, &n_data) == nparams)
    {
        weather_data data[n_data];
        int rc = storage_weather_data_get(DAT_WEATHER_SYSTEM, data, n_data);

        int data_i;
        for(data_i = 0; data_i < n_data; data_i++)
        {
            data_iridium_t iridium;
            weather_data sonda = data[data_i];
            print_weather_data(&sonda);
            iridium.GPS_VAL = sonda.GPS_VAL;
            iridium.GPS_Speed = sonda.GPS_Speed;
            iridium.GPS_Crse = sonda.GPS_Crse;
            iridium.GPS_Alt = sonda.GPS_Alt;
            iridium.GPS_Lng = sonda.GPS_Lng;
            iridium.GPS_Lat = sonda.GPS_Lat;
            iridium.Alt = sonda.Alt;
            iridium.Humidity = sonda.Humidity;
            iridium.Pressure = sonda.Pressure;
            iridium.Temp1 = sonda.Temp1;
            iridium.Temp2 = sonda.Temp2;

            com_send_data(SCH_NODE_IRIDIUM, (char *)&iridium, sizeof(data_iridium_t));
        }

        if(rc > 0)
            return CMD_OK;
    }
    return CMD_FAIL;
}

//int cmd_send_photo(char *fmt, char *params, int nparams){
//    const char* path = "./26_07_-_20_46_45_image14_1500.jpg";
//
//}
