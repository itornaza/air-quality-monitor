/******************************************************************************
 * sensor_server.h
 *
 * A library to create an http server using the Arduino Ethernet Shield 2 that 
 * provides http get responses filled with the correspondent sensor data to 
 * http clients.
 *
 * Created by Ioannis Tornazakis ( itornaza@gmail.com ) on 2021-03-13
 * 
 * Version 1.0.1
 * - Added i2c init function
 ******************************************************************************/
#ifndef ION_SENSOR_SERVER_H
#define ION_SENSOR_SERVER_H

#include <Ethernet.h>
#include <Wire.h>

#define BAUD 9600
#define SENSOR_INTERVAL 1000

/* Debuging: Uncomment the following line to enable debugging. Remember to 
the line again before deployment */
/* #define DEBUG */

/******************************************************************************
 *                              GLOBAL VARIABLES                              *
 ******************************************************************************/

extern byte mac[];
extern IPAddress ip;
extern IPAddress gateway;
extern IPAddress subnet;
extern IPAddress myDns;
EthernetServer server(80);

/******************************************************************************
 *                            FUNCTION PROTOTYPES                             *
 ******************************************************************************/
 
#ifdef DEBUG
void init_serial();
void init_system();
void print_info_on_connect(EthernetClient client);
void print_info_on_disconnect();
#endif

void init_i2c();
void init_ethernet();
void init_server();
void html_header(EthernetClient client, String page_title, String refresh_rate);
void html_sensor(EthernetClient client, String (*sensor)(void));
void html_footer(EthernetClient client);

/******************************************************************************
 *                            FUNCTION DEFINITIONS                            *
 ******************************************************************************/
 
#ifdef DEBUG
void init_serial() {
  Serial.begin(BAUD);
  Serial.print("Initializing serial coms... \t\t\t"); 
  delay(100);
  while(!Serial);
  Serial.println("[ok]"); 
  delay(100);
}
#endif

void init_ethernet() {
#ifdef DEBUG
  Serial.print("Initializing ethernet... \t\t\t");
#endif
 
  Ethernet.begin(mac, ip, myDns, gateway, subnet);
  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    
#ifdef DEBUG
    Serial.println("Ethernet shield not found.");
#endif

    while (true);
  }

#ifdef DEBUG
  /* Check for ethernet cable */
  if (Ethernet.linkStatus() == LinkOFF) {
    Serial.println("Ethernet cable is not connected.");
  }
  Serial.println("[ok]");
#endif
}

void init_i2c() {

#ifdef DEBUG
  Serial.print("Initializing i2c... \t\t\t\t");
#endif

  Wire.begin();

#ifdef DEBUG  
  Serial.println("[ok]"); 
  delay(10);
#endif
}

void init_server() {
  
#ifdef DEBUG
  Serial.print("Initializing ethernet shield 2 server... \t"); 
  delay(10);
#endif

  server.begin();

#ifdef DEBUG  
  Serial.println("[ok]"); 
  delay(10);
#endif

#ifdef DEBUG
  /* Mac address */
  byte macBuffer[6];
  Ethernet.MACAddress(macBuffer);
  Serial.print("  mac: ");
  for (byte octet = 0; octet < 6; octet++) {
    Serial.print(macBuffer[octet], HEX);
    if (octet < 5) {
      Serial.print(':');
    }
  }
  Serial.println();

  /* IP addressed */
  Serial.print("  ip: ");
  Serial.println(Ethernet.localIP());
  Serial.print("  gateway: ");
  Serial.println(Ethernet.gatewayIP());
  Serial.print("  dns: ");
  Serial.println(Ethernet.dnsServerIP());
#endif
}

#ifdef DEBUG
void init_system() {
  Serial.println("System boot... \t\t\t\t\t[ok]\n");
}
#endif

#ifdef DEBUG
void print_info_on_connect(EthernetClient client) {
  Serial.print("\n*** New client found @ ");
  Serial.print(client.remoteIP());
  Serial.println(" ***");
}
#endif

#ifdef DEBUG
void print_info_on_disconnect() {
  Serial.println("*** Client disconnected. ***"); 
}
#endif

void html_header(EthernetClient client, String page_title, String refresh_rate) {
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("Connection: close");
  client.print("Refresh: ");
  client.println(refresh_rate);
  client.println();
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println("<body bgcolor=\"#000000\" text=\"ffffff\">");
  client.println("<h1 style=\"color:blue; font-size:64px;\">");
  client.println(page_title);
  client.println("</h1>");
}

void html_sensor(EthernetClient client, String (*sensor)(void)) {
  client.println("<h1 style=\"color:green; font-size:36px;\">");
  client.println((*sensor)()); 
  client.println("</h1>");
  
#ifdef DEBUG
  Serial.println((*sensor)());
#endif
}

void html_footer(EthernetClient client) {
  client.println("</body>");
  client.println("</html>");
}

#endif /* ION_SENSOR_SERVER_H */
