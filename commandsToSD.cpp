#include "commandsToSD.h"
#include "Arduino.h"
#include "global.h"
#include "eepromInterface.h"
#include "ConfigSettings.h"


/*
 * This Funciton gets I/O configuration commands from server and writes them to SD card
 */
void map_new_sensor_to_sd(byte data[], byte cmd, int array_length) {
  String data_to_be_written;
  int start_comma;
  int first_comma, second_comma, third_comma, forth_comma, fifth_comma, sixth_comma, seventh_comma, eigth_comma;
  byte command_id, arm_disarm;

  String sensor_id, last_sensor_pin;
  String file_name, last_file_name;
  String sensor_label = "";
  String sensor_name = "";
  String threshold = "";
  int index = 0;
  int check_comma = 0;

  check_comma = 0;
  first_comma = 0;
  second_comma = 0;
  third_comma = 0;
  forth_comma = 0;
  fifth_comma = 0;
  sixth_comma = 0;
  seventh_comma = 0;
  eigth_comma = 0;
  index = 0;

  for (int i = 0; i < array_length; i++) {
    if (data[i] == '|' && check_comma == 7 ) {
      eigth_comma = i;
      check_comma = 8;
    }
    if (data[i] == '|' && check_comma == 6 ) {
      seventh_comma = i;
      check_comma = 7;
    }
    if (data[i] == '|' && check_comma == 5 ) {
      sixth_comma = i;
      check_comma = 6;
    }
    if (data[i] == '|' && check_comma == 4 ) {
      fifth_comma = i;
      check_comma = 5;
    }
    if (data[i] == '|' && check_comma == 3 ) {
      forth_comma = i;
      check_comma = 4;
    }
    if (data[i] == '|' && check_comma == 2 ) {
      third_comma = i;
      check_comma = 3;
    }
    if (data[i] == '|' && check_comma == 1 ) {
      second_comma = i;
      check_comma = 2;
    }
    if (data[i] == '|' && check_comma == 0 ) {
      first_comma = i;
      check_comma = 1;
    }
  }

  for (int j = third_comma + 1; j < forth_comma; j++) {
    sensor_id += (char)data[j];
  }

  for (int j = forth_comma + 1; j < fifth_comma; j++) {
    sensor_name += (char)data[j];
  }

  for (int j = sixth_comma + 1; j < seventh_comma; j++) {
    threshold += (char)data[j];
  }
  index = 0;

  for (int j = seventh_comma + 1; j < eigth_comma; j++) {
    sensor_label += (char)data[j];
  }

  for (int j = eigth_comma + 1; j < array_length - 1; j++) {
    last_sensor_pin += (char)data[j];
  }

  arm_disarm = data[fifth_comma + 1];

  Serial.print("Sensor id = ");
  Serial.println(sensor_id);
  Serial.print("Last connected Pin = ");
  Serial.println(last_sensor_pin);
  Serial.print("Sensor Name = ");
  Serial.println(sensor_name);
  Serial.print("Arm Disarm = ");
  Serial.println(arm_disarm);
  Serial.print("Sensor Label = ");
  Serial.println(sensor_label);
  Serial.print("Sensor Threshold = ");
  Serial.println(threshold);

  if (sensor_id == "1" || sensor_id == "2" || sensor_id == "3" || sensor_id == "4" || sensor_id == "5" || sensor_id == "6" || sensor_id == "7" || sensor_id == "8" || sensor_id == "9" || sensor_id == "10" || sensor_id == "11" || sensor_id == "12" || sensor_id == "13" || sensor_id == "14" || sensor_id == "15") {
    index = 0;
    file_name = sensor_id;
    Serial.print("Current File name at which sensor is connected = ");
    file_name.concat(".txt");
    char final_file_name[file_name.length() + 1];
    file_name.toCharArray(final_file_name, sizeof(final_file_name));
    Serial.println(final_file_name);

    for (int i = 0; i < eigth_comma; i++) {
      data_to_be_written += (char)data[i];
    }

    if (SD.exists(final_file_name)) {
      SD.remove(final_file_name);
    }
    if (last_sensor_pin != "0") {
      Serial.println("Last Sensor Pin exists");
      last_file_name = last_sensor_pin;
      last_file_name.concat(".txt");
      char last_final_file_name[last_file_name.length() + 1];
      last_file_name.toCharArray(last_final_file_name, sizeof(last_final_file_name));
      Serial.print("Last File name at which sensor was connected = ");
      Serial.println(last_final_file_name);
      if (SD.exists(last_final_file_name)) {
        SD.remove(last_final_file_name);
        Serial.println("Deleted Last sensor Pin txt file from SD card");
      }
      EEPROM.write(last_sensor_pin.toInt(), '0');
    }

    add_new_sensor_to_sd(data_to_be_written, final_file_name);
    EEPROM.write(sensor_id.toInt(), '1');
    parse_data_and_configure_sensor();
  }

  if (sensor_id == "A" || sensor_id == "B" || sensor_id == "C" || sensor_id == "D" || sensor_id == "E" || sensor_id == "F" || sensor_id == "G" || sensor_id == "H" ) {
    if (sensor_id == "A") {
      sensor_id = "";
      sensor_id = "16";
      if (arm_disarm == '1') {
        digitalWrite(relay, HIGH);
      }
      if (arm_disarm == '0') {
        digitalWrite(relay, LOW);
      }
    }
    if (sensor_id == "B") {
      sensor_id = "";
      sensor_id = "17";
      if (arm_disarm == '1') {
        digitalWrite(relay, HIGH);
      }
      if (arm_disarm == '0') {
        digitalWrite(relay, LOW);
      }
    }
    if (sensor_id == "C") {
      sensor_id = "";
      sensor_id = "18";
      if (arm_disarm == '1') {
        digitalWrite(relay, HIGH);
      }
      if (arm_disarm == '0') {
        digitalWrite(relay, LOW);
      }
    }
    if (sensor_id == "D") {
      sensor_id = "";
      sensor_id = "19";
      if (arm_disarm == '1') {
        digitalWrite(relay, HIGH);
      }
      if (arm_disarm == '0') {
        digitalWrite(relay, LOW);
      }
    }
    if (sensor_id == "E") {
      sensor_id = "";
      sensor_id = "20";
      if (arm_disarm == '1') {
        digitalWrite(relay, HIGH);
      }
      if (arm_disarm == '0') {
        digitalWrite(relay, LOW);
      }
    }
    if (sensor_id == "F") {
      sensor_id = "";
      sensor_id = "21";
      if (arm_disarm == '1') {
        digitalWrite(relay, HIGH);
      }
      if (arm_disarm == '0') {
        digitalWrite(relay, LOW);
      }
    }
    if (sensor_id == "G") {
      sensor_id = "";
      sensor_id = "22";
      if (arm_disarm == '1') {
        digitalWrite(relay, HIGH);
      }
      if (arm_disarm == '0') {
        digitalWrite(relay, LOW);
      }
    }
    if (sensor_id == "H") {
      sensor_id = "";
      sensor_id = "23";
      if (arm_disarm == '1') {
        digitalWrite(relay, HIGH);
      }
      if (arm_disarm == '0') {
        digitalWrite(relay, LOW);
      }
    }

    file_name = sensor_id;
    Serial.print("Current File name at which actuator is connected = ");
    file_name.concat(".txt");
    char final_file_name[file_name.length() + 1];
    file_name.toCharArray(final_file_name, sizeof(final_file_name));
    Serial.println(final_file_name);

    for (int i = 0; i < eigth_comma; i++) {
      data_to_be_written += (char)data[i];
    }

    if (SD.exists(final_file_name)) {
      SD.remove(final_file_name);
    }
    if (last_sensor_pin != "0") {
      Serial.println("Last Actuator Pin exists");
      last_file_name = last_sensor_pin;
      last_file_name.concat(".txt");
      char last_final_file_name[last_file_name.length() + 1];
      last_file_name.toCharArray(last_final_file_name, sizeof(last_final_file_name));
      Serial.print("Last File name at which Actuator was connected = ");
      Serial.println(last_final_file_name);
      if (SD.exists(last_final_file_name)) {
        SD.remove(last_final_file_name);
        Serial.println("Deleted Last Actuator Pin txt file from SD card");
      }
      EEPROM.write(last_sensor_pin.toInt(), '0');
    }
    add_new_sensor_to_sd(data_to_be_written, final_file_name);
    EEPROM.write(sensor_id.toInt(), '1');
  }
}

void delete_sensor_from_sd(byte data[], byte cmd, int array_length) {
  int first_comma, second_comma, third_comma, check_comma = 0;
  String delete_sensor_id;
  String delete_file_name;
  for (int i = 0; i < array_length; i++) {
    if (data[i] == '|' && check_comma == 2 ) {
      third_comma = i;
      check_comma = 3;
    }
    if (data[i] == '|' && check_comma == 1 ) {
      second_comma = i;
      check_comma = 2;
    }
    if (data[i] == '|' && check_comma == 0 ) {
      first_comma = i;
      check_comma = 1;
    }
  }

  for (int j = third_comma + 1; j < array_length - 1; j++) {
    delete_sensor_id += (char)data[j];
  }


  EEPROM.write(delete_sensor_id.toInt(), '0');
  delete_file_name = delete_sensor_id;
  Serial.print("Delete File name at which sensor is connected = ");
  delete_file_name.concat(".txt");
  char delete_final_file_name[delete_file_name.length() + 1];
  delete_file_name.toCharArray(delete_final_file_name, sizeof(delete_final_file_name));
  Serial.println(delete_final_file_name);
  if (SD.exists(delete_final_file_name)) {
    SD.remove(delete_final_file_name);
  }
}

void read_added_sensors_from_eeprom() {
  for (int i = 1; i <= 15; i++) {
    if (EEPROM.read(i) == '1') {
      Serial.print("Sensor Connected at pin number = ");
      Serial.println((String)i);
    }
    else {
      Serial.print("No Sensor Connected at pin number = ");
      Serial.println((String)i);
    }
  }
}


