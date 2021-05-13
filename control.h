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

//Each cs 3pin
#define STEP_MOTOR_ID_1                     A4
#define STEP_MOTOR_ID_2                     8
#define STEP_MOTOR_ID_3                     9

//DIR
#define STEP_MOTOR_DIR_FORWARD              0
#define STEP_MOTOR_DIR_REVERSE              1

//servo motor pan tilt
#define SERVO_MOTOR_PAN                     25
#define SERVO_MOTOR_TILLT                   26

#define LED_DATA1                           6
#define LED_DATA2                           7



typedef struct SERVO_MOTOR_CONTROL_TYPE{
    void (*control)(int, void*);
}SERVO_MOTOR_CONTROL;

typedef struct STEP_MOTOR_CONTROL_TYPE{
    void (*control)(int, void*);
}STEP_MOTOR_CONTROL;

typedef struct LED_CONTROL_TYPE{
    void (*control)(int, void*);
}LED_CONTROL;

void control(int PARAMETER, void* value);