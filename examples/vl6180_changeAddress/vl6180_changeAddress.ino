#include <Wire.h>
#include "Adafruit_VL6180X.h"

Adafruit_VL6180X vl = Adafruit_VL6180X();

uint8_t serialReadAddress() {
  //Wait for data on serial port
  while(!Serial.available()) {}
  return (uint8_t) Serial.parseInt();
}

void setup() {
  Serial.begin(115200);

  // wait for serial port to open on native usb devices
  while (!Serial) {
    delay(1);
  }
  
  Serial.println("Adafruit VL6180x adress change sample!");
  Serial.print("Please enter current VL6180x. Default adress is ");
  Serial.println(VL6180X_DEFAULT_I2C_ADDR);
  uint8_t currentAddress = serialReadAddress();
  Serial.print("Entered: ");
  Serial.println(currentAddress);  
  while (!vl.begin(NULL, currentAddress)) {
    Serial.println("Unable to connect to VL6180x or the adress is wrong! Please enter again: ");
    currentAddress = serialReadAddress();
  }
  
  Serial.println("Please new adress of VL6180x:");
  uint8_t newAdress = serialReadAddress();
  Serial.print("Entered: ");
  Serial.println(newAdress);  
  if (newAdress != currentAddress) {
    vl.changeAddress((uint8_t) newAdress);
    if (!vl.begin(NULL, (uint8_t) newAdress)) {
      Serial.println("Failed to find sensor using a new adress: ");
    } else {
      Serial.print("The adress is changed! New adress is ");
    }
    Serial.println(newAdress);
  } else {
    Serial.println("The address is the same, the change is skipped!");
  }
}

void loop() {
  float lux = vl.readLux(VL6180X_ALS_GAIN_5);

  Serial.print("Lux: "); Serial.println(lux);
  
  uint8_t range = vl.readRange();
  uint8_t status = vl.readRangeStatus();

  if (status == VL6180X_ERROR_NONE) {
    Serial.print("Range: "); Serial.println(range);
  }

  // Some error occurred, print it out!
  
  if  ((status >= VL6180X_ERROR_SYSERR_1) && (status <= VL6180X_ERROR_SYSERR_5)) {
    Serial.println("System error");
  }
  else if (status == VL6180X_ERROR_ECEFAIL) {
    Serial.println("ECE failure");
  }
  else if (status == VL6180X_ERROR_NOCONVERGE) {
    Serial.println("No convergence");
  }
  else if (status == VL6180X_ERROR_RANGEIGNORE) {
    Serial.println("Ignoring range");
  }
  else if (status == VL6180X_ERROR_SNR) {
    Serial.println("Signal/Noise error");
  }
  else if (status == VL6180X_ERROR_RAWUFLOW) {
    Serial.println("Raw reading underflow");
  }
  else if (status == VL6180X_ERROR_RAWOFLOW) {
    Serial.println("Raw reading overflow");
  }
  else if (status == VL6180X_ERROR_RANGEUFLOW) {
    Serial.println("Range reading underflow");
  }
  else if (status == VL6180X_ERROR_RANGEOFLOW) {
    Serial.println("Range reading overflow");
  }
  delay(50);
}
