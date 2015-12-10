#include "global.h"
#include "Arduino.h"
#include "drawGUI.h"


void read_touch_buttons() {

  /*
  * Home Screen Touch Events
  */
  if (screen == 1) {
    pressed_button = myButtons.checkButtons();
    if (pressed_button == btnProceed) {
      digitalWrite(buzzer, HIGH);
      delay(5);
      digitalWrite(buzzer, LOW);
      screen = 2;
      stCurrent[0] = '\0';
      stCurrentLen = 0;
      draw_keypad();
    }
  }

  /*
  * Keypad Screen Touch Events
  */
  if (screen == 2) {
    pressed_button = myButtons.checkButtons();
    if (pressed_button == btnExit) {
      digitalWrite(buzzer, HIGH);
      delay(5);
      digitalWrite(buzzer, LOW);
      screen = 1;
      stCurrent[0] = '\0';
      stCurrentLen = 0;
      drawHome();
    }
    if (pressed_button == btnClear) {
      digitalWrite(buzzer, HIGH);
      delay(5);
      digitalWrite(buzzer, LOW);
      myGLCD.print("              ", CENTER, 23);
      myGLCD.print("           ", CENTER, 20);
      myGLCD.print("               ", CENTER, 40);
      if (stCurrentLen > 0)
      {
        stCurrentLen--;
        stCurrent[stCurrentLen] = '\0';
      }
      myGLCD.setColor(0, 255, 0);
      myGLCD.print(stCurrent, CENTER, 23);
    }
    if (pressed_button == btnEnter) {
      digitalWrite(buzzer, HIGH);
      delay(5);
      digitalWrite(buzzer, LOW);
      myGLCD.print("              ", CENTER, 23);
      myGLCD.print("Processing", CENTER, 20);
      myGLCD.print("Please Wait...", CENTER, 40);
      delay(1500);
      if (strcmp(stCurrent, pin_code) == 0) {
        myGLCD.print("              ", CENTER, 23);
        myGLCD.print("           ", CENTER, 20);
        myGLCD.print("               ", CENTER, 40);
        myGLCD.print("PASSED", CENTER, 20);
        delay(500);
        stCurrent[0] = '\0';
        stCurrentLen = 0;
        draw_main_screen();
      }
      else {
        myGLCD.print("              ", CENTER, 23);
        myGLCD.print("           ", CENTER, 20);
        myGLCD.print("               ", CENTER, 40);
        myGLCD.setColor(VGA_RED);
        myGLCD.print("Failed", CENTER, 20);
        myGLCD.print("Try Again", CENTER, 40);
        myGLCD.setColor(VGA_GREEN);
        delay(1500);
        myGLCD.print("              ", CENTER, 23);
        myGLCD.print("           ", CENTER, 20);
        myGLCD.print("               ", CENTER, 40);
        myGLCD.print("Enter Pin Code", CENTER, 23);
        stCurrent[0] = '\0';
        stCurrentLen = 0;

      }
    }
    if (pressed_button == btn1) {
      updateStr('1');
      digitalWrite(buzzer, HIGH);
      delay(5);
      digitalWrite(buzzer, LOW);
    }
    if (pressed_button == btn2) {
      updateStr('2');
      digitalWrite(buzzer, HIGH);
      delay(5);
      digitalWrite(buzzer, LOW);
    }
    if (pressed_button == btn3) {
      updateStr('3');
      digitalWrite(buzzer, HIGH);
      delay(5);
      digitalWrite(buzzer, LOW);
    }
    if (pressed_button == btn4) {
      updateStr('4');
      digitalWrite(buzzer, HIGH);
      delay(5);
      digitalWrite(buzzer, LOW);
    }
    if (pressed_button == btn5) {
      updateStr('5');
      digitalWrite(buzzer, HIGH);
      delay(5);
      digitalWrite(buzzer, LOW);
    }
    if (pressed_button == btn6) {
      updateStr('6');
      digitalWrite(buzzer, HIGH);
      delay(5);
      digitalWrite(buzzer, LOW);
    }
    if (pressed_button == btn7) {
      updateStr('7');
      digitalWrite(buzzer, HIGH);
      delay(5);
      digitalWrite(buzzer, LOW);
    }
    if (pressed_button == btn8) {
      updateStr('8');
      digitalWrite(buzzer, HIGH);
      delay(5);
      digitalWrite(buzzer, LOW);
    }
    if (pressed_button == btn9) {
      updateStr('9');
      digitalWrite(buzzer, HIGH);
      delay(5);
      digitalWrite(buzzer, LOW);
    }
  }


  /*
   * Menu Screen Touch Events
   */
  if (screen == 3) {
    pressed_button = myButtons.checkButtons();
    if (pressed_button == btnHome) {
      digitalWrite(buzzer, HIGH);
      delay(5);
      digitalWrite(buzzer, LOW);
      screen = 1;
      drawHome();
    }
    if (pressed_button == btnSensorList) {
      digitalWrite(buzzer, HIGH);
      delay(5);
      digitalWrite(buzzer, LOW);
      draw_sensor_list();
      screen = 4;
    }
    if (pressed_button == btnAlarmLog) {
      digitalWrite(buzzer, HIGH);
      delay(5);
      digitalWrite(buzzer, LOW);
      draw_log_screen();
      screen = 5;
    }
    if (pressed_button == btnTestStatus) {
      digitalWrite(buzzer, HIGH);
      delay(5);
      digitalWrite(buzzer, LOW);
      draw_test_status_screen();
      screen = 6;
    }
    if (pressed_button == btnArmDisarm) {
      digitalWrite(buzzer, HIGH);
      delay(5);
      digitalWrite(buzzer, LOW);
      draw_arm_disarm_screen();
      screen = 7;
    }
    if (pressed_button == btnAddNewSensor) {
      digitalWrite(buzzer, HIGH);
      delay(5);
      digitalWrite(buzzer, LOW);
      draw_add_new_sensor_screen();
      screen = 8;
    }
  }


  /*
  * Sensor List Screen Touch Events
  */
  if (screen == 4) {
    pressed_button = myButtons.checkButtons();
    if (pressed_button == btnBack) {
      digitalWrite(buzzer, HIGH);
      delay(5);
      digitalWrite(buzzer, LOW);
      draw_main_screen();
      screen = 3;
    }
  }

  /*
   * Alarm Log Screen Touch Events
   */
  if (screen == 5) {
    pressed_button = myButtons.checkButtons();
    if (pressed_button == btnBackLog) {
      digitalWrite(buzzer, HIGH);
      delay(5);
      digitalWrite(buzzer, LOW);
      draw_main_screen();
      screen = 3;
    }
  }

  /*
   * Test Status Screen Touch Events
   */
  if (screen == 6) {
    pressed_button = myButtons.checkButtons();
    if (pressed_button == btnBackTestStatus) {
      digitalWrite(buzzer, HIGH);
      delay(5);
      digitalWrite(buzzer, LOW);
      draw_main_screen();
      screen = 3;
    }
  }


  /*
   * Arm/Disarm Screen Touch Events
   */
  if (screen == 7) {
    digitalWrite(chip_select_sd, LOW);
    digitalWrite(chip_select_ethernet, HIGH);
    digitalWrite(chip_select_wifi, HIGH);
    pressed_button = myButtons.checkButtons();
    if (pressed_button == btnBackArmDisarm) {
      digitalWrite(buzzer, HIGH);
      delay(5);
      digitalWrite(buzzer, LOW);
      draw_main_screen();
      screen = 3;
    }
    if (pressed_button == btnArm) {
      digitalWrite(buzzer, HIGH);
      delay(5);
      digitalWrite(buzzer, LOW);
      SD.remove("arm.txt");
      myButtons.setButtonColors(VGA_BLACK, VGA_LIME, VGA_WHITE, VGA_RED, VGA_LIME);
      myButtons.relabelButton(btnArm, "ARMED", true);
      myButtons.setButtonColors(VGA_WHITE, VGA_LIME, VGA_WHITE, VGA_RED, VGA_BLUE);
      myButtons.relabelButton(btnDisarm, "DISARM", true);
      myGLCD.setColor(VGA_LIME);
      myGLCD.print("                  ", CENTER, 130);
      myGLCD.print("System is Armed", CENTER, 130);
      myGLCD.setColor(VGA_WHITE);
      File write_armed;
      write_armed = SD.open("arm.txt", FILE_WRITE);
      write_armed.print(1);
      write_armed.close();
    }
    if (pressed_button == btnDisarm) {
      digitalWrite(buzzer, HIGH);
      delay(5);
      digitalWrite(buzzer, LOW);
      SD.remove("arm.txt");
      myButtons.setButtonColors(VGA_WHITE, VGA_LIME, VGA_WHITE, VGA_RED, VGA_RED);
      myButtons.relabelButton(btnDisarm, "DISARMED", true);
      myButtons.setButtonColors(VGA_WHITE, VGA_LIME, VGA_WHITE, VGA_RED, VGA_BLUE);
      myButtons.relabelButton(btnArm, "ARM", true);
      myGLCD.setColor(VGA_RED);
      myGLCD.print("                  ", CENTER, 130);
      myGLCD.print("System is DisArmed", CENTER, 130);
      myGLCD.setColor(VGA_WHITE);
      File write_disarmed;
      write_disarmed = SD.open("arm.txt", FILE_WRITE);
      write_disarmed.print(0);
      write_disarmed.close();
    }
  }

  if (screen == 8) {

    pressed_button = myButtons.checkButtons();
    if (pressed_button == btnAddSensorBack) {
      digitalWrite(buzzer, HIGH);
      delay(5);
      digitalWrite(buzzer, LOW);
      myGLCD.print("                       ", 15 , 200);
      add_pir_sensor = 0;
      add_smoke_sensor = 0;
      add_ir_sensor = 0;
      add_door_sensor = 0;
      add_glass_sensor = 0;
      add_mq2_sensor = 0;
      add_mq7_sensor = 0;
      add_panic_sensor = 0;
      draw_main_screen();
      screen = 3;
    }
    if (pressed_button == btnadd_pir) {
      digitalWrite(buzzer, HIGH);
      delay(5);
      digitalWrite(buzzer, LOW);
      add_pir_sensor = 1;
      add_smoke_sensor = 0;
      add_ir_sensor = 0;
      add_door_sensor = 0;
      add_glass_sensor = 0;
      add_mq2_sensor = 0;
      add_mq7_sensor = 0;
      add_panic_sensor = 0;
      myGLCD.setFont(SmallFont);
      myGLCD.print("                       ", 15 , 200);
      myGLCD.print("Searching PIR Sensors", 15 , 200);
    }
    if (pressed_button == btnadd_smoke) {
      digitalWrite(buzzer, HIGH);
      delay(5);
      digitalWrite(buzzer, LOW);
      add_pir_sensor = 0;
      add_smoke_sensor = 1;
      add_ir_sensor = 0;
      add_door_sensor = 0;
      add_glass_sensor = 0;
      add_mq2_sensor = 0;
      add_mq7_sensor = 0;
      add_panic_sensor = 0;
      myGLCD.setFont(SmallFont);
      myGLCD.print("                       ", 15 , 200);
      myGLCD.print("Searching Smoke Sensors", 15 , 200);
    }
    if (pressed_button == btnadd_ir) {
      digitalWrite(buzzer, HIGH);
      delay(5);
      digitalWrite(buzzer, LOW);
      add_pir_sensor = 0;
      add_smoke_sensor = 0;
      add_ir_sensor = 1;
      add_door_sensor = 0;
      add_glass_sensor = 0;
      add_mq2_sensor = 0;
      add_mq7_sensor = 0;
      add_panic_sensor = 0;
      myGLCD.setFont(SmallFont);
      myGLCD.print("                       ", 15 , 200);
      myGLCD.print("Searching IR Sensors", 15 , 200);
    }
    if (pressed_button == btnadd_door) {
      digitalWrite(buzzer, HIGH);
      delay(5);
      digitalWrite(buzzer, LOW);
      add_pir_sensor = 0;
      add_smoke_sensor = 0;
      add_ir_sensor = 0;
      add_door_sensor = 1;
      add_glass_sensor = 0;
      add_mq2_sensor = 0;
      add_mq7_sensor = 0;
      add_panic_sensor = 0;
      myGLCD.setFont(SmallFont);
      myGLCD.print("                       ", 15 , 200);
      myGLCD.print("Searching Door Sensors", 15 , 200);
    }
    if (pressed_button == btnadd_glass) {
      digitalWrite(buzzer, HIGH);
      delay(5);
      digitalWrite(buzzer, LOW);
      add_pir_sensor = 0;
      add_smoke_sensor = 0;
      add_ir_sensor = 0;
      add_door_sensor = 0;
      add_glass_sensor = 1;
      add_mq2_sensor = 0;
      add_mq7_sensor = 0;
      add_panic_sensor = 0;
      myGLCD.setFont(SmallFont);
      myGLCD.print("                       ", 15 , 200);
      myGLCD.print("Searching Glass Sensors", 15 , 200);
    }
    if (pressed_button == btnadd_mq2) {
      digitalWrite(buzzer, HIGH);
      delay(5);
      digitalWrite(buzzer, LOW);
      add_pir_sensor = 0;
      add_smoke_sensor = 0;
      add_ir_sensor = 0;
      add_door_sensor = 0;
      add_glass_sensor = 0;
      add_mq2_sensor = 1;
      add_mq7_sensor = 0;
      add_panic_sensor = 0;
      myGLCD.setFont(SmallFont);
      myGLCD.print("                       ", 15 , 200);
      myGLCD.print("Searching MQ2 Sensors", 15 , 200);
    }
    if (pressed_button == btnadd_mq7) {
      digitalWrite(buzzer, HIGH);
      delay(5);
      digitalWrite(buzzer, LOW);
      add_pir_sensor = 0;
      add_smoke_sensor = 0;
      add_ir_sensor = 0;
      add_door_sensor = 0;
      add_glass_sensor = 0;
      add_mq2_sensor = 0;
      add_mq7_sensor = 1;
      add_panic_sensor = 0;
      myGLCD.setFont(SmallFont);
      myGLCD.print("                       ", 15 , 200);
      myGLCD.print("Searching MQ7 Sensors", 15 , 200);
    }
    if (pressed_button == btnadd_panic) {
      digitalWrite(buzzer, HIGH);
      delay(5);
      digitalWrite(buzzer, LOW);
      add_pir_sensor = 0;
      add_smoke_sensor = 0;
      add_ir_sensor = 0;
      add_door_sensor = 0;
      add_glass_sensor = 0;
      add_mq2_sensor = 0;
      add_mq7_sensor = 0;
      add_panic_sensor = 1;
      myGLCD.setFont(SmallFont);
      myGLCD.print("                       ", 15 , 200);
      myGLCD.print("Searching Panic Buttons", 15 , 200);
    }
  }

  digitalWrite(chip_select_sd, HIGH);
  digitalWrite(chip_select_ethernet, LOW);
  digitalWrite(chip_select_wifi, HIGH);
}

