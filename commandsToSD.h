#ifndef INTERNET_CONNECTION
#define INTERNET_CONNECTION

#include "Arduino.h"
#include "global.h"


void map_new_sensor_to_sd(byte data[], byte cmd, int array_length);
void add_new_sensor_to_sd(String add_sensor_data, String file_name);
void map_command_to_eeprom(byte cmd,  byte data[], int array_length);
String read_from_sd_and_display(String file_name);
void write_command_to_sd(String data, String file_name);
void read_added_sensors_from_eeprom();
void delete_sensor_from_sd(byte data[], byte cmd, int array_length);


#define write_flag 16
#define SDcardLogIndex 17

//#define ip_address_start_address 3
//#define ip_address_end_address 30
//#define port_number_start_address 31
//#define port_number_end_address 60
//#define username_start_address 61
//#define username_end_address 90
//#define password_start_address 91
//#define password_end_address 120
//
//#define phone_number1_start_address 121
//#define phone_number1_end_address 150
//#define phone_number2_start_address 151
//#define phone_number2_end_address 180
//#define phone_number3_start_address 181
//#define phone_number3_end_address 210
//
//#define wifi_ssid_start_address 211
//#define wifi_ssid_end_address 240
//#define wifi_password_start_address 241 
//#define wifi_password_end_address 270

#endif
