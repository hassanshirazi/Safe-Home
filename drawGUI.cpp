

#include "drawGUI.h"
#include "global.h"
#include "Arduino.h"

void make_control_panel_interface() {
  drawHome();
}

void initLCD() {
  // Initial TFT  and Touch setup
  myGLCD.InitLCD();
  myGLCD.clrScr();
  myTouch.InitTouch();
  myTouch.setPrecision(PREC_MEDIUM);
  myGLCD.setFont(SmallFont);
  myButtons.setTextFont(SmallFont);

  myGLCD.setFont(SmallFont);
  myGLCD.clrScr();
  myGLCD.fillScr(VGA_BLACK);
}


void drawHome() {
  myButtons.deleteAllButtons();
  myGLCD.setFont(SmallFont);
  myGLCD.clrScr();
  myGLCD.fillScr(VGA_BLACK);
  screen = 1;

  display_images();

  myButtons.setTextFont(BigFont);
  btnProceed = myButtons.addButton(  225,  187, 86,  46, "Menu");
  myButtons.drawButton(btnProceed);
  myButtons.setTextFont(SmallFont);

  // Page Border
  myGLCD.setColor(VGA_TEAL);
  myGLCD.drawRoundRect (1, 1, 319, 239);
  myGLCD.drawRoundRect (2, 2, 318, 238);

  // Alert Box

  myGLCD.setColor(VGA_RED);
  myGLCD.drawRoundRect (9, 7, 310, 74);
  myGLCD.setColor(VGA_WHITE);
  myGLCD.drawRoundRect (8, 6, 311, 75);
  myGLCD.setColor(VGA_RED);
  myGLCD.drawRoundRect (7, 5, 312, 76);
  myGLCD.setColor(VGA_YELLOW);
  myGLCD.print("Alert Notifications", 105, 10);

  // Connection Status
  myGLCD.setColor(VGA_WHITE);
  myGLCD.drawRoundRect (129, 78, 310, 183);
  myGLCD.setColor(VGA_YELLOW);
  myGLCD.drawLine(176, 78, 176, 182);
  myGLCD.drawLine(221, 78, 221, 182);
  myGLCD.drawLine(269, 78, 269, 182);
  myGLCD.setColor(VGA_WHITE);

  // Device status Box

  myGLCD.setColor(VGA_NAVY);
  myGLCD.drawRoundRect (7, 187, 216, 233);
  myGLCD.setColor(VGA_LIME);
  myGLCD.drawRoundRect (8, 188, 215, 232);
  myGLCD.setColor(VGA_WHITE);
  myGLCD.drawRoundRect (9, 189, 214, 231);
  //myGLCD.print("Device Status", 15, 190);

  update_status(1);
  update_status(2);
  if (ethernet_connection_status == 0) {
    display_tick_cross(3, 1);
    update_status(3);
  }
  if (ethernet_connection_status == 1) {
    display_tick_cross(3, 2);
    update_status(4);
  }
  display_tick_cross(1, 1);
  display_tick_cross(2, 1);
  display_tick_cross(4, 2);
  get_armdisarm_settings_from_sd(2);
}



