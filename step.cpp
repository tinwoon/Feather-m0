//jiwon 데이터 매핑 구조 설계 21-04-28
//외부 라이브러리 include 이유는 모르겠지만 컴파일 확인

#include "Arduino.h"
#include "step.h"
#include "main.h"
#include <HighPowerStepperDriver.h>


//default 값 1,0,0,0,0
static STEP_MOTOR step_motor;
static STEP_MOTOR_CONFIG step_motor_config;

//value에 &step_motor를 넣을거임
void set_init(int PARAMETER, void* value){

  switch(PARAMETER){
    case PARAMETER_SET_INIT:
      ((STEP_MOTOR*)value)-> motor_id = step_motor.motor_id;
      ((STEP_MOTOR*)value)-> dir = step_motor.dir;
      ((STEP_MOTOR*)value)-> angle = step_motor.angle;
      ((STEP_MOTOR*)value)-> rotation_number = step_motor.rotation_number;
      ((STEP_MOTOR*)value)-> rpm = step_motor.rpm;
    break;
  }
}

//value에 &step_motor를 넣을거임
void get_init(int PARAMETER, void* value){
  
  switch(PARAMETER){
    case PARAMETER_GET_INIT:
      step_motor.motor_id =((STEP_MOTOR*)value)-> motor_id;
      step_motor.dir = ((STEP_MOTOR*)value)-> dir;
      step_motor.angle = ((STEP_MOTOR*)value)-> angle;
      step_motor.rotation_number = ((STEP_MOTOR*)value)-> rotation_number;
      step_motor.rpm = ((STEP_MOTOR*)value)-> rpm;
    break;
  }
}


void set(int PARAMETER, void* value){
  //지정 함수 포인터 생성
  void (*set_para)(int* );
  
  switch(PARAMETER){
     case PARAMETER_MOTOR_ID :  
      set_para = set_motor_id;
      set_para((int*)value);
      break;
      
    case PARAMETER_DIR:
      set_para = set_dir;
      set_para((int*)value);
      break;
      
    case PARAMETER_ANGLE:
      set_para = set_angle;
      set_para((int*)value);
      break;
      
    case PARAMETER_ROTATE:
      set_para = set_rotate;
      set_para((int*)value);
      break;
      
    case PARAMETER_RPM:
      set_para = set_rpm;
      set_para((int*)value);
      break;
  }
  
  return;
}

void get(int PARAMETER, void* value){
  //지정 함수 포인터 생성
  void (*get_para)(int* );
  
  switch(PARAMETER){
     case PARAMETER_MOTOR_ID :  
      get_para = get_motor_id;
      get_para((int*)value);
      break;
      
    case PARAMETER_DIR:
      get_para = get_dir;
      get_para((int*)value);
      break;
      
    case PARAMETER_ANGLE:
      get_para = get_angle;
      get_para((int*)value);
      break;
      
    case PARAMETER_ROTATE:
       get_para = get_rotate;
      get_para((int*)value);
      break;
      
    case PARAMETER_RPM:
       get_para = get_rpm;
      get_para((int*)value);
      break;
  }
  
  return;
}

void set_motor_id(int* motor_id){
  step_motor.motor_id = *motor_id;
  return;
}

//dir 0 and 1 is everything
void set_dir(int* dir){
  step_motor.dir = *dir;
  return;
}

void set_angle(int* angle){
  step_motor.angle = *angle;
  return;
}

void set_rotate(int* rotation_number){
  step_motor.rotation_number = *rotation_number;
  return;
}

void set_rpm(int* rpm){
  step_motor.rpm = *rpm;
  return;
}

void get_motor_id(int* motor_id){
  *motor_id = step_motor.motor_id;
  return;
}

//dir 0 and 1 is everything
void get_dir(int* dir){
  *dir = step_motor.dir;
  return;
}

void get_angle(int* angle){
  *angle = step_motor.angle;
  return;
}

void get_rotate(int* rotation_number){
  *rotation_number = step_motor.rotation_number;
  return;
}

void get_rpm(int* rpm){
  *rpm = step_motor.rpm;
  return;
}
