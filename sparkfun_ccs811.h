/******************************************************************************
 * sparkfun_ccs811.h
 *
 * Sparkfun SEN-14348 - SparkFun Environmental Combo Breakout - CCS811/BME280
 * CCS811 component
 *
 * Created by Ioannis Tornazakis ( itornaza@gmail.com ) on 2021-03-28
 * 
 * https://learn.sparkfun.com/tutorials/ccs811bme280-qwiic-environmental-combo-breakout-hookup-guide
 ******************************************************************************/
#ifndef ION_SPARKFUN_CCS811_H
#define ION_SPARKFUN_CCS811_H

#include <Wire.h>
#include "SparkFunCCS811.h"
#include "sensor_server.h"

#define CCS811_ADDR 0x5B /* Default I2C Address */

/******************************************************************************
 *                              GLOBAL VARIABLES                              *
 ******************************************************************************/

CCS811 ccs811(CCS811_ADDR);
extern float bme_calibration_temp;
extern float bme_calibration_hum;

/******************************************************************************
 *                            FUNCTION PROTOTYPES                             *
 ******************************************************************************/

void init_sparkfun_ccs811();
String print_sparkfun_ccs811();

/******************************************************************************
 *                            FUNCTION DEFINITIONS                            *
 ******************************************************************************/

void init_sparkfun_ccs811() {

#ifdef DEBUG
  Serial.print("Initializing Sparkfun CCS811 sensor at I2C... \t");
#endif

  if (ccs811.begin() == false) {
    
#ifdef DEBUG
    Serial.println("The sensor did not respond. Please check wiring.");
#endif
    
    while (true);
  }

#ifdef DEBUG
  Serial.println("[ok]"); 
  delay(10);
#endif
}

String print_sparkfun_ccs811() {
  String ccs811_output = "";
  if (ccs811.dataAvailable()) {
    ccs811.readAlgorithmResults();
    /* Calibrate CCS811 with current temperature and humidity from BME280 */
    ccs811.setEnvironmentalData(bme_calibration_hum, bme_calibration_temp);
    ccs811_output += "[sparkfun-ccs811]: ";

    /* Display the eCO2 value with the respective color to its value */
    int eco2 = ccs811.getCO2();
    if (eco2 < 700) {
      ccs811_output += "<span style=\"color:green;\">";
      ccs811_output += eco2;
      ccs811_output += "</span>";
    } else if (eco2 >= 700 && eco2 < 1000) {
      ccs811_output += "<span style=\"color:yellow;\">";
      ccs811_output += eco2;
      ccs811_output += "</span>";
    } else {
      ccs811_output += "<span style=\"color:red;\">";
      ccs811_output += eco2;
      ccs811_output += "</span>";
    }
    ccs811_output += " eCO2 ppm | ";

    /* Display the TVOC */
    ccs811_output += ccs811.getTVOC();
    ccs811_output += " TVOC ppb";
  }
  return ccs811_output;
}

#endif /* ION_SPARKFUN_CCS811_H */
