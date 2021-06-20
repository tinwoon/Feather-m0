#include <iostream>
#include <vector>
#include <sstream>

#define PARAMETER_STEP_MOTOR_CMD          0x01
#define PARAMETER_SERVO_MOTOR_CMD         0x02
#define PARAMETER_LED_CMD                 0x03
#define PARAMETER_SWITCH_1                38


typedef struct SERIAL_CMD_TYPE{
    void (*read_cmd)();
    void (*attach_cmd)(int, std::vector< String > &);
    void (*serial_init)();
    void (*serialInput)();
    int (*return_check_sum)(std::vector< String > &);
    void (*read_motor_stepping)();
    void (*serial_interrupt_Input)();
}SERIAL_CMD;


void read_cmd();
void attach_cmd(int PARAMETER, std::vector< String >& cmd_stack);
void serial_init();
void serialInput();
int return_check_sum(std::vector< String > &cmd_stack);
void read_motor_stepping();
void serial_interrupt_Input();