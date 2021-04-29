#define PARAMETER_STEP_CONTROL_ALL          0x01
#define PARAMETER_STEP_CONTROL_MOTOR_ID     0x02
#define PARAMETER_STEP_CONTROL_DIR          0x03
#define PARAMETER_STEP_CONTROL_ANGLE        0x04
#define PARAMETER_STEP_CONTROL_ROTATE       0x05
#define PARAMETER_STEP_CONTROL_RPM          0x06

#define PARAMETER_SERVO_CONTROL_ALL         0x07
#define PARAMETER_SERVO_CONTROL_MOTOR_ID    0x08 
#define PARAMETER_SERVO_CONTROL_ANGLE       0x09

#define PARAMETER_LED_CONTROL_ALL           0x0A
#define PARAMETER_LED_CONTROL_ID            0x0B
#define PARAMETER_LED_CONTROL_R             0x0C
#define PARAMETER_LED_CONTROL_G             0x0D
#define PARAMETER_LED_CONTROL_B             0x0E
#define PARAMETER_LED_CONTROL_W             0x10

typedef struct SERVO_CONTROL_TYPE{
    void (*control)(int, void*);
}SERVO_CONTOL;

typedef struct STEP_CONTROL_TYPE{
    void (*control)(int, void*);
}STEP_CONTROL;

typedef struct LED_CONTROL_TYPE{
    void (*control)(int, void*);
}LED_CONTROL;

void control(int PARAMETER, void* value);