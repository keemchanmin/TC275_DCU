/***********************************************************************/
/*Include*/
/***********************************************************************/
#include <Asclin1.h>
#include <Asclin3.h>
#include "Rfid.h"
#include "Ifx_Types.h"
#include "IfxPort.h"
#include "IfxPort_PinMap.h"

/***********************************************************************/
/*Define*/
/***********************************************************************/
#define RX_RFID_BYTE_SIZE 6
#define TX_RFID_BYTE_SIZE 5

#define RX_RFID_CARD_ID_OFFSET 0
#define RX_RFID_CARD_TYPE_OFFSET 1
#define RX_RFID_CARD_MONEY_OFFSET 2

#define TX_RFID_CARD_ID_OFFSET 0
#define TX_RFID_CARD_MONEY_OFFSET 1

#define RFID_CARD_ID_SIZE 1
#define RFID_CARD_TYPE_SIZE 1
#define RFID_CARD_MONEY_SIZE 4

/***********************************************************************/
/*Typedef*/
/***********************************************************************/


/***********************************************************************/
/*Static Function Prototype*/
/***********************************************************************/


/***********************************************************************/
/*Variable*/
/***********************************************************************/
static CardInfo g_CardInfo;
uint8 readComplFlag = 0;

/***********************************************************************/
/*Function*/
/***********************************************************************/
void Rfid_Init()
{
    Asclin1_Init();
    Asclin3_Init();
}

void Reset_Card_Info()
{
    g_CardInfo.id = 0;
    g_CardInfo.type = 0;
    g_CardInfo.money = 0;
}

CardInfo GetCardInfo()
{
    return g_CardInfo;
}

uint8 ReadCardInfoFromRfid()
{
    uint8* rxData = Asclin1_Read();

    if(rxData != -1)
    {
        Reset_Card_Info();
        int idx = 0, money_mult = 1;
        while(idx < RX_RFID_BYTE_SIZE)
        {
            if(idx == RX_RFID_CARD_ID_OFFSET)
            {
                g_CardInfo.id = rxData[idx];
            }
            else if(idx == RX_RFID_CARD_TYPE_OFFSET)
            {
                g_CardInfo.type = rxData[idx];
            }
            else if(idx >= RX_RFID_CARD_MONEY_OFFSET)
            {
                g_CardInfo.money += (rxData[idx] * money_mult);
                money_mult *= 256;
            }
            idx++;
        }
        return 1;
    }

    return 0;
}

void WriteCardInfoToRfid()
{
    int idx = 0, money_shift = 0;
    while(idx < TX_RFID_BYTE_SIZE)
    {
        if(idx == TX_RFID_CARD_ID_OFFSET)
        {
            Asclin1_Write(g_CardInfo.id);
        }
        else if(idx >= TX_RFID_CARD_MONEY_OFFSET)
        {
            Asclin1_Write((g_CardInfo.money >> (8*money_shift)) % 256);
            money_shift++;
        }
        idx++;
    }

}

void MinusCardMoney(uint16 amount)
{
    g_CardInfo.money -= amount;
}
