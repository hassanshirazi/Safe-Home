#include "drawGUI.h"
#include "ConfigSettings.h"
#include "global.h"
#include "ethernetConnection.h"
#include "ping_command.h"
#include "commandsToSD.h"
#include "logSensorData.h"

#include <ShiftRegister74HC595.h>
#include <RCSwitch.h>

// Include files for TFT LCD and Touch
#include <UTFT_Buttons.h>
#include <UTouch.h>
#include <UTouchCD.h>
#include <memorysaver.h>
#include <UTFT.h>
#include <avr/pgmspace.h>
#include <UTFT_tinyFAT.h>
#include <tinyFAT.h>

// Include file for SD card
#include <SD.h>


// Include file for MQTT Client
#include <PubSubClient.h>

// Include file for Temperature/Humidity Sensor
#include <dht.h>

// Include files for RTC
#include <Wire.h>
#include <Time.h>
#include <DS1307RTC.h>

// Include file for Software serial
#include <SoftwareSerial.h>

// Include file for SPI
#include <SPI.h>

// Include file for EEPROM
#include <EEPROM.h>

// Include files for Wi-Fi Shield
#include <Adafruit_CC3000.h>
#include <ccspi.h>
#include <string.h>
#include "utility/debug.h"

// Include files for Eternent Shield
#include <Dhcp.h>
#include <Dns.h>
#include <Ethernet.h>
#include <EthernetClient.h>

/*
 * MQTT Call_Back Function for recieving data from server
 */
void callback(char* topic, byte* payload, unsigned int length)
{
  String recieved_device_id = "";
  byte command_ref;
  String file_name;
  String data_to_be_written;
  String ack_data;
  int first_comma = 0, second_comma = 0, check_comma = 0;

  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.println("] ");

  for (int i = 0; i < length; i++) {
    if (payload[i] == '|' && check_comma == 1 ) {
      second_comma = i;
      check_comma = 2;
    }
    if (payload[i] == '|' && check_comma == 0 ) {
      first_comma = i;
      check_comma = 1;
    }
  }

  for (int i = first_comma + 1; i < second_comma; i++) {
    recieved_device_id += (char)payload[i];
  }


  if (recieved_device_id == device_id) {
    command_ref = payload[second_comma + 1];
    Serial.print("Command ref = ");
    Serial.println(command_ref, HEX);

    if (command_ref == 0x38) {
      Serial.println("Factory Reset Command Received");
      for (int i = 0; i <= 16; i++) {
        sensor_pin_map_sensor_id[i] = '\0';
        sensor_pin_map_sensor_pin[i] = '\0';
        sensor_armed_disarmed[i] = '\0';
        sensor_threshold[i] = '\0';
      }
      delete_all_sensors();
      for (int i = 1; i <= 1000; i++) {
        EEPROM.write(i, '0');
      }
    }

    if (command_ref == 0x37) {
      Serial.println("Ping Command Recieved");
      ping_response();
    }

    if (command_ref == 0x31) {
      Serial.println("Add Sensor Command Recieved");
      map_new_sensor_to_sd(payload, command_ref, length);
    }

    if (command_ref == 0x36) {
      Serial.println("Delete Sensor Command Recieved");
      delete_sensor_from_sd(payload, command_ref, length);
    }

    if (command_ref == 0x32 || command_ref == 0x33 || command_ref == 0x34 || command_ref == 0x35 ) {
      map_command_to_eeprom(command_ref, payload , length); //  Update value in eeprom and update in local variables
    }

    for (int k = 0; k < length; k++) {
      ack_data += (char)payload[k];
    }
    char final_ack_data[ack_data.length() + 1];
    ack_data.toCharArray(final_ack_data, sizeof(final_ack_data));
    Serial.println(final_ack_data);
    mqtt_client.publish("Response", final_ack_data);
  }
}

byte pin_number = 0xFF;
int read_value = 0;

int current_value_door1, current_value_door2, current_value_door3, current_value_door4, current_value_door5, current_value_door6, current_value_door7;
int current_value_panic1, current_value_panic2, current_value_panic3, current_value_panic4, current_value_panic5, current_value_panic6, current_value_panic7;
int current_value_ir1, current_value_ir2, current_value_ir3, current_value_ir4, current_value_ir5, current_value_ir6, current_value_ir7;
int current_value_smoke1, current_value_smoke2, current_value_smoke3, current_value_smoke4, current_value_smoke5, current_value_smoke6, current_value_smoke7, current_value_smoke8;

int previous_value_door1, previous_value_door2, previous_value_door3, previous_value_door4, previous_value_door5, previous_value_door6, previous_value_door7;
int previous_value_panic1, previous_value_panic2, previous_value_panic3, previous_value_panic4, previous_value_panic5, previous_value_panic6, previous_value_panic7;
int previous_value_ir1, previous_value_ir2, previous_value_ir3, previous_value_ir4, previous_value_ir5, previous_value_ir6, previous_value_ir7;
int previous_value_smoke1, previous_value_smoke2, previous_value_smoke3, previous_value_smoke4, previous_value_smoke5, previous_value_smoke6, previous_value_smoke7, previous_value_smoke8;

