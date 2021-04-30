//jiwon 데이터 매핑 구조 설계 21-04-28
//외부 라이브러리 include 이유는 모르겠지만 컴파일 확인

#include "Arduino.h"
#include "control.h"
#include "main.h"
#include "servo_motor.h"
#include "step_motor.h"
#include "LED.h"
#include <Servo.h>
#include <HighPowerStepperDriver.h>

static SERVO_MOTOR_CONTROL servo_motor_control{ control };
static STEP_MOTOR_CONTROL step_motor_control{ control };
static LED_CONTROL led_control{ control };

static STEP_MOTOR_DATA step_motor_data;
static STEP_MOTOR step_motor;
static SERVO_MOTOR_DATA servo_motor_data;
static SERVO_MOTOR servo_motor;
static LED_DATA led_data;
static LED led;

static HighPowerStepperDriver sd;
static Servo servo;

void control(int PARAMETER, void* value){

    step_motor_data.get_init(PARAMETER_MOTOR_GET_INIT, &step_motor);
    servo_motor_data.get_init(PARAMETER_MOTOR_GET_INIT, &servo_motor);
    led_data.get_init(PARAMETER_LED_GET_INIT, &led);

    switch(PARAMETER){
        case PARAMETER_STEP_CONTROL_ALL:
            SPI.begin();
            //set motor id(CS or SCS pin init)
            {
                if(step_motor.motor_id == 1) sd.setChipSelectPin(STEP_MOTOR_ID_1);
                else if(step_motor.motor_id == 2) sd.setChipSelectPin(STEP_MOTOR_ID_2);
                else if(step_motor.motor_id == 3) sd.setChipSelectPin(STEP_MOTOR_ID_3);
                // Give the driver some time to power up.
                delay(1);
                sd.resetSettings();
                sd.clearStatus();
            }

            //set dir(DIR to SPI)
            {
                if(step_motor.dir == 0) sd.setDirection(STEP_MOTOR_DIR_FORWARD);
                else if(step_motor.dir == 1) sd.setDirection(STEP_MOTOR_DIR_REVERSE);
            }

            //set step for 기준각 360/256 = 1.40625도 (STEP to SPI)
            {
                // Set the number of microsteps that correspond to one full step.
                sd.setStepMode(HPSDStepMode::MicroStep256);
            }

            //the other setting
            // Select auto mixed decay.  TI's DRV8711 documentation recommends this mode
            // for most applications, and we find that it usually works well.   
            sd.setDecayMode(HPSDDecayMode::AutoMixed);    

            // Set the current limit. You should change the number here to an appropriate
            // value for your particular system.
            sd.setCurrentMilliamps36v4(1000);
            // Enable the motor outputs. and 
            sd.enableDriver();

            //set angle and rotation_number and rpm
            {
                for(int rotation_number = 0; rotation_number < step_motor.rotation_number; rotation_number++){
                    for(int angle = 0; angle < step_motor.angle; angle++){
                        sd.step();
                        delayMicroseconds(1000-(step_motor.rpm*10));
                    }
                }
            }
            break;   

        case PARAMETER_SERVO_CONTROL_ALL:
        {
            //set servo motor id
            {
            if(servo_motor.motor_id==0) servo.attach(SERVO_MOTOR_TILLT);
            else if(servo_motor.motor_id==0) servo.attach(SERVO_MOTOR_PAN);
            }     
            //set servo angle
            {
               servo.write(servo_motor.angle);
            }
            break;
        }
    }
}
