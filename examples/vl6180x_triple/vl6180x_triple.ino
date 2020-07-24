#include <Adafruit_VL6180X.h>

// address we will assign if dual sensor is present
#define LOX1_ADDRESS 0x30
#define LOX2_ADDRESS 0x31
#define LOX3_ADDRESS 0x32

// set the pins to shutdown
#define SHT_LOX1 7
#define SHT_LOX2 6
#define SHT_LOX3 5

// objects for the VL6180X
Adafruit_VL6180X lox1 = Adafruit_VL6180X();
Adafruit_VL6180X lox2 = Adafruit_VL6180X();
Adafruit_VL6180X lox3 = Adafruit_VL6180X();

// Setup mode for doing reads
typedef enum {RUN_MODE_DEFAULT, RUN_MODE_TIMED, RUN_MODE_ASYNC} runmode_t;

runmode_t run_mode = RUN_MODE_DEFAULT;


/*
    Reset all sensors by setting all of their XSHUT pins low for delay(10), then set all XSHUT high to bring out of reset
    Keep sensor #1 awake by keeping XSHUT pin high
    Put all other sensors into shutdown by pulling XSHUT pins low
    Initialize sensor #1 with lox.begin(new_i2c_address) Pick any number but 0x29 and it must be under 0x7F. Going with 0x30 to 0x3F is probably OK.
    Keep sensor #1 awake, and now bring sensor #2 out of reset by setting its XSHUT pin high.
    Initialize sensor #2 with lox.begin(new_i2c_address) Pick any number but 0x29 and whatever you set the first sensor to
*/
void setID() {
  // all reset
  digitalWrite(SHT_LOX1, LOW);
  digitalWrite(SHT_LOX2, LOW);
  digitalWrite(SHT_LOX3, LOW);
  delay(10);

  // all unreset
  digitalWrite(SHT_LOX1, HIGH);
  digitalWrite(SHT_LOX2, HIGH);
  digitalWrite(SHT_LOX3, HIGH);
  delay(10);

  // activating LOX1 and reseting LOX2
  digitalWrite(SHT_LOX1, HIGH);
  digitalWrite(SHT_LOX2, LOW);
  digitalWrite(SHT_LOX3, LOW);

  // initing LOX1
  if (!lox1.begin()) {
    Serial.println(F("Failed to boot first VL6180X"));
    while (1);
  }
  lox1.setAddress(LOX1_ADDRESS);
  delay(10);

  // activating LOX2
  digitalWrite(SHT_LOX2, HIGH);
  delay(10);

  //initing LOX2
  if (!lox2.begin()) {
    Serial.println(F("Failed to boot second VL6180X"));
    while (1);
  }
  lox2.setAddress(LOX2_ADDRESS);

  // activating LOX3
  digitalWrite(SHT_LOX3, HIGH);
  delay(10);

  //initing LOX3
  if (!lox3.begin()) {
    Serial.println(F("Failed to boot third VL6180X"));
    while (1);
  }
  lox3.setAddress(LOX3_ADDRESS);
}

void readSensor(Adafruit_VL6180X &vl) {
  Serial.print(" Addr: ");
  Serial.print(vl.getAddress(), HEX);

  float lux = vl.readLux(VL6180X_ALS_GAIN_5);
  Serial.print(" Lux: "); Serial.print(lux);
  uint8_t range = vl.readRange();
  uint8_t status = vl.readRangeStatus();

  if (status == VL6180X_ERROR_NONE) {
    Serial.print(" Range: "); Serial.print(range);
  }

  // Some error occurred, print it out!

  if  ((status >= VL6180X_ERROR_SYSERR_1) && (status <= VL6180X_ERROR_SYSERR_5)) {
    Serial.print("(System error)");
  }
  else if (status == VL6180X_ERROR_ECEFAIL) {
    Serial.print("(ECE failure)");
  }
  else if (status == VL6180X_ERROR_NOCONVERGE) {
    Serial.print("(No convergence)");
  }
  else if (status == VL6180X_ERROR_RANGEIGNORE) {
    Serial.print("(Ignoring range)");
  }
  else if (status == VL6180X_ERROR_SNR) {
    Serial.print("Signal/Noise error");
  }
  else if (status == VL6180X_ERROR_RAWUFLOW) {
    Serial.print("Raw reading underflow");
  }
  else if (status == VL6180X_ERROR_RAWOFLOW) {
    Serial.print("Raw reading overflow");
  }
  else if (status == VL6180X_ERROR_RANGEUFLOW) {
    Serial.print("Range reading underflow");
  }
  else if (status == VL6180X_ERROR_RANGEOFLOW) {
    Serial.print("Range reading overflow");
  }
}

