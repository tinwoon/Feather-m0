// This example shows basic use of a Pololu High Power Stepper Motor Driver.
//
// It shows how to initialize the driver, configure various settings, and enable
// the driver.  It shows how to step the motor and switch directions using the
// driver's SPI interface through the library's step() and setDirection() member
// functions.
//
// Since SPI is used to trigger steps and set the direction, connecting the
// driver's STEP and DIR pins is optional for this example.  However, note that
// using SPI control adds some overhead compared to using the STEP and DIR pins.
// In addition, since the library caches SPI register values, SPI control is
// more likely to re-enable the driver with the wrong settings (e.g. current
// limit) after a power interruption, although using the verifySettings() and
// applySettings() functions appropriately can help prevent this.
//
// Before using this example, be sure to change the setCurrentMilliamps36v4 line
// to have an appropriate current limit for your system.  Also, see this
// library's documentation for information about how to connect the driver:
//   http://pololu.github.io/high-power-stepper-driver

#include "step_motor.h"
#include "LED.h"
#include "servo_motor.h"
#include "control.h"
#include "Serial_port.h"
#include "booting_led.h"
//#include <Arduino.h>
#include <Servo.h>
#include <SPI.h>
#include <HighPowerStepperDriver.h>
#include <Adafruit_NeoPixel_ZeroDMA.h>
#include <iostream>
#include <vector>
#include "wiring_private.h"
#include <MemoryFree.h>

const uint8_t CSPin = 18;

static int value = 0;

static STEP_MOTOR step_motor_main;
static SERVO_MOTOR servo_motor_main;
static LED led_main;
//static HighPowerStepperDriver sd;
//static Servo servo;

//set booting led
extern BOOTING_LED_SET booting_led_set;

//Data driver
extern STEP_MOTOR_DATA step_motor_data;
extern SERVO_MOTOR_DATA servo_motor_data;
extern LED_DATA led_data;

//Control driver
extern SERVO_MOTOR_CONTROL servo_motor_control;
extern STEP_MOTOR_CONTROL step_motor_control;
extern LED_CONTROL led_control;

//CMD driver
extern SERIAL_CMD serial_cmd;
static String cmd = "";
static boolean stringComplete = false;


void setup()
{
  Serial.begin(9600);
  cmd.reserve(200);
  booting_led_set.set_booting_led_init();

  //serial init
  {
    Serial5.begin(9600);
    pinPeripheral(31, PIO_SERCOM_ALT);
    pinPeripheral(30, PIO_SERCOM_ALT);
    Serial5.println("serial success");
  }

  //for data init
  {
    step_motor_data.get_init(PARAMETER_STEP_MOTOR_GET_INIT, &step_motor_main);
    servo_motor_data.get_init(PARAMETER_SERVO_MOTOR_GET_INIT, &servo_motor_main);
    led_data.get_init(PARAMETER_LED_GET_INIT, &led_main);
  }
/*
  SPI.begin();
  sd.setChipSelectPin(CSPin);
  

  // Give the driver some time to power up.
  delay(1);

  // Reset the driver to its default settings and clear latched status
  // conditions.
  sd.resetSettings();
  sd.clearStatus();

  // Select auto mixed decay.  TI's DRV8711 documentation recommends this mode
  // for most applications, and we find that it usually works well.   
  sd.setDecayMode(HPSDDecayMode::AutoMixed);    

  // Set the current limit. You should change the number here to an appropriate
  // value for your particular system.
  sd.setCurrentMilliamps36v4(1000);

  // Set the number of microsteps that correspond to one full step.
  sd.setStepMode(HPSDStepMode::MicroStep32);

  // Enable the motor outputs.
  sd.enableDriver();
*/

}

void loop()
{
  serialInput();
  booting_led_set.set_booting_led(PARAMETER_SET_BOOTING_LED_REVERSE);
  
  //step motor 0시작
  {
    step_motor_data.get_init(PARAMETER_STEP_MOTOR_GET_INIT, &step_motor_main);
    step_motor_control.control(PARAMETER_STEP_CONTROL_ALL, &step_motor_main);
  }

  //servo motor 시작
  {
    
  }

  //led시작
  {
    led_data.get_init(PARAMETER_LED_GET_INIT, &led_main);
    led_control.control(PARAMETER_LED_CONTROL_ALL, &led_main);
  }

/*
  // Step in the default direction 1000 times.
  sd.setDirection(0);
  for(unsigned int x = 0; x < 1000; x++)
  {
    sd.step();
    delayMicroseconds(StepPeriodUs);
    //Serial.print(x);
    //step_motor.dir = x;
    //step_motor_data.set(PARAMETER_DIR, &step_motor.dir);
  }

  // Wait for 300 ms.
  delay(300);

  // Step in the other direction 1000 times.
  sd.setDirection(1);
  for(unsigned int x = 0; x < 1000; x++)
  {
    sd.step();
    delayMicroseconds(StepPeriodUs);
  }

  // Wait for 300 ms.
  delay(300);
*/
  //servo.write(0);
  //delay(1000);
  //servo.write(180);
  //delay(1000);
  while(Serial5.available()){
    Serial.println("5 is available");
    value = 10;
  }
  Serial5.println("serial success");
  Serial.println(value);
  Serial.print("free memory");
  Serial.println(freeMemory());


}

void serialInput(){
  while(Serial.available()){
    char inChar = (char)Serial.read();
    cmd += inChar;
    if(inChar == '\n'){
      stringComplete = true;
    }
    if(stringComplete){
      serial_cmd.read_cmd(cmd);
      cmd = "";
      stringComplete = false;
    } 
  }
}