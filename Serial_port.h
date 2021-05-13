#include <iostream>
#include <vector>
#include <sstream>

#define PARAMETER_STEP_MOTOR_CMD          0x01
#define PARAMETER_SERVO_MOTOR_CMD         0x02
#define PARAMETER_LED_CMD           0x03

typedef struct SERIAL_CMD_TYPE{
    void (*read_cmd)(String);
    void (*attach_cmd)(int, std::vector< String >& );
}SERIAL_CMD;


void read_cmd(String cmd);
void attach_cmd(int PARAMETER, std::vector< String >& cmd_stack);