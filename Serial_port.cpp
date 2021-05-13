//jiwon 데이터 매핑 구조 설계 21-04-28
//외부 라이브러리 include 이유는 모르겠지만 컴파일 확인

#include "Arduino.h"
#include "control.h"
#include "main.h"
#include "servo_motor.h"
#include "step_motor.h"
#include "LED.h"
#include "Serial_port.h"
#include <string.h>
#include <Servo.h>
#include <HighPowerStepperDriver.h>


SERIAL_CMD serial_cmd{ read_cmd, attach_cmd};

static STEP_MOTOR step_motor_cmd;
static SERVO_MOTOR servo_motor_cmd;
static LED led_cmd;


extern STEP_MOTOR_DATA step_motor_data;
extern SERVO_MOTOR_DATA servo_motor_data;
extern LED_DATA led_data;

void read_cmd(String cmd){

    std::vector< String > cmd_stack;
    int prv_token = 0;
    int length = cmd.length();
    for(int k=0; k<length; k++){
        int token = cmd.indexOf('#', prv_token);
        String Sep_String = cmd.substring(prv_token, token);
        cmd_stack.emplace_back(Sep_String);
        prv_token = token + 1;
    }
    if(cmd_stack.front() == "STM") attach_cmd(PARAMETER_STEP_MOTOR_CMD, cmd_stack);
    else if(cmd_stack.front() == "SVM") attach_cmd(PARAMETER_SERVO_MOTOR_CMD, cmd_stack);
    else if(cmd_stack.front() == "LED") attach_cmd(PARAMETER_LED_CMD, cmd_stack);

}

void attach_cmd(int PARAMETER, std::vector< String > &cmd_stack){
    switch(PARAMETER){
        case PARAMETER_STEP_MOTOR_CMD:
        {
            Serial.print("Serial parameter");
            Serial.println(PARAMETER);
            Serial.print(cmd_stack[1].toInt());
            Serial.print(cmd_stack[2].toInt());
            Serial.print(cmd_stack[3].toInt());
            Serial.print(cmd_stack[4].toInt());
            Serial.println(cmd_stack[5].toInt());

            step_motor_data.get_init(PARAMETER_STEP_MOTOR_GET_INIT, &step_motor_cmd);
            step_motor_cmd.motor_id = cmd_stack[1].toInt();
            step_motor_cmd.dir = cmd_stack[2].toInt();
            step_motor_cmd.angle = cmd_stack[3].toInt();
            step_motor_cmd.rotation_number = cmd_stack[4].toInt();
            step_motor_cmd.rpm = cmd_stack[5].toInt();
            step_motor_data.set_init(PARAMETER_STEP_MOTOR_SET_INIT, &step_motor_cmd);
            break;
        }

        case PARAMETER_SERVO_MOTOR_CMD:
        {
            servo_motor_data.get_init(PARAMETER_SERVO_MOTOR_GET_INIT, &servo_motor_cmd);
            servo_motor_cmd.motor_id = cmd_stack[1].toInt();
            servo_motor_cmd.angle = cmd_stack[2].toInt();
            servo_motor_data.set_init(PARAMETER_SERVO_MOTOR_SET_INIT, &servo_motor_cmd);
            break;
        }

        case PARAMETER_LED_CMD:
        {
            Serial.print("Serial parameter");
            Serial.println(PARAMETER);
            led_data.get_init(PARAMETER_LED_GET_INIT, &led_cmd);
            led_cmd.LED_id = cmd_stack[1].toInt();
            led_cmd.R = cmd_stack[2].toInt();
            led_cmd.G = cmd_stack[3].toInt();
            led_cmd.B = cmd_stack[4].toInt();
            led_cmd.W = cmd_stack[5].toInt();
            led_data.set_init(PARAMETER_LED_SET_INIT, &led_cmd);
            break;
        }
    }
}

