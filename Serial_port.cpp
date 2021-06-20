//jiwon 데이터 매핑 구조 설계 21-04-28
//외부 라이브러리 include 이유는 모르겠지만 컴파일 확인

#include "Arduino.h"
#include "control.h"
#include "main.h"
#include "servo_motor.h"
#include "step_motor.h"
#include "LED.h"
#include "Serial_port.h"
#include "buzzer.h"
#include <string.h>
#include <Servo.h>
#include <HighPowerStepperDriver.h>
#include <iostream>
#include <vector>
#include "wiring_private.h"


SERIAL_CMD serial_cmd{ read_cmd, attach_cmd, serial_init, serialInput, return_check_sum, read_motor_stepping, serial_interrupt_Input };

static STEP_MOTOR step_motor_cmd;
static SERVO_MOTOR servo_motor_cmd;
static LED led_cmd;
static String cmd = "";
static String interrupt_cmd = "";
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
extern bool motor_three_stepping;

//buzzer 
extern BUZZER buzzer;

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

    if( cmd_stack[0].charAt(0) != 'S' || cmd_stack[0].charAt(1) != 'T' || cmd_stack[0].charAt(2) != 'X' ){
        Serial5.print(cmd_stack[0].charAt(0));
        Serial5.print(cmd_stack[0].charAt(1));
        Serial5.println(cmd_stack[0].charAt(2));
        Serial5.println("Error : STX is not match start!");
        buzzer.warning();
        return;
    }

    if( cmd_stack[cmd_stack.size()-1].charAt(0) != 'E' || cmd_stack[cmd_stack.size()-1].charAt(1) != 'T' || cmd_stack[cmd_stack.size()-1].charAt(2) != 'X' ){
        Serial5.print(cmd_stack[cmd_stack.size()-1].charAt(0));
        Serial5.print(cmd_stack[cmd_stack.size()-1].charAt(1));
        Serial5.println(cmd_stack[cmd_stack.size()-1].charAt(2));
        Serial5.println("Error : ETX is not match end!");
        buzzer.warning();
        return;
    }

    if( (cmd_stack[1].toInt()) != cmd_stack.size()-4) {
        Serial5.println(cmd_stack[1].toInt());
        Serial5.println(cmd_stack.size()-4);
        Serial5.println("Error : cmd length is not match!");
        buzzer.warning();
        return;
    }
    
    if(cmd_stack[cmd_stack.size()-2].toInt() != return_check_sum(cmd_stack)){
        Serial5.println(cmd_stack[cmd_stack.size()-2].toInt());
        Serial5.println(return_check_sum(cmd_stack));
        Serial5.println("Error : cmd checksum is not correct!");
        buzzer.warning();
        return;
    }

    if(cmd_stack[2] == "STM") attach_cmd(PARAMETER_STEP_MOTOR_CMD, cmd_stack);
    else if(cmd_stack[2] == "SVM") attach_cmd(PARAMETER_SERVO_MOTOR_CMD, cmd_stack);
    else if(cmd_stack[2] == "LED") attach_cmd(PARAMETER_LED_CMD, cmd_stack);

}

void attach_cmd(int PARAMETER, std::vector< String > &cmd_stack){
    switch(PARAMETER){
        case PARAMETER_STEP_MOTOR_CMD:
        {
            Serial5.println("STEP_MOTOR CMD");
            step_motor_data.get_init(PARAMETER_STEP_MOTOR_GET_INIT, &step_motor_cmd);
            step_motor_cmd.motor_id = cmd_stack[3].toInt();
            step_motor_cmd.dir = cmd_stack[4].toInt();
            step_motor_cmd.step_angle = cmd_stack[5].toInt();
            step_motor_cmd.step_rotate_num = cmd_stack[6].toInt();
            step_motor_cmd.step_delay = cmd_stack[7].toInt();
            step_motor_data.set_init(PARAMETER_STEP_MOTOR_SET_INIT, &step_motor_cmd);

            
            step_motor_control.control(PARAMETER_STEP_CONTROL_ALL, &step_motor_cmd);
            break;
        }

        case PARAMETER_SERVO_MOTOR_CMD:
        {
            Serial5.println("SERVO_MOTOR CMD");
            servo_motor_data.get_init(PARAMETER_SERVO_MOTOR_GET_INIT, &servo_motor_cmd);
            servo_motor_cmd.motor_id = cmd_stack[3].toInt();
            servo_motor_cmd.angle = cmd_stack[4].toInt();
            servo_motor_data.set_init(PARAMETER_SERVO_MOTOR_SET_INIT, &servo_motor_cmd);

            servo_motor_control.control(PARAMETER_SERVO_CONTROL_ALL, &servo_motor_cmd);
            break;
        }

        case PARAMETER_LED_CMD:
        {
            Serial5.println("LED CMD");
            led_data.get_init(PARAMETER_LED_GET_INIT, &led_cmd);
            led_cmd.LED_id = cmd_stack[3].toInt();
            led_cmd.R = cmd_stack[4].toInt();
            led_cmd.G = cmd_stack[5].toInt();
            led_cmd.B = cmd_stack[6].toInt();
            led_cmd.W = cmd_stack[7].toInt();
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
    interrupt_cmd.reserve(200);
  }
}

void serialInput(){
  while(Serial5.available()){
    char inChar = (char)Serial5.read();
    cmd += inChar;
    if(inChar == '\n'){
      cmd.trim();
      stringComplete = true;
    }
    if(stringComplete){
      if(!motor_one_stepping && !motor_two_stepping && !motor_three_stepping) serial_cmd.read_cmd();
      else serial_cmd.read_motor_stepping();
      cmd = "";
      stringComplete = false;
    } 
  }
}

void serial_interrupt_Input(){
  while(Serial5.available()){
    char inChar = (char)Serial5.read();
    interrupt_cmd += inChar;
    if(inChar == '\n'){
      interrupt_cmd.trim();
      if(motor_one_stepping || motor_two_stepping || motor_three_stepping) serial_cmd.read_motor_stepping();
      interrupt_cmd = "";
    }
  }
}

int return_check_sum(std::vector< String > &cmd_stack){
    int ans = 0;
    for(int i=3; i<cmd_stack.size()-2; i++){
        ans += cmd_stack[i].toInt();
    }
    return ans % 256;
}

void read_motor_stepping(){
    std::vector< String > cmd_stack;
    int prv_token = 0;
    int length = interrupt_cmd.length();
    for(int k=0; k<length; k++){
        int token = interrupt_cmd.indexOf('#', prv_token);
        if(token == -1){
            String Sep_String = interrupt_cmd.substring(prv_token, length);
            cmd_stack.emplace_back(Sep_String);
            prv_token = token + 1;
            break;
        }
        String Sep_String = interrupt_cmd.substring(prv_token, token);
        cmd_stack.emplace_back(Sep_String);
        prv_token = token + 1;
    }

    if( cmd_stack[0].charAt(0) != 'S' || cmd_stack[0].charAt(1) != 'T' || cmd_stack[0].charAt(2) != 'X' ){
        Serial5.print(cmd_stack[0].charAt(0));
        Serial5.print(cmd_stack[0].charAt(1));
        Serial5.println(cmd_stack[0].charAt(2));
        Serial5.println("Error : STX is not match start!");
        buzzer.warning();
        return;
    }

    if( cmd_stack[cmd_stack.size()-1].charAt(0) != 'E' || cmd_stack[cmd_stack.size()-1].charAt(1) != 'T' || cmd_stack[cmd_stack.size()-1].charAt(2) != 'X' ){
        Serial5.print(cmd_stack[cmd_stack.size()-1].charAt(0));
        Serial5.print(cmd_stack[cmd_stack.size()-1].charAt(1));
        Serial5.println(cmd_stack[cmd_stack.size()-1].charAt(2));
        Serial5.println("Error : ETX is not match end!");
        buzzer.warning();
        return;
    }

    if( (cmd_stack[1].toInt()) != cmd_stack.size()-4) {
        Serial5.println(cmd_stack[1].toInt());
        Serial5.println(cmd_stack.size()-4);
        Serial5.println("Error : cmd length is not match!");
        buzzer.warning();
        return;
    }
    
    if(cmd_stack[cmd_stack.size()-2].toInt() != return_check_sum(cmd_stack)){
        Serial5.println(cmd_stack[cmd_stack.size()-2].toInt());
        Serial5.println(return_check_sum(cmd_stack));
        Serial5.println("Error : cmd checksum is not correct!");
        buzzer.warning();
        return;
    }

    if(cmd_stack[2] == "STM" && cmd_stack[4].toInt() == 0){
      if(cmd_stack[3].toInt() == 1){
        motor_one_stepping = false;
        Serial5.println("Motor one stop is occured");
      } 
      else if(cmd_stack[3].toInt() == 2){
        motor_two_stepping = false;
        Serial5.println("Motor two stop is occured");
      } 
      else if(cmd_stack[3].toInt() == 3){
        motor_three_stepping = false;
        Serial5.println("Motor three stop is occured");
      } 
    }

    else if(cmd_stack[2] == "STM"){
      if(cmd_stack[3].toInt() == 1){
        Serial5.println("Motor one move is occured");
      } 
      else if(cmd_stack[3].toInt() == 2){
        Serial5.println("Motor two move is occured");
      } 
      else if(cmd_stack[3].toInt() == 3){
        Serial5.println("Motor three move is occured");
      } 

      step_motor_data.get_init(PARAMETER_STEP_MOTOR_GET_INIT, &step_motor_cmd);
      step_motor_cmd.motor_id = cmd_stack[3].toInt();
      step_motor_cmd.dir = cmd_stack[4].toInt();
      step_motor_cmd.step_angle = cmd_stack[5].toInt();
      step_motor_cmd.step_rotate_num = cmd_stack[6].toInt();
      step_motor_cmd.step_delay = cmd_stack[7].toInt();
      step_motor_data.set_init(PARAMETER_STEP_MOTOR_SET_INIT, &step_motor_cmd);

      if(step_motor_cmd.dir == 3){
        Serial5.println("INIT LOCATION is Impossible while the motor is running");
        buzzer.warning();
      }
      else step_motor_control.controlling(PARAMETER_STEP_CONTROL_ALL, &step_motor_cmd);
    }
    else{
        Serial5.println("Error: Only motor stop and move command is allowed");
        buzzer.warning();
    }
}