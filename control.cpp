//jiwon 데이터 매핑 구조 설계 21-04-28

#include "Arduino.h"
#include "control.h"
#include "main.h"
#include "servo_motor.h"
#include "step_motor.h"
#include "LED.h"
#include "timer_handler.h"
#include "buzzer.h"
#include "Limit_switch.h"
#include <Servo.h>
#include <HighPowerStepperDriver.h>
#include <limits.h>

//for motor interrrupt
bool step_motor_one_one_imergency = false;
bool step_motor_one_two_imergency = false;
bool step_motor_two_one_imergency = false;
bool step_motor_two_two_imergency = false;
bool step_motor_three_imergency = false;
bool motor_one_stepping = false;
bool motor_two_stepping = false;
bool motor_three_stepping = false;

STEP_MOTOR_CONTROL step_motor_control{ control, init, controlling, control_init, control_data_update, control_imergency_init, control_stepping_init };
SERVO_MOTOR_CONTROL servo_motor_control{ control, init };
LED_CONTROL led_control{ control, init };

//timer handler
extern TIMER_HANDLER timer_handler;

//buzzer 
extern BUZZER buzzer;

static HighPowerStepperDriver step_motor[3];
static Servo servo_pan;
static Servo servo_tilt;
static NeoPixelLED neoPixelLED;

//motor 데이터 값을 저장하기 위해 필요
//단순히 저장이 아닌 값을 활용해 변경함으로 필요한 객체이다.
static STEP_MOTOR step_motor_control_data[3];
//모터의 step 회전 수를 저장하고 있는 변수
static int motor_step_count[3] = {0,0,0};

//for motor init location
//1번 모터를 1번 방향으로 Microstep1로 무한번 전송하는 코드
static STEP_MOTOR step_motor_one_init{ 1, 1, 8, INT_MAX, 100};
static STEP_MOTOR step_motor_two_init{ 2, 1, 16, INT_MAX, 100};
static STEP_MOTOR step_motor_three_init{ 3, 1, 256, 51200, 100};

