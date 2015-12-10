

#ifndef GLOBAL
#define GLOBAL

#include "Arduino.h"

#include <RCSwitch.h>
#include "logSensorData.h"

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
#include <ShiftRegister74HC595.h>

void callback(char* topic, byte* payload, unsigned int length);

// Global Definition
#define chip_select_sd 4
#define chip_select_ethernet 10
#define chip_select_wifi 10

extern RCSwitch mySwitch;
extern ShiftRegister74HC595 sr;
// Status Variables
extern int ethernet_connection_status;

// Sensor Arm/Disarm Status
extern int sensor_armed_disarmed_status;

// Device Arm/Disarm
extern char device_arm_disarm_status;

// Configuration vaiables with default values to be used only once at first power up
extern String mqtt_ip_address;
extern String mqtt_port_number;
extern String mqtt_username;
extern String mqtt_password;
extern String phone_number1;
extern String phone_number2;
extern String phone_number3;
extern String wifi_internet_ssid;
extern String wifi_internet_password;

extern int mqtt_values_changed;

extern int add_pir_sensor;
extern int add_smoke_sensor;
extern int add_ir_sensor;
extern int add_door_sensor;
extern int add_glass_sensor;
extern int add_mq2_sensor;
extern int add_mq7_sensor;
extern int add_panic_sensor;


// Create an Ethernet client using Ethernet Shield
extern EthernetClient eth_client;

// Create an MQTT Client for Ethernet Shield
extern PubSubClient mqtt_client;


extern byte ethernet_mac_address[];
extern String device_id;

// Touch Screen variables
extern UTFT myGLCD;
extern UTouch myTouch;
extern UTFT_tinyFAT myFiles;
extern UTFT_Buttons myButtons;

// Touch Screen variables
extern int btn1, btn2, btn3, btn4, btn5,  btn6, btn7, btn8, btn9;
extern int btnProceed, btnEnter, btnExit, btnClear;
extern int btnTestStatus, btnSensorList, btnAlarmLog, btnHome, btnArmDisarm, btnAddNewSensor;
extern int btnBack, btnSave;
extern int btnBackLog, btnDeleteLog;
extern int btnBackTestStatus;
extern int btnBackArmDisarm;
extern int btnAddSensorBack;
extern int btnSearchSensors;
extern int btnTestStatusBack, btnTestStatusSave;
extern int btnArmDisarmBack, btnArmDisarmSave, btnArm, btnDisarm;
extern int pressed_button;
extern int btnLeftArrow, btnRightArrow;
extern int btnDetails1, btnDetails2, btnDetails3;
extern int home_screen;
extern int screen;

extern int btnadd_pir;
extern int btnadd_smoke;
extern int btnadd_ir;
extern int btnadd_door;
extern int btnadd_glass;
extern int btnadd_mq2;
extern int btnadd_mq7;
extern int btnadd_panic;


// Touch variables
extern int x, y;
extern char stCurrent[7];
extern int stCurrentLen;
extern char pin_code[7];

extern unsigned long sd_card_log_index;

// Connected Sensor's Pin map and Sensor ID

extern int index_of_sensor_array;
extern byte sensor_pin_map_sensor_id[16];
extern int sensor_pin_map_sensor_pin[16];
extern byte sensor_armed_disarmed[16];
extern int sensor_threshold[16];

// RTC Time variables
extern int rtc_year;
extern int rtc_month;
extern int rtc_day;
extern int rtc_hour;
extern int rtc_minute;
extern int rtc_second;

#define LED1 6
#define LED2 7
#define LED3 8

#define buzzer 11
#define relay 9
#define modem_on 48
//
//#define Analog0 A0
//#define Analog1 A1
//#define Analog2 A2
//#define Analog3 A3
//#define Analog4 A4
//#define Analog5 A5
//#define Analog6 A6
//#define Analog7 A7
//#define Analog8 A8
//#define Analog9 A9
//#define Analog10 A10
//#define Analog11 A11
//#define Analog12 A12
//#define Analog13 A13
//#define Analog14 A14


#endif
