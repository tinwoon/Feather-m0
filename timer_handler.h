#define CPU_HZ 48000000
#define TIMER_PRESCALER_DIV 1024

typedef struct TIMER_HANDLER_TYPE{
    void (*startTimer)(int);
    void (*setTimerFrequency)(int);
    void (*TC3_Handler)();
    void (*serial_interrupt)();
}TIMER_HANDLER;

void startTimer(int frequencyHz);
void setTimerFrequency(int frequencyHz);
void TC3_Handler();
void serial_interrupt();