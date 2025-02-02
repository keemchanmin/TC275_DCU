#ifndef STEP_MOTOR_H
#define STEP_MOTOR_H

#include <Asclin3.h>
#include "Platform_Types.h"
/* Step Motor 스케줄러 구조체 정의 */
typedef struct {
    uint32 steps_cnt;            // 남은 스텝 수
    int pulse_interval;          // 500µs 간격 (us)
    uint32 elapsed_time;          // 경과 시간 (us)
    int is_high;                 // 현재 PULSE 핀 상태 (1: High, 0: Low)
    int direction;               // 방향 (0: Right, 1: Left)
    int is_active;              // 모터 동작 활성화 (1: Active, 0: Inactive)
} StepMotorScheduler;


#define STEP_DIR_PIN IfxPort_P14_1
#define STEP_ENABLE_PIN IfxPort_P14_0
#define STEP_PULSE_PIN IfxPort_P15_6

typedef void (*event_stepmotor)(uint32);

/* 함수 프로토타입 */
void StepMotor_init(void);            // Step Motor 초기화
void StepMotor_enable(void);          // Step Motor 활성화
void StepMotor_disable(void);         // Step Motor 비활성화
void StepMotor_Dir(int dir);          // Step Motor 방향 설정
void StepMotor_High(void);            // PULSE 핀 HIGH 상태
void StepMotor_Low(void);             // PULSE 핀 LOW 상태
void StepMotor_Start(uint32 steps, int direction); // Step Motor 동작 시작
void StepMotor_Stop(void);            // Step Motor 동작 중지
void StepMotor_event_500us();

void StepMotor_register_distance_event(event_stepmotor func);

#endif /* STEP_MOTOR_H */
