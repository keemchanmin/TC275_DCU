#ifndef DCU
#define DCU

/***********************************************************************/
/*Include*/
/***********************************************************************/
#include "Ifx_Types.h"
#include "IfxPort.h"
#include "IfxPort_PinMap.h"
#include "Platform_Types.h"
#include "IfxStm.h"
#include "Bsp.h"
#include "Ultrasonic.h"
#include "Step_Motor.h"
#include "Collision_Detection.h"
#include "AppScheduler.h"
#include "Rfid.h"
#include "Can.h"
#include "Mp3.h"

/***********************************************************************/
/*Typedef*/
/***********************************************************************/
typedef enum DoorStatus {
    DOOR_CLOSE, DOOR_OPEN, DOOR_RUNNING, DOOR_STUCK
} DoorStatus;

typedef enum CardStatus {
    CARD_ERROR =0,
    CARD_IN=1, CARD_OUT=2, CARD_EXIST=3
} Cardstatus;

typedef enum DoorDirection {
    DOOR_DIR_CLOSE = 0, DOOR_DIR_OPEN = 1
} DoorDirection;

typedef enum CardType {
    CARD_TYPE_YOUTH=1,CARD_TYPE_DEFAULT=2
} CardType;

typedef struct
{
    volatile int count_100us;
    volatile int count_1ms;
    volatile int count_10ms;
    volatile int count_100ms;
    volatile int count_200ms;
} TimerCounts;


/***********************************************************************/
/*Define*/
/***********************************************************************/

#define RDCU
//#define FDCU


#ifdef FDCU
#define DCU_TYPE 0   // FDCU -> 0   RDCU -> 1
#endif

#ifdef RDCU
#define DCU_TYPE 1   // FDCU -> 0   RDCU -> 1
#endif

#define DOOR_CONTROL_CAN_MSG_ID ((DCU_TYPE == 0) ? 0x001 : 0x002)           // FDCU -> 0x001  RDCU -> 0x002
#define DOOR_STATUS_CAN_MSG_ID ((DCU_TYPE == 0) ? 0x003 : 0x004)            // FDCU -> 0x003  RDCU -> 0x004
#define DOOR_RETURN_OK_CAN_MSG_ID ((DCU_TYPE == 0) ? 0x005 : 0x006)         // FDCU -> 0x005  RDUC -> 0x006
#define RFID_CARD_INFO_CAN_MSG_ID ((DCU_TYPE == 0) ?  0x101 : 0x102)        // FDCU -> 0x101  RDCU -> 0x102
#define RFID_CARD_RESPONSE_CAN_MSG_ID ((DCU_TYPE == 0) ?  0x103 : 0x104)    // FDCU -> 0x101  RDCU -> 0x102

#ifdef FDCU
#define DEFAULT_IN_SOUND 1      // 삑 승차입니다
#define DEFAULT_OUT_SOUND 2     // 삑 하차입니다
#define YOUTH_IN_SOUND 3        // 삐빅 승차입니다
#define YOUTH_OUT_SOUND 4       // 삐빅 하차입니다
#define MANY_IN_SOUND 5         // 다인승입니다
#define ALREADY_SOUND 6         // 이미 태그된 카드입니다
#define NO_MONEY_SAD_SOUND 7           // 잔액이 부족합니다
#endif


#ifdef RDCU
#define DEFAULT_IN_SOUND 2      // 삑 승차입니다
#define DEFAULT_OUT_SOUND 3     // 삑 하차입니다
#define YOUTH_IN_SOUND 4        // 삐빅 승차입니다 3 하차입니다
#define YOUTH_OUT_SOUND 5       // 삐빅 하차입니다
#define MANY_IN_SOUND 6         // 다인승입니다
#define ALREADY_SOUND 7         // 이미 태그된 카드입니다 6다인승
#define NO_MONEY_SAD_SOUND 1           // 잔액이 부족합니다
#endif


#define DOOR_STEPS 6000
#define DISTANCE_MIN_THRESHOLD 4
#define DISTANCE_MAX_THRESHOLD 15

#define ISR_PRIORITY_STM        40                              /* Priority for interrupt ISR                       */
#define STM                     &MODULE_STM0                    /* STM0 is used in this example                     */

/***********************************************************************/
/*External Variable*/
/***********************************************************************/

/***********************************************************************/
/*Global Function Prototype*/
/***********************************************************************/
void DCU_Init();
void Dcu_Main();
void Message_Handler();

void Rfid_TxHandler(CardInfo cardInfo);
void Door_TxHandler(uint32 msg_type, uint8 is_collision);

void Door_RxHandler(uint8 door_open, uint8 door_close);
void Rfid_RxHandler(uint16 card_used_money, uint8 card_status);
void Mp3_RxHandler(uint8 card_status, uint8 card_people_count);

void Door_Observe_Status();

void event_10us(void);
void event_100us(void);
void event_500us(void);
void event_1ms(void);
void event_10ms(void);
void event_1000ms(void);

void print_distance(uint32 value);
#endif
