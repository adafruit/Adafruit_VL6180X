/**************************************************************************/
/*! 
    @file     Adafruit_VL6180X.h
    @author   Limor Fried (Adafruit Industries)
	@license  BSD (see license.txt)
	
	This is a library for the Adafruit VL6180 ToF Sensor breakout board
	----> http://www.adafruit.com/products/3316
	
	Adafruit invests time and resources providing this open source code, 
	please support Adafruit and open-source hardware by purchasing 
	products from Adafruit!

	@section  HISTORY

    v1.0  - First release
*/
/**************************************************************************/

#include "Arduino.h"
#include <Wire.h>

//#define I2C_DEBUG

// the i2c address
#define VL6180X_DEFAULT_I2C_ADDR 0x29

#define VL6180X_REG_IDENTIFICATION_MODEL_ID    0x000
#define VL6180X_REG_SYSTEM_INTERRUPT_CONFIG    0x014
#define VL6180X_REG_SYSTEM_INTERRUPT_CLEAR     0x015
#define VL6180X_REG_SYSTEM_FRESH_OUT_OF_RESET  0x016
#define VL6180X_REG_SYSRANGE_START             0x018
#define VL6180X_REG_SYSALS_START               0x038
#define VL6180X_REG_SYSALS_ANALOGUE_GAIN       0x03F
#define VL6180X_REG_SYSALS_INTEGRATION_PERIOD_HI  0x040
#define VL6180X_REG_SYSALS_INTEGRATION_PERIOD_LO  0x041
#define VL6180X_REG_RESULT_ALS_VAL             0x050
#define VL6180X_REG_RESULT_RANGE_VAL           0x062
#define VL6180X_REG_RESULT_RANGE_STATUS        0x04d
#define VL6180X_REG_RESULT_INTERRUPT_STATUS_GPIO       0x04f

#define VL6180X_ALS_GAIN_1         0x06
#define VL6180X_ALS_GAIN_1_25      0x05
#define VL6180X_ALS_GAIN_1_67      0x04
#define VL6180X_ALS_GAIN_2_5       0x03
#define VL6180X_ALS_GAIN_5         0x02
#define VL6180X_ALS_GAIN_10        0x01
#define VL6180X_ALS_GAIN_20        0x00
#define VL6180X_ALS_GAIN_40        0x07

#define VL6180X_ERROR_NONE         0
#define VL6180X_ERROR_SYSERR_1     1
#define VL6180X_ERROR_SYSERR_5     5
#define VL6180X_ERROR_ECEFAIL      6
#define VL6180X_ERROR_NOCONVERGE   7
#define VL6180X_ERROR_RANGEIGNORE  8
#define VL6180X_ERROR_SNR          11
#define VL6180X_ERROR_RAWUFLOW     12
#define VL6180X_ERROR_RAWOFLOW     13
#define VL6180X_ERROR_RANGEUFLOW   14
#define VL6180X_ERROR_RANGEOFLOW   15



class Adafruit_VL6180X {
 public:
  Adafruit_VL6180X();
  boolean begin(void);
  uint8_t readRange(void);
  float   readLux(uint8_t gain);
  uint8_t readRangeStatus(void);

 private:
  void loadSettings(void);

  void write8(uint16_t address, uint8_t data);
  void write16(uint16_t address, uint16_t data);

  uint16_t read16(uint16_t address);
  uint8_t read8(uint16_t address);

  uint8_t _i2caddr;
};
