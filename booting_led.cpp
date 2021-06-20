#include "Arduino.h"
#include "control.h"
#include "main.h"
#include "servo_motor.h"
#include "step_motor.h"
#include "LED.h"
#include "booting_led.h"
#include <Servo.h>
#include <HighPowerStepperDriver.h>


BOOTING_LED_SET booting_led_set { set_booting_led, set_booting_led_init };
static BOOTING_LED booting_led{LOW, HIGH};
static unsigned long prevMillis = 0;

void set_booting_led(int PARAMETER){
    //현재 시간을 측정
    unsigned long currentMillis = millis();
    //측정된 시간이 이전 시간보다 1초 이상 지났다면 상태를 변환한다.
    if(currentMillis - prevMillis >= 1000){
        prevMillis = currentMillis;

        switch(PARAMETER){
            case PARAMETER_SET_BOOTING_LED_REVERSE:
                if( booting_led.booting_led_1 == LOW ){
                    booting_led.booting_led_1 = HIGH;
                }else booting_led.booting_led_1 = LOW;

                if( booting_led.booting_led_2 == LOW ){
                    booting_led.booting_led_2 = HIGH;
                }else booting_led.booting_led_2 = LOW;
            break;
        }

        digitalWrite(PARAMETER_BOOTING_LED_1, booting_led.booting_led_1);
        digitalWrite(PARAMETER_BOOTING_LED_2, booting_led.booting_led_2);
    }
}

void set_booting_led_init(){
    pinMode(PARAMETER_BOOTING_LED_1,OUTPUT);
    pinMode(PARAMETER_BOOTING_LED_2,OUTPUT);
}