void read_sensors() {
  readSensor(lox1);
  readSensor(lox2);
  readSensor(lox3);
  Serial.println();
}

void timed_read_sensors() {
  uint32_t start_time = millis();
  uint8_t range_lox1 = lox1.readRange();
  uint8_t status_lox1 = lox1.readRangeStatus();
  uint8_t range_lox2 = lox2.readRange();
  uint8_t status_lox2 = lox2.readRangeStatus();
  uint8_t range_lox3 = lox3.readRange();
  uint8_t status_lox3 = lox3.readRangeStatus();
  uint32_t delta_time = millis() - start_time;
  Serial.print(delta_time, DEC);
  Serial.print(" : ");
  if (status_lox1 == VL6180X_ERROR_NONE) Serial.print(range_lox1, DEC);
  else Serial.print("###");
  Serial.print(" : ");
  if (status_lox2 == VL6180X_ERROR_NONE) Serial.print(range_lox2, DEC);
  else Serial.print("###");
  Serial.print(" : ");
  if (status_lox3 == VL6180X_ERROR_NONE) Serial.print(range_lox3, DEC);
  else Serial.print("###");

  Serial.println();
}

void timed_async_read_sensors() {
  uint32_t start_time = millis();

  // lets start up all three
  lox1.startRange();
  lox2.startRange();
  lox3.startRange();
  
  // wait for each of them to complete
  lox1.waitRangeComplete();
  lox2.waitRangeComplete();
  lox3.waitRangeComplete();
  
  uint8_t range_lox1 = lox1.readRangeResult();
  uint8_t status_lox1 = lox1.readRangeStatus();
  uint8_t range_lox2 = lox2.readRangeResult();
  uint8_t status_lox2 = lox2.readRangeStatus();
  uint8_t range_lox3 = lox3.readRangeResult();
  uint8_t status_lox3 = lox3.readRangeStatus();
  uint32_t delta_time = millis() - start_time;
  Serial.print(delta_time, DEC);
  Serial.print(" : ");
  if (status_lox1 == VL6180X_ERROR_NONE) Serial.print(range_lox1, DEC);
  else Serial.print("###");
  Serial.print(" : ");
  if (status_lox2 == VL6180X_ERROR_NONE) Serial.print(range_lox2, DEC);
  else Serial.print("###");
  Serial.print(" : ");
  if (status_lox3 == VL6180X_ERROR_NONE) Serial.print(range_lox3, DEC);
  else Serial.print("###");

  Serial.println();
  
}


void setup() {
  Serial.begin(115200);

  // wait until serial port opens for native USB devices
  while (! Serial) {
    delay(1);
  }

  pinMode(SHT_LOX1, OUTPUT);
  pinMode(SHT_LOX2, OUTPUT);
  pinMode(SHT_LOX3, OUTPUT);

  Serial.println("Shutdown pins inited...");

  digitalWrite(SHT_LOX1, LOW);
  digitalWrite(SHT_LOX2, LOW);
  digitalWrite(SHT_LOX3, LOW);

  Serial.println("All in reset mode...(pins are low)");


  Serial.println("Starting...");
  setID();

}

void loop() {
  if (Serial.available()) {
    uint8_t ch = Serial.read();
    while (Serial.read() != -1) ; // remove the rest

    // See what the user typed in
    switch (ch) {
      case 'd':
      case 'D':
        run_mode = RUN_MODE_DEFAULT;
        break;
      case 't':
      case 'T':
        run_mode = RUN_MODE_TIMED;
        break;
      case 'a':
      case 'A':
        run_mode = RUN_MODE_ASYNC;
        break;
      default:
        Serial.println("\nSet run mode by entering one of the following letters");
        Serial.println("    D - Default mode");
        Serial.println("    T - Timed mode - Reads done one after another");
        Serial.println("    A - Asynchronous mode - Try starting all three at once");
        run_mode = RUN_MODE_DEFAULT;
    }
  }
  switch (run_mode) {
    case RUN_MODE_DEFAULT:
      read_sensors();
      break;
    case RUN_MODE_TIMED:
      timed_read_sensors();
      break;
    case RUN_MODE_ASYNC:
      timed_async_read_sensors();
      break;
  }
  delay(100);
}
