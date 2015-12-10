#ifndef READ_CONFIG_SETTINGS
#define READ_CONFIG_SETTINGS

#include "Arduino.h"
#include "global.h"

void initConfiguration();
bool initialize_sd_card();
bool read_config_and_apply_settings();
void parse_data_and_configure_sensor();
void configure_analog_sensors(byte cmd);
bool read_added_sensors_and_configure_io(String file_name);
bool set_rtc_time(int rtc_hour, int rtc_minute, int rtc_second, int rtc_day, int rtc_month, int rtc_year); 
void read_rtc_display();
String read_sd_card_configure_variables(String file_name);

#endif