void control(int PARAMETER, void* value){
    Serial5.println("control is start");
    int motor_id_index = (((STEP_MOTOR*)value)-> motor_id) - 1;
    int state;
    control_init(PARAMETER_CONTROL_INIT_BEGIN, value, motor_id_index);

    switch(PARAMETER){
        case PARAMETER_STEP_CONTROL_ALL:
            //정지
            if(step_motor_control_data[motor_id_index].dir == 0){
                Serial5.println("MOTOR STOP");
                break;
            } 

            //위치 초기화를 state로 판단해서 넣으려고 작성한 코드 => if(((STEP_MOTOR*)value)-> dir == 3) 같은 구문을 state로 판단하려고 작성한 내용이지만 사용을 하고 있지는 않다.
            //나중에 변경하기 위해 삭제는 하지 않음. 
            if(((STEP_MOTOR*)value)-> dir == 3) state = PARAMETER_CONTROL_INIT_LOC;
            else state = PARAMETER_CONTROL_NOT_INIT_LOC;

            //init location(위치 초기화)라면
            if(((STEP_MOTOR*)value)-> dir == 3){
                
                Serial5.println("INIT LOCATION");
                step_motor_control_data[0] = step_motor_one_init;
                step_motor_control_data[1] = step_motor_two_init;
                step_motor_control_data[2] = step_motor_three_init;
                motor_step_count[0] = step_motor_control_data[0].step_rotate_num;
                motor_step_count[1] = step_motor_control_data[1].step_rotate_num;
                motor_step_count[2] = step_motor_control_data[2].step_rotate_num;
                motor_one_stepping = true;
                motor_two_stepping = true;
                motor_three_stepping = true;

                //이미 리미트 스위치가 눌려있는가.(인터럽트는 falling이라 이미 눌러있는 상태에서는 발생하지 않아 진행하게 됨.)
                //init의 경우 break를 하면 안되고, 눌려진 리미트 스위치 구역에 해당하는 모터만 동작을 멈춰야한다.
                if( (digitalRead(PARAMETER_LIMIT_SWITCH_1) == LOW) && (step_motor_control_data[0].motor_id == 1) && (step_motor_control_data[0].dir == 1) ){
                    Serial5.println("Error: INIT LIMIT_SWITCH_ONE_ONE is already on");
                    buzzer.warning();
                    motor_one_stepping = false;
                    motor_step_count[0] = 0;
                }
                if( (digitalRead(PARAMETER_LIMIT_SWITCH_2) == LOW) && (step_motor_control_data[0].motor_id == 1) && (step_motor_control_data[0].dir == 2) ) {
                    Serial5.println("Error: INIT LIMIT_SWITCH_ONE_TWO is already on");
                    buzzer.warning();
                    motor_one_stepping = false;
                    motor_step_count[0] = 0;
                }
                if( (digitalRead(PARAMETER_LIMIT_SWITCH_3) == LOW) && (step_motor_control_data[1].motor_id == 2) && (step_motor_control_data[1].dir == 1) ) {
                    Serial5.println("Error: INIT LIMIT_SWITCH_TWO_ONE is already on");
                    buzzer.warning();
                    motor_two_stepping = false;
                    motor_step_count[1] = 0;
                }
                if( (digitalRead(PARAMETER_LIMIT_SWITCH_4) == LOW) && (step_motor_control_data[1].motor_id == 2) && (step_motor_control_data[1].dir == 2) ) {
                    Serial5.println("Error: INIT LIMIT_SWITCH_TWO_TWO is already on");
                    buzzer.warning();
                    motor_two_stepping = false;
                    motor_step_count[1] = 0;
                }
            }else{
                //이미 리미트 스위치가 눌려있는가.(인터럽트는 falling이라 이미 눌러있는 상태에서는 발생하지 않아 진행하게 됨.)
                if( (digitalRead(PARAMETER_LIMIT_SWITCH_1) == LOW) && (step_motor_control_data[motor_id_index].motor_id == 1) && (step_motor_control_data[motor_id_index].dir == 1) ){
                    Serial5.println("Error: LIMIT_SWITCH_ONE_ONE is already on");
                    buzzer.warning();
                    step_motor[0].disableDriver();
                    break;
                }
                if( (digitalRead(PARAMETER_LIMIT_SWITCH_2) == LOW) && (step_motor_control_data[motor_id_index].motor_id == 1) && (step_motor_control_data[motor_id_index].dir == 2) ) {
                    Serial5.println("Error: LIMIT_SWITCH_ONE_TWO is already on");
                    buzzer.warning();
                    step_motor[0].disableDriver();
                    break;
                }
                if( (digitalRead(PARAMETER_LIMIT_SWITCH_3) == LOW) && (step_motor_control_data[motor_id_index].motor_id == 2) && (step_motor_control_data[motor_id_index].dir == 1) ) {
                    Serial5.println("Error: LIMIT_SWITCH_TWO_ONE is already on");
                    buzzer.warning();
                    step_motor[1].disableDriver();
                    break;
                }
                if( (digitalRead(PARAMETER_LIMIT_SWITCH_4) == LOW) && (step_motor_control_data[motor_id_index].motor_id == 2) && (step_motor_control_data[motor_id_index].dir == 2) ) {
                    Serial5.println("Error: LIMIT_SWITCH_TWO_TWO is already on");
                    buzzer.warning();
                    step_motor[1].disableDriver();
                    break;
                }
            }

            SPI.begin();

            //set dir(DIR to SPI)
            {
                //위치 초기화의 경우
                if(((STEP_MOTOR*)value)-> dir == 3){
                    if(step_motor_control_data[0].dir == 1) step_motor[0].setDirection(STEP_MOTOR_DIR_FORWARD);
                    else if(step_motor_control_data[0].dir == 2) step_motor[0].setDirection(STEP_MOTOR_DIR_REVERSE);
                    if(step_motor_control_data[1].dir == 1) step_motor[1].setDirection(STEP_MOTOR_DIR_FORWARD);
                    else if(step_motor_control_data[1].dir == 2) step_motor[1].setDirection(STEP_MOTOR_DIR_REVERSE);
                    if(step_motor_control_data[2].dir == 1) step_motor[2].setDirection(STEP_MOTOR_DIR_FORWARD);
                    else if(step_motor_control_data[2].dir == 2) step_motor[2].setDirection(STEP_MOTOR_DIR_REVERSE);
                }
                else if(step_motor_control_data[motor_id_index].dir == 1) step_motor[motor_id_index].setDirection(STEP_MOTOR_DIR_FORWARD);
                else if(step_motor_control_data[motor_id_index].dir == 2) step_motor[motor_id_index].setDirection(STEP_MOTOR_DIR_REVERSE);
            }

            //set step for 기준각 1step 당 = 1.8도 (STEP to SPI)
            //1~256
            {
                if(((STEP_MOTOR*)value)-> dir == 3){
                    for(int k=0; k<3; k++){
                        if( step_motor_control_data[k].step_angle == 1) step_motor[k].setStepMode(HPSDStepMode::MicroStep1);
                        else if( step_motor_control_data[k].step_angle == 2) step_motor[k].setStepMode(HPSDStepMode::MicroStep2);
                        else if( step_motor_control_data[k].step_angle == 4) step_motor[k].setStepMode(HPSDStepMode::MicroStep4);
                        else if( step_motor_control_data[k].step_angle == 8) step_motor[k].setStepMode(HPSDStepMode::MicroStep8);
                        else if( step_motor_control_data[k].step_angle == 16) step_motor[k].setStepMode(HPSDStepMode::MicroStep16);
                        else if( step_motor_control_data[k].step_angle == 32) step_motor[k].setStepMode(HPSDStepMode::MicroStep32);
                        else if( step_motor_control_data[k].step_angle == 64) step_motor[k].setStepMode(HPSDStepMode::MicroStep64);
                        else if( step_motor_control_data[k].step_angle == 128) step_motor[k].setStepMode(HPSDStepMode::MicroStep128);
                        else if( step_motor_control_data[k].step_angle == 256) step_motor[k].setStepMode(HPSDStepMode::MicroStep256);
                    }
                }else{
                    // Set the number of microsteps that correspond to one full step.
                    if( step_motor_control_data[motor_id_index].step_angle == 1) step_motor[motor_id_index].setStepMode(HPSDStepMode::MicroStep1);
                    else if( step_motor_control_data[motor_id_index].step_angle == 2) step_motor[motor_id_index].setStepMode(HPSDStepMode::MicroStep2);
                    else if( step_motor_control_data[motor_id_index].step_angle == 4) step_motor[motor_id_index].setStepMode(HPSDStepMode::MicroStep4);
                    else if( step_motor_control_data[motor_id_index].step_angle == 8) step_motor[motor_id_index].setStepMode(HPSDStepMode::MicroStep8);
                    else if( step_motor_control_data[motor_id_index].step_angle == 16) step_motor[motor_id_index].setStepMode(HPSDStepMode::MicroStep16);
                    else if( step_motor_control_data[motor_id_index].step_angle == 32) step_motor[motor_id_index].setStepMode(HPSDStepMode::MicroStep32);
                    else if( step_motor_control_data[motor_id_index].step_angle == 64) step_motor[motor_id_index].setStepMode(HPSDStepMode::MicroStep64);
                    else if( step_motor_control_data[motor_id_index].step_angle == 128) step_motor[motor_id_index].setStepMode(HPSDStepMode::MicroStep128);
                    else if( step_motor_control_data[motor_id_index].step_angle == 256) step_motor[motor_id_index].setStepMode(HPSDStepMode::MicroStep256);
                }
                
            }

            // Enable the motor outputs. and 
            if(((STEP_MOTOR*)value)-> dir == 3){
                step_motor[0].enableDriver();
                step_motor[1].enableDriver();
                step_motor[2].enableDriver();
            }
            else step_motor[motor_id_index].enableDriver();

            //set angle and rotation_number and rpm
            {
                //motor_NUM_stepping은 모터의 작동 중 유무를 나타내는 변수
                if( step_motor_control_data[motor_id_index].motor_id == 1 ) motor_one_stepping = true;
                if( step_motor_control_data[motor_id_index].motor_id == 2 ) motor_two_stepping = true;
                if( step_motor_control_data[motor_id_index].motor_id == 3 ) motor_three_stepping = true;

                unsigned long prev_clock[3];
                unsigned long pres_clock;
                prev_clock[0] = prev_clock[1] = prev_clock[2] = micros();
                unsigned int stack = 0;

                while(motor_step_count[0] || motor_step_count[1] || motor_step_count[2]){
                    //10번에 한번씩만 시도해보자
                    if(stack%10 == 0){
                        //시리얼 통신을 확인해 전송된 값에 따라 flag(motor_NUM_stepping, step_motor_NUM_NUM_imergency)를 변경하는 함수
                        timer_handler.serial_interrupt();


                        //리미트 스위치 구현
                        {
                            //리미트 스위치의 동작 여부에 따라 동작 제어
                            //리미트 스위치가 눌려 있더라도 반대쪽 방향으로 이동하는 것은 허용해야 함으로 id와 dir을 확인한다.
                            if(step_motor_one_one_imergency && (step_motor_control_data[0].motor_id == 1) && (step_motor_control_data[0].dir == 1) ){
                                step_motor_one_one_imergency = false;
                                motor_one_stepping = false;
                                motor_step_count[0] = 0;
                                step_motor[0].disableDriver();
                            }
                            if(step_motor_one_two_imergency && (step_motor_control_data[0].motor_id == 1) && (step_motor_control_data[0].dir == 2) ){ 
                                step_motor_one_two_imergency = false;
                                motor_one_stepping = false;
                                motor_step_count[0] = 0;
                                step_motor[0].disableDriver();
                            } 
                            if(step_motor_two_one_imergency && (step_motor_control_data[1].motor_id == 2) && (step_motor_control_data[1].dir == 1)){ 
                                step_motor_two_one_imergency = false;
                                motor_two_stepping = false;
                                motor_step_count[1] = 0;
                                step_motor[1].disableDriver();
                            }
                            if(step_motor_two_two_imergency && (step_motor_control_data[1].motor_id == 2) && (step_motor_control_data[1].dir == 2)){ 
                                step_motor_two_two_imergency = false;
                                motor_two_stepping = false;
                                motor_step_count[1] = 0;
                                step_motor[1].disableDriver();
                            }
                            if(step_motor_three_imergency && (step_motor_control_data[2].motor_id == 3) ){
                                step_motor_three_imergency = false;
                                motor_three_stepping = false;
                                motor_step_count[2] = 0;
                                step_motor[2].disableDriver();
                            } 
                        }
                        stack = 1;
                    }
                    
                    else{
                        pres_clock = micros();
                        if(pres_clock - prev_clock[0] >= step_motor_control_data[0].step_delay){
                            //각 모터 별 동작 count가 0 초과이면 모터를 동작하도록 작성
                            if(motor_step_count[0] != 0) step_motor[0].step();
                            if(motor_step_count[0] != INT_MAX) motor_step_count[0]--;
                            prev_clock[0] = pres_clock;
                        }
                        if(pres_clock - prev_clock[1] >= step_motor_control_data[1].step_delay){
                            if(motor_step_count[1] != 0) step_motor[1].step();
                            if(motor_step_count[1] != INT_MAX) motor_step_count[1]--;
                            prev_clock[1] = pres_clock;
                        }
                        if(pres_clock - prev_clock[2] >= step_motor_control_data[2].step_delay){
                            if(motor_step_count[2] != 0) step_motor[2].step();
                            if(motor_step_count[2] != INT_MAX) motor_step_count[2]--;
                            prev_clock[2] = pres_clock;
                        }

                        //1번 모터가 동작 중이 아니거나, 동작 step count가 0보다 작다면
                        if(!motor_one_stepping || motor_step_count[0] <= 0){
                            //동작 수를 0으로 초기화 하고 1번 모터를 중지 시켜 전류 차단
                            motor_step_count[0] = 0;
                            step_motor[0].disableDriver();
                        } 
                        if(!motor_two_stepping || motor_step_count[1] <= 0){
                            motor_step_count[1] = 0;
                            step_motor[1].disableDriver();
                        }
                        if(!motor_three_stepping || motor_step_count[2] <= 0){
                            motor_step_count[2] = 0;
                            step_motor[2].disableDriver();
                        } 
                        stack++;
                    }
                    
                }

            }
            break;   

        case PARAMETER_SERVO_CONTROL_ALL:
        {
            //set servo motor id
            {
                servo_pan.attach(SERVO_MOTOR_PAN,1000,2000);
                servo_tilt.attach(SERVO_MOTOR_TILLT,1000,2000);
            }     
            //set servo angle
            {   
                Serial5.print("angle is: ");
                Serial5.println(((SERVO_MOTOR*)value)-> angle);
                if(((SERVO_MOTOR*)value)-> motor_id==1) servo_pan.write(((SERVO_MOTOR*)value)-> angle);
                else if(((SERVO_MOTOR*)value)-> motor_id==2) servo_tilt.write(((SERVO_MOTOR*)value)-> angle);
                //if(((SERVO_MOTOR*)value)-> motor_id==1) servo_pan.writeMicroseconds( 1000 + ( (((SERVO_MOTOR*)value)->angle)*(float)(1000/180) ));
                //else if(((SERVO_MOTOR*)value)-> motor_id==2) servo_tilt.writeMicroseconds( 1000 + ( (((SERVO_MOTOR*)value)->angle)*(float)(1000/180) ));
                delay(1000);
                if(((SERVO_MOTOR*)value)-> motor_id==1) servo_pan.detach();
                else if(((SERVO_MOTOR*)value)-> motor_id==2) servo_tilt.detach();
            }
            break;
        }

        case PARAMETER_LED_CONTROL_ALL:
        {
            //set led motor id
            {
                Serial5.println("LED CMD");
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
                neoPixelLED.setBrightness(100);
                neoPixelLED.clear();
                for(int led_num = 0; led_num < 90; led_num++){
                    neoPixelLED.setColor(led_num, ((LED*)value)->R, ((LED*)value)->G, ((LED*)value)->B, ((LED*)value)->W );
                }
                neoPixelLED.show();
            }
            break;
        }
    }

    control_init(PARAMETER_CONTROL_INIT_END, value, motor_id_index);

}

void init(int PARAMETER){
    Serial5.println("init is start");

    switch(PARAMETER){
        case PARAMETER_STEP_CONTROL_INIT_ALL:
        {
            //init step motor 1
            {
                SPI.begin();

                step_motor[0].setChipSelectPin(STEP_MOTOR_ID_1);
                delay(1);
                step_motor[0].resetSettings();
                step_motor[0].clearStatus();
                //the other setting
                // Select auto mixed decay.  TI's DRV8711 documentation recommends this mode
                // for most applications, and we find that it usually works well.   
                step_motor[0].setDecayMode(HPSDDecayMode::AutoMixed);

                // Set the current limit. You should change the number here to an appropriate
                // value for your particular system.
                step_motor[0].setCurrentMilliamps36v4(3000);

                // Set the number of microsteps that correspond to one full step.
                step_motor[0].setStepMode(HPSDStepMode::MicroStep1);

                // Enable the motor outputs. 
                step_motor[0].enableDriver();

                step_motor[0].setDirection(0);
                for(unsigned int x = 0; x < 100; x++)
                {
                    if( (digitalRead(PARAMETER_LIMIT_SWITCH_1) == HIGH) && (digitalRead(PARAMETER_LIMIT_SWITCH_2) == HIGH) ) step_motor[0].step();
                    delayMicroseconds(1300);
                }

                // Wait for 300 ms.
                delay(300);

                // Step in the other direction 1000 times.
                step_motor[0].setDirection(1);
                for(unsigned int x = 0; x < 100; x++)
                {
                    if((digitalRead(PARAMETER_LIMIT_SWITCH_1) == HIGH) && (digitalRead(PARAMETER_LIMIT_SWITCH_2) == HIGH) ) step_motor[0].step();
                    delayMicroseconds(1300);
                }

                // Wait for 300 ms.
                delay(300);
                step_motor[0].disableDriver();
            }

            //init step motor 2
            {
                SPI.begin();

                step_motor[1].setChipSelectPin(STEP_MOTOR_ID_2);
                delay(1);
                step_motor[1].resetSettings();
                step_motor[1].clearStatus();
                //the other setting
                // Select auto mixed decay.  TI's DRV8711 documentation recommends this mode
                // for most applications, and we find that it usually works well.   
                step_motor[1].setDecayMode(HPSDDecayMode::AutoMixed);    

                // Set the current limit. You should change the number here to an appropriate
                // value for your particular system.
                step_motor[1].setCurrentMilliamps36v4(3000);

                // Set the number of microsteps that correspond to one full step.
                step_motor[1].setStepMode(HPSDStepMode::MicroStep1);

                // Enable the motor outputs. 
                step_motor[1].enableDriver();

                step_motor[1].setDirection(0);
                for(unsigned int x = 0; x < 100; x++)
                {
                    if( (digitalRead(PARAMETER_LIMIT_SWITCH_3) == HIGH) && (digitalRead(PARAMETER_LIMIT_SWITCH_4) == HIGH) ) step_motor[1].step();
                    delayMicroseconds(1300);
                }

                // Wait for 300 ms.
                delay(300);

                // Step in the other direction 1000 times.
                step_motor[1].setDirection(1);
                for(unsigned int x = 0; x < 100; x++)
                {
                    if( (digitalRead(PARAMETER_LIMIT_SWITCH_3) == HIGH) && (digitalRead(PARAMETER_LIMIT_SWITCH_4) == HIGH) ) step_motor[1].step();
                    delayMicroseconds(1300);
                }

                // Wait for 300 ms.
                delay(300);
                step_motor[1].disableDriver();
            }

            //init step motor 3
            {
                SPI.begin();

                step_motor[2].setChipSelectPin(STEP_MOTOR_ID_3);
                delay(1);
                step_motor[2].resetSettings();
                step_motor[2].clearStatus();
                //the other setting
                // Select auto mixed decay.  TI's DRV8711 documentation recommends this mode
                // for most applications, and we find that it usually works well.   
                step_motor[2].setDecayMode(HPSDDecayMode::AutoMixed);

                // Set the current limit. You should change the number here to an appropriate
                // value for your particular system.
                step_motor[2].setCurrentMilliamps36v4(3000);

                // Set the number of microsteps that correspond to one full step.
                step_motor[2].setStepMode(HPSDStepMode::MicroStep1);

                // Enable the motor outputs. 
                step_motor[2].enableDriver();

                step_motor[2].setDirection(0);
                for(unsigned int x = 0; x < 100; x++)
                {
                    step_motor[2].step();
                    delayMicroseconds(2000);
                }

                // Wait for 300 ms.
                delay(300);

                // Step in the other direction 1000 times.
                step_motor[2].setDirection(1);
                for(unsigned int x = 0; x < 100; x++)
                {
                    step_motor[2].step();
                    delayMicroseconds(2000);
                }

                // Wait for 300 ms.
                delay(300);
                step_motor[2].disableDriver();
            }
            break;   
        }
            
            

        case PARAMETER_SERVO_CONTROL_INIT_ALL:
        {
            //init servo motor id
            {
                servo_pan.attach(SERVO_MOTOR_PAN, 1000, 2000);
                servo_tilt.attach(SERVO_MOTOR_TILLT, 1000, 2000);

                for(int k=0; k<=110; k +=10){
                    servo_pan.write(k);
                    delay(70);
                }
                servo_pan.detach();
                
                delay(1000);

                for(int k=0; k<=90; k +=10){
                    servo_tilt.write(k);
                    delay(70);
                }
                servo_tilt.detach();
            }
            break;
        }

        case PARAMETER_LED_CONTROL_INIT_ALL:
        {
            //init led
            /*
            {
                neoPixelLED.init(LED_DATA1, LED_NUM);
                neoPixelLED.init(LED_DATA2, LED_NUM);
                neoPixelLED.setBrightness(255);
                neoPixelLED.clear();
                for(int led_num = 0; led_num < LED_NUM; led_num++){
                    neoPixelLED.setColor(led_num, 0, 255, 255, 255 );
                }
                neoPixelLED.show();
            }
            */
            break;
        }
    }
}

void controlling(int PARAMETER, void* value){
    Serial5.println("controlling is start");
    int motor_id_index = (((STEP_MOTOR*)value)-> motor_id) - 1;
    step_motor_control_data[motor_id_index].motor_id = ((STEP_MOTOR*)value)->motor_id;
    step_motor_control_data[motor_id_index].dir = ((STEP_MOTOR*)value)->dir;
    step_motor_control_data[motor_id_index].step_angle = ((STEP_MOTOR*)value)->step_angle;
    step_motor_control_data[motor_id_index].step_rotate_num = ((STEP_MOTOR*)value)-> step_rotate_num == 0 ? INT_MAX:((STEP_MOTOR*)value)-> step_rotate_num;
    step_motor_control_data[motor_id_index].step_delay = ((STEP_MOTOR*)value)->step_delay;
    motor_step_count[motor_id_index] = step_motor_control_data[motor_id_index].step_rotate_num;

    switch(PARAMETER){
        case PARAMETER_STEP_CONTROL_ALL:
        //정지
            if(((STEP_MOTOR*)value)-> dir == 0){
                Serial5.println("MOTOR STOP");
                break;
            } 

            //이미 리미트 스위치가 눌려있는가.(인터럽트는 falling이라 이미 눌러있는 상태에서는 발생하지 않아 진행하게 됨.)
            if( (digitalRead(PARAMETER_LIMIT_SWITCH_1) == LOW) && (((STEP_MOTOR*)value)-> motor_id == 1) &&  (((STEP_MOTOR*)value)-> dir == 1)) {
                Serial5.println("Error: LIMIT_SWITCH_ONE_ONE is already on");
                buzzer.warning();
                break;
            }
            if( (digitalRead(PARAMETER_LIMIT_SWITCH_2) == LOW) && (((STEP_MOTOR*)value)-> motor_id == 1) &&  (((STEP_MOTOR*)value)-> dir == 2)) {
                Serial5.println("Error: LIMIT_SWITCH_ONE_TWO is already on");
                buzzer.warning();
                break;
            }
            if( (digitalRead(PARAMETER_LIMIT_SWITCH_3) == LOW) && (((STEP_MOTOR*)value)-> motor_id == 2) &&  (((STEP_MOTOR*)value)-> dir == 1)) {
                Serial5.println("Error: LIMIT_SWITCH_TWO_ONE is already on");
                buzzer.warning();
                break;
            }
            if( (digitalRead(PARAMETER_LIMIT_SWITCH_4) == LOW) && (((STEP_MOTOR*)value)-> motor_id == 2) &&  (((STEP_MOTOR*)value)-> dir == 2)) {
                Serial5.println("Error: LIMIT_SWITCH_TWO_TWO is already on");
                buzzer.warning();
                break;
            }

            SPI.begin();
            //set motor id(CS or SCS pin init)
            {
                if(((STEP_MOTOR*)value)-> motor_id == 1) step_motor[0].setChipSelectPin(STEP_MOTOR_ID_1);
                else if(((STEP_MOTOR*)value)-> motor_id == 2) step_motor[1].setChipSelectPin(STEP_MOTOR_ID_2);
                else if(((STEP_MOTOR*)value)-> motor_id == 3) step_motor[2].setChipSelectPin(STEP_MOTOR_ID_3);
                // Give the driver some time to power up.
                delay(1);
                step_motor[motor_id_index].resetSettings();
                step_motor[motor_id_index].clearStatus();
            }

            //set dir(DIR to SPI)
            {
                if(((STEP_MOTOR*)value)-> dir == 1) step_motor[motor_id_index].setDirection(STEP_MOTOR_DIR_FORWARD);
                else if(((STEP_MOTOR*)value)-> dir == 2) step_motor[motor_id_index].setDirection(STEP_MOTOR_DIR_REVERSE);
            }

            //set step for 기준각 1step 당 = 1.8도 (STEP to SPI)
            //1~256
            {
                // Set the number of microsteps that correspond to one full step.
                if( ((STEP_MOTOR*)value)-> step_angle == 1) step_motor[motor_id_index].setStepMode(HPSDStepMode::MicroStep1);
                else if( ((STEP_MOTOR*)value)-> step_angle == 2) step_motor[motor_id_index].setStepMode(HPSDStepMode::MicroStep2);
                else if( ((STEP_MOTOR*)value)-> step_angle == 4) step_motor[motor_id_index].setStepMode(HPSDStepMode::MicroStep4);
                else if( ((STEP_MOTOR*)value)-> step_angle == 8) step_motor[motor_id_index].setStepMode(HPSDStepMode::MicroStep8);
                else if( ((STEP_MOTOR*)value)-> step_angle == 16) step_motor[motor_id_index].setStepMode(HPSDStepMode::MicroStep16);
                else if( ((STEP_MOTOR*)value)-> step_angle == 32) step_motor[motor_id_index].setStepMode(HPSDStepMode::MicroStep32);
                else if( ((STEP_MOTOR*)value)-> step_angle == 64) step_motor[motor_id_index].setStepMode(HPSDStepMode::MicroStep64);
                else if( ((STEP_MOTOR*)value)-> step_angle == 128) step_motor[motor_id_index].setStepMode(HPSDStepMode::MicroStep128);
                else if( ((STEP_MOTOR*)value)-> step_angle == 256) step_motor[motor_id_index].setStepMode(HPSDStepMode::MicroStep256);
            }

            //the other setting
            // Select auto mixed decay.  TI's DRV8711 documentation recommends this mode
            // for most applications, and we find that it usually works well.   
            step_motor[motor_id_index].setDecayMode(HPSDDecayMode::AutoMixed);

            // Set the current limit. You should change the number here to an appropriate
            // value for your particular system.
            step_motor[motor_id_index].setCurrentMilliamps36v4(3000);
            // Enable the motor outputs. and 
            step_motor[motor_id_index].enableDriver();

            //set motor id => 모터 동작 중의 유무
            {
                if( ((STEP_MOTOR*)value)-> motor_id == 1 ){
                    motor_one_stepping = true;
                } 
                if( ((STEP_MOTOR*)value)-> motor_id == 2 ){
                    motor_two_stepping = true;
                } 
                if( ((STEP_MOTOR*)value)-> motor_id == 3 ){
                    motor_three_stepping = true;
                } 
            }
    }
}

