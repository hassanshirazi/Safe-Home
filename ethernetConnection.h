
#ifndef ETHERNET_CONNECTION
#define ETHERNET_CONNECTION

#include "Arduino.h"
#include "global.h"


bool internet_connection_using_ethernet();
void set_mqtt_server(String mqtt_ip_address, String mqtt_port_number);
void reconnect_using_ethernet(String username, String password);


#endif
