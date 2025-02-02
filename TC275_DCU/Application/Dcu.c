/***********************************************************************/
/*Include*/
/***********************************************************************/
#include "Dcu.h"

/***********************************************************************/
/*Define*/
/***********************************************************************/


/***********************************************************************/
/*Typedef*/
/***********************************************************************/


/***********************************************************************/
/*Static Function Prototype*/
/***********************************************************************/


/***********************************************************************/
/*Variable*/
/***********************************************************************/
DoorStatus prev_status, door_status;

uint8 rfid_readFlag;
uint8 door_readFlag;
uint8 door_stuckFlag;

void on_motor_move_end(uint32 distance);

/***********************************************************************/
/*Function*/
/***********************************************************************/
void DCU_Init()
{
    AppScheduler_Init();
    Rfid_Init();
    Can_Init();
    StepMotor_init();
    StepMotor_register_distance_event(on_motor_move_end);
    Collision_Detection_Init();
    Mp3_Init();
    Mp3_Volume_Set(20);

    door_status = DOOR_CLOSE;

    rfid_readFlag = 0;
    door_readFlag = 0;
}

void Dcu_Main()
{
    while(1)
    {
        CanMessage message;

        rfid_readFlag = ReadCardInfoFromRfid();
        Rfid_TxHandler(GetCardInfo());
        Message_Handler();
        Door_Observe_Status();

        if(Check_Flag(FLAG_500US))
        {
            Clear_Flag(FLAG_500US);
            event_500us();
        }

        if(CanTxQueue_Pop(&message))
        {
            CanMessage_Send(&message);
        }

    }
}

void Message_Handler()
{
    CanMessage msg = getCanMessage();
    if(msg.id != 0xfff)
    {
        switch(msg.id)
        {
            case DOOR_CONTROL_CAN_MSG_ID :
            {
                uint8 door_open = (uint8)CanMessage_Parse(msg.dataLow, msg.dataHigh, 0, 1);
                uint8 door_close = (uint8)CanMessage_Parse(msg.dataLow, msg.dataHigh, 1, 1);

                Door_RxHandler(door_open, door_close);
                break;
            }
            case RFID_CARD_RESPONSE_CAN_MSG_ID :
            {
                uint16 card_used_money = CanMessage_Parse(msg.dataLow, msg.dataHigh, 0, 16);
                uint8 card_id = (uint8)CanMessage_Parse(msg.dataLow, msg.dataHigh, 16, 8);
                uint8 card_status = (uint8) CanMessage_Parse(msg.dataLow, msg.dataHigh, 24, 2);
                uint8 card_people_count = (uint8)CanMessage_Parse(msg.dataLow, msg.dataHigh, 26, 6);

                Rfid_RxHandler(card_used_money,card_status);
                Mp3_RxHandler(card_status, card_people_count);
                break;
            }
            default :
                break;
        }
    }
}

void Door_Observe_Status()
{
    if(door_status != DOOR_RUNNING)
    {
        Reset_Collision_Detected();
        return;
    }

    if(prev_status == DOOR_OPEN)
    {
        if(Is_Collision_Detected() == 1)
        {
            Reset_Collision_Detected();
            prev_status = DOOR_CLOSE;
            door_stuckFlag = 1;
            StepMotor_Elapse();
            Door_TxHandler(DOOR_STATUS_CAN_MSG_ID, 1);
        }
    }
    else{
        Reset_Collision_Detected();
    }
}

void Door_TxHandler(uint32 msg_type, uint8 is_collision)
{
    CanMessage msg;
    if(msg_type == DOOR_STATUS_CAN_MSG_ID)
    {
        CanMessage_Init(&msg, DOOR_STATUS_CAN_MSG_ID, DOOR_STATUS_CAN_MSG_ID, IfxMultican_DataLengthCode_1);
        CanMessage_AddSignal(&msg, 0, 1, !is_collision);
        CanMessage_AddSignal(&msg, 1, 1, is_collision);
    }
    else if(msg_type == DOOR_RETURN_OK_CAN_MSG_ID)
    {

        CanMessage_Init(&msg, DOOR_RETURN_OK_CAN_MSG_ID, DOOR_RETURN_OK_CAN_MSG_ID, IfxMultican_DataLengthCode_1);
        CanMessage_AddSignal(&msg, 0, 1, 1);
    }
    CanTxQueue_Push(&msg);
}

void Rfid_TxHandler(CardInfo cardInfo)
{
    if(rfid_readFlag)
    {
        CanMessage msg;
        CanMessage_Init(&msg, RFID_CARD_INFO_CAN_MSG_ID, RFID_CARD_INFO_CAN_MSG_ID, IfxMultican_DataLengthCode_6);
        CanMessage_AddSignal(&msg, 0, 32, cardInfo.money);
        CanMessage_AddSignal(&msg, 32, 8, cardInfo.id);
        CanMessage_AddSignal(&msg, 40, 4, cardInfo.type);
        CanTxQueue_Push(&msg);

    }
}

void Door_RxHandler(uint8 door_open, uint8 door_close)
{
    if(door_status == DOOR_OPEN && door_open == 1) return;
    if(door_status == DOOR_CLOSE && door_close == 1) return;
    if(door_status == DOOR_OPEN || door_status == DOOR_CLOSE)
    {
        prev_status = door_status;
        door_status = DOOR_RUNNING;
        if(prev_status == DOOR_OPEN)
        {
            StepMotor_Start(DOOR_STEPS, DOOR_DIR_CLOSE);
        }
        else if(prev_status == DOOR_CLOSE)
        {
            StepMotor_Start(DOOR_STEPS, DOOR_DIR_OPEN);
        }
    }
}

void Rfid_RxHandler(uint16 card_used_money, uint8 card_status)
{
    if(card_status!=CARD_ERROR)
    {
        MinusCardMoney(card_used_money);
    }

    WriteCardInfoToRfid();
}

void Mp3_RxHandler(uint8 card_status, uint8 card_people_count)
{
    if(card_status == CARD_ERROR)
    {
        Mp3_Music_Set(NO_MONEY_SAD_SOUND);
    }
    else if(card_status == CARD_EXIST)
    {
        Mp3_Music_Set(ALREADY_SOUND);
    }
    else if(card_status == CARD_OUT)
    {
        CardType type = GetCardInfo().type;;
        if(type == CARD_TYPE_DEFAULT)
        {
            Mp3_Music_Set(DEFAULT_OUT_SOUND);
        }
        else if(type == CARD_TYPE_YOUTH)
        {
            Mp3_Music_Set(YOUTH_OUT_SOUND);
        }
    }
    else if(card_status == CARD_IN)
    {

        if(card_people_count >= 2)
        {
            Mp3_Music_Set(MANY_IN_SOUND);
        }
        else
        {
            CardType type = GetCardInfo().type;;
            if(type == CARD_TYPE_DEFAULT)
            {

                Mp3_Music_Set(DEFAULT_IN_SOUND);
            }
            else if(type == CARD_TYPE_YOUTH)
            {
                Mp3_Music_Set(YOUTH_IN_SOUND);
            }
        }
    }
}


void on_motor_move_end(uint32 distance)
{
    if(distance == DOOR_DIR_OPEN)
    {
        door_status = DOOR_OPEN;

        if(door_stuckFlag > 0)
        {
            Door_TxHandler(DOOR_RETURN_OK_CAN_MSG_ID, 1);
            door_stuckFlag = 0;
        }
        else
        {
            Door_TxHandler(DOOR_STATUS_CAN_MSG_ID, 0);
        }
    }

    else if(distance == DOOR_DIR_CLOSE)
    {
        door_status = DOOR_CLOSE;
        Door_TxHandler(DOOR_STATUS_CAN_MSG_ID, 0);
    }
}


void event_10us(void){
}
void event_100us(void)
{

}
void event_500us(void)
{
    StepMotor_event_500us();
}
void event_1ms(void)
{
}
void event_10ms(void)
{

}
void event_1000ms(void)
{

}

