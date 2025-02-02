#include "Step_Motor.h"

#include "IfxPort.h"
#include "IfxPort_PinMap.h"
#include "IfxStm.h"
#include "Bsp.h"

StepMotorScheduler stepMotorScheduler ;//= {0};

event_stepmotor on_move_end;

/* Step Motor 초기화 */
void StepMotor_init(void)
{
    IfxPort_setPinMode(STEP_DIR_PIN.port, STEP_DIR_PIN.pinIndex, IfxPort_Mode_outputPushPullGeneral);
    IfxPort_setPinMode(STEP_ENABLE_PIN.port, STEP_ENABLE_PIN.pinIndex, IfxPort_Mode_outputPushPullGeneral);
    IfxPort_setPinMode(STEP_PULSE_PIN.port, STEP_PULSE_PIN.pinIndex, IfxPort_Mode_outputPushPullGeneral);
    StepMotor_disable();
}

/* Step Motor 활성화 */
void StepMotor_enable(void)
{
    IfxPort_setPinLow(STEP_ENABLE_PIN.port, STEP_ENABLE_PIN.pinIndex);
}

/* Step Motor 비활성화 */
void StepMotor_disable(void)
{
    IfxPort_setPinHigh(STEP_ENABLE_PIN.port, STEP_ENABLE_PIN.pinIndex);
}

/* Step Motor 방향 설정 */
void StepMotor_Dir(int dir)
{
    if (dir == 0) {
        IfxPort_setPinHigh(STEP_DIR_PIN.port, STEP_DIR_PIN.pinIndex); // 오른쪽
    } else {
        IfxPort_setPinLow(STEP_DIR_PIN.port, STEP_DIR_PIN.pinIndex);  // 왼쪽
    }
}

/* PULSE 핀 HIGH 상태 */
void StepMotor_High(void)
{
    IfxPort_setPinHigh(STEP_PULSE_PIN.port, STEP_PULSE_PIN.pinIndex);
}

/* PULSE 핀 LOW 상태 */
void StepMotor_Low(void)
{
    IfxPort_setPinLow(STEP_PULSE_PIN.port, STEP_PULSE_PIN.pinIndex);
}

/* Step Motor 시작 */
void StepMotor_Start(uint32 steps, int direction)
{
    stepMotorScheduler.steps_cnt = steps;
    stepMotorScheduler.pulse_interval = 500; // 500µs 간격
    stepMotorScheduler.elapsed_time = 0;
    stepMotorScheduler.is_high = 0; // 초기 상태: Low
    stepMotorScheduler.direction = direction;
    stepMotorScheduler.is_active = 1; // 동작 활성화
    StepMotor_Dir(direction);
    StepMotor_enable();
}

void StepMotor_Elapse()
{
    if(!stepMotorScheduler.is_active) return;
    stepMotorScheduler.direction = !stepMotorScheduler.direction;
    stepMotorScheduler.steps_cnt = stepMotorScheduler.elapsed_time;
    if(stepMotorScheduler.steps_cnt % 2 != 0) stepMotorScheduler.steps_cnt++;
    stepMotorScheduler.elapsed_time = 0;
    StepMotor_Dir(stepMotorScheduler.direction);
}

/* Step Motor 정지 */
void StepMotor_Stop(void)
{
    stepMotorScheduler.is_active = 0; // 동작 비활성화
    StepMotor_disable();
}

void StepMotor_event_500us(){
    if (stepMotorScheduler.is_active)
    {
        stepMotorScheduler.elapsed_time++;

       if (stepMotorScheduler.is_high)
       {
           StepMotor_Low();
           stepMotorScheduler.is_high = 0;
       }
       else
       {
           StepMotor_High();
           stepMotorScheduler.is_high = 1;
       }

       if (stepMotorScheduler.elapsed_time >= stepMotorScheduler.steps_cnt)
       {
           StepMotor_Stop();
           if(on_move_end != NULL)
           {
               on_move_end(stepMotorScheduler.direction);
           }
       }
    }
}

void StepMotor_register_distance_event(event_stepmotor func)
{
    on_move_end = func;
}

void StepMotor_Run()
{
    if (stepMotorScheduler.is_active)
       {
           stepMotorScheduler.elapsed_time += 100; // 100µs 증가
           if (stepMotorScheduler.elapsed_time >= stepMotorScheduler.pulse_interval)
           {
               stepMotorScheduler.elapsed_time = 0;

               if (stepMotorScheduler.is_high)
               {
                   StepMotor_Low();
                   stepMotorScheduler.is_high = 0;
                   stepMotorScheduler.steps_cnt--;
                  // _asclin3_printf("StepMotor_Run: %d \n", stepMotorScheduler.steps_remaining);
               }
               else
               {
                   StepMotor_High();
                   stepMotorScheduler.is_high = 1;
               }

               if (stepMotorScheduler.steps_cnt <= 0)
               {
                   stepMotorScheduler.is_active = 0;
                   StepMotor_Stop();
               }
           }
       }
}

uint8 StepMotor_IsActive()
{
    return stepMotorScheduler.is_active;
}
