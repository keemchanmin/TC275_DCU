#ifndef CAN_H_
#define CAN_H_

#include "IfxMultican.h"
#include "./Libraries/iLLD/TC27D/Tricore/Multican/Can/IfxMultican_Can.h"

//#define FDCU_
#define RDCU_

#define SRC_MESSAGE_OBJECT_ID       (IfxMultican_MsgObjId)0     /* Source message object ID                          */
#define DST_MESSAGE_OBJECT_ID       (IfxMultican_MsgObjId)1     /* Destination message object ID                     */
#define CAN_MESSAGE_ID              0x777                       /* Message ID that will be used in arbitration phase */
#define TX_INTERRUPT_SRC_ID         IfxMultican_SrcId_0         /* Transmit interrupt service request ID             */
#define RX_INTERRUPT_SRC_ID         IfxMultican_SrcId_1         /* Receive interrupt service request ID              */
#define PIN5                        5                           /* LED1 used in TX ISR is connected to this pin      */
#define PIN6                        6                           /* LED2 used in RX ISR is connected to this pin      */
#define INVALID_DATA_VALUE          (uint32)0xDEADBEEF          /* Used to invalidate RX message data content        */
#define INVALID_ID_VALUE            (uint32)0xFFFFFFFF          /* Used to invalidate RX message ID value            */
#define ISR_PRIORITY_CAN_TX         2                           /* Define the CAN TX interrupt priority              */
#define ISR_PRIORITY_CAN_RX         1                           /* Define the CAN RX interrupt priority              */
#define ECU1_TX_OBJ_NUM              10u
#define ECU1_RX_OBJ_NUM              10u
#define CAN_QUEUE_SIZE              256
#define CAN_TX_QUEUE_SIZE            256

typedef struct
{
    IfxMultican_Can        CanEcu1;
    IfxMultican_Can_Node   CanEcu1Node;
    IfxMultican_Can_MsgObj CanEcu1MsgTxObj[ECU1_TX_OBJ_NUM];
    IfxMultican_Can_MsgObj CanEcu1MsgRxObj[ECU1_RX_OBJ_NUM];
} Ecu1Can;


typedef enum {
    DOOR_CONTROL_BCU_to_FDCU = 0x001,
    DOOR_CONTROL_BCU_to_RDCU = 0x002,
    DOOR_STATUS_FDCU_to_BCU  = 0x003,
    DOOR_STATUS_RDCU_to_BCU  = 0x004,
    DOOR_RETURN_OK_FDCU_to_BCU = 0x005,
    DOOR_RETURN_OK_RDCU_to_BCU = 0x006,
    RFID_CARD_INFO_FDCU_to_BCU = 0x101,
    RFID_CARD_INFO_RDCU_to_BCU = 0x102,
    RFID_CARD_RESPONSE_BCU_to_FDCU = 0x103,
    RFID_CARD_RESPONSE_BCU_to_RDCU = 0x104
} CanMessageId;


typedef struct {
    CanMessageId msg;
    uint32 id;
    uint8 dlc;
    uint32 dataLow;
    uint32 dataHigh;
} CanMessage;


typedef struct{
    CanMessage messages[CAN_QUEUE_SIZE];
    uint32 head;
    uint32 tail;
}CanQueue;




void Can_Init(void);
void Can_EnrollObject(sint32 msgObjId,  uint32 msgId, uint8 frameType, uint8 msgDlc,  uint32 extendedFrame, IfxMultican_Can_MsgObj* pArrObjNum);
void CanMessage_Init(CanMessage *message,CanMessageId msg, uint32 id, uint8 dlc);
void CanMessage_AddSignal(CanMessage *message, uint8 startBit, uint8 length, uint32 value);
void CanMessage_Send(CanMessage *message);
uint32 CanMessage_Parse(uint32 dataLow, uint32 dataHigh, uint8 startBit, uint8 length);
void CanMessage_To_Signal(const IfxMultican_Message *msg, CanMessage *parsedMessage);
int CanRxQueue_Push(const CanMessage *message);
int CanRxQueue_Pop(CanMessage *message);
int CanTxQueue_Push(const CanMessage *message);
int CanTxQueue_Pop(CanMessage *message);
CanMessage getCanMessage(void);


#endif
