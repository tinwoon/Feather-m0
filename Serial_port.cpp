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
#include <iostream>
#include <vector>
#include "wiring_private.h"


SERIAL_CMD serial_cmd{ read_cmd, attach_cmd, serial_init, serialInput, return_check_sum, read_motor_stop };
static STEP_MOTOR step_motor_one_init{ 1, 1, 1, 0, 1300};
static STEP_MOTOR step_motor_two_init{ 2, 1, 1, 0, 1300};
static STEP_MOTOR step_motor_cmd;
static SERVO_MOTOR servo_motor_cmd;
static LED led_cmd;
static String cmd = "";
static boolean stringComplete = false;

//data driver
extern STEP_MOTOR_DATA step_motor_data;
extern SERVO_MOTOR_DATA servo_motor_data;
extern LED_DATA led_data;

//Control driver
extern SERVO_MOTOR_CONTROL servo_motor_control;
extern STEP_MOTOR_CONTROL step_motor_control;
extern LED_CONTROL led_control;

//motor state
extern bool motor_one_stepping;
extern bool motor_two_stepping;

void read_cmd(){
    std::vector< String > cmd_stack;
    int prv_token = 0;
    int length = cmd.length();
    for(int k=0; k<length; k++){
        int token = cmd.indexOf('#', prv_token);
        if(token == -1){
            String Sep_String = cmd.substring(prv_token, length);
            cmd_stack.emplace_back(Sep_String);
            prv_token = token + 1;
            break;
        }
        String Sep_String = cmd.substring(prv_token, token);
        cmd_stack.emplace_back(Sep_String);
        prv_token = token + 1;
    }

    if( cmd_stack[0].charAt(0) != '0' || cmd_stack[0].charAt(1) != 'x' || cmd_stack[0].charAt(2) != 'B' ){
        Serial5.print(cmd_stack[0].charAt(0));
        Serial5.print(cmd_stack[0].charAt(1));
        Serial5.println(cmd_stack[0].charAt(2));
        Serial5.println("Error : STX is not start bit!");
        return;
    }

    if( (cmd_stack[0].charAt(3) - '0') != cmd_stack.size()-2){
        Serial5.println(cmd_stack[0].charAt(3) - '0');
        Serial5.println(cmd_stack.size()-2);
        Serial5.println("Error : cmd length is not match!");
        return;
    }
    
    if(cmd_stack[cmd_stack.size()-1].toInt() != return_check_sum(cmd_stack)){
        Serial5.println(cmd_stack[cmd_stack.size()-1].toInt());
        Serial5.println(return_check_sum(cmd_stack));
        Serial5.println("Error : cmd checksum is not correct!");
        return;
    }

    if(cmd_stack[1] == "STM") attach_cmd(PARAMETER_STEP_MOTOR_CMD, cmd_stack);
    else if(cmd_stack[1] == "SVM") attach_cmd(PARAMETER_SERVO_MOTOR_CMD, cmd_stack);
    else if(cmd_stack[1] == "LED") attach_cmd(PARAMETER_LED_CMD, cmd_stack);

}