void draw_keypad() {
  myButtons.deleteAllButtons();

  myGLCD.setFont(BigFont);
  myGLCD.clrScr();
  myGLCD.fillScr(VGA_BLACK);
  screen = 2;
  myGLCD.setColor(VGA_WHITE);
  myGLCD.drawRoundRect (9, 7, 310, 74);

  myGLCD.setColor(VGA_LIME);
  myGLCD.drawRoundRect (1, 1, 319, 239);
  myGLCD.setColor(VGA_BLUE);
  myGLCD.drawRoundRect (2, 2, 318, 238);
  myGLCD.setColor(VGA_LIME);
  myGLCD.drawRoundRect (3, 3, 317, 237);

  myGLCD.setColor(VGA_YELLOW);
  myGLCD.print("Enter Pin Code", CENTER, 23);

  myButtons.setTextFont(BigFont);
  btn1 = myButtons.addButton(  45,  85, 44,  39, "1");
  btn2 = myButtons.addButton(  101,  85, 44,  39, "2");
  btn3 = myButtons.addButton(  158,  85, 44,  39, "3");
  btn4 = myButtons.addButton(  45,  134, 44,  39, "4");
  btn5 = myButtons.addButton(  101,  134, 44,  39, "5");
  btn6 = myButtons.addButton(  158,  134, 44,  39, "6");
  btn7 = myButtons.addButton(  45,  183, 44,  39, "7");
  btn8 = myButtons.addButton(  101,  183, 44,  39, "8");
  btn9 = myButtons.addButton(  158,  183, 44,  39, "9");
  btnExit = myButtons.addButton(  213,  183, 67,  39, "Exit");
  btnClear = myButtons.addButton(  213,  134, 67,  39, "Clear");
  btnEnter = myButtons.addButton(  213,  85, 67,  39, "Enter");
  myButtons.drawButton(btn1);
  myButtons.drawButton(btn2);
  myButtons.drawButton(btn3);
  myButtons.drawButton(btn4);
  myButtons.drawButton(btn5);
  myButtons.drawButton(btn6);
  myButtons.drawButton(btn7);
  myButtons.drawButton(btn8);
  myButtons.drawButton(btn9);
  myButtons.setTextFont(SmallFont);
  myButtons.drawButton(btnExit);
  myButtons.drawButton(btnEnter);
  myButtons.drawButton(btnClear);
}

void display_arrows() {
  word res;
  long sm, em;

  myButtons.setButtonColors(VGA_WHITE, VGA_LIME, VGA_WHITE, VGA_WHITE, VGA_RED);
  btnLeftArrow = myButtons.addButton(  113,  191, 46,  32, "");
  btnRightArrow = myButtons.addButton(  168,  191, 46,  32, "");
  myButtons.drawButton(btnLeftArrow);
  myButtons.drawButton(btnRightArrow);

  digitalWrite(chip_select_sd, LOW);
  digitalWrite(chip_select_ethernet, HIGH);
  digitalWrite(chip_select_wifi, HIGH);

  file.setSSpin(4);
  file.initFAT();

  myGLCD.setColor(255, 255, 255);
  sm = millis();
  res = myFiles.loadBitmap(114, 192, 45, 31, "left.RAW");       // 110*84 pixels
  res = myFiles.loadBitmap(169, 192, 45, 31, "right.RAW");      // 40*16 pixels
  em = millis();

  digitalWrite(chip_select_sd, HIGH);
  digitalWrite(chip_select_ethernet, LOW);
  digitalWrite(chip_select_wifi, HIGH);

  myButtons.setButtonColors(VGA_WHITE, VGA_LIME, VGA_WHITE, VGA_RED, VGA_BLUE);
  myGLCD.setColor(VGA_WHITE);
}


void draw_log_screen() {
  myButtons.deleteAllButtons();
  myGLCD.setFont(BigFont);
  myGLCD.clrScr();
  myGLCD.fillScr(VGA_BLACK);

  myGLCD.setColor(VGA_GREEN);
  myGLCD.drawRoundRect (1, 1, 319, 239);
  myGLCD.setColor(VGA_BLUE);
  myGLCD.drawRoundRect (2, 2, 318, 238);
  myGLCD.setColor(VGA_GREEN);
  myGLCD.drawRoundRect (3, 3, 317, 237);
  myGLCD.setColor(VGA_LIME);
  myGLCD.drawRoundRect (4, 4, 316, 236);

  myGLCD.print("ALARM LOG", CENTER, 20);
  myButtons.setTextFont(BigFont);
  btnBackLog = myButtons.addButton( 220,  185, 85,  47, "Back");
  myButtons.drawButton(btnBackLog);
  myButtons.setTextFont(SmallFont);
  myGLCD.setFont(SmallFont);
  btnDeleteLog = myButtons.addButton( 15,  185, 90,  47, "Delete Log");
  myButtons.drawButton(btnDeleteLog);


  btnDetails1 = myButtons.addButton( 220,  75, 70,  25, "Details");
  btnDetails2 = myButtons.addButton( 220,  110, 70,  25, "Details");
  btnDetails3 = myButtons.addButton( 220,  145, 70,  25, "Details");
  myButtons.drawButton(btnDetails1);
  myButtons.drawButton(btnDetails2);
  myButtons.drawButton(btnDetails3);


  myGLCD.setColor(VGA_TEAL);
  myGLCD.drawRoundRect (15, 42, 305, 175);

  myGLCD.setColor(VGA_LIME);
  myGLCD.print("Alarm Name", 20, 50);
  myGLCD.print("Zone", CENTER, 50);
  myGLCD.print("Details", 225, 50);

  myGLCD.setColor(VGA_GREEN);
  myGLCD.drawLine(117, 42, 117, 175);
  myGLCD.drawLine(206, 42, 206, 175);

  myGLCD.drawLine(15, 71, 305, 71);
  myGLCD.drawLine(15, 105, 305, 105);
  myGLCD.drawLine(15, 140, 305, 140);


  display_arrows();
  screen = 5;
}

