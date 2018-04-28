//
// Created by carlos on 22-08-17.
//

#ifndef SCH_PERSISTENT_H
#define SCH_PERSISTENT_H

#include "utils.h"
#include <stdio.h>
#include <sqlite3.h>
#include <time.h>
#include "config.h"


/**
 * Init data storage system.
 * In this case we use SQLite, so this function open a database in file
 *
 * @note: non-reentrant function, use mutex to sync access
 *
 * @param file Str. File path to SQLite database
 * @return 0 OK, -1 Error
 */
int storage_init(const char *file);

/**
 * Create new table in the opened database (@relatesalso storage_init) in the
 * form (index, name, value). If the table exists do nothing. If drop is set to
 * 1 then drop an existing table and then creates an empty one.
 *
 * @note: non-reentrant function, use mutex to sync access
 *
 * @param table Str. Table name
 * @param drop Int. Set to 1 to drop the existing table before create one
 * @return 0 OK, -1 Error
 */
int storage_table_repo_init(char *table, int drop);

/**
 * Create new table in the opened database (@relatesalso storage_init) in the
 * form (time, command, args, repeat). If the table exists do nothing. If drop is set to
 * 1 then drop an existing table and then creates an empty one.
 *
 * @note: non-reentrant function, use mutex to sync access
 *
 * @param drop Int. Set to 1 to drop the existing table before create one
 * @return 0 OK, -1 Error
 */

int storage_table_flight_plan_init(int drop);

/**
 * Get an INT (integer) value from table by index
 *
 * @note: non-reentrant function, use mutex to sync access
 *
 * @param index Int. Value index
 * @param table Str. Table name
 * @return 0 OK, -1 Error
 */
int storage_repo_get_value_idx(int index, char *table);

/**
 * Get a INT (integer) value from table by name
 *
 * @note: non-reentrant function, use mutex to sync access
 *
 * @param name Str. Value name
 * @param table Str. Table name
 * @return 0 OK, -1 Error
 */
int storage_repo_get_value_str(char *name, char *table);

/**
 * Set or update the value of a INT (integer) variable by index.
 *
 * @note: non-reentrant function, use mutex to sync access
 *
 * @param index Int. Variable index
 * @param value Int. Value to set
 * @param table Str. Table name
 * @return 0 OK, -1 Error
 */
int storage_repo_set_value_idx(int index, int value, char *table);

/**
 * Set or update the value of a INT (integer) variable by name.
 *
 * @note: non-reentrant function, use mutex to sync access
 *
 * @param name Str. Variable name
 * @param value Int. Value to set
 * @param table Str. Table name
 * @return 0 OK, -1 Error
 */
int storage_repo_set_value_str(char *name, int value, char *table);

/**
 * Set or update the row of a certain time
 *
 * @note: non-reentrant function, use mutex to sync access
 *
 * @param timetodo Int. time to do the action
 * @param command Str. Command to set
 * @param args Str. command's arguments
 * @param repeat Int. Value of time to run the command
 * @return 0 OK, -1 Error
 */
int storage_flight_plan_set(int timetodo, char* command, char* args, int repeat, int periodical);

/**
 * Get the row of a certain time and set the values in the variables committed
 *
 * @note: non-reentrant function, use mutex to sync access
 *
 * @param timetodo Int. time to do the action
 * @param command Str. Command to get
 * @param args Str. command's arguments
 * @param repeat Int. Value of times to run the command
 * @return 0 OK, -1 Error
 */
int storage_flight_plan_get(int timetodo, char** command, char** args, int** repeat, int** periodical);

/**
 * Erase the row in the table in the opened database (@relatesalso storage_init) that
 * have the same timetodo.
 *
 * @note: non-reentrant function, use mutex to sync access
 *
 * @param timetodo Int. time to do the action
 * @return 0 OK, -1 Error
 */
int storage_flight_plan_erase(int timetodo);

/**
 * Reset the table in the opened database (@relatesalso storage_init) in the
 * form (time, command, args, repeat).
 *
 * @note: non-reentrant function, use mutex to sync access
 *
 * @return 0 OK, -1 Error
 */
int storage_flight_plan_reset(void);

/**
 * Show the table in the opened database (@relatesalso storage_init) in the
 * form (time, command, args, repeat).
 *
 * @note: non-reentrant function, use mutex to sync access
 *
 * @return 0 OK
 */
int storage_show_table(void);

/**
 * Close the opened database
 *
 * @note: non-reentrant function, use mutex to sync access
 *
 * @return 0 OK, -1 Error
 */
int storage_close(void);

// Weather functions
//typedef struct weather_data{
//    float temp1;
//    float temp2;
//    float press1;
//    float height;
//    float imu1;
//    float imu2;
//    float imu3;
//    float gps_lat;
//    float gps_lon;
//    float gps_height;
//    float gps_cur;
//    float gps_v;
//    int gps_HH;
//    int gps_MM;
//    int gps_SS;
//    int gps_SAT;
//    int rssi;
//} weather_data;

typedef struct weather_data {
    float Temp1;
    float Pressure;
    float Alt;
    float Temp2;
    float Humidity;
    float IMU1;
    float IMU2;
    float IMU3;
    float GPS_Lat;
    float GPS_Lng;
    float GPS_Alt;
    float GPS_Crse;
    float GPS_Speed;
    uint8_t GPS_HH;
    uint8_t GPS_MM;
    uint8_t GPS_SS;
    uint8_t GPS_VAL;
    uint32_t GPS_Sat;
} weather_data;

int storage_table_weather_init(char* table, int drop);
int storage_weather_data_get(const char *table, weather_data *data, int n);
int storage_weather_data_set(const char *table, weather_data *data);
void print_weather_data(weather_data *data);

#endif //SCH_PERSISTENT_H
