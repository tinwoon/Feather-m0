//jiwon 데이터 매핑 구조 설계 21-04-28
//외부 라이브러리 include 이유는 모르겠지만 컴파일 확인

#include "Arduino.h"
#include "servo_motor.h"
#include "main.h"
#include <HighPowerStepperDriver.h>


//default 값 1,0,0,0,0

static SERVO_MOTOR servo_motor{1,0};

static SERVO_MOTOR_DATA servo_motor_data{
  set_init,
  get_init,
  set,
  get,
  set_motor_id,
  set_angle,
  get_motor_id,
  get_angle,
};

//value에 &step_motor를 넣을거임
static void set_init(int PARAMETER, void* value){
  switch(PARAMETER){
    case PARAMETER_MOTOR_SET_INIT:
      ((SERVO_MOTOR*)value)-> motor_id = servo_motor.motor_id;
      ((SERVO_MOTOR*)value)-> angle = servo_motor.angle;
    break;
  }
}

//value에 &step_motor를 넣을거임
static void get_init(int PARAMETER, void* value){
  switch(PARAMETER){
    case PARAMETER_MOTOR_GET_INIT:
      servo_motor.motor_id =((SERVO_MOTOR*)value)-> motor_id;
      servo_motor.angle = ((SERVO_MOTOR*)value)-> angle;
    break;
  }
}


static void set(int PARAMETER, void* value){
  //지정 함수 포인터 생성
  void (*set_para)(int* );
  
  switch(PARAMETER){
     case PARAMETER_MOTOR_ID :  
      set_para = set_motor_id;
      set_para((int*)value);
      break;
      
    case PARAMETER_ANGLE:
      set_para = set_angle;
      set_para((int*)value);
      break;
  }
  
  return;
}

static void get(int PARAMETER, void* value){
  //지정 함수 포인터 생성
  void (*get_para)(int* );
  
  switch(PARAMETER){
     case PARAMETER_MOTOR_ID :  
      get_para = get_motor_id;
      get_para((int*)value);
      break;
      
    case PARAMETER_ANGLE:
      get_para = get_angle;
      get_para((int*)value);
      break;
  }
  
  return;
}

static void set_motor_id(int* motor_id){
  servo_motor.motor_id = *motor_id;
  return;
}

static void set_angle(int* angle){
  servo_motor.angle = *angle;
  return;
}


static void get_motor_id(int* motor_id){
  *motor_id = servo_motor.motor_id;
  return;
}


static void get_angle(int* angle){
  *angle = servo_motor.angle;
  return;
}
