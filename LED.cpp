
//jiwon 데이터 매핑 구조 설계 21-04-28
//외부 라이브러리 include 이유는 모르겠지만 컴파일 확인

#include "Arduino.h"
#include "LED.h"
#include "main.h"
#include <HighPowerStepperDriver.h>


//default 값 1,0,0,0,0

static LED led{1,0,0,0,0};

static LED_DATA led_data{
  set_init,
  get_init,
  set,
  get,
  set_LED_id,
  set_LED_R,
  set_LED_G,
  set_LED_B,
  set_LED_W,
  get_LED_id,
  get_LED_R,
  get_LED_G,
  get_LED_B,
  get_LED_W,
};

//value에 &step_motor를 넣을거임
static void set_init(int PARAMETER, void* value){

  switch(PARAMETER){
    case PARAMETER_LED_SET_INIT:
      ((LED*)value)-> LED_id = led.LED_id;
      ((LED*)value)-> R = led.R;
      ((LED*)value)-> G = led.G;
      ((LED*)value)-> B = led.B;
      ((LED*)value)-> W = led.W;
    break;
  }
}

//value에 &step_motor를 넣을거임
static void get_init(int PARAMETER, void* value){

  switch(PARAMETER){
    case PARAMETER_LED_GET_INIT:
      led.LED_id = ((LED*)value)-> LED_id;
      led.R = ((LED*)value)-> R;
      led.G = ((LED*)value)-> G;
      led.B = ((LED*)value)-> B;
      led.W = ((LED*)value)-> W;
    break;
  }
}


static void set(int PARAMETER, void* value){
  //지정 함수 포인터 생성
  void (*set_para)(int* );
  
  switch(PARAMETER){
     case PARAMETER_LED_ID :  
      set_para = set_LED_id;
      set_para((int*)value);
      break;
      
    case PARAMETER_LED_R:
      set_para = set_LED_R;
      set_para((int*)value);
      break;
      
    case PARAMETER_LED_G:
      set_para = set_LED_G;
      set_para((int*)value);
      break;
      
    case PARAMETER_LED_B:
      set_para = set_LED_B;
      set_para((int*)value);
      break;
      
    case PARAMETER_LED_W:
      set_para = set_LED_W;
      set_para((int*)value);
      break;
  }
  
  return;
}

static void get(int PARAMETER, void* value){
  //지정 함수 포인터 생성
  void (*get_para)(int* );
  
  switch(PARAMETER){
     case PARAMETER_LED_ID :  
      get_para = get_LED_id;
      get_para((int*)value);
      break;
      
    case PARAMETER_LED_R:
      get_para = get_LED_R;
      get_para((int*)value);
      break;
      
    case PARAMETER_LED_G:
      get_para = get_LED_G;
      get_para((int*)value);
      break;
      
    case PARAMETER_LED_B:
      get_para = get_LED_B;
      get_para((int*)value);
      break;
      
    case PARAMETER_LED_W:
      get_para = get_LED_W;
      get_para((int*)value);
      break;
  }
  
  return;
}

static void set_LED_id(int* LED_id){
  led.LED_id = *LED_id;
  return;
}

//dir 0 and 1 is everything
static void set_LED_R(int* R){
  led.R = *R;
  return;
}

static void set_LED_G(int* G){
  led.G = *G;
  return;
}

static void set_LED_B(int* B){
  led.B = *B;
  return;
}

static void set_LED_W(int* W){
  led.W = *W;
  return;
}

static void get_LED_id(int* LED_id){
  *LED_id = led.LED_id;
  return;
}

//dir 0 and 1 is everything
static void get_LED_R(int* R){
  *R = led.R;
  return;
}

static void get_LED_G(int* G){
  *G = led.G;
  return;
}

static void get_LED_B(int* B){
  *B = led.B;
  return;
}

static void get_LED_W(int* W){
  *W = led.W;
  return;
}
