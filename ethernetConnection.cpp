#include "drawGUI.h"
#include "global.h"
#include "ethernetConnection.h"

/*
 * Function for connecting to internet using Ethernet Shield
 */
bool internet_connection_using_ethernet() {

  Serial.println("Connecting Arduino to network...");
  Serial.println();

  IPAddress ip(192, 168, 1, 73);
  Ethernet.begin(ethernet_mac_address, ip);
  delay(1000);

  //  if (Ethernet.begin(ethernet_mac_address) == 0)
  //  {
  //    Serial.println("DHCP Failed, reset Arduino to try again");
  //    Serial.println();
  //    return false;
  //  }
  //  else
  //  {
  //    Serial.println("Arduino connected to network using DHCP");
  //    Serial.println();
  //
  //    // print your local IP address:
  //    Serial.print("My IP address: ");
  //    for (byte thisByte = 0; thisByte < 4; thisByte++) {
  //      // print the value of each byte of the IP address:
  //      Serial.print(Ethernet.localIP()[thisByte], DEC);
  //      Serial.print(".");
  //    }
  //    //    return true;
  //  }
  //  delay(1500);
  //    delay(1000);
  //  }
}


/*
 * Function to keep trying to connect to brokerr using Ethernet Shield
 */
void reconnect_using_ethernet(String username, String password) {
  char user[username.length() + 1];
  username.toCharArray(user, sizeof(user));
  char pass[password.length() + 1];
  password.toCharArray(pass, sizeof(pass));

  if (internet_connection_using_ethernet()) {
    if (!mqtt_client.connected()) {
      Serial.print("Attempting MQTT connection...");
      // Attempt to connect
      if (mqtt_client.connect(NULL, "etirwufc", "mWP0oLV39Aor")) {
        ethernet_connection_status = 1;
        digitalWrite(LED2, LOW);
        digitalWrite(LED3, HIGH);
        if (screen == 1) {
          display_tick_cross(3, 2);
          update_status(4);
        }
        Serial.println("connected");
        // Once connected, publish an announcement...
        mqtt_client.subscribe("Command");
        //mqtt_client.subscribe("Response");
        //mqtt_client.subscribe("Alerts");
        mqtt_client.publish("Response", "I am Connected");
        digitalWrite(buzzer, 1);
        delay(200);
        digitalWrite(buzzer, 0);
      }
      else {
        digitalWrite(LED2, HIGH);
        digitalWrite(LED3, LOW);
        Serial.println("Connection with MQTT Broker Failed, trying again is 2sec");
        ethernet_connection_status = 0;
        if (screen == 1) {
          display_tick_cross(3, 1);
          update_status(3);
        }
      }
    }
  }
  else {
    digitalWrite(LED2, HIGH);
    if (screen == 1) {
      display_tick_cross(3, 1);
      update_status(3);
    }
  }
}


/*
 * Function to connect to Internet using Ethernet Shield
 */
void set_mqtt_server(String mqtt_ip_address, String mqtt_port_number) {
  int commaIndex = mqtt_ip_address.indexOf('.');
  int secondCommaIndex = mqtt_ip_address.indexOf('.', commaIndex + 1);

  int thirdCommaIndex = mqtt_ip_address.indexOf('.', secondCommaIndex + 1);
  String firstValue = mqtt_ip_address.substring(0, commaIndex);
  String secondValue = mqtt_ip_address.substring(commaIndex + 1, secondCommaIndex);
  String thirdValue = mqtt_ip_address.substring(secondCommaIndex + 1, thirdCommaIndex);
  String fourthValue = mqtt_ip_address.substring(thirdCommaIndex + 1);
  int a = firstValue.toInt();
  int b = secondValue.toInt();
  int c = thirdValue.toInt();
  int d = fourthValue.toInt();
  int port = mqtt_port_number.toInt();
  char charServer[mqtt_ip_address.length() + 1];
  mqtt_ip_address.toCharArray(charServer, 50);
  IPAddress mqtt_server(a, b, c, d);
  //IPAddress mqtt_server(192, 168, 137, 1);

  //mqtt_client.setServer(mqtt_server, mqtt_port_number.toInt());

  mqtt_client.setServer("m11.cloudmqtt.com", 11159);
  mqtt_client.setCallback(callback);
}


