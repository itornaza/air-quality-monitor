/******************************************************************************
 * sparkfun_bme280.h
 *
 * Sparkfun SEN-14348 - SparkFun Environmental Combo Breakout - CCS811/BME280
 * BME280 component
 *
 * Created by Ioannis Tornazakis ( itornaza@gmail.com ) on 2021-03-28
 ******************************************************************************/
#ifndef ION_SPARKFUN_BME280_H
#define ION_SPARKFUN_BME280_H

#include "sensor_server.h"
#include "SparkFunBME280.h"

/* Calibration values for specific board if needed */
#define TEMP_CALIBRATION 0.0
#define HUM_CALIBRATION 0.0

/******************************************************************************
 *                              GLOBAL VARIABLES                              *
 ******************************************************************************/

BME280 bme;
extern float bme_calibration_temp;
extern float bme_calibration_hum;

/******************************************************************************
 *                            FUNCTION PROTOTYPES                             *
 ******************************************************************************/

void init_sparkfun_bme280();
String print_sparkfun_bme280();

/******************************************************************************
 *                            FUNCTION DEFINITIONS                            *
 ******************************************************************************/

void init_sparkfun_bme280() {

#ifdef DEBUG
  Serial.print("Initializing Sparkfun BME280 sensor at I2C... \t");
#endif
  
  if (bme.beginI2C() == false) {

#ifdef DEBUG
    Serial.println("The sensor did not respond. Please check wiring.");
#endif

    while(true);
  }

#ifdef DEBUG
  Serial.println("[ok]"); 
  delay(10);
#endif
}

String print_sparkfun_bme280() {
  String bme280_output = "";
  bme280_output += "[sparkfun-bme280]: ";
  bme_calibration_temp = bme.readTempC() + TEMP_CALIBRATION;
  bme280_output += bme_calibration_temp; /* Update global CCS811 calibration */
  bme280_output += " Celsius | ";
  bme_calibration_hum = bme.readFloatHumidity() + HUM_CALIBRATION;
  bme280_output += bme_calibration_hum ; /* Update global CCS811 calibration */
  bme280_output += " % | ";
  bme280_output += bme.readFloatPressure() / 100.0F;
  bme280_output += " hPa | ";
  bme280_output += bme.readFloatAltitudeMeters();
  bme280_output += " m";
  return bme280_output;
}

#endif /* ION_SPARKFUN_BME280_H */
