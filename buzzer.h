#define PARAMETER_BUZZER_PIN A5

typedef struct BUZZER_TYPE{
    void (*buzzer_init)();
    void (*warning)();
}BUZZER;

void buzzer_init();
void warning();
