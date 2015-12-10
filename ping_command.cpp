#include "global.h"
#include "ping_command.h"
#include <string.h>

#include "commandsToSD.h"
#include "Arduino.h"

void ping_response() {
  String file_name;
  String read_data;
  for (int i = 1; i <= 23; i++) {
    if (EEPROM.read(i) == '1') {
      Serial.print("Reading sensor/actuator from ");
      file_name += (byte)i;
      file_name.concat(".txt");
      char final_ping_file_name[file_name.length() + 1];
      file_name.toCharArray(final_ping_file_name, sizeof(final_ping_file_name));
      Serial.println(final_ping_file_name);
      read_data = read_from_sd_and_display(final_ping_file_name);
      char final_read_data[read_data.length() + 1];
      read_data.toCharArray(final_read_data, sizeof(final_read_data));
      mqtt_client.publish("Response", final_read_data);
      file_name = "";
    }
  }
}
void delete_all_sensors() {
  String file_name;
  String read_data;
  for (int i = 1; i <= 25; i++) {
    if (EEPROM.read(i) == '1') {
      Serial.print("Deleting Sensor = ");
      file_name += (byte)i;
      file_name.concat(".txt");
      char final_file_name[file_name.length() + 1];
      file_name.toCharArray(final_file_name, sizeof(final_file_name));
      Serial.println(final_file_name);
      if (SD.exists(final_file_name)) {
        SD.remove(final_file_name);
      }
      file_name = "";
    }
  }
}

