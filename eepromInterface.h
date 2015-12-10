
#ifndef EEPROM_INTERFACE
#define EEPROM_INTERFACE


#include "Arduino.h"
#include "global.h"



void EEPROMWriteInt(int p_address, int p_value);
String read_from_eeprom(int start_address, int end_address, int arr_length);
void clear_eeprom(int start_address, int end_address);
void write_to_eeprom(byte value[], int arr_length, int start_address, int end_address);
unsigned int EEPROMReadInt(int p_address);
#endif