int current_value_mq21, current_value_mq22, current_value_mq23, current_value_mq24, current_value_mq25, current_value_mq26, current_value_mq27, current_value_mq28;
int previous_value_mq21, previous_value_mq22, previous_value_mq23, previous_value_mq24, previous_value_mq25, previous_value_mq26, previous_value_mq27, previous_value_mq28;

int current_value_mq71, current_value_mq72, current_value_mq73, current_value_mq74, current_value_mq75, current_value_mq76, current_value_mq77, current_value_mq78;
int previous_value_mq71, previous_value_mq72, previous_value_mq73, previous_value_mq74, previous_value_mq75, previous_value_mq76, previous_value_mq77, previous_value_mq78;

long unsigned int current_value_wireless_pir;
long unsigned int previous_value_wireless_pir;

String file_name = "";
String final_file_name = "";

int check_internet = 1;
int check_internet_delay = 0;


void setup() {
  Serial.begin(9600);
  initConfiguration();
  initLCD();
  make_control_panel_interface();
  
  read_added_sensors_from_eeprom();
  parse_data_and_configure_sensor();

  read_rtc_display();

  mqtt_ip_address = read_sd_card_configure_variables("ip.txt");
  mqtt_port_number = read_sd_card_configure_variables("port.txt");
  mqtt_username = read_sd_card_configure_variables("user.txt");
  mqtt_password = read_sd_card_configure_variables("password.txt");
  phone_number1 = read_sd_card_configure_variables("phone1.txt");
  phone_number2 = read_sd_card_configure_variables("phone2.txt");
  phone_number3 = read_sd_card_configure_variables("phone3.txt");
  wifi_internet_ssid = read_sd_card_configure_variables("ssid.txt");
  wifi_internet_password = read_sd_card_configure_variables("pwd.txt");
  Serial.print("MQTT IP Address = ");
  Serial.println(mqtt_ip_address);
  Serial.print("MQTT Port Number = ");
  Serial.println(mqtt_port_number);
  Serial.print("MQTT Username = ");
  Serial.println(mqtt_username);
  Serial.print("MQTT Password = ");
  Serial.println(mqtt_password);
  Serial.print("Phone Number 1 = ");
  Serial.println(phone_number1);
  Serial.print("Phone Number 2 = ");
  Serial.println(phone_number2);
  Serial.print("Phone Number 3 = ");
  Serial.println(phone_number3);
  Serial.print("Wifi SSID = ");
  Serial.println(wifi_internet_ssid);
  Serial.print("Wifi Password = ");
  Serial.println(wifi_internet_password);

  set_mqtt_server(mqtt_ip_address, mqtt_port_number);
}



void loop() {
  while (1) {
    if (mySwitch.available()) {
      int value = mySwitch.getReceivedValue();
      if (value == 0) {
        Serial.print("Unknown encoding");
      } else {
        Serial.print("Received ");
        Serial.println( mySwitch.getReceivedValue() );
        digitalWrite(buzzer, HIGH);
        delay(500);
        digitalWrite(buzzer, LOW);


        String file_name = String(mySwitch.getReceivedValue());
        file_name.concat(".txt");
        char final_file_name[file_name.length() + 1];
        file_name.toCharArray(final_file_name, sizeof(final_file_name));
        Serial.println(final_file_name);


        // Read Wireless Sensors here
        if (add_pir_sensor == 0 && add_smoke_sensor == 0 && add_ir_sensor == 0 && add_door_sensor == 0 && add_glass_sensor == 0 && add_mq2_sensor == 0 && add_mq7_sensor == 0 && add_panic_sensor == 0)
        {
          if (SD.exists(final_file_name)) {
            if (mqtt_client.connected()) {
              send_log_data_to_server(mySwitch.getReceivedValue(), 1 , "A wireless Sensor is Alarmed");
            }
          }
        }

        // Add Wireless Sensors Here
        if (add_pir_sensor == 1) {
          myGLCD.print("                       ", 15 , 200);
          String write_data = "-1|" + device_id + '|' + '1' + '|' + (String(mySwitch.getReceivedValue())) + '|' + "PIR" + '|' + '1' + '|' + '0' + '|' + "Wireless PIR Sensor" + "|0";
          if (SD.exists(final_file_name)) {
            myGLCD.print("Duplicate Sensor", 20 , 200);
          }
          else {
            myGLCD.print("Saving PIR", 20 , 200);
            add_new_sensor_to_sd(write_data, final_file_name);
            if (mqtt_client.connected()) {
              char final_log_data[write_data.length() + 1];
              write_data.toCharArray(final_log_data, sizeof(final_log_data));
              mqtt_client.publish("Response", final_log_data);
            }
          }
          delay(1000);
          myGLCD.print("                       ", 15 , 200);
        }

        if (add_smoke_sensor == 1) {
          myGLCD.print("                       ", 15 , 200);
          myGLCD.print("Saving Smoke", 20 , 200);
          String write_data = "-1|" + device_id + '|' + '1' + '|' + mySwitch.getReceivedValue() + '|' + "SMOKE" + '|' + '1' + '|' + '0' + '|' + "Wireless Smoke Sensor" + "|0";
          if (SD.exists(final_file_name)) {
            myGLCD.print("Duplicate Sensor", 20 , 200);
          }
          else {
            myGLCD.print("Saving Smoke", 20 , 200);
            add_new_sensor_to_sd(write_data, final_file_name);
            if (mqtt_client.connected()) {
              char final_log_data[write_data.length() + 1];
              write_data.toCharArray(final_log_data, sizeof(final_log_data));
              mqtt_client.publish("Response", final_log_data);
            }
          }
          delay(1000);
          myGLCD.print("                       ", 15 , 200);
        }

        if (add_ir_sensor == 1) {
          myGLCD.print("                       ", 15 , 200);
          String write_data = "-1|" + device_id + '|' + '1' + '|' + mySwitch.getReceivedValue() + '|' + "IR" + '|' + '1' + '|' + '0' + '|' + "Wireless IR Sensor" + "|0";
          if (SD.exists(final_file_name)) {
            myGLCD.print("Duplicate Sensor", 20 , 200);
          }
          else {
            myGLCD.print("Saving IR", 20 , 200);
            add_new_sensor_to_sd(write_data, final_file_name);
            if (mqtt_client.connected()) {
              char final_log_data[write_data.length() + 1];
              write_data.toCharArray(final_log_data, sizeof(final_log_data));
              mqtt_client.publish("Response", final_log_data);
            }
          }
          delay(1000);
          myGLCD.print("                       ", 15 , 200);
        }

        if (add_door_sensor == 1) {
          myGLCD.print("                       ", 15 , 200);
          myGLCD.print("Saving Door", 20 , 200);
          String write_data = "-1|" + device_id + '|' + '1' + '|' + mySwitch.getReceivedValue() + '|' + "DOOR" + '|' + '1' + '|' + '0' + '|' + "Wireless Door Sensor" + "|0";
          if (SD.exists(final_file_name)) {
            myGLCD.print("Duplicate Sensor", 20 , 200);
          }
          else {
            myGLCD.print("Saving Door", 20 , 200);
            add_new_sensor_to_sd(write_data, final_file_name);
            if (mqtt_client.connected()) {
              char final_log_data[write_data.length() + 1];
              write_data.toCharArray(final_log_data, sizeof(final_log_data));
              mqtt_client.publish("Response", final_log_data);
            }
          }
          delay(1000);
          myGLCD.print("                       ", 15 , 200);
        }

        if (add_glass_sensor == 1) {
          myGLCD.print("                       ", 15 , 200);
          String write_data = "-1|" + device_id + '|' + '1' + '|' + mySwitch.getReceivedValue() + '|' + "GLASS" + '|' + '1' + '|' + '0' + '|' + "Wireless Glass Break Sensor" + "|0";
          if (SD.exists(final_file_name)) {
            myGLCD.print("Duplicate Sensor", 20 , 200);
          }
          else {
            myGLCD.print("Saving Glass", 20 , 200);
            add_new_sensor_to_sd(write_data, final_file_name);
            if (mqtt_client.connected()) {
              char final_log_data[write_data.length() + 1];
              write_data.toCharArray(final_log_data, sizeof(final_log_data));
              mqtt_client.publish("Response", final_log_data);
            }
          }
          delay(1000);
          myGLCD.print("                       ", 15 , 200);
        }

        if (add_mq2_sensor == 1) {
          myGLCD.print("                       ", 15 , 200);
          myGLCD.print("Saving MQ2", 20 , 200);
          String write_data = "-1|" + device_id + '|' + '1' + '|' + mySwitch.getReceivedValue() + '|' + "MQ2" + '|' + '1' + '|' + '0' + '|' + "Wireless MQ2 Sensor" + "|0";
          if (SD.exists(final_file_name)) {
            myGLCD.print("Duplicate Sensor", 20 , 200);
          }
          else {
            myGLCD.print("Saving MQ2", 20 , 200);
            add_new_sensor_to_sd(write_data, final_file_name);
            if (mqtt_client.connected()) {
              char final_log_data[write_data.length() + 1];
              write_data.toCharArray(final_log_data, sizeof(final_log_data));
              mqtt_client.publish("Response", final_log_data);
            }
          }
          delay(1000);
          myGLCD.print("                       ", 15 , 200);
        }

        if (add_mq7_sensor == 1) {
          myGLCD.print("                       ", 15 , 200);
          String write_data = "-1|" + device_id + '|' + '1' + '|' + mySwitch.getReceivedValue() + '|' + "MQ7" + '|' + '1' + '|' + '0' + '|' + "Wireless MQ7 Sensor" + "|0";
          if (SD.exists(final_file_name)) {
            myGLCD.print("Duplicate Sensor", 20 , 200);
          }
          else {
            myGLCD.print("Saving MQ7", 20 , 200);
            add_new_sensor_to_sd(write_data, final_file_name);
            if (mqtt_client.connected()) {
              char final_log_data[write_data.length() + 1];
              write_data.toCharArray(final_log_data, sizeof(final_log_data));
              mqtt_client.publish("Response", final_log_data);
            }
          }
          delay(1000);
          myGLCD.print("                       ", 15 , 200);
        }

        if (add_panic_sensor == 1) {
          myGLCD.print("                       ", 15 , 200);
          String write_data = "-1|" + device_id + '|' + '1' + '|' + mySwitch.getReceivedValue() + '|' + "PANIC" + '|' + '1' + '|' + '0' + '|' + "Wireless Panic Button" + "|0";
          if (SD.exists(final_file_name)) {
            myGLCD.print("Duplicate Sensor", 20 , 200);
          }
          else {
            myGLCD.print("Saving Panic", 20 , 200);
            add_new_sensor_to_sd(write_data, final_file_name);
            if (mqtt_client.connected()) {
              char final_log_data[write_data.length() + 1];
              write_data.toCharArray(final_log_data, sizeof(final_log_data));
              mqtt_client.publish("Response", final_log_data);
            }
          }
          delay(1000);
          myGLCD.print("                       ", 15 , 200);
        }
      }
      mySwitch.resetAvailable();
    }

    if (!mqtt_client.connected()) {
      digitalWrite(LED2, HIGH);
      if (check_internet == 1) {
        if (screen == 1) {
          display_tick_cross(3, 1);
          update_status(3);
        }
        reconnect_using_ethernet(mqtt_username, mqtt_password);
        check_internet = 2;
      }
    }
    mqtt_client.loop();

    if (check_internet == 2) {
      check_internet_delay++;
      delay(1);
    }

    if (check_internet_delay == 10000) {
      check_internet = 1;
      check_internet_delay = 0;
    }

    if (myTouch.dataAvailable() == true) {
      read_touch_buttons();
    }

    if (mqtt_values_changed == 1) {
      mqtt_values_changed = 0;
      if (internet_connection_using_ethernet()) {
        digitalWrite(buzzer, 1);
        delay(100);
        digitalWrite(buzzer, 0);
        set_mqtt_server(mqtt_ip_address, mqtt_port_number);
      }
    }

    if (device_arm_disarm_status == '1') {
      read_log_data_from_sd();
      // Read sensors Here
      for (int i = 1; i <= 15; i++) {
        if (sensor_armed_disarmed[i] == '1') {
          pin_number = sensor_pin_map_sensor_pin[i];

          //  Smoke Sensor Reading
          if (sensor_pin_map_sensor_id[i] == 0x17) {
            if (pin_number == 1) {
              current_value_smoke1 = digitalRead(A0);
              if (current_value_smoke1 != previous_value_smoke1) {
                if (current_value_smoke1 == 1) {
                  send_log_data_to_server(pin_number, current_value_smoke1, "Smoke sensor is detecting smoke in kitchen");
                  delay(4000);
                }
                if (current_value_smoke1 == 0) {
                  send_log_data_to_server(pin_number, current_value_smoke1, "No smoke in kitchen, Smoke sensor is stable");
                  delay(100);
                }
                previous_value_smoke1 = current_value_smoke1;
              }
              previous_value_smoke1 = current_value_smoke1;
            }
            if (pin_number == 2) {
              current_value_smoke2 = digitalRead(A1);
              if (current_value_smoke2 != previous_value_smoke2) {
                if (current_value_smoke2 == 1) {
                  send_log_data_to_server(pin_number, current_value_smoke2, "Smoke sensor is detecting smoke in kitchen");
                  delay(2000);
                }
                if (current_value_smoke2 == 0) {
                  send_log_data_to_server(pin_number, current_value_smoke2, "No smoke in kitchen, Smoke sensor is stable");
                  delay(100);
                }
                previous_value_smoke2 = current_value_smoke2;
              }
              previous_value_smoke2 = current_value_smoke2;
            }
            if (pin_number == 3) {
              current_value_smoke3 = digitalRead(A2);
              if (current_value_smoke3 != previous_value_smoke3) {
                if (current_value_smoke3 == 1) {
                  send_log_data_to_server(pin_number, current_value_smoke3, "Smoke sensor is detecting smoke in kitchen");
                  delay(2000);
                }
                if (current_value_smoke3 == 0) {
                  send_log_data_to_server(pin_number, current_value_smoke3, "No smoke in kitchen, Smoke sensor is stable");
                  delay(100);
                }
                previous_value_smoke3 = current_value_smoke3;
              }
              previous_value_smoke3 = current_value_smoke3;
            }
            if (pin_number == 4) {
              current_value_smoke4 = digitalRead(A3);
              if (current_value_smoke4 != previous_value_smoke4) {
                if (current_value_smoke4 == 1) {
                  send_log_data_to_server(pin_number, current_value_smoke4, "Smoke sensor is detecting smoke in kitchen");
                  delay(2000);
                }
                if (current_value_smoke4 == 0) {
                  send_log_data_to_server(pin_number, current_value_smoke4, "No smoke in kitchen, Smoke sensor is stable");
                  delay(100);
                }
                previous_value_smoke4 = current_value_smoke4;
              }
              previous_value_smoke4 = current_value_smoke4;
            }
            if (pin_number == 5) {
              current_value_smoke5 = digitalRead(A4);
              if (current_value_smoke5 != previous_value_smoke5) {
                if (current_value_smoke5 == 1) {
                  send_log_data_to_server(pin_number, current_value_smoke5, "Smoke sensor is detecting smoke in kitchen");
                  delay(2000);
                }
                if (current_value_smoke5 == 0) {
                  send_log_data_to_server(pin_number, current_value_smoke5, "No smoke in kitchen, Smoke sensor is stable");
                  delay(100);
                }
                previous_value_smoke5 = current_value_smoke5;
              }
              previous_value_smoke5 = current_value_smoke5;
            }
            if (pin_number == 6) {
              current_value_smoke6 = digitalRead(A5);
              if (current_value_smoke6 != previous_value_smoke6) {
                if (current_value_smoke6 == 1) {
                  send_log_data_to_server(pin_number, current_value_smoke6, "Smoke sensor is detecting smoke in kitchen");
                  delay(2000);
                }
                if (current_value_smoke6 == 0) {
                  send_log_data_to_server(pin_number, current_value_smoke6, "No smoke in kitchen, Smoke sensor is stable");
                  delay(100);
                }
                previous_value_smoke6 = current_value_smoke6;
              }
              previous_value_smoke6 = current_value_smoke6;
            }
            if (pin_number == 7) {
              current_value_smoke7 = digitalRead(A6);
              if (current_value_smoke7 != previous_value_smoke7) {
                if (current_value_smoke7 == 1) {
                  send_log_data_to_server(pin_number, current_value_smoke7, "Smoke sensor is detecting smoke in kitchen");
                  delay(2000);
                }
                if (current_value_smoke7 == 0) {
                  send_log_data_to_server(pin_number, current_value_smoke7, "No smoke in kitchen, Smoke sensor is stable");
                  delay(100);
                }
                previous_value_smoke7 = current_value_smoke7;
              }
              previous_value_smoke7 = current_value_smoke7;
            }
            if (pin_number == 8) {
              current_value_smoke8 = digitalRead(A7);
              if (current_value_smoke8 != previous_value_smoke8) {
                if (current_value_smoke8 == 1) {
                  send_log_data_to_server(pin_number, current_value_smoke8, "Smoke sensor is detecting smoke in kitchen");
                  delay(2000);
                }
                if (current_value_smoke8 == 0) {
                  send_log_data_to_server(pin_number, current_value_smoke8, "No smoke in kitchen, Smoke sensor is stable");
                  delay(100);
                }
                previous_value_smoke8 = current_value_smoke8;
              }
              previous_value_smoke8 = current_value_smoke8;
            }
          }


          // Panic Button Reading
          if (sensor_pin_map_sensor_id[i] == 0x23) {
            if (pin_number == 9) {
              current_value_panic1 = digitalRead(A8);
              if (current_value_panic1 != previous_value_panic1) {
                if (current_value_panic1 == 1) {
                  send_log_data_to_server(pin_number, current_value_panic1, "Panic Button is being pushed");
                  delay(100);
                }
                if (current_value_panic1 == 0) {
                  send_log_data_to_server(pin_number, current_value_panic1, "No Panic");
                  delay(100);
                }
                previous_value_panic1 = current_value_panic1;
              }
              previous_value_panic1 = current_value_panic1;
            }
            if (pin_number == 10) {
              current_value_panic2 = digitalRead(A9);
              if (current_value_panic2 != previous_value_panic2) {
                if (current_value_panic2 == 1) {
                  send_log_data_to_server(pin_number, current_value_panic2, "Panic Button is being pushed");
                  delay(100);
                }
                if (current_value_panic2 == 0) {
                  send_log_data_to_server(pin_number, current_value_panic2, "No Panic");
                  delay(100);
                }
                previous_value_panic2 = current_value_panic2;
              }
              previous_value_panic2 = current_value_panic2;
            }
            if (pin_number == 11) {
              current_value_panic3 = digitalRead(A10);
              if (current_value_panic3 != previous_value_panic3) {
                if (current_value_panic3 == 1) {
                  send_log_data_to_server(pin_number, current_value_panic3, "Panic Button is being pushed");
                  delay(100);
                }
                if (current_value_panic3 == 0) {
                  send_log_data_to_server(pin_number, current_value_panic3, "No Panic");
                  delay(100);
                }
                previous_value_panic3 = current_value_panic3;
              }
              previous_value_panic3 = current_value_panic3;
            }
            if (pin_number == 12) {
              current_value_panic4 = digitalRead(A11);
              if (current_value_panic4 != previous_value_panic4) {
                if (current_value_panic4 == 1) {
                  send_log_data_to_server(pin_number, current_value_panic4, "Panic Button is being pushed");
                  delay(100);
                }
                if (current_value_panic4 == 0) {
                  send_log_data_to_server(pin_number, current_value_panic4, "No Panic");
                  delay(100);
                }
                previous_value_panic4 = current_value_panic4;
              }
              previous_value_panic4 = current_value_panic4;
            }
            if (pin_number == 13) {
              current_value_panic5 = digitalRead(A12);
              if (current_value_panic5 != previous_value_panic5) {
                if (current_value_panic5 == 1) {
                  send_log_data_to_server(pin_number, current_value_panic5, "Panic Button is being pushed");
                  delay(100);
                }
                if (current_value_panic5 == 0) {
                  send_log_data_to_server(pin_number, current_value_panic5, "No Panic");
                  delay(100);
                }
                previous_value_panic5 = current_value_panic5;
              }
              previous_value_panic5 = current_value_panic5;
            }
            if (pin_number == 14) {
              current_value_panic6 = digitalRead(A13);
              if (current_value_panic6 != previous_value_panic6) {
                if (current_value_panic6 == 1) {
                  send_log_data_to_server(pin_number, current_value_panic6, "Panic Button is being pushed");
                  delay(100);
                }
                if (current_value_panic6 == 0) {
                  send_log_data_to_server(pin_number, current_value_panic6, "No Panic");
                  delay(100);
                }
                previous_value_panic6 = current_value_panic6;
              }
              previous_value_panic6 = current_value_panic6;
            }
            if (pin_number == 15) {
              current_value_panic7 = digitalRead(A14);
              if (current_value_panic7 != previous_value_panic7) {
                if (current_value_panic7 == 1) {
                  send_log_data_to_server(pin_number, current_value_panic7, "Panic Button is being pushed");
                  delay(100);
                }
                if (current_value_panic7 == 0) {
                  send_log_data_to_server(pin_number, current_value_panic7, "No Panic");
                  delay(100);
                }
                previous_value_panic7 = current_value_panic7;
              }
              previous_value_panic7 = current_value_panic7;
            }
          }


          // IR Beam Reading
          if (sensor_pin_map_sensor_id[i] == 0x19) {
            if (pin_number == 9) {
              current_value_ir1 = digitalRead(A8);
              if (current_value_ir1 != previous_value_ir1) {
                if (current_value_ir1 == 1) {
                  send_log_data_to_server(pin_number, current_value_ir1, "Some one crossed the IR Beam");
                  delay(500);
                }
                if (current_value_ir1 == 0) {
                  send_log_data_to_server(pin_number, current_value_ir1, "IR Beam is stable");
                  delay(500);
                }
                previous_value_ir1 = current_value_ir1;
              }
              previous_value_ir1 = current_value_ir1;
            }
            if (pin_number == 10) {
              current_value_ir2 = digitalRead(A9);
              if (current_value_ir2 != previous_value_ir2) {
                if (current_value_ir2 == 1) {
                  send_log_data_to_server(pin_number, current_value_ir2, "Some one crossed the IR Beam");
                  delay(500);
                }
                if (current_value_ir2 == 0) {
                  send_log_data_to_server(pin_number, current_value_ir2, "IR Beam is stable");
                  delay(500);
                }
                previous_value_ir2 = current_value_ir2;
              }
              previous_value_ir2 = current_value_ir2;
            }
            if (pin_number == 11) {
              current_value_ir3 = digitalRead(A10);
              if (current_value_ir3 != previous_value_ir3) {
                if (current_value_ir3 == 1) {
                  send_log_data_to_server(pin_number, current_value_ir3, "Some one crossed the IR Beam");
                  delay(500);
                }
                if (current_value_ir3 == 0) {
                  send_log_data_to_server(pin_number, current_value_ir3, "IR Beam is stable");
                  delay(500);
                }
                previous_value_ir3 = current_value_ir3;
              }
              previous_value_ir3 = current_value_ir3;
            }
            if (pin_number == 12) {
              current_value_ir4 = digitalRead(A11);
              if (current_value_ir4 != previous_value_ir4) {
                if (current_value_ir4 == 1) {
                  send_log_data_to_server(pin_number, current_value_ir4, "Some one crossed the IR Beam");
                  delay(500);
                }
                if (current_value_ir4 == 0) {
                  send_log_data_to_server(pin_number, current_value_ir4, "IR Beam is stable");
                  delay(500);
                }
                previous_value_ir4 = current_value_ir4;
              }
              previous_value_ir4 = current_value_ir4;
            }
            if (pin_number == 13) {
              current_value_ir5 = digitalRead(A12);
              if (current_value_ir5 != previous_value_ir5) {
                if (current_value_ir5 == 1) {
                  send_log_data_to_server(pin_number, current_value_ir5, "Some one crossed the IR Beam");
                  delay(500);
                }
                if (current_value_ir5 == 0) {
                  send_log_data_to_server(pin_number, current_value_ir5, "IR Beam is stable");
                  delay(500);
                }
                previous_value_ir5 = current_value_ir5;
              }
              previous_value_ir5 = current_value_ir5;
            }
            if (pin_number == 14) {
              current_value_ir6 = digitalRead(A13);
              if (current_value_ir6 != previous_value_ir6) {
                if (current_value_ir6 == 1) {
                  send_log_data_to_server(pin_number, current_value_ir6, "Some one crossed the IR Beam");
                  delay(500);
                }
                if (current_value_ir6 == 0) {
                  send_log_data_to_server(pin_number, current_value_ir6, "IR Beam is stable");
                  delay(500);
                }
                previous_value_ir6 = current_value_ir6;
              }
              previous_value_ir6 = current_value_ir6;
            }
            if (pin_number == 15) {
              current_value_ir7 = digitalRead(A14);
              if (current_value_ir7 != previous_value_ir7) {
                if (current_value_ir7 == 1) {
                  send_log_data_to_server(pin_number, current_value_ir7, "Some one crossed the IR Beam");
                  delay(500);
                }
                if (current_value_ir7 == 0) {
                  send_log_data_to_server(pin_number, current_value_ir7, "IR Beam is stable");
                  delay(500);
                }
                previous_value_ir7 = current_value_ir7;
              }
              previous_value_ir7 = current_value_ir7;
            }
          }



          // Door sensor reading
          if (sensor_pin_map_sensor_id[i] == 0x18) {
            if (pin_number == 9) {
              current_value_door1 = digitalRead(A8);
              if (current_value_door1 != previous_value_door1) {
                if (current_value_door1 == 0) {
                  send_log_data_to_server(pin_number, current_value_door1, "Some one opened the CTO Door");
                  delay(100);
                }
                if (current_value_door1 == 1) {
                  send_log_data_to_server(pin_number, current_value_door1, "CTO door closed");
                  delay(100);
                }
                previous_value_door1 = current_value_door1;
              }
              previous_value_door1 = current_value_door1;
            }
            if (pin_number == 10) {
              current_value_door2 = digitalRead(A9);
              if (current_value_door2 != previous_value_door2) {
                if (current_value_door2 == 0) {
                  send_log_data_to_server(pin_number, current_value_door2, "Some one opened the CTO Door");
                  delay(100);
                }
                if (current_value_door2 == 1) {
                  send_log_data_to_server(pin_number, current_value_door2, "CTO door closed");
                  delay(100);
                }
                previous_value_door2 = current_value_door2;
              }
              previous_value_door2 = current_value_door2;
            }
            if (pin_number == 11) {
              current_value_door3 = digitalRead(A10);
              if (current_value_door3 != previous_value_door3) {
                if (current_value_door3 == 0) {
                  send_log_data_to_server(pin_number, current_value_door3, "Some one opened the CTO Door");
                  delay(100);
                }
                if (current_value_door3 == 1) {
                  send_log_data_to_server(pin_number, current_value_door3, "CTO door closed");
                  delay(100);
                }
                previous_value_door3 = current_value_door3;
              }
              previous_value_door3 = current_value_door3;
            }
            if (pin_number == 12) {
              current_value_door4 = digitalRead(A11);
              if (current_value_door4 != previous_value_door4) {
                if (current_value_door4 == 0) {
                  send_log_data_to_server(pin_number, current_value_door4, "Some one opened the CTO Door");
                  delay(100);
                }
                if (current_value_door4 == 1) {
                  send_log_data_to_server(pin_number, current_value_door4, "CTO door closed");
                  delay(100);
                }
                previous_value_door4 = current_value_door4;
              }
              previous_value_door4 = current_value_door4;
            }
            if (pin_number == 13) {
              current_value_door5 = digitalRead(A12);
              if (current_value_door5 != previous_value_door5) {
                if (current_value_door5 == 0) {
                  send_log_data_to_server(pin_number, current_value_door5, "Some one opened the CTO Door");
                  delay(100);
                }
                if (current_value_door5 == 1) {
                  send_log_data_to_server(pin_number, current_value_door5, "CTO door closed");
                  delay(100);
                }
                previous_value_door5 = current_value_door5;
              }
              previous_value_door5 = current_value_door5;
            }
            if (pin_number == 14) {
              current_value_door6 = digitalRead(A13);
              if (current_value_door6 != previous_value_door6) {
                if (current_value_door6 == 0) {
                  send_log_data_to_server(pin_number, current_value_door6, "Some one opened the CTO Door");
                  delay(100);
                }
                if (current_value_door6 == 1) {
                  send_log_data_to_server(pin_number, current_value_door6, "CTO door closed");
                  delay(100);
                }
                previous_value_door6 = current_value_door6;
              }
              previous_value_door6 = current_value_door6;
            }
            if (pin_number == 15) {
              current_value_door7 = digitalRead(A14);
              if (current_value_door7 != previous_value_door7) {
                if (current_value_door7 == 0) {
                  send_log_data_to_server(pin_number, current_value_door7, "Some one opened the CTO Door");
                  delay(100);
                }
                if (current_value_door7 == 1) {
                  send_log_data_to_server(pin_number, current_value_door7, "CTO door closed");
                  delay(100);
                }
                previous_value_door7 = current_value_door7;
              }
              previous_value_door7 = current_value_door7;
            }
          }

          // Read Analog Sensors Here
          if (sensor_pin_map_sensor_id[i] == 0x20 || sensor_pin_map_sensor_id[i] == 0x21) {
            // Read Analog Sensor Here
            if (pin_number == 1) {
              read_value = analogRead(A0);
              if (read_value > sensor_threshold[i]) {
                if (sensor_pin_map_sensor_id[i] == 0x21) {
                  send_log_data_to_server(pin_number, read_value, "MQ2 Gas sensor reached threshold");
                }
                if (sensor_pin_map_sensor_id[i] == 0x20) {
                  send_log_data_to_server(pin_number, read_value, "MQ7 Gas sensor reached threshold");
                }
              }
            }
            if (pin_number == 2) {
              read_value = analogRead(A1);
              if (read_value > sensor_threshold[i]) {
                if (sensor_pin_map_sensor_id[i] == 0x21) {
                  send_log_data_to_server(pin_number, read_value, "MQ2 Gas sensor reached threshold");
                }
                if (sensor_pin_map_sensor_id[i] == 0x20) {
                  send_log_data_to_server(pin_number, read_value, "MQ7 Gas sensor reached threshold");
                }
              }
            }
            if (pin_number == 3) {
              read_value = analogRead(A2);
              if (read_value > sensor_threshold[i]) {
                if (sensor_pin_map_sensor_id[i] == 0x21) {
                  send_log_data_to_server(pin_number, read_value, "MQ2 Gas sensor reached threshold");
                }
                if (sensor_pin_map_sensor_id[i] == 0x20) {
                  send_log_data_to_server(pin_number, read_value, "MQ7 Gas sensor reached threshold");
                }
              }
            }
            if (pin_number == 4) {
              read_value = analogRead(A3);
              if (read_value > sensor_threshold[i]) {
                if (sensor_pin_map_sensor_id[i] == 0x21) {
                  send_log_data_to_server(pin_number, read_value, "MQ2 Gas sensor reached threshold");
                }
                if (sensor_pin_map_sensor_id[i] == 0x20) {
                  send_log_data_to_server(pin_number, read_value, "MQ7 Gas sensor reached threshold");
                }
              }
            }
            if (pin_number == 5) {
              read_value = analogRead(A4);
              if (read_value > sensor_threshold[i]) {
                if (sensor_pin_map_sensor_id[i] == 0x21) {
                  send_log_data_to_server(pin_number, read_value, "MQ2 Gas sensor reached threshold");
                }
                if (sensor_pin_map_sensor_id[i] == 0x20) {
                  send_log_data_to_server(pin_number, read_value, "MQ7 Gas sensor reached threshold");
                }
              }
            }
            if (pin_number == 6) {
              read_value = analogRead(A5);
              if (read_value > sensor_threshold[i]) {
                if (sensor_pin_map_sensor_id[i] == 0x21) {
                  send_log_data_to_server(pin_number, read_value, "MQ2 Gas sensor reached threshold");
                }
                if (sensor_pin_map_sensor_id[i] == 0x20) {
                  send_log_data_to_server(pin_number, read_value, "MQ7 Gas sensor reached threshold");
                }
              }
            }
            if (pin_number == 7) {
              read_value = analogRead(A6);
              if (read_value > sensor_threshold[i]) {
                if (sensor_pin_map_sensor_id[i] == 0x21) {
                  send_log_data_to_server(pin_number, read_value, "MQ2 Gas sensor reached threshold");
                }
                if (sensor_pin_map_sensor_id[i] == 0x20) {
                  send_log_data_to_server(pin_number, read_value, "MQ7 Gas sensor reached threshold");
                }
              }
            }
            if (pin_number == 8) {
              read_value = analogRead(A7);
              if (read_value > sensor_threshold[i]) {
                if (sensor_pin_map_sensor_id[i] == 0x21) {
                  send_log_data_to_server(pin_number, read_value, "MQ2 Gas sensor reached threshold");
                }
                if (sensor_pin_map_sensor_id[i] == 0x20) {
                  send_log_data_to_server(pin_number, read_value, "MQ7 Gas sensor reached threshold");
                }
              }
            }
          }
        }
      }
    }
  }
}

