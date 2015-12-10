

#include "eepromInterface.h"
#include "global.h"

unsigned int EEPROMReadInt(int p_address){
  byte lowByte = EEPROM.read(p_address);
  byte highByte = EEPROM.read(p_address + 1);

  return ((lowByte << 0) & 0xFF) + ((highByte << 8) & 0xFF00);
}

void EEPROMWriteInt(int p_address, int p_value)
{
  byte lowByte = ((p_value >> 0) & 0xFF);
  byte highByte = ((p_value >> 8) & 0xFF);

  EEPROM.write(p_address, lowByte);
  EEPROM.write(p_address + 1, highByte);
}


/*
 * This Function writes a byte[] into EEPROM
 */
void write_to_eeprom(byte value[], int arr_length, int start_address, int end_address) {
  clear_eeprom(start_address, end_address);
  for (int i = 0; i <= arr_length - 1 ; i++) {
    EEPROM.write(start_address + i, (char)value[i]);
  }
}

/*
 * This Function clears eeprom locations from start_address to end_address
 */
void clear_eeprom(int start_address, int end_address) {
  for (int i = 0  ; i <= EEPROM.length() ; i++) {
    EEPROM.write(start_address + i, NULL);
  }
}

/*
 * This Function reads data from eeprom from start_address to end_address and returns a String
 */
String read_from_eeprom(int start_address, int end_address, int arr_length) {
  String read_data;
  for (int i = 0 ; i <= arr_length ; i++) {
    read_data += (char)EEPROM.read(start_address + i);
  }
  return read_data;
}