/*
 * This Function gets called from map_new_sensor_to_sd(), to write a sensor add/update command into SD card
 * File name in sd card is the pin nimber at which the sesnor is attached
 * Data written is in follwoing foramt
 * Sensor id(pin number), Sensor name(hex value), Arm/disarm, threshold, Sensor Label (string description)#
 */
void add_new_sensor_to_sd(String add_sensor_data, String file_name) {
  File add_new_sensor;
  add_new_sensor = SD.open(file_name, FILE_WRITE);
  if (add_new_sensor) {
    Serial.println("Writing/Updating Sensor in SD Card ");
    Serial.println(add_sensor_data);
    add_new_sensor.println(add_sensor_data);
    add_new_sensor.close();
    Serial.println("Writing Successfull");
  }
  else {
    Serial.println("Error Accessing SD Card, Please restart Control Panel");
  }
}

void write_command_to_sd(String data, String file_name) {
  File cmd;
  cmd = SD.open(file_name, FILE_WRITE);
  Serial.println("Writing/Updating Data in SD Card ");
  if (cmd) {
    cmd.println(data);
    cmd.close();
    Serial.println("Writing Successfull");
  }
  else {
    Serial.println("Error Accessing SD Card, Please restart Control Panel");
  }
}

String read_from_sd_and_display(String file_name) {
  File read_cmd;
  String read_data;
  read_cmd = SD.open(file_name);
  if (read_cmd) {
    while (read_cmd.available()) {
      read_data = read_cmd.readStringUntil('\n');
      Serial.print("Data read from SD file = ");
      Serial.println(read_data);
    }
    read_cmd.close();
    return read_data;
  }
  else {
    Serial.println("Error Accessing SD Card, Please restart Control Panel");
  }
}


/*
 * This Function reads from commands from server and writes them in sd card prefined locations
 */
void map_command_to_eeprom(byte cmd,  byte data[], int array_length) {
  int first_comma, second_comma, third_comma, forth_comma, fifth_comma, sixth_comma;
  int first_slash, second_slash;
  int first_colon, second_colon;
  int first_space;
  int check_comma = 0;
  int index = 0;

  String ip_address;
  String port_number;
  String username;
  String password;

  String wifi_ssid;
  String wifi_password;

  String p1, p2, p3;
  String t_year, t_month, t_day, t_hour, t_min, t_sec;


  switch (cmd) {
    case 0x32:
      Serial.println("Set Date/Time Command recieved");
      check_comma = 0;
      first_comma = 0;
      second_comma = 0;
      third_comma = 0;
      forth_comma = 0;
      fifth_comma = 0;
      sixth_comma = 0;
      first_colon = 0;
      second_colon = 0;
      first_space = 0;
      first_slash = 0;
      second_slash = 0;
      index = 0;
      for (int i = 0; i < array_length; i++) {
        if (data[i] == ':' && check_comma == 7) {
          second_colon = i;
          check_comma = 8;
        }
        if (data[i] == ':' && check_comma == 6) {
          first_colon = i;
          check_comma = 7;
        }
        if (data[i] == ' ' && check_comma == 5) {
          first_space = i;
          check_comma = 6;
        }
        if (data[i] == '-' && check_comma == 4) {
          second_slash = i;
          check_comma = 5;
        }
        if (data[i] == '-' && check_comma == 3) {
          first_slash = i;
          check_comma = 4;
        }
        if (data[i] == '|' && check_comma == 2 ) {
          third_comma = i;
          check_comma = 3;
        }
        if (data[i] == '|' && check_comma == 1 ) {
          second_comma = i;
          check_comma = 2;
        }
        if (data[i] == '|' && check_comma == 0 ) {
          first_comma = i;
          check_comma = 1;
        }
      }

      index = 0;
      for (int j = third_comma + 1; j < first_slash; j++) {
        t_year += (char)data[j];
      }
      index = 0;
      for (int j = first_slash + 1; j < second_slash; j++) {
        t_month += (char)data[j];
      }
      index = 0;
      for (int j = second_slash + 1; j < first_space; j++) {
        t_day += (char)data[j];
      }
      index = 0;
      for (int j = first_space + 1; j < first_colon; j++) {
        t_hour += (char)data[j];
      }
      index = 0;
      for (int j = first_colon + 1; j < second_colon; j++) {
        t_min += (char)data[j];
      }
      index = 0;
      for (int j = second_colon + 1; j < array_length - 1; j++) {
        t_sec += (char)data[j];
      }
      index = 0;

      Serial.print(t_year);
      Serial.print("-");
      Serial.print(t_month);
      Serial.print("-");
      Serial.print(t_day);
      Serial.print(" ");
      Serial.print(t_hour);
      Serial.print(":");
      Serial.print(t_min);
      Serial.print(":");
      Serial.println(t_sec);

      // Set the Date Time value Here
      set_rtc_time(t_hour.toInt(), t_min.toInt(), t_sec.toInt(), t_day.toInt(), t_month.toInt(), t_year.toInt());
      read_rtc_display();
      break;

    case 0x33:
      Serial.println("Set Phone Number Command recieved");
      check_comma = 0;
      first_comma = 0;
      second_comma = 0;
      third_comma = 0;
      forth_comma = 0;
      fifth_comma = 0;
      index = 0;
      for (int i = 0; i < array_length; i++) {
        if (data[i] == '|' && check_comma == 4 ) {
          fifth_comma = i;
          check_comma = 5;
        }
        if (data[i] == '|' && check_comma == 3 ) {
          forth_comma = i;
          check_comma = 4;
        }
        if (data[i] == '|' && check_comma == 2 ) {
          third_comma = i;
          check_comma = 3;
        }
        if (data[i] == '|' && check_comma == 1 ) {
          second_comma = i;
          check_comma = 2;
        }
        if (data[i] == '|' && check_comma == 0 ) {
          first_comma = i;
          check_comma = 1;
        }
      }
      index = 0;
      for (int j = third_comma + 1; j < forth_comma; j++) {
        p1 += (char)data[j];
      }
      index = 0;
      for (int j = forth_comma + 1; j < fifth_comma; j++) {
        p2 += (char)data[j];
      }
      index = 0;
      for (int j = fifth_comma + 1; j < array_length - 1; j++) {
        p3 += (char)data[j];
      }
      index = 0;
      Serial.print("Phone # 1 = ");
      Serial.println(p1);
      Serial.print("Phone # 2 = ");
      Serial.println(p2);
      Serial.print("Phone # 3 = ");
      Serial.println(p3);

      if (SD.exists("phone1.txt")) {
        SD.remove("phone1.txt");
      }
      if (SD.exists("phone2.txt")) {
        SD.remove("phone2.txt");
      }
      if (SD.exists("phone3.txt")) {
        SD.remove("phone3.txt");
      }

      write_command_to_sd(p1, "phone1.txt");
      write_command_to_sd(p2, "phone2.txt");
      write_command_to_sd(p3, "phone3.txt");

      read_from_sd_and_display("phone1.txt");
      read_from_sd_and_display("phone2.txt");
      read_from_sd_and_display("phone3.txt");
      break;


    case 0x34: // Wifi SSID, Password
      Serial.println("Set Wifi SSID and Password command received ");
      check_comma = 0;
      first_comma = 0;
      second_comma = 0;
      third_comma = 0;
      forth_comma = 0;
      fifth_comma = 0;
      index = 0;
      for (int i = 0; i < array_length; i++) {
        if (data[i] == '|' && check_comma == 3 ) {
          forth_comma = i;
          check_comma = 4;
        }
        if (data[i] == '|' && check_comma == 2 ) {
          third_comma = i;
          check_comma = 3;
        }
        if (data[i] == '|' && check_comma == 1 ) {
          second_comma = i;
          check_comma = 2;
        }
        if (data[i] == '|' && check_comma == 0 ) {
          first_comma = i;
          check_comma = 1;
        }
      }
      index = 0;
      for (int j = third_comma + 1; j < forth_comma; j++) {
        wifi_ssid +=  (char)data[j];
      }
      index = 0;
      for (int j = forth_comma + 1; j < array_length - 1; j++) {
        wifi_password +=  (char)data[j];
      }

      Serial.print("Wifi SSID = ");
      Serial.println(wifi_ssid);
      Serial.print("Wifi Password = ");
      Serial.println(wifi_password);

      if (SD.exists("ssid.txt")) {
        SD.remove("ssid.txt");
      }
      if (SD.exists("pwd.txt")) {
        SD.remove("pwd.txt");
      }

      write_command_to_sd(wifi_ssid, "ssid.txt");
      write_command_to_sd(wifi_password, "pwd.txt");

      read_from_sd_and_display("ssid.txt");
      read_from_sd_and_display("pwd.txt");
      break;



    case 0x35: // MQTT servers IP , Port, Username, Password
      check_comma = 0;
      first_comma = 0;
      second_comma = 0;
      third_comma = 0;
      forth_comma = 0;
      fifth_comma = 0;
      sixth_comma = 0;
      index = 0;
      for (int i = 0; i < array_length; i++) {
        if (data[i] == '|' && check_comma == 5 ) {
          sixth_comma = i;
          check_comma = 6;
        }
        if (data[i] == '|' && check_comma == 4 ) {
          fifth_comma = i;
          check_comma = 5;
        }
        if (data[i] == '|' && check_comma == 3 ) {
          forth_comma = i;
          check_comma = 4;
        }
        if (data[i] == '|' && check_comma == 2 ) {
          third_comma = i;
          check_comma = 3;
        }
        if (data[i] == '|' && check_comma == 1 ) {
          second_comma = i;
          check_comma = 2;
        }
        if (data[i] == '|' && check_comma == 0 ) {
          first_comma = i;
          check_comma = 1;
        }
      }
      index = 0;
      for (int j = third_comma + 1; j < forth_comma; j++) {
        ip_address += (char)data[j];
      }
      index = 0;
      for (int j = forth_comma + 1; j < fifth_comma; j++) {
        port_number += (char)data[j];
      }
      index = 0;
      for (int j = fifth_comma + 1; j < sixth_comma; j++) {
        username += (char)data[j];
      }
      index = 0;
      for (int j = sixth_comma + 1; j < array_length - 1; j++) {
        password += (char)data[j];
      }

      Serial.print("MQTT IP Address = ");
      Serial.println(ip_address);
      Serial.print("MQTT Port = ");
      Serial.println(port_number);
      Serial.print("MQTT username = ");
      Serial.println(username);
      Serial.print("MQTT password = ");
      Serial.println(password);

      if (SD.exists("ip.txt")) {
        SD.remove("ip.txt");
      }
      if (SD.exists("port.txt")) {
        SD.remove("port.txt");
      }
      if (SD.exists("user.txt")) {
        SD.remove("user.txt");
      }
      if (SD.exists("password.txt")) {
        SD.remove("password.txt");
      }

      write_command_to_sd(ip_address, "ip.txt");
      write_command_to_sd(port_number, "port.txt");
      write_command_to_sd(username, "user.txt");
      write_command_to_sd(password, "password.txt");

      mqtt_ip_address = read_sd_card_configure_variables("ip.txt");
      mqtt_port_number = read_sd_card_configure_variables("port.txt");
      mqtt_username = read_sd_card_configure_variables("user.txt");
      mqtt_password = read_sd_card_configure_variables("password.txt");

      mqtt_values_changed = 1;
      break;
  }
}


