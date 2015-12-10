#include "Arduino.h"
#include "global.h"
#include "logSensorData.h"
#include "eepromInterface.h"

void send_log_data_to_server(byte pin_number, int status_value, String description) {
  String log_data = "";
  int array_length = 0;
  tmElements_t time_date;
  if (RTC.read(time_date)) {
    rtc_hour = time_date.Hour;
    rtc_minute = time_date.Minute;
    rtc_second = time_date.Second;
    rtc_day = time_date.Day;
    rtc_month = time_date.Month;
    rtc_year = tmYearToCalendar(time_date.Year);
  }
  else {
    Serial.println("Error reading current Time");
    Serial.println("Trying to read again");
  }


  log_data = device_id + '|' + (String)rtc_year + '-' + (String)rtc_month + '-' + (String)rtc_day + ' ' + (String)rtc_hour + ':' + (String)rtc_minute + ':' + (String)rtc_second + '|' + pin_number + '|' + status_value + '|' + description + '#';

  char final_log_data[log_data.length() + 1];
  log_data.toCharArray(final_log_data, sizeof(final_log_data));
  Serial.print("Alert Data = ");
  Serial.println(final_log_data);
  if (mqtt_client.connected()) {
    Serial.println("Published Log Data to Broker");
    mqtt_client.publish("Alerts", final_log_data);
  }
  else {
    if (log_sensor_data_in_sd(log_data)) {
      Serial.println("Wrote 1 line of sensor LOG in sd becasue internet was not available");
      Serial.println();
    }
  }
}

bool log_sensor_data_in_sd(String sensor_log_data ) {
  File sensor_log;
  File log_index;
  String log_index_end_value = "";
  unsigned long end_index;

  if (SD.exists("EINDEX.txt")) {
    log_index = SD.open("EINDEX.txt");
    if (log_index) {
      while (log_index.available()) {
        log_index_end_value = log_index.readStringUntil('\n');
      }
      log_index.close();
      end_index = log_index_end_value.toInt();
    }
  }
  else {
    log_index = SD.open("EINDEX.txt", FILE_WRITE);
    if (log_index) {
      log_index.println("0");
      log_index.close();
      end_index = 0;
    }
  }

  sensor_log = SD.open("LOG.txt", FILE_WRITE);

  if (sensor_log) {
    Serial.println("Writing Sensor Log to LOG.txt...");
    sensor_log.println(sensor_log_data);
    Serial.println(sensor_log_data);    
    sensor_log.close();

    end_index++;
    log_index = SD.open("EINDEX.txt", FILE_WRITE);
    if (log_index) {
      log_index.println((String)end_index);
      log_index.close();
    }
    return true;
  }
  else {
    Serial.println("Error Opening LOG.txt to write LOG in SD card...");
    return false;
  }
}

void read_log_data_from_sd() {
  String log_index_start_value = "";
  String log_index_end_value = "";
  unsigned long start_index = 0;
  unsigned long end_index = 0;
  unsigned long current_index = 0;
  File end_log_index;
  File start_log_index;
  File sensor_log;
  String read_log_line = "";

  if (mqtt_client.connected()) {
    // Read saved start index value from SINDEX.txt
    if (SD.exists("SINDEX.txt")) {
      start_log_index = SD.open("SINDEX.txt");
      if (start_log_index) {
        while (start_log_index.available()) {
          log_index_start_value = start_log_index.readStringUntil('\n');
        }
        start_log_index.close();
        start_index = log_index_start_value.toInt();
      }
    }
    else {
      start_log_index = SD.open("SINDEX.txt", FILE_WRITE);
      if (start_log_index) {
        start_log_index.println("0");
        start_log_index.close();
        start_index = 0;
      }
    }

    // Read saved end index value from EINDEX.txt
    end_log_index = SD.open("EINDEX.txt");
    if (end_log_index) {
      while (end_log_index.available()) {
        log_index_end_value = end_log_index.readStringUntil('\n');
      }
      end_log_index.close();
      end_index = log_index_end_value.toInt();
    }


    if (start_index < end_index) {
      sensor_log = SD.open("LOG.txt");
      if (sensor_log) {
        while (sensor_log.available()) {
          read_log_line = sensor_log.readStringUntil('\n');
          if (read_log_line != "") {
            current_index++;
            if (current_index > start_index)  {
              // Transmit read_log_line to server here
              char final_log_data[read_log_line.length() + 1];
              read_log_line.toCharArray(final_log_data, sizeof(final_log_data));              
              Serial.println("Publishing Log Data to Broker read from Sd card LOG file");
              Serial.println(final_log_data);
              mqtt_client.publish("Alerts", final_log_data);
              start_index++;
              start_log_index = SD.open("SINDEX.txt", FILE_WRITE);
              if (start_log_index) {
                start_log_index.println((String)start_index);
                start_log_index.close();
              }
            }
          }
        }
      }
    }
  }
}
