#include "Arduino.h"
#include "control.h"
#include "main.h"
#include "servo_motor.h"
#include "step_motor.h"
#include "LED.h"
#include "buzzer.h"
#include <Servo.h>
#include <HighPowerStepperDriver.h>
#include <limits.h>

BUZZER buzzer{ buzzer_init, warning };

//tone 명령어는 상태 실행 후 프로세스를 잡고 있지 않기때문에 기다려야 한다.
void buzzer_init(){
    tone(PARAMETER_BUZZER_PIN, 880, 100);
    delay(100);
    tone(PARAMETER_BUZZER_PIN, 1760, 100);
}

void warning(){
    tone(PARAMETER_BUZZER_PIN, 4000, 700);
}