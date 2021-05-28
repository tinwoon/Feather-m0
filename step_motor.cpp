//jiwon 데이터 매핑 구조 설계 21-04-28
//외부 라이브러리 include 이유는 모르겠지만 컴파일 확인

#include "Arduino.h"
#include "step_motor.h"
#include "main.h"
#include <HighPowerStepperDriver.h>


//default 값 1,0,0,0,0

STEP_MOTOR step_motor{1,1,20,100,50};

STEP_MOTOR_DATA step_motor_data{
  set_init,
  get_init,
  set,
  get,
  set_motor_id,
  set_dir,
  set_step_angle,
  set_step_rotate_num,
  set_step_delay,
  get_motor_id,
  get_dir,
  get_step_angle,
  get_step_rotate_num,
  get_step_delay,
};

//value에 &step_motor를 넣을거임
static void get_init(int PARAMETER, void* value){
  //Serial.println("get init in");
  switch(PARAMETER){
    case PARAMETER_STEP_MOTOR_GET_INIT:
      ((STEP_MOTOR*)value)-> motor_id = step_motor.motor_id;
      ((STEP_MOTOR*)value)-> dir = step_motor.dir;
      ((STEP_MOTOR*)value)-> step_angle = step_motor.step_angle;
      ((STEP_MOTOR*)value)-> step_rotate_num = step_motor.step_rotate_num;
      ((STEP_MOTOR*)value)-> step_delay = step_motor.step_delay;
    break;
  }
}

//value에 &step_motor를 넣을거임
static void set_init(int PARAMETER, void* value){
  
  switch(PARAMETER){
    case PARAMETER_STEP_MOTOR_SET_INIT:
      step_motor.motor_id =((STEP_MOTOR*)value)-> motor_id;
      step_motor.dir = ((STEP_MOTOR*)value)-> dir;
      step_motor.step_angle = ((STEP_MOTOR*)value)-> step_angle;
      step_motor.step_rotate_num = ((STEP_MOTOR*)value)-> step_rotate_num;
      step_motor.step_delay = ((STEP_MOTOR*)value)-> step_delay;
    break;
  }
}


static void set(int PARAMETER, void* value){
  //지정 함수 포인터 생성
  void (*set_para)(int* );
  
  switch(PARAMETER){
     case PARAMETER_STEP_MOTOR_ID :  
      set_para = set_motor_id;
      set_para((int*)value);
      break;
      
    case PARAMETER_STEP_MOTOR_DIR:
      set_para = set_dir;
      set_para((int*)value);
      break;
      
    case PARAMETER_STEP_MOTOR_STEP_ANGLE:
      set_para = set_step_angle;
      set_para((int*)value);
      break;
      
    case PARAMETER_STEP_MOTOR_STEP_ROTATE_NUM:
      set_para = set_step_rotate_num;
      set_para((int*)value);
      break;
      
    case PARAMETER_STEP_MOTOR_STEP_DELAY:
      set_para = set_step_delay;
      set_para((int*)value);
      break;
  }
  
  return;
}

static void get(int PARAMETER, void* value){
  //지정 함수 포인터 생성
  void (*get_para)(int* );
  
  switch(PARAMETER){
     case PARAMETER_STEP_MOTOR_ID :  
      get_para = get_motor_id;
      get_para((int*)value);
      break;
      
    case PARAMETER_STEP_MOTOR_DIR:
      get_para = get_dir;
      get_para((int*)value);
      break;
      
    case PARAMETER_STEP_MOTOR_STEP_ANGLE:
      get_para = get_step_angle;
      get_para((int*)value);
      break;
      
    case PARAMETER_STEP_MOTOR_STEP_ROTATE_NUM:
      get_para = get_step_rotate_num;
      get_para((int*)value);
      break;
      
    case PARAMETER_STEP_MOTOR_STEP_DELAY:
      get_para = get_step_delay;
      get_para((int*)value);
      break;
  }
  
  return;
}

static void set_motor_id(int* motor_id){
  step_motor.motor_id = *motor_id;
  return;
}

//dir 0 and 1 is everything
static void set_dir(int* dir){
  step_motor.dir = *dir;
  return;
}

static void set_step_angle(int* step_angle){
  step_motor.step_angle = *step_angle;
  return;
}

static void set_step_rotate_num(int* step_rotate_num){
  step_motor.step_rotate_num = *step_rotate_num;
  return;
}

static void set_step_delay(int* step_delay){
  step_motor.step_delay = *step_delay;
  return;
}

static void get_motor_id(int* motor_id){
  *motor_id = step_motor.motor_id;
  return;
}

//dir 0 and 1 is everything
static void get_dir(int* dir){
  *dir = step_motor.dir;
  return;
}

static void get_step_angle(int* step_angle){
  *step_angle = step_motor.step_angle;
  return;
}

static void get_step_rotate_num(int* step_rotate_num){
  *step_rotate_num = step_motor.step_rotate_num;
  return;
}

static void get_step_delay(int* step_delay){
  *step_delay = step_motor.step_delay;
  return;
}
