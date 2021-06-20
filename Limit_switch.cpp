//jiwon 데이터 매핑 구조 설계 21-04-28
//외부 라이브러리 include 이유는 모르겠지만 컴파일 확인

#include "Arduino.h"
#include "control.h"
#include "main.h"
#include "servo_motor.h"
#include "step_motor.h"
#include "LED.h"
#include "Serial_port.h"
#include "Limit_switch.h"
#include "buzzer.h"
#include <string.h>
#include <Servo.h>
#include <HighPowerStepperDriver.h>
#include <iostream>
#include <vector>
#include "wiring_private.h"


LIMIT_SWITCH limit_switch{ switch_init, swInterrupt, LIMIT_MOTOR_ONE_SW_ONE_INTERRUPT, LIMIT_MOTOR_ONE_SW_TWO_INTERRUPT, LIMIT_MOTOR_TWO_SW_ONE_INTERRUPT, LIMIT_MOTOR_TWO_SW_TWO_INTERRUPT };

extern bool step_motor_one_one_imergency;
extern bool step_motor_one_two_imergency;
extern bool step_motor_two_one_imergency;
extern bool step_motor_two_two_imergency;
extern bool step_motor_three_imergency;

extern BUZZER buzzer;

void switch_init(){
    pinMode(PARAMETER_SWITCH_1, INPUT);
    pinMode(PARAMETER_LIMIT_SWITCH_1, INPUT);
    pinMode(PARAMETER_LIMIT_SWITCH_2, INPUT);
    pinMode(PARAMETER_LIMIT_SWITCH_3, INPUT);
    pinMode(PARAMETER_LIMIT_SWITCH_4, INPUT);

    attachInterrupt(digitalPinToInterrupt(PARAMETER_SWITCH_1), swInterrupt, FALLING);
    attachInterrupt(digitalPinToInterrupt(PARAMETER_LIMIT_SWITCH_1), LIMIT_MOTOR_ONE_SW_ONE_INTERRUPT, FALLING);
    attachInterrupt(digitalPinToInterrupt(PARAMETER_LIMIT_SWITCH_2), LIMIT_MOTOR_ONE_SW_TWO_INTERRUPT, FALLING);
    attachInterrupt(digitalPinToInterrupt(PARAMETER_LIMIT_SWITCH_3), LIMIT_MOTOR_TWO_SW_ONE_INTERRUPT, FALLING);
    attachInterrupt(digitalPinToInterrupt(PARAMETER_LIMIT_SWITCH_4), LIMIT_MOTOR_TWO_SW_TWO_INTERRUPT, FALLING);
}

void swInterrupt(){
    if(digitalRead(PARAMETER_SWITCH_1) != LOW) return;
    Serial5.println("WARNING : STOP SW ON!");
    buzzer.warning();
    step_motor_one_one_imergency = true;
    step_motor_one_two_imergency = true;
    step_motor_two_one_imergency = true;
    step_motor_two_two_imergency = true;
    step_motor_three_imergency = true;
}

void LIMIT_MOTOR_ONE_SW_ONE_INTERRUPT(){
    if(digitalRead(PARAMETER_LIMIT_SWITCH_1) != LOW) return;
    Serial5.println("LIMIT ONE ONE IS OCCURED");
    step_motor_one_one_imergency = true;
}

void LIMIT_MOTOR_ONE_SW_TWO_INTERRUPT(){
    if(digitalRead(PARAMETER_LIMIT_SWITCH_2) != LOW) return;
    Serial5.println("LIMIT ONE TWO IS OCCURED");
    step_motor_one_two_imergency = true;
}

void LIMIT_MOTOR_TWO_SW_ONE_INTERRUPT(){
    if(digitalRead(PARAMETER_LIMIT_SWITCH_3) != LOW) return;
    Serial5.println("LIMIT TWO ONE IS OCCURED");
    step_motor_two_one_imergency = true;
}

void LIMIT_MOTOR_TWO_SW_TWO_INTERRUPT(){
    if(digitalRead(PARAMETER_LIMIT_SWITCH_4) != LOW) return;
    Serial5.println("LIMIT TWO TWO IS OCCURED");
    step_motor_two_two_imergency = true;
}

