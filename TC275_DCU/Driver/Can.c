
#include <Asclin3.h>
#include <Can.h>
#include "IfxMultican_PinMap.h"
#include "IfxMultican_cfg.h"
#include "Ifx_Types.h"
#include "IfxPort.h"
#include "IfxCan_reg.h"


#include "IfxCan_reg.h"
#include "IfxCan_regdef.h"


CanQueue canRxQueue;
CanQueue canTxQueue;

Ecu1Can stEcu1Can;
IfxMultican_Message stRxMsgData[10];

IFX_INTERRUPT(CAN_RxIn0Handler,0,30);

void CAN_RxIn0Handler(void)
{
    IfxMultican_Status readStatus;

    readStatus = IfxMultican_Can_MsgObj_readMessage(&stEcu1Can.CanEcu1MsgRxObj[0], &stRxMsgData[0]);

    if (readStatus == IfxMultican_Status_newData)
    {
         CanMessage RxMessageTemp;
         RxMessageTemp.id = stRxMsgData[0].id;
         RxMessageTemp.dlc = stRxMsgData[0].lengthCode;
         RxMessageTemp.dataLow=stRxMsgData[0].data[0];
         RxMessageTemp.dataHigh=stRxMsgData[0].data[1];

        CanRxQueue_Push(&RxMessageTemp);
    }
}

void CanMessage_Init(CanMessage *message,CanMessageId msg, uint32 id, uint8 dlc)
{
    message->msg= msg;
    message->id = id;
    message->dlc = dlc;
    message->dataLow = 0;
    message->dataHigh = 0;
}

void CanMessage_AddSignal(CanMessage *message, uint8 startBit, uint8 length, uint32 value)
{
    uint32 mask = (1<<length) -1;
    uint32 maskedvalue = value & mask;
    if (startBit < 32)
    {
        message->dataLow |= (maskedvalue << startBit);
    }

    else
    {
        uint8 shiftedStartBit = startBit - 32;
        message->dataHigh |= (maskedvalue << shiftedStartBit);
    }
}
void CanMessage_Send(CanMessage *message)
{
    int idx=0;
    IfxMultican_Message msg;
    IfxMultican_Message_init(&msg, message->id, message->dataLow, message->dataHigh, message->dlc);

#ifdef FDCU_
    if(message->id==DOOR_STATUS_FDCU_to_BCU)
    {
        idx=0;
    }
    else if(message->id==DOOR_RETURN_OK_FDCU_to_BCU )
    {
        idx=1;
    }
    else if(message->id==RFID_CARD_INFO_FDCU_to_BCU)
    {
        idx=2;
    }
#endif

#ifdef RDCU_
    if(message->id==DOOR_STATUS_RDCU_to_BCU)
    {
        idx=0;
    }
    else if(message->id==DOOR_RETURN_OK_RDCU_to_BCU )
    {
        idx=1;
    }
    else if(message->id==RFID_CARD_INFO_RDCU_to_BCU)
    {
        idx=2;
    }
#endif
    while (IfxMultican_Can_MsgObj_sendMessage(&stEcu1Can.CanEcu1MsgTxObj[idx], &msg) == IfxMultican_Status_notSentBusy) {}



}

uint32 CanMessage_Parse(uint32 dataLow, uint32 dataHigh, uint8 startBit, uint8 length)
{
    uint64 data = ((uint64)dataHigh << 32) | dataLow;
    uint64 mask = (1 << length) - 1;
    return (data >> startBit) & mask;
}

void CanMessage_To_Signal(const IfxMultican_Message *msg, CanMessage *parsedMessage)
{
    parsedMessage->id = msg->id;

    if(msg->id==0x101)
    {
        parsedMessage->dataLow = msg->data[0];
        parsedMessage->dataHigh = msg->data[1];

        uint32 sig_card_money = CanMessage_Parse(parsedMessage->dataLow, parsedMessage->dataHigh, 0, 32);
        uint8 sig_card_id = (uint8)CanMessage_Parse(parsedMessage->dataLow, parsedMessage->dataHigh, 32, 8);
        uint8 sig_card_user_type = (uint8)CanMessage_Parse(parsedMessage->dataLow, parsedMessage->dataHigh, 40, 4);

    }
    else if(msg->id==0x001)
    {
       uint8 sig_door_open = CanMessage_Parse(parsedMessage->dataLow,parsedMessage->dataHigh,0,1);
       uint8 sig_door_close = CanMessage_Parse(parsedMessage->dataLow,parsedMessage->dataHigh,1,1);
    }
}


void Can_Init(void)
{

    IfxMultican_Can_Config canConfig;
    IfxMultican_Can_initModuleConfig(&canConfig, &MODULE_CAN);

    IfxMultican_Can_initModule(&stEcu1Can.CanEcu1, &canConfig);

    IfxMultican_Can_NodeConfig canNodeConfig;
    IfxMultican_Can_Node_initConfig(&canNodeConfig, &stEcu1Can.CanEcu1);

    canNodeConfig.baudrate = 500000UL;
    canNodeConfig.nodeId    = (IfxMultican_NodeId)((int)IfxMultican_NodeId_0);
    canNodeConfig.rxPin     = &IfxMultican_RXD0B_P20_7_IN;
    canNodeConfig.rxPinMode = IfxPort_InputMode_pullUp;
    canNodeConfig.txPin     = &IfxMultican_TXD0_P20_8_OUT;
    canNodeConfig.txPinMode = IfxPort_OutputMode_pushPull;

    IfxMultican_Can_Node_init(&stEcu1Can.CanEcu1Node, &canNodeConfig);


    /*Object Enrollment*/

#ifdef FDCU_
   Can_EnrollObject(1u, 0x003, IfxMultican_Frame_transmit,  IfxMultican_DataLengthCode_8, FALSE, &stEcu1Can.CanEcu1MsgTxObj[0]);
   Can_EnrollObject(2u, 0x005, IfxMultican_Frame_transmit,  IfxMultican_DataLengthCode_8, FALSE, &stEcu1Can.CanEcu1MsgTxObj[1]);
   Can_EnrollObject(3u, 0x101, IfxMultican_Frame_transmit,  IfxMultican_DataLengthCode_8, FALSE, &stEcu1Can.CanEcu1MsgTxObj[2]);
#endif

#ifdef RDCU_
   Can_EnrollObject(4u, 0x004, IfxMultican_Frame_transmit,  IfxMultican_DataLengthCode_8, FALSE, &stEcu1Can.CanEcu1MsgTxObj[0]);
   Can_EnrollObject(5u, 0x006, IfxMultican_Frame_transmit,  IfxMultican_DataLengthCode_8, FALSE, &stEcu1Can.CanEcu1MsgTxObj[1]);
   Can_EnrollObject(6u, 0x102, IfxMultican_Frame_transmit,  IfxMultican_DataLengthCode_8, FALSE, &stEcu1Can.CanEcu1MsgTxObj[2]);
#endif


    Can_EnrollObject(11u, 0x002, IfxMultican_Frame_receive, IfxMultican_DataLengthCode_8, FALSE, &stEcu1Can.CanEcu1MsgRxObj[0]);
}


void Can_EnrollObject(sint32 msgObjId,  uint32 msgId, uint8 frameType, uint8 msgDlc,  uint32 extendedFrame, IfxMultican_Can_MsgObj* pArrObjNum)
{
    /* create message object config */
    IfxMultican_Can_MsgObjConfig canMsgObjConfig;
    IfxMultican_Can_MsgObj_initConfig(&canMsgObjConfig, &stEcu1Can.CanEcu1Node);

    canMsgObjConfig.msgObjId              = msgObjId;
    canMsgObjConfig.messageId             = msgId;
    canMsgObjConfig.frame                 = frameType;
    canMsgObjConfig.control.messageLen    = msgDlc;
    canMsgObjConfig.control.extendedFrame = extendedFrame;
    canMsgObjConfig.acceptanceMask        = 0x00000000;
    canMsgObjConfig.control.matchingId    = TRUE;

    if(frameType==IfxMultican_Frame_receive)
    {
        canMsgObjConfig.rxInterrupt.enabled=TRUE;
        canMsgObjConfig.rxInterrupt.srcId=0;
        SRC_CAN_CAN0_INT0.B.SRPN =30u;
        SRC_CAN_CAN0_INT0.B.TOS =0u;
        SRC_CAN_CAN0_INT0.B.SRE =1u;

    }
    /* initialize message object */
    IfxMultican_Can_MsgObj_init(pArrObjNum, &canMsgObjConfig);
}




int CanRxQueue_Push(const CanMessage *message)
{
    uint32 nextHead = (canRxQueue.head + 1) % CAN_QUEUE_SIZE;
    if (nextHead == canRxQueue.tail)
    {
        return 0;
    }

    canRxQueue.messages[canRxQueue.head] = *message;
    canRxQueue.head = nextHead;

    return 1;
}


int CanRxQueue_Pop(CanMessage *message)
{
    if (canRxQueue.head == canRxQueue.tail)
    {
        return 0;
    }

    *message = canRxQueue.messages[canRxQueue.tail];
    canRxQueue.tail = (canRxQueue.tail + 1) % CAN_QUEUE_SIZE;

    return 1;
}

int CanTxQueue_Push(const CanMessage *message)
{
    uint32 nextHead = (canTxQueue.head + 1) % CAN_QUEUE_SIZE;
    if (nextHead == canTxQueue.tail)
    {
        return 0;
    }

    canTxQueue.messages[canTxQueue.head] = *message;
    canTxQueue.head = nextHead;

    return 1;
}


int CanTxQueue_Pop(CanMessage *message)
{
    if (canTxQueue.head == canTxQueue.tail)
    {
        return 0;
    }

    *message = canTxQueue.messages[canTxQueue.tail];
    canTxQueue.tail = (canTxQueue.tail + 1) % CAN_QUEUE_SIZE;

    return 1;
}

CanMessage getCanMessage()
{
    CanMessage message;
    if(CanRxQueue_Pop(&message)) return message;
    else {
        message.id = 0xfff;
        return message; // no can Message
    }
}
