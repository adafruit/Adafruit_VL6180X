/**
 * This example shows the usage of 3 VL6180X on the same I2c bus.
 * Use vl6180_changeAddress example to change the address of the device
 */
#include <Wire.h>
#include "Adafruit_VL6180X.h"

//First devices uses default address, other devices use default + 1 and default + 2
#define VL1_ADDRES VL6180X_DEFAULT_I2C_ADDR
#define VL2_ADDRES VL6180X_DEFAULT_I2C_ADDR + 1
#define VL3_ADDRES VL6180X_DEFAULT_I2C_ADDR + 2

Adafruit_VL6180X vl1 = Adafruit_VL6180X();
Adafruit_VL6180X vl2 = Adafruit_VL6180X();
Adafruit_VL6180X vl3 = Adafruit_VL6180X();

/**************************************************************************/
/*! 
    @brief  Initializes the sensor on the given address and prints Serial messages
    @param  vl sensor handler to be initialized
    @param  address i2c address of the sensor
*/
/**************************************************************************/
void connect(Adafruit_VL6180X& vl, uint8_t address) {
  Serial.print("Connecting to VL6180x using address: ");
  Serial.println(address);
  if (! vl.begin(NULL, address)) {
    Serial.println("Failed to find sensor");
    while (1);
  }
  Serial.println("Sensor found!");
}

/**************************************************************************/
/*! 
    @brief  Prints the range reported by sensor to Serial
    @param  vl sensor handler to be initialized
*/
/**************************************************************************/
void printRange(Adafruit_VL6180X& vl) {
  
  uint8_t range = vl.readRange();
  uint8_t status = vl.readRangeStatus();

  if (status == VL6180X_ERROR_NONE) {
    Serial.print("Range of device on adress "); 
    Serial.print(vl.getAddress()); 
    Serial.print(": ");
    Serial.println(range);
  }
}

void setup() {
  Serial.begin(115200);

  // wait for serial port to open on native usb devices
  while (!Serial) {
    delay(1);
  }

  //Connect 3 sensors
  connect(vl1, VL1_ADDRES);
  connect(vl2, VL2_ADDRES);
  connect(vl3, VL3_ADDRES);
}

void loop() {
  //Print range reported by each sensor
  printRange(vl1);  
  printRange(vl2);  
  printRange(vl3);  
  delay(50);
}
