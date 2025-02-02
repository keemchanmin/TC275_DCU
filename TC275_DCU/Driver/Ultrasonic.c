#include <Asclin3.h>
#include "Ifx_Types.h"
#include "IfxCpu.h"
#include "IfxScuWdt.h"
#include "IfxPort.h"
#include "IfxPort_PinMap.h"
#include "IfxScuEru.h"
#include "IfxSrc.h"
#include "Bsp.h"
#include "Ultrasonic.h"

//#define POWER_PIN IfxPort_P02_4
#define TRIGGER_PIN IfxPort_P02_3
#define ECHO_PIN IfxPort_P02_1

#define MAX_ERROR_CNT  50000

uint32 distance = 0;

boolean is_check_end = FALSE;
uint8 cnt_trigger_pin = 0;
uint32 cnt_echo_pin = 0;
uint32 cnt_error_tick = 0;

typedef enum {
    ULTRASONIC_STATE_IDLE = 0,
    ULTRASONIC_STATE_TRIGGER = 1,
    ULTRASONIC_STATE_ECHO = 2,
    ULTRASONIC_STATE_ERROR = 3,
} Ultrasonic_State;
Ultrasonic_State ultrasonic_state = ULTRASONIC_STATE_IDLE;

event_ultrasonic on_distance_end;

void ultrasonic_move_to_next_state();

void ultrasonic_init (void)
{
    /*초음파 센서 gpio*/
//    IfxPort_setPinMode(POWER_PIN.port, POWER_PIN.pinIndex, IfxPort_Mode_outputPushPullGeneral);
    IfxPort_setPinMode(TRIGGER_PIN.port, TRIGGER_PIN.pinIndex, IfxPort_Mode_outputPushPullGeneral);
    IfxPort_setPinMode(ECHO_PIN.port, ECHO_PIN.pinIndex, IfxPort_Mode_inputPullDown);
    IfxScuEru_selectExternalInput(IfxScuEru_InputChannel_2, IfxScuEru_ExternalInputSelection_1);
    IfxScuEru_enableRisingEdgeDetection(IfxScuEru_InputChannel_2);
    IfxScuEru_enableFallingEdgeDetection(IfxScuEru_InputChannel_2);
    IfxScuEru_enableTriggerPulse(IfxScuEru_InputChannel_2);
    IfxScuEru_connectTrigger(IfxScuEru_InputChannel_2, IfxScuEru_InputNodePointer_0);
    IfxScuEru_setInterruptGatingPattern(IfxScuEru_OutputChannel_0, IfxScuEru_InterruptGatingPattern_alwaysActive);
    IfxSrc_init(&SRC_SCU_SCU_ERU0, IfxSrc_Tos_cpu1, 0x50);
    IfxSrc_enable(&SRC_SCU_SCU_ERU0);

//    IfxPort_setPinHigh(POWER_PIN.port, POWER_PIN.pinIndex);
}

void ultrasonic_event_10us(){
    if(is_check_end == TRUE){
        if(on_distance_end != NULL){
            on_distance_end(distance);
        }
        is_check_end = FALSE;
    }

    if(     ultrasonic_state != ULTRASONIC_STATE_IDLE &&
            ultrasonic_state != ULTRASONIC_STATE_ERROR){
        if(ultrasonic_state == ULTRASONIC_STATE_TRIGGER){
            if(cnt_trigger_pin == 0){
                cnt_trigger_pin = 1;
            }
            else if(cnt_trigger_pin == 1){
                IfxPort_setPinHigh(TRIGGER_PIN.port, TRIGGER_PIN.pinIndex);
                cnt_trigger_pin = 2;
            }
            else if(cnt_trigger_pin == 2){
                IfxPort_setPinLow(TRIGGER_PIN.port, TRIGGER_PIN.pinIndex);
                cnt_trigger_pin = 3;
            }
        }
        else if(ultrasonic_state == ULTRASONIC_STATE_ECHO){
            cnt_echo_pin++;
            if(cnt_echo_pin > 3900){
                // 측정 가능 거리 초과, 강제 초기화
                ultrasonic_move_to_next_state();
            }
        }
        cnt_error_tick++;
    }

    if(cnt_error_tick > MAX_ERROR_CNT){
        ultrasonic_state = ULTRASONIC_STATE_ERROR;
        ultrasonic_move_to_next_state();
    }

}

void ultrasonic_trigger(){
    if(ultrasonic_state == ULTRASONIC_STATE_IDLE){
        ultrasonic_move_to_next_state();
    }
}

void ultrasonic_move_to_next_state(){
    switch(ultrasonic_state){
        case ULTRASONIC_STATE_IDLE:
        {
            cnt_trigger_pin = 0;
            ultrasonic_state = ULTRASONIC_STATE_TRIGGER;
        }
        break;
        case ULTRASONIC_STATE_TRIGGER:
        {
            cnt_echo_pin = 0;
            ultrasonic_state = ULTRASONIC_STATE_ECHO;
        }
        break;
        case ULTRASONIC_STATE_ECHO:
        {
            uint16 newdistance = (cnt_echo_pin * 0.343) / 2;
            if(newdistance < 20){
                distance = newdistance;
            }
            cnt_error_tick = 0;
            is_check_end = TRUE;
            ultrasonic_state = ULTRASONIC_STATE_IDLE;
        }
        break;
        case ULTRASONIC_STATE_ERROR:
        {
            cnt_error_tick = 0;
            ultrasonic_state = ULTRASONIC_STATE_IDLE;
        }
        break;
    }
}

void ultrasonic_register_distance_event(event_ultrasonic func){
    on_distance_end = func;
}

IFX_INTERRUPT(Echo_ISR, 1, 0x50);
void Echo_ISR(void) {
    switch(ultrasonic_state){
        case ULTRASONIC_STATE_TRIGGER:
        {
            if(IfxPort_getPinState(ECHO_PIN.port, ECHO_PIN.pinIndex) == TRUE){
                ultrasonic_move_to_next_state();
            }
        }
        break;
        case ULTRASONIC_STATE_ECHO:
        {
            if(IfxPort_getPinState(ECHO_PIN.port, ECHO_PIN.pinIndex) == FALSE){
                ultrasonic_move_to_next_state();
            }
        }
        break;
    }
}

int Display_Distance(void)
{
    if(distance < 4) return 3;
    if(distance > 15) return 16;
    return distance;
}

