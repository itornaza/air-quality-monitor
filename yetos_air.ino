/******************************************************************************
 * yetos_air.ino
 *
 * An http server for Arduino Ethernet Shield 2 that provides high precision 
 * temperature, humidity atmospheric pressure and eCO2 based on the Sparkfun 
 * SEN-14348 CCS811 and BME280 combo sensor.
 *
 * Created by Ioannis Tornazakis ( itornaza@gmail.com ) on 2021-03-28
 * 
 * * Hardware:
 * - Arduino Uno R3
 * - Arduino Ethernet Shield 2
 * - Sparkfun SEN-14348
 * 
 * https://learn.sparkfun.com/tutorials/ccs811bme280-qwiic-environmental-combo-breakout-hookup-guide?_ga=2.127395085.1624253088.1616957378-581135722.1616957378
 * 
 * Pinout:
 * +---------+---------+
 * |SEN-14348| Arduino |
 * +---------+---------+
 * | VCC     |  3.3V   |
 * | GND     |  GND    |
 * | SCL     |  A5     |
 * | SDA     |  A4     |
 * +---------+---------+
 * 
 ******************************************************************************/

#include <Ethernet.h>
#include <SPI.h>

#ifdef DEBUG
#include <Arduino.h>
#include <SoftwareSerial.h> 
#endif

#include "sparkfun_bme280.h"
#include "sparkfun_ccs811.h"
#include "sensor_server.h"

/******************************************************************************
 *                                      GLOBALS                               *
 ******************************************************************************/

/* Fill in the gaps depending on your network needs*/
byte mac[] = {0x__, 0x__, 0x__, 0x__, 0x__, 0x__};
IPAddress ip(192, 168, ___, ___);

IPAddress gateway(192, 168, 1, 254);
IPAddress subnet(255, 255, 255, 0);
IPAddress myDns(192, 168, 1, 254);

/* Values from the BME280 sensor needed to calibrate the CCS811 sensor */
float bme_calibration_temp = 0.0;
float bme_calibration_hum = 0.0;

/******************************************************************************
 *                                      SET UP                                *
 ******************************************************************************/

void setup() {
#ifdef DEBUG
  init_serial();
#endif
  init_i2c();
  init_ethernet();
  init_server();
  init_sparkfun_bme280();
  init_sparkfun_ccs811();
#ifdef DEBUG
  init_system();
#endif
}

/******************************************************************************
 *                                       LOOP                                 *
 ******************************************************************************/
 
void loop() {    
  EthernetClient client = server.available();
  if (client) {
    
#ifdef DEBUG
    print_info_on_connect(client);
#endif

    boolean current_line_is_blank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        
#ifdef DEBUG
        Serial.write(c);
#endif

        if (c == '\n' && current_line_is_blank) {
          html_header(client, "Yetos Air 1", "60");
          html_sensor(client, print_sparkfun_bme280);
          html_sensor(client, print_sparkfun_ccs811);
          client.println("<h1 style=\"color:blue; font-size:24px;\">");
          client.println("> ");
          client.println("<a href=\"https://iaqscience.lbl.gov/voc-intro\">Indoor Air Quality</a>");
          client.println("<br />");
          client.println("> ");
          client.println("<a href=\"https://www.kane.co.uk/knowledge-centre/what-are-safe-levels-of-co-and-co2-in-rooms\">Kane</a>");
          client.println("</h1>");
          html_footer(client);
          delay(SENSOR_INTERVAL); /* Do not overheat the board!!! */
          break; 
        }

        if (c == '\n') {
          current_line_is_blank = true;
        } else if(c != '\r') {
          current_line_is_blank = false;
        } 
      } /* end if (client.available()) */
    } /* end if (client.connected()) */
    delay(1);
    client.stop();
    
#ifdef DEBUG
    print_info_on_disconnect();
#endif
  } /* End if (client) */
}