void draw_arm_disarm_screen() {
  myButtons.deleteAllButtons();
  myGLCD.setFont(BigFont);
  myGLCD.clrScr();
  myGLCD.fillScr(VGA_BLACK);

  myGLCD.setColor(VGA_GREEN);
  myGLCD.drawRoundRect (1, 1, 319, 239);
  myGLCD.setColor(VGA_BLUE);
  myGLCD.drawRoundRect (2, 2, 318, 238);
  myGLCD.setColor(VGA_GREEN);
  myGLCD.drawRoundRect (3, 3, 317, 237);
  myGLCD.setColor(VGA_LIME);
  myGLCD.drawRoundRect (4, 4, 316, 236);

  myGLCD.print("ARM/DISARM", CENTER, 20);
  myButtons.setTextFont(BigFont);
  btnArmDisarmBack = myButtons.addButton( 220,  185, 85,  47, "Back");
  myButtons.drawButton(btnArmDisarmBack);
  myButtons.setTextFont(SmallFont);
  btnArm = myButtons.addButton( 45,  62, 100,  47, "ARM");
  btnDisarm = myButtons.addButton( 170,  62, 100,  47, "DISARM");
  myButtons.drawButton(btnDisarm);
  myButtons.drawButton(btnArm);
  myButtons.setTextFont(SmallFont);
  get_armdisarm_settings_from_sd(1);
  screen = 7;
}

void get_armdisarm_settings_from_sd(int flag) {
  digitalWrite(chip_select_sd, LOW);
  digitalWrite(chip_select_ethernet, HIGH);
  digitalWrite(chip_select_wifi, HIGH);
  byte read_bool;
  File read_arm_disarm;
  read_arm_disarm = SD.open("arm.txt");
  if (read_arm_disarm) {
    while (read_arm_disarm.available()) {
      read_bool = read_arm_disarm.read();
      if (flag == 1) {
        if (read_bool == 0x31) {
          myButtons.setButtonColors(VGA_BLACK, VGA_LIME, VGA_WHITE, VGA_RED, VGA_LIME);
          myButtons.relabelButton(btnArm, "ARMED", true);
          myButtons.setButtonColors(VGA_WHITE, VGA_LIME, VGA_WHITE, VGA_RED, VGA_BLUE);
          myButtons.relabelButton(btnDisarm, "DISARM", true);
          myGLCD.setColor(VGA_LIME);
          myGLCD.print("                  ", CENTER, 130);
          myGLCD.print("System is Armed", CENTER, 130);
          myGLCD.setColor(VGA_WHITE);
        }
        if (read_bool == 0x30) {
          myButtons.setButtonColors(VGA_WHITE, VGA_LIME, VGA_WHITE, VGA_RED, VGA_RED);
          myButtons.relabelButton(btnDisarm, "DISARMED", true);
          myButtons.setButtonColors(VGA_WHITE, VGA_LIME, VGA_WHITE, VGA_RED, VGA_BLUE);
          myButtons.relabelButton(btnArm, "ARM", true);
          myGLCD.setColor(VGA_RED);
          myGLCD.print("                  ", CENTER, 130);
          myGLCD.print("System is DisArmed", CENTER, 130);
          myGLCD.setColor(VGA_WHITE);
        }
      }
      if (flag == 2) {
        if (read_bool == 0x31) {
          myGLCD.setFont(BigFont);
          myGLCD.setColor(VGA_LIME);
          myGLCD.print("ARMED", 70, 200);
          myGLCD.setFont(SmallFont);
        }
        if (read_bool == 0x30) {
          myGLCD.setFont(BigFont);
          myGLCD.setColor(VGA_RED);
          myGLCD.print("DISARMED", 50, 200);
          myGLCD.setFont(SmallFont);
        }
      }
    }
    read_arm_disarm.close();
  }
  device_arm_disarm_status = (char)read_bool;
  digitalWrite(chip_select_sd, HIGH);
  digitalWrite(chip_select_ethernet, LOW);
  digitalWrite(chip_select_wifi, HIGH);
}


