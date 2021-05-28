//jiwon 데이터 매핑 구조 설계 21-04-28

#include "Arduino.h"
#include "control.h"
#include "main.h"
#include "servo_motor.h"
#include "step_motor.h"
#include "LED.h"
#include <Servo.h>
#include <HighPowerStepperDriver.h>
#include <limits.h>

//for motor interrrupt
bool step_motor_one_imergency = false;
bool step_motor_two_imergency = false;
bool motor_one_stepping = false;
bool motor_two_stepping = false;

STEP_MOTOR_CONTROL step_motor_control{ control, init };
SERVO_MOTOR_CONTROL servo_motor_control{ control, init };
LED_CONTROL led_control{ control, init };

extern STEP_MOTOR_DATA step_motor_data;
extern SERVO_MOTOR_DATA servo_motor_data;
extern LED_DATA led_data;

static HighPowerStepperDriver sd;
static Servo servo_pan;
static Servo servo_tilt;
static NeoPixelLED neoPixelLED;

void control(int PARAMETER, void* value){

    switch(PARAMETER){
        case PARAMETER_STEP_CONTROL_ALL:
            //정지
            if(((STEP_MOTOR*)value)-> dir == 0){
                Serial5.println("motor stop");
                break;
            } 

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
                if(((STEP_MOTOR*)value)-> dir == 1) sd.setDirection(STEP_MOTOR_DIR_FORWARD);
                else if(((STEP_MOTOR*)value)-> dir == 2) sd.setDirection(STEP_MOTOR_DIR_REVERSE);
            }

            //set step for 기준각 1step 당 = 1.8도 (STEP to SPI)
            //1~256
            {
                // Set the number of microsteps that correspond to one full step.
                if( ((STEP_MOTOR*)value)-> step_angle == 1) sd.setStepMode(HPSDStepMode::MicroStep1);
                else if( ((STEP_MOTOR*)value)-> step_angle == 2) sd.setStepMode(HPSDStepMode::MicroStep2);
                else if( ((STEP_MOTOR*)value)-> step_angle == 4) sd.setStepMode(HPSDStepMode::MicroStep4);
                else if( ((STEP_MOTOR*)value)-> step_angle == 8) sd.setStepMode(HPSDStepMode::MicroStep8);
                else if( ((STEP_MOTOR*)value)-> step_angle == 16) sd.setStepMode(HPSDStepMode::MicroStep16);
                else if( ((STEP_MOTOR*)value)-> step_angle == 32) sd.setStepMode(HPSDStepMode::MicroStep32);
                else if( ((STEP_MOTOR*)value)-> step_angle == 64) sd.setStepMode(HPSDStepMode::MicroStep64);
                else if( ((STEP_MOTOR*)value)-> step_angle == 128) sd.setStepMode(HPSDStepMode::MicroStep128);
                else if( ((STEP_MOTOR*)value)-> step_angle == 256) sd.setStepMode(HPSDStepMode::MicroStep256);
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
                if( ((STEP_MOTOR*)value)-> motor_id == 1 ) motor_one_stepping = true;
                if( ((STEP_MOTOR*)value)-> motor_id == 2 ) motor_two_stepping = true;

                for(int rotation_number = 0; rotation_number < ( ((STEP_MOTOR*)value)-> step_rotate_num==0?INT_MAX:((STEP_MOTOR*)value)-> step_rotate_num ); rotation_number++){
                    
                    if(((STEP_MOTOR*)value)-> motor_id == 1 && !motor_one_stepping) break;
                    if(((STEP_MOTOR*)value)-> motor_id == 2 && !motor_two_stepping) break;

                    if(step_motor_one_imergency && (((STEP_MOTOR*)value)-> motor_id == 1) ){ 
                        step_motor_one_imergency = false;
                        break;
                    } 
                    if(step_motor_two_imergency && (((STEP_MOTOR*)value)-> motor_id == 2) ){
                        step_motor_two_imergency = false;
                        break;
                    } 
                    
                    sd.step();
                    delayMicroseconds(((STEP_MOTOR*)value)-> step_delay);
                }
                motor_one_stepping = false;
                motor_two_stepping = false;
            }
            break;   

        case PARAMETER_SERVO_CONTROL_ALL:
        {
            //set servo motor id
            {
                servo_tilt.attach(SERVO_MOTOR_TILLT);
                servo_pan.attach(SERVO_MOTOR_PAN);
            }     
            //set servo angle
            {
                if(((SERVO_MOTOR*)value)-> motor_id==1) servo_pan.write(((SERVO_MOTOR*)value)-> angle);
                else if(((SERVO_MOTOR*)value)-> motor_id==2) servo_tilt.write(((SERVO_MOTOR*)value)-> angle);
                Serial.print("angle is: ");
                Serial.println(((SERVO_MOTOR*)value)-> angle);
                delay(10);
            }
            break;
        }

        case PARAMETER_LED_CONTROL_ALL:
        {
            //set led motor id
            {
                switch( ((LED*)value)->LED_id){
                    case 1:
                        neoPixelLED.init(LED_DATA1, LED_NUM);
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
            break;
        }
    }
}

void init(int PARAMETER){

    switch(PARAMETER){
        case PARAMETER_STEP_CONTROL_INIT_ALL:
        {
            //init step motor
            {
                SPI.begin();

                //the other setting
                // Select auto mixed decay.  TI's DRV8711 documentation recommends this mode
                // for most applications, and we find that it usually works well.   
                sd.setDecayMode(HPSDDecayMode::AutoMixed);    

                // Set the current limit. You should change the number here to an appropriate
                // value for your particular system.
                sd.setCurrentMilliamps36v4(1000);
                // Enable the motor outputs. and 
                sd.enableDriver();
            }
            break;   
        }
            
            

        case PARAMETER_SERVO_CONTROL_INIT_ALL:
        {
            //init servo motor id
            {
                servo_tilt.attach(SERVO_MOTOR_TILLT);
                servo_pan.attach(SERVO_MOTOR_PAN);
            }
            break;
        }

        case PARAMETER_LED_CONTROL_INIT_ALL:
        {
            //init led motor id
            {

            }
            break;
        }
    }
}
