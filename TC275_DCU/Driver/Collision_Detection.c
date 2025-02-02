#include "IfxStm.h"
#include "IfxScuEru.h"
#include "IfxPort_PinMap.h"
#include "IfxPort.h"
#include "Bsp.h"
#include "Collision_Detection.h"

IFX_INTERRUPT(Collision_ISR, 0, 0x51);

static uint8 collision_detected;

void Collision_Detection_Init (void)
{

    IfxPort_setPinMode(COLLISION_PIN.port, COLLISION_PIN.pinIndex, IfxPort_Mode_inputPullDown);
        IfxScuEru_selectExternalInput(IfxScuEru_InputChannel_3, IfxScuEru_ExternalInputSelection_2);
        IfxScuEru_enableRisingEdgeDetection(IfxScuEru_InputChannel_3);
        IfxScuEru_enableTriggerPulse(IfxScuEru_InputChannel_3);
        IfxScuEru_connectTrigger(IfxScuEru_InputChannel_3, IfxScuEru_InputNodePointer_1);
        IfxScuEru_setInterruptGatingPattern(IfxScuEru_OutputChannel_1, IfxScuEru_InterruptGatingPattern_alwaysActive);
        IfxSrc_init(&SRC_SCU_SCU_ERU1, IfxSrc_Tos_cpu0, 0x51);
        IfxSrc_enable(&SRC_SCU_SCU_ERU1);

    collision_detected = 0;
}

uint8 Is_Collision_Detected(void)
{
    return collision_detected;
}

void Reset_Collision_Detected(void)
{
    collision_detected = 0;
}

void Collision_ISR (void)
{
    collision_detected = 1;
}
