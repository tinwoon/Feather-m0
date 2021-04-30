
#ifndef LED_H
#define LED_H

#define PARAMETER_LED_ID 0
#define PARAMETER_LED_R 1
#define PARAMETER_LED_G 2
#define PARAMETER_LED_B 3
#define PARAMETER_LED_W 4
#define PARAMETER_LED_SET_INIT 5
#define PARAMETER_LED_GET_INIT 6

#define NEOPIXEL_DATA_PIN 6
#define LED_NUM 90

#include <Adafruit_NeoPixel_ZeroDMA.h>

//stem motor 객체를 모두 set하기위해
static void set_init(int PARAMETER, void* value);
//stem motor 객체를 모두 get하기위해
static void get_init(int PARAMETER, void* value);

//파라미터 값에 따라 아래의 set을 수행함
//예를 들면 PARAMETER_MOTOR_ID면 set_motor_id가 수행되서 stem_motor의 motor_id에 value로 초기화 함
static void set(int PARAMETER, void* value);
//파라미터 값에 따라 아래의 get을 수행함
//예를 들면 PARAMETER_MOTOR_ID면 get_motor_id가 수행되서 stem_motor의 value에 motor_id로 초기화 함
static void get(int PARAMETER, void* value);
static void set_LED_id(int* LED_id);
static void set_LED_R(int* R);
static void set_LED_G(int* G);
static void set_LED_B(int* B);
static void set_LED_W(int* W);
static void get_LED_id(int* LED_id);
static void get_LED_R(int* R);
static void get_LED_G(int* G);
static void get_LED_B(int* B);
static void get_LED_W(int* W);

//모터 ID, 방향, 회전각, 회전 수, 회전 속도
typedef struct LED_TYPE{
  int LED_id;
  int R;
  int G;
  int B;
  int W;
}LED;

typedef struct LED_DATA_TYPE{
  void (*set_init)(int, void*);
  void (*get_init)(int, void*);
  void (*set)(int, void*);
  void (*get)(int, void*);
  void (*set_LED_id)(int*);
  void (*set_LED_R)(int*);
  void (*set_LED_G)(int*);
  void (*set_LED_B)(int*);
  void (*set_LED_W)(int*);
  void (*get_LED_id)(int*);
  void (*get_LED_R)(int*);
  void (*get_LED_G)(int*);
  void (*get_LED_B)(int*);
  void (*get_LED_W)(int*);
}LED_DATA;

class NeoPixelLED
{
  public :
    void init(uint8_t dataPin = NEOPIXEL_DATA_PIN, uint16_t ledNum = LED_NUM);
    void setColorFullTest(uint8_t mode, uint8_t bright);
    void setColor(uint16_t index, uint32_t color);
    void setColor(uint16_t index, uint8_t r, uint8_t g, uint8_t b, uint8_t w);

    void setLedsNumber(uint16_t ledNum);

    
    uint32_t getPixelColor(uint16_t n);
    void  setBrightness(uint8_t b);

    void clear();
    void show();
  private : 
    uint8_t mDataPin;
    Adafruit_NeoPixel* mPixels;
    uint8_t mLedNum;
};

#endif