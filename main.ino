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
#include "servo_motor.h"
#include "LED.h"
#include "control.h"
#include "Serial_port.h"
#include "booting_led.h"
#include "Limit_switch.h"
#include "timer_handler.h"
#include <Servo.h>
#include <SPI.h>
#include <HighPowerStepperDriver.h>
#include <Adafruit_NeoPixel_ZeroDMA.h>
#include "wiring_private.h"


static STEP_MOTOR step_motor_main;
static SERVO_MOTOR servo_motor_main;
static LED led_main;

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

//limit switch driver
extern LIMIT_SWITCH limit_switch;

//timer handler
extern TIMER_HANDLER timer_handler;

//motor state
extern bool motor_one_stepping;
extern bool motor_two_stepping;


void setup()
{
  serial_cmd.serial_init();
  booting_led_set.set_booting_led_init();
  limit_switch.switch_init();
  step_motor_control.init(PARAMETER_STEP_CONTROL_INIT_ALL);
  servo_motor_control.init(PARAMETER_SERVO_CONTROL_INIT_ALL);
  led_control.init(PARAMETER_LED_CONTROL_INIT_ALL);
  timer_handler.startTimer(10);
}

void loop()
{
  serial_cmd.serialInput();
  booting_led_set.set_booting_led(PARAMETER_SET_BOOTING_LED_REVERSE);
}