void draw_test_status_screen() {
  myButtons.deleteAllButtons();
  myGLCD.setFont(BigFont);
  myGLCD.clrScr();
  myGLCD.fillScr(VGA_BLACK);

  myGLCD.setColor(VGA_GREEN);
  myGLCD.drawRoundRect (1, 1, 319, 239);
  myGLCD.setColor(VGA_BLUE);
  myGLCD.drawRoundRect (2, 2, 318, 238);
  myGLCD.setColor(VGA_GREEN);
  myGLCD.drawRoundRect (3, 3, 317, 237);
  myGLCD.setColor(VGA_LIME);
  myGLCD.drawRoundRect (4, 4, 316, 236);

  myGLCD.print("Test Status", CENTER, 20);
  myButtons.setTextFont(BigFont);
  btnTestStatusBack = myButtons.addButton( 220,  185, 85,  47, "Back");
  myButtons.drawButton(btnTestStatusBack);
  myButtons.setTextFont(SmallFont);

}
void draw_sensor_list() {
  myButtons.deleteAllButtons();
  myGLCD.setFont(BigFont);
  myGLCD.clrScr();
  myGLCD.fillScr(VGA_BLACK);

  myGLCD.setColor(VGA_GREEN);
  myGLCD.drawRoundRect (1, 1, 319, 239);
  myGLCD.setColor(VGA_BLUE);
  myGLCD.drawRoundRect (2, 2, 318, 238);
  myGLCD.setColor(VGA_GREEN);
  myGLCD.drawRoundRect (3, 3, 317, 237);
  myGLCD.setColor(VGA_LIME);
  myGLCD.drawRoundRect (4, 4, 316, 236);

  myGLCD.print("SENSORS", CENTER, 20);
  myButtons.setTextFont(BigFont);
  btnBack = myButtons.addButton( 220,  185, 85,  47, "Back");
  myButtons.drawButton(btnBack);
  btnSave = myButtons.addButton( 15,  185, 90,  47, "Save");
  myButtons.drawButton(btnSave);

  myButtons.setTextFont(SmallFont);
  myGLCD.setFont(SmallFont);

  myGLCD.setColor(VGA_TEAL);
  myGLCD.drawRoundRect (15, 42, 305, 175);

  myGLCD.setColor(VGA_LIME);
  myGLCD.print("Sensor Name", 20, 50);
  myGLCD.print("Zone", CENTER, 50);
  myGLCD.print("Arm/Disarm", 215, 50);

  myGLCD.setColor(VGA_GREEN);
  myGLCD.drawLine(117, 42, 117, 175);
  myGLCD.drawLine(206, 42, 206, 175);

  myGLCD.drawLine(15, 71, 305, 71);
  myGLCD.drawLine(15, 105, 305, 105);
  myGLCD.drawLine(15, 140, 305, 140);


  display_arrows();

  screen = 4;
}

void draw_main_screen() {
  myButtons.deleteAllButtons();
  myGLCD.setFont(BigFont);
  myGLCD.clrScr();
  myGLCD.fillScr(VGA_BLACK);

  myGLCD.setColor(VGA_GREEN);
  myGLCD.drawRoundRect (1, 1, 319, 239);
  myGLCD.setColor(VGA_BLUE);
  myGLCD.drawRoundRect (2, 2, 318, 238);
  myGLCD.setColor(VGA_GREEN);
  myGLCD.drawRoundRect (3, 3, 317, 237);
  myGLCD.setColor(VGA_YELLOW);
  myGLCD.drawRoundRect (4, 4, 316, 236);

  myGLCD.print("SETUP", 230, 20);
  btnArmDisarm = myButtons.addButton(  9,  16, 100,  47, "Arm/Disarm");
  btnAlarmLog = myButtons.addButton(  9,  79, 100,  47, "Alarm Log");
  btnTestStatus = myButtons.addButton(  119,  16, 100,  47, "Test Status");
  btnSensorList = myButtons.addButton(  119,  76, 100,  47, "Sensor List");
  btnAddNewSensor = myButtons.addButton(  9,  141, 100,  47, "ADD Sensor");


  myButtons.drawButton(btnArmDisarm);
  myButtons.drawButton(btnAlarmLog);
  myButtons.drawButton(btnTestStatus);
  myButtons.drawButton(btnSensorList);
  myButtons.drawButton(btnAddNewSensor);

  myButtons.setTextFont(BigFont);
  btnHome = myButtons.addButton(  220,  176, 85,  47, "Home");
  myButtons.drawButton(btnHome);
  myButtons.setTextFont(SmallFont);
  screen = 3;
}

void updateStr(int val)
{
  if (stCurrentLen < 4)
  {
    myGLCD.print("              ", CENTER, 23);
    myGLCD.print("           ", CENTER, 20);
    myGLCD.print("               ", CENTER, 40);
    stCurrent[stCurrentLen] = val;
    stCurrent[stCurrentLen + 1] = '\0';
    stCurrentLen++;
    myGLCD.setColor(0, 255, 0);
    myGLCD.print(stCurrent, CENTER, 23);
  }
}


void update_status(int flag) {
  myGLCD.setColor(VGA_WHITE);
  switch (flag) {
    case 1:
      myGLCD.print("                  ", 15, 25);
      myGLCD.print("GSM Down", 15, 25);
      break;
    case 2:
      myGLCD.print("                  ", 15, 40);
      myGLCD.print("WiFi Down", 15, 40);
      break;
    case 3:
      myGLCD.print("                  ", 15, 55);
      myGLCD.print("Ethernet Down", 15, 55);
      break;
    case 4:
      myGLCD.print("                  ", 15, 55);
      myGLCD.print("Ethernet Connected", 15, 55);
      break;
    case 5:
      myGLCD.print("                  ", 15, 40);
      myGLCD.print("WiFi Connected", 15, 40);
      break;
    case 6:
      myGLCD.print("                  ", 15, 25);
      myGLCD.print("GSM Connected", 15, 25);
      break;

      //    case 4:
      //      myGLCD.setFont(BigFont);
      //      myGLCD.print("Armed", 50, 210);
      //      myGLCD.setFont(SmallFont);
      //      break;
      //    case 5:
      //      myGLCD.setFont(BigFont);
      //      myGLCD.print("Disarmed", 50, 210);
      //      myGLCD.setFont(SmallFont);
      break;
  }
}


void display_images() {
  word res;
  long sm, em;

  digitalWrite(chip_select_sd, LOW);
  digitalWrite(chip_select_ethernet, HIGH);
  digitalWrite(chip_select_wifi, HIGH);

  file.setSSpin(4);
  file.initFAT();

  myGLCD.setColor(255, 255, 255);
  sm = millis();
  res = myFiles.loadBitmap(9, 89, 110, 84, "logo.RAW");       // 110*84 pixels
  res = myFiles.loadBitmap(133, 102, 40, 16, "gprs.RAW");      // 40*16 pixels
  res = myFiles.loadBitmap(180, 100, 40, 23, "wifi.RAW");      // 40*15 pixels
  res = myFiles.loadBitmap(225, 98, 40, 20, "ethernet.RAW");  // 40*20 pixels
  res = myFiles.loadBitmap(268, 93, 40, 33, "battery.RAW");   // 40*33 pixels
  em = millis();

  digitalWrite(chip_select_sd, HIGH);
  digitalWrite(chip_select_ethernet, LOW);
  digitalWrite(chip_select_wifi, HIGH);

}


