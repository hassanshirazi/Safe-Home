#include "global.h"
#include "Arduino.h"
#include "commandsToSD.h"

 RCSwitch mySwitch = RCSwitch();
 ShiftRegister74HC595 sr (1, 13, 12, 14); 

// Configuration vaiables with default values to be used only once at first power up
String mqtt_ip_address;
String mqtt_port_number;
String mqtt_username;
String mqtt_password;
String wifi_internet_ssid;
String wifi_internet_password;
String phone_number1;
String phone_number2;
String phone_number3;

int mqtt_values_changed=0;

#define ADAFRUIT_CC3000_IRQ   3  // MUST be an interrupt pin!
#define ADAFRUIT_CC3000_VBAT  5
#define ADAFRUIT_CC3000_CS    10

// Create an Ethernet client using Ethernet Shield
EthernetClient eth_client;

// Create an MQTT Client for Ethernet Shield
PubSubClient mqtt_client(eth_client);


byte ethernet_mac_address[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

String device_id="DE-AD-BE-EF-ED";


// Create instance of TFT LCD
UTFT myGLCD(SSD1289, 38, 39, 40, 41);

// Create instance of Touch Panel
UTouch myTouch(42, 43, 44, 45, 46);

UTFT_tinyFAT myFiles(&myGLCD);
UTFT_Buttons myButtons(&myGLCD, &myTouch);



int add_pir_sensor=0;
int add_smoke_sensor=0;
int add_ir_sensor=0;
int add_door_sensor=0;
int add_glass_sensor=0;
int add_mq2_sensor=0;
int add_mq7_sensor=0;
int add_panic_sensor=0;


int btnadd_pir=0;
int btnadd_smoke=0;
int btnadd_ir=0;
int btnadd_door=0;
int btnadd_glass=0;
int btnadd_mq2=0;
int btnadd_mq7=0;
int btnadd_panic=0;

int btn1, btn2, btn3, btn4, btn5,  btn6, btn7, btn8, btn9;
int btnProceed, btnEnter, btnExit, btnClear;
int btnTestStatus, btnSensorList, btnAlarmLog, btnHome, btnArmDisarm, btnAddNewSensor;
int btnBack, btnSave;
int btnBackLog, btnDeleteLog;
int btnBackTestStatus;
int btnAddSensorBack;
int btnSearchSensors;
int btnBackArmDisarm;
int btnTestStatusBack, btnTestStatusSave;
int btnArmDisarmBack, btnArmDisarmSave, btnArm, btnDisarm;
int pressed_button;
int btnLeftArrow, btnRightArrow;
int btnDetails1, btnDetails2, btnDetails3;

int home_screen;
int screen = 1;
// Touch variables
int x, y;
char stCurrent[7] = "";
int stCurrentLen = 0;
char pin_code[7] = "1234";

unsigned long sd_card_log_index = 0;

// Connected Sensor's Pin map and Sensor ID
int index_of_sensor_array = 0;
byte sensor_pin_map_sensor_id[16];
int sensor_pin_map_sensor_pin[16];
byte sensor_armed_disarmed[16];
int sensor_threshold[16];

int rtc_year;
int rtc_month;
int rtc_day;
int rtc_hour;
int rtc_minute;
int rtc_second;

int sensor_armed_disarmed_status = 0;

char device_arm_disarm_status='0';

int ethernet_connection_status=0;