void control_init(int PARAMETER, void* value, int motor_id_index){

    control_data_update(value, motor_id_index);

    switch(PARAMETER){
        case PARAMETER_CONTROL_INIT_BEGIN:
            control_imergency_init();
            //이론적으로는 control_stepping_init() 도 넣는게 맞지만
            //코드 논리 상으론 동작 중이지 않은 상태에서 control이 수행될 리가 없으므로 넣지 않았다.
            //control_step_motor_data_init()도 마찬가지
            break;
        case PARAMETER_CONTROL_INIT_END:
            control_imergency_init();
            control_stepping_init();
            control_step_motor_data_init();
            break;
    }
}

void control_data_update(void* value, int motor_id_index){
    step_motor_control_data[motor_id_index].motor_id = ((STEP_MOTOR*)value)->motor_id;
    step_motor_control_data[motor_id_index].dir = ((STEP_MOTOR*)value)->dir;
    step_motor_control_data[motor_id_index].step_angle = ((STEP_MOTOR*)value)->step_angle;
    step_motor_control_data[motor_id_index].step_rotate_num = ((STEP_MOTOR*)value)-> step_rotate_num == 0 ? INT_MAX:((STEP_MOTOR*)value)-> step_rotate_num;
    step_motor_control_data[motor_id_index].step_delay = ((STEP_MOTOR*)value)->step_delay;
    motor_step_count[motor_id_index] = step_motor_control_data[motor_id_index].step_rotate_num;
}

void control_imergency_init(){
    //맨 처음에 imergency 버튼을 누르고 시작하면 imergency가 true로 되어 있어 동작을 한번 무시하는 버그가 있었다.
    //따라서, 기존 imergency 값을 모두 false로 초기화하고 시작한다.(어차피 동작중에만 확인하면 된다.)
    {
        step_motor_one_one_imergency = false;
        step_motor_one_two_imergency = false;
        step_motor_two_one_imergency = false;
        step_motor_two_two_imergency = false;
        step_motor_three_imergency = false;
    }
}

void control_stepping_init(){
    //init 동작중 flag 초기화
    {
        motor_one_stepping = false;
        motor_two_stepping = false;
        motor_three_stepping = false;
    }
}

void control_step_motor_data_init(){
    //control data 초기화
    {
        memset(step_motor_control_data, 0, sizeof(step_motor_control_data));
        memset(motor_step_count, 0, sizeof(motor_step_count));
    }
}
