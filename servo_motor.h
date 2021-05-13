
#define PARAMETER_SERVO_MOTOR_ID 0
#define PARAMETER_SERVO_ANGLE 1
#define PARAMETER_SERVO_MOTOR_SET_INIT 2
#define PARAMETER_SERVO_MOTOR_GET_INIT 3

//stem motor 객체를 모두 set하기위해
static void set_init(int PARAMETER, void* value);
//stem motor 객체를 모두 get하기위해
static void get_init(int PARAMETER, void* value);

static void set(int PARAMETER, void* value);
static void get(int PARAMETER, void* value);
static void set_motor_id(int* motor_id);
static void set_angle(int* angle);
static void get_motor_id(int* motor_id);
static void get_angle(int* angle);

//모터 ID, 방향, 회전각, 회전 수, 회전 속도
typedef struct SERVO_MOTOR_TYPE{
  int motor_id;
  int angle;
}SERVO_MOTOR;

typedef struct SERVO_MOTOR_DATA_TYPE{
  void (*set_init)(int, void*);
  void (*get_init)(int, void*);
  void (*set)(int, void*);
  void (*get)(int, void*);
  void (*set_motor_id)(int*);
  void (*set_dir)(int*);
  void (*set_angle)(int*);
  void (*set_rotate)(int*);
  void (*set_rpm)(int*);
  void (*get_motor_id)(int*);
  void (*get_dir)(int*);
  void (*get_angle)(int*);
  void (*get_rotate)(int*);
  void (*get_rpm)(int*);
}SERVO_MOTOR_DATA;