void display_tick_cross(int loc, int flag) {
  word res;
  long sm, em;

  digitalWrite(chip_select_sd, LOW);
  digitalWrite(chip_select_ethernet, HIGH);
  digitalWrite(chip_select_wifi, HIGH);

  file.setSSpin(4);
  file.initFAT();

  myGLCD.setColor(255, 255, 255);
  sm = millis();

  // 1 Means Tick
  if (flag == 1) {
    switch (loc) {
      case 1: // GPRS
        res = myFiles.loadBitmap(138, 135, 25, 22, "cross.RAW");      // 25*17 pixels
        break;

      case 2: // Wifi
        res = myFiles.loadBitmap(187, 135, 25, 22, "cross.RAW");      // 25*17 pixels
        break;

      case 3: // Ethernet
        res = myFiles.loadBitmap(232, 135, 25, 22, "cross.RAW");      // 25*17 pixels
        break;

      case 4: // Battery
        res = myFiles.loadBitmap(275, 135, 25, 22, "cross.RAW");      // 25*17 pixels
        break;
    }
  }

  // 2 Means Tick
  if (flag == 2) {
    switch (loc) {
      case 1: // GPRS
        res = myFiles.loadBitmap(138, 135, 25, 21, "tick.RAW");      // 25*17 pixels
        break;

      case 2: // Wifi
        res = myFiles.loadBitmap(187, 135, 25, 21, "tick.RAW");      // 25*17 pixels
        break;

      case 3: // Ethernet
        res = myFiles.loadBitmap(232, 135, 25, 21, "tick.RAW");      // 25*17 pixels
        break;

      case 4: // Battery
        res = myFiles.loadBitmap(275, 135, 25, 21, "tick.RAW");      // 25*17 pixels
        break;
    }
  }
}

void draw_add_new_sensor_screen(){
  myButtons.deleteAllButtons();
  myGLCD.setFont(BigFont);
  myGLCD.clrScr();
  myGLCD.fillScr(VGA_BLACK);

  myGLCD.setColor(VGA_GREEN);
  myGLCD.drawRoundRect (1, 1, 319, 239);
  myGLCD.setColor(VGA_BLUE);
  myGLCD.drawRoundRect (2, 2, 318, 238);
  myGLCD.setColor(VGA_GREEN);
  myGLCD.drawRoundRect (3, 3, 317, 237);
  myGLCD.setColor(VGA_LIME);
  myGLCD.drawRoundRect (4, 4, 316, 236);

  myGLCD.print("Add Sensor", CENTER, 20);
  myButtons.setTextFont(BigFont);
  btnAddSensorBack = myButtons.addButton( 220,  185, 85,  47, "Back");
  myButtons.drawButton(btnAddSensorBack);  
  myButtons.setTextFont(SmallFont);

  btnadd_pir = myButtons.addButton(15,56,81,35, "PIR");
  btnadd_smoke = myButtons.addButton(105,56,81,35, "Smoke");
  btnadd_ir = myButtons.addButton(194,56,81,35, "IR");
  btnadd_door = myButtons.addButton(15,97,81,35, "Door");
  btnadd_glass = myButtons.addButton(105,97,81,35, "Glass");
  btnadd_mq2 = myButtons.addButton(194,97,81,35, "MQ2");
  btnadd_mq7 = myButtons.addButton(15,141,81,35, "MQ7");
  btnadd_panic = myButtons.addButton(105,141,81,35, "Panic");

  myButtons.drawButton(btnadd_pir);  
  myButtons.drawButton(btnadd_smoke);  
  myButtons.drawButton(btnadd_ir);  
  myButtons.drawButton(btnadd_door);  
  myButtons.drawButton(btnadd_glass);  
  myButtons.drawButton(btnadd_mq2);  
  myButtons.drawButton(btnadd_mq7);  
  myButtons.drawButton(btnadd_panic);  

  
}



