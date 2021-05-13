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


STEP_MOTOR_CONTROL step_motor_control{ control };
SERVO_MOTOR_CONTROL servo_motor_control{ control };
LED_CONTROL led_control{ control };

extern STEP_MOTOR_DATA step_motor_data;
extern SERVO_MOTOR_DATA servo_motor_data;
extern LED_DATA led_data;

static HighPowerStepperDriver sd;
static Servo servo;
static NeoPixelLED neoPixelLED;

void control(int PARAMETER, void* value){

    switch(PARAMETER){
        case PARAMETER_STEP_CONTROL_ALL:
            SPI.begin();
            //set motor id(CS or SCS pin init)
            {
                if(((STEP_MOTOR*)value)-> motor_id == 1) sd.setChipSelectPin(STEP_MOTOR_ID_1);
                else if(((STEP_MOTOR*)value)-> motor_id == 2) sd.setChipSelectPin(STEP_MOTOR_ID_2);
                else if(((STEP_MOTOR*)value)-> motor_id == 3) sd.setChipSelectPin(STEP_MOTOR_ID_3);
                // Give the driver some time to power up.
                delay(1);
                sd.resetSettings();
                sd.clearStatus();
            }

            //set dir(DIR to SPI)
            {
                if(((STEP_MOTOR*)value)-> dir == 0) sd.setDirection(STEP_MOTOR_DIR_FORWARD);
                else if(((STEP_MOTOR*)value)-> dir == 1) sd.setDirection(STEP_MOTOR_DIR_REVERSE);
            }

            //set step for 기준각 360/256 = 1.40625도 (STEP to SPI)
            //256
            {
                // Set the number of microsteps that correspond to one full step.
                sd.setStepMode(HPSDStepMode::MicroStep128);
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
                for(int rotation_number = 0; rotation_number < ((STEP_MOTOR*)value)->rotation_number; rotation_number++){
                    for(int angle = 0; angle < ((STEP_MOTOR*)value)-> angle; angle++){
                        sd.step();
                        delayMicroseconds(1001-( (((STEP_MOTOR*)value)-> rpm)*10 ) );
                    }
                }
            }
            break;   

        case PARAMETER_SERVO_CONTROL_ALL:
        {
            //set servo motor id
            {
            if(((SERVO_MOTOR*)value)-> motor_id==0) servo.attach(SERVO_MOTOR_TILLT);
            else if(((SERVO_MOTOR*)value)-> motor_id==1) servo.attach(SERVO_MOTOR_PAN);
            }     
            //set servo angle
            {
               servo.write(((SERVO_MOTOR*)value)-> angle);
            }
            break;
        }

        case PARAMETER_LED_CONTROL_ALL:
        {
            //set led motor id
            {
                Serial.println("led control in is success");
                Serial.print(((LED*)value)->LED_id);
                Serial.print(((LED*)value)->R);
                Serial.print(((LED*)value)->G);
                Serial.print(((LED*)value)->B);
                Serial.println(((LED*)value)->W);
                switch( ((LED*)value)->LED_id){
                    case 1:
                        neoPixelLED.init(LED_DATA1, LED_NUM);
                        Serial.print("led id init success and PARAMETER : ");
                        Serial.println(((LED*)value)->LED_id);
                        Serial.print("PIN_SERIAL_RX : ");
                        Serial.println(PIN_SERIAL_RX);
                        break;
                    case 2:
                        neoPixelLED.init(LED_DATA2, LED_NUM);
                        break;
                    default:
                        neoPixelLED.init(LED_DATA1, LED_NUM);
                }
                neoPixelLED.setBrightness(255);
                neoPixelLED.clear();
                for(int led_num = 0; led_num < LED_NUM; led_num++){
                    neoPixelLED.setColor(led_num, ((LED*)value)->R, ((LED*)value)->G, ((LED*)value)->B, ((LED*)value)->W );
                }
                neoPixelLED.show();
            }
        }
    }
}