void attach_cmd(int PARAMETER, std::vector< String > &cmd_stack){
    switch(PARAMETER){
        case PARAMETER_STEP_MOTOR_CMD:
        {
            Serial5.println("STEP_MOTOR CMD");
            step_motor_data.get_init(PARAMETER_STEP_MOTOR_GET_INIT, &step_motor_cmd);
            step_motor_cmd.motor_id = cmd_stack[2].toInt();
            step_motor_cmd.dir = cmd_stack[3].toInt();
            step_motor_cmd.step_angle = cmd_stack[4].toInt();
            step_motor_cmd.step_rotate_num = cmd_stack[5].toInt();
            step_motor_cmd.step_delay = cmd_stack[6].toInt();
            step_motor_data.set_init(PARAMETER_STEP_MOTOR_SET_INIT, &step_motor_cmd);

            if(step_motor_cmd.dir == 3){
              Serial5.println("INIT LOCATION");
              step_motor_control.control(PARAMETER_STEP_CONTROL_ALL, &step_motor_one_init);
              step_motor_control.control(PARAMETER_STEP_CONTROL_ALL, &step_motor_two_init);
            }
            else step_motor_control.control(PARAMETER_STEP_CONTROL_ALL, &step_motor_cmd);
            break;
        }

        case PARAMETER_SERVO_MOTOR_CMD:
        {
            Serial5.println("SERVO_MOTOR CMD");
            servo_motor_data.get_init(PARAMETER_SERVO_MOTOR_GET_INIT, &servo_motor_cmd);
            servo_motor_cmd.motor_id = cmd_stack[2].toInt();
            servo_motor_cmd.angle = cmd_stack[3].toInt();
            servo_motor_data.set_init(PARAMETER_SERVO_MOTOR_SET_INIT, &servo_motor_cmd);

            servo_motor_control.control(PARAMETER_SERVO_CONTROL_ALL, &servo_motor_cmd);
            break;
        }

        case PARAMETER_LED_CMD:
        {
            Serial5.println("LED CMD");
            led_data.get_init(PARAMETER_LED_GET_INIT, &led_cmd);
            led_cmd.LED_id = cmd_stack[2].toInt();
            led_cmd.R = cmd_stack[3].toInt();
            led_cmd.G = cmd_stack[4].toInt();
            led_cmd.B = cmd_stack[5].toInt();
            led_cmd.W = cmd_stack[6].toInt();
            led_data.set_init(PARAMETER_LED_SET_INIT, &led_cmd);

            led_control.control(PARAMETER_LED_CONTROL_ALL, &led_cmd);
            break;
        }
    }
}

void serial_init(){
  //serial init
  {
    pinPeripheral(31, PIO_SERCOM_ALT);
    pinPeripheral(30, PIO_SERCOM_ALT);
    Serial5.begin(9600);
    Serial.begin(9600);
    cmd.reserve(200);
  }
}

void serialInput(){
  while(Serial5.available()){
    char inChar = (char)Serial5.read();
    cmd += inChar;
    if(inChar == '\n'){
      stringComplete = true;
    }
    if(stringComplete){
      if(!motor_one_stepping && !motor_two_stepping) serial_cmd.read_cmd();
      else serial_cmd.read_motor_stop();
      cmd = "";
      stringComplete = false;
    } 
  }
}

int return_check_sum(std::vector< String > &cmd_stack){
    int ans = 0;
    for(int i=2; i<cmd_stack.size()-1; i++){
        ans += cmd_stack[i].toInt();
    }
    return ans % 256;
}

void read_motor_stop(){
    std::vector< String > cmd_stack;
    int prv_token = 0;
    int length = cmd.length();
    for(int k=0; k<length; k++){
        int token = cmd.indexOf('#', prv_token);
        if(token == -1){
            String Sep_String = cmd.substring(prv_token, length);
            cmd_stack.emplace_back(Sep_String);
            prv_token = token + 1;
            break;
        }
        String Sep_String = cmd.substring(prv_token, token);
        cmd_stack.emplace_back(Sep_String);
        prv_token = token + 1;
    }

    if( cmd_stack[0].charAt(0) != '0' || cmd_stack[0].charAt(1) != 'x' || cmd_stack[0].charAt(2) != 'B' ){
        Serial5.print(cmd_stack[0].charAt(0));
        Serial5.print(cmd_stack[0].charAt(1));
        Serial5.println(cmd_stack[0].charAt(2));
        Serial5.println("Error : STX is not start bit!");
        return;
    }

    if( (cmd_stack[0].charAt(3) - '0') != cmd_stack.size()-2){
        Serial5.println(cmd_stack[0].charAt(3) - '0');
        Serial5.println(cmd_stack.size()-2);
        Serial5.println("Error : cmd length is not match!");
        return;
    }
    
    if(cmd_stack[cmd_stack.size()-1].toInt() != return_check_sum(cmd_stack)){
        Serial5.println(cmd_stack[cmd_stack.size()-1].toInt());
        Serial5.println(return_check_sum(cmd_stack));
        Serial5.println("Error : cmd checksum is not correct!");
        return;
    }

    if(cmd_stack[1] == "STM" && cmd_stack[3].toInt() == 0){
      if(cmd_stack[2].toInt() == 1){
        motor_one_stepping = false;
        Serial5.println("Motor one stop is occured");
      } 
      else if(cmd_stack[2].toInt() == 2){
        motor_two_stepping = false;
        Serial5.println("Motor two stop is occured");
      } 
    }else{
      Serial5.println("Error: Only motor stop command is allowed");
    }
}