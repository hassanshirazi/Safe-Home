#include "ConfigSettings.h"
#include "Arduino.h"
#include "global.h"
#include "eepromInterface.h"
#include "commandsToSD.h"
#include <RCSwitch.h>


/*
 * Function for Initializing SD Card
 * Return True if Initialization successfull
 * Else False
 */
bool initialize_sd_card() {
  return SD.begin(4); // 4 is SD_CS pin on arduino
}

bool set_rtc_time(int rtc_hour, int rtc_minute, int rtc_second, int rtc_day, int rtc_month, int rtc_year) {
  setTime(rtc_hour, rtc_minute, rtc_second, rtc_day, rtc_month, rtc_year);
  RTC.set(now());
}


void read_rtc_display() {
  int rtc_hour, rtc_minute, rtc_second, rtc_year, rtc_month, rtc_day;
  tmElements_t time_date;
  String current_time = "";
  String current_date = "";
  if (RTC.read(time_date)) {
    rtc_hour = time_date.Hour;
    rtc_minute = time_date.Minute;
    rtc_second = time_date.Second;
    rtc_day = time_date.Day;
    rtc_month = time_date.Month;
    rtc_year = tmYearToCalendar(time_date.Year);
    Serial.print("Read Date/Time is = ");
    Serial.print((String)rtc_year);
    Serial.print("/");
    Serial.print((String)rtc_month);
    Serial.print("/");
    Serial.print((String)rtc_day);
    Serial.print("|");
    Serial.print((String)rtc_hour);
    Serial.print(":");
    Serial.print((String)rtc_minute);
    Serial.print(":");
    Serial.println((String)rtc_second);
    Serial.println();
  }
  else {
    Serial.println("RTC Not Attached");
  }
}


void initConfiguration() {
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(relay, OUTPUT);
  pinMode(modem_on, OUTPUT);

  pinMode(chip_select_sd, OUTPUT);
  pinMode(chip_select_ethernet, OUTPUT);
  pinMode(chip_select_wifi, OUTPUT);

  digitalWrite(chip_select_sd, LOW);
  digitalWrite(chip_select_ethernet, HIGH);
  digitalWrite(chip_select_wifi, HIGH);

 

  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  pinMode(A4, INPUT);
  pinMode(A5, INPUT);
  pinMode(A6, INPUT);
  pinMode(A7, INPUT);
  pinMode(A8, INPUT);
  pinMode(A9, INPUT);
  pinMode(A10, INPUT);
  pinMode(A11, INPUT);
  pinMode(A12, INPUT);
  pinMode(A13, INPUT);
  pinMode(A14, INPUT);

  // Initialize SD Card
  if (initialize_sd_card()) {
    Serial.println("SD Card Initialization done.....");
  }
  else {
    Serial.println("SD Card Initialization Failed.....");
  }

  digitalWrite(chip_select_sd, HIGH);
  digitalWrite(chip_select_ethernet, LOW);
  digitalWrite(chip_select_wifi, HIGH);

  digitalWrite(modem_on, HIGH);
  digitalWrite(buzzer, HIGH);
  digitalWrite(LED1, HIGH);
  digitalWrite(LED2, HIGH);
  digitalWrite(LED3, HIGH);
  delay(1500);
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, LOW);
  digitalWrite(buzzer, LOW);
  digitalWrite(modem_on, LOW);

  mySwitch.enableReceive(0);  // Receiver on inerrupt 0 => that is pin #2
}

String read_sd_card_configure_variables(String file_name) {
  File read_cmd;
  String read_data;
  read_cmd = SD.open(file_name);
  if (read_cmd) {
    while (read_cmd.available()) {
      read_data = read_cmd.readStringUntil('\n');
    }
    read_cmd.close();
    return read_data;
  }
  else {
    Serial.println("Error Accessing SD Card, Please restart Control Panel");
  }
}

/*
 * This Function gets command from read_added_sensors_and_configure_io(), parses the command and configures I/O pins accordingly
 */
void parse_data_and_configure_sensor() {
  byte id;
  int first_comma, second_comma, third_comma, forth_comma, fifth_comma, sixth_comma, seventh_comma, eigth_comma;
  byte arm_disarm;
  String threshold, sensor_id;
  String sensor_name = "";
  int index = 0;
  int check_comma = 0;
  int sensor_config_index = 0;

  check_comma = 0;
  first_comma = 0;
  second_comma = 0;
  third_comma = 0;
  forth_comma = 0;
  fifth_comma = 0;
  sixth_comma = 0;
  seventh_comma = 0;
  index = 0;

  for (int i = 0; i <= 16; i++) {
    sensor_pin_map_sensor_id[i] = '\0';
    sensor_pin_map_sensor_pin[i] = '\0';
    sensor_armed_disarmed[i] = '\0';
    sensor_threshold[i] = '\0';
  }

  String file_name;
  String read_data;

    
  for (int i = 1; i <= 15; i++) {
    if (EEPROM.read(i) == '1') {
      Serial.print("Sensor found at pin # ");
      Serial.println(i);
      file_name += (byte)i;
      file_name.concat(".txt");
      char final_ping_file_name[file_name.length() + 1];
      file_name.toCharArray(final_ping_file_name, sizeof(final_ping_file_name));

      read_data = read_from_sd_and_display(final_ping_file_name);
      file_name = "";

      check_comma = 0;
      first_comma = 0;
      second_comma = 0;
      third_comma = 0;
      forth_comma = 0;
      fifth_comma = 0;
      sixth_comma = 0;
      seventh_comma = 0;
      eigth_comma = 0;


      for (int i = 0; i < read_data.length(); i++) {
        if (read_data[i] == '|' && check_comma == 7 ) {
          eigth_comma = i;
          check_comma = 8;
        }
        if (read_data[i] == '|' && check_comma == 6 ) {
          seventh_comma = i;
          check_comma = 7;
        }
        if (read_data[i] == '|' && check_comma == 5 ) {
          sixth_comma = i;
          check_comma = 6;
        }
        if (read_data[i] == '|' && check_comma == 4 ) {
          fifth_comma = i;
          check_comma = 5;
        }
        if (read_data[i] == '|' && check_comma == 3 ) {
          forth_comma = i;
          check_comma = 4;
        }
        if (read_data[i] == '|' && check_comma == 2 ) {
          third_comma = i;
          check_comma = 3;
        }
        if (read_data[i] == '|' && check_comma == 1 ) {
          second_comma = i;
          check_comma = 2;
        }
        if (read_data[i] == '|' && check_comma == 0 ) {
          first_comma = i;
          check_comma = 1;
        }
      }


      for (int j = third_comma + 1; j < forth_comma; j++) {
        sensor_id += (char)read_data[j];
      }

      for (int j = forth_comma + 1; j < fifth_comma; j++) {
        sensor_name += (char)read_data[j];
      }

      for (int j = sixth_comma + 1; j < seventh_comma; j++) {
        threshold += (char)read_data[j];
      }

      Serial.print("Sensor id = ");
      Serial.println(sensor_id);
      Serial.print("Sensor Name = ");
      Serial.println(sensor_name);
      Serial.print("Arm Disarm = ");
      Serial.println(arm_disarm);
      Serial.print("Sensor Threshold = ");
      Serial.println(threshold);

      if (sensor_name == "PIR") {
        id = 0x16;
      }
      if (sensor_name == "SMOKE") {
        id = 0x17;
      }
      if (sensor_name == "DOOR") {
        id = 0x18;
      }
      if (sensor_name == "IR") {
        id = 0x19;
      }
      if (sensor_name == "MQ7") {
        id = 0x20;
      }
      if (sensor_name == "MQ2") {
        id = 0x21;
      }
      if (sensor_name == "GLASS") {
        id = 0x22;
      }
      if (sensor_name == "PANIC") {
        id = 0x23;
      }
      

      arm_disarm = read_data[fifth_comma + 1];

      sensor_pin_map_sensor_id[i] = id;                 // id represents the name of the sensor
      sensor_pin_map_sensor_pin[i] = sensor_id.toInt(); // sensor_id represents the pin number at which the sensor is connected
      sensor_armed_disarmed[i] = arm_disarm;            // arm_disarm indicats if the sensor is armed or disarmed
      sensor_threshold[i] = threshold.toInt();          // threshold indicates the threshold value for analog sensors
      Serial.println("Updated 4 array's");
      threshold = "";
      sensor_id = "";
      sensor_name = "";
    }
  }
}


