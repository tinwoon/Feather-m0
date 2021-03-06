
#define PARAMETER_BOOTING_LED_1 A2
#define PARAMETER_BOOTING_LED_2 A1

#define PARAMETER_SET_BOOTING_LED_REVERSE       0
#define PARAMETER_GET_BOOTING_LED               1

//stem motor 객체를 모두 set하기위해
static void set_booting_led(int PARAMETER);
//부팅을 위한 초기화
static void set_booting_led_init();

//Blynk led를 위한 두가지
typedef struct BOOTING_LED_TYPE{
  int booting_led_1;
  int booting_led_2;
}BOOTING_LED;

typedef struct BOOTING_LED_SET_TYPE{
  void (*set_booting_led)(int);
  void (*set_booting_led_init)();
}BOOTING_LED_SET;
