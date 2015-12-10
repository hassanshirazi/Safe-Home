#ifndef LOG_SENSOR_DATA
#define LOG_SENSOR_DATA

#include "Arduino.h"
#include "global.h"

bool log_sensor_data_in_sd(String sensor_log_data );
void read_log_data_from_sd();
void send_log_data_to_server(byte pin_number, int status_value, String description);
void send_wireless_sensor_log(unsigned long int value, String description );

#endif
