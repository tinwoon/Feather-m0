
#define PARAMETER_DIR 0
#define PARAMETER_ANGLE 1
#define PARAMETER_ROTATE 2
#define PARAMETER_RPM 3
#define PARAMETER_MOTOR_ID 4
#define PARAMETER_MOTOR_SET_INIT 5
#define PARAMETER_MOTOR_GET_INIT 6

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
static void set_motor_id(int* motor_id);
static void set_dir(int* dir);
static void set_angle(int* angle);
static void set_rotate(int* rotate);
static void set_rpm(int* rpm);
static void get_motor_id(int* motor_id);
static void get_dir(int* dir);
static void get_angle(int* angle);
static void get_rotate(int* rotate);
static void get_rpm(int* rpm);

//모터 ID, 방향, 회전각, 회전 수, 회전 속도
typedef struct STEP_MOTOR_TYPE{
  int motor_id;
  int dir;
  int angle;
  int rotation_number;
  int rpm;
}STEP_MOTOR;

typedef struct STEP_MOTOR_DATA_TYPE{
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
}STEP_MOTOR_DATA;
