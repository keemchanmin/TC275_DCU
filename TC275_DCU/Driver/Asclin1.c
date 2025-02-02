#include <Asclin1.h>
#include <Asclin3.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

#include "IfxAsclin_Asc.h"
#include "IfxPort.h"


#define USB_UART_MAX_PRINT_SIZE (255)

#define ASC1_TX_BUFFER_SIZE          256                                     /* Define the TX buffer size in byte    */
#define ASC1_RX_BUFFER_SIZE          256                                     /* Define the RX buffer size in byte    */
#define ASC1_BAUDRATE                9600                                     /* Define the UART baud rate            */


#define _M_ASCLIN1_TX_OUT       IfxAsclin1_TX_P15_0_OUT
#define _M_ASCLIN1_RX_IN       IfxAsclin1_RXA_P15_1_IN

/* USB - ASCLIN2 */
#define ISR_PRIORITY_ASCLIN1_TX     0x21    /* Priority for interrupt ASCLIN2 Transmit  */
#define ISR_PRIORITY_ASCLIN1_RX     0x20    /* Priority for interrupt ASCLIN2 Receive   */
#define ISR_PRIORITY_ASCLIN1_ER     0x22    /* Priority for interrupt ASCLIN2 Errors    */

/***********************************************************************/
/*Typedef*/
/***********************************************************************/

/***********************************************************************/
/*Static Function Prototype*/
/***********************************************************************/

/***********************************************************************/
/*Variable*/
/***********************************************************************/
static IfxAsclin_Asc s_asclin1;

static uint8 s_asclin1_tx_buf[ASC1_TX_BUFFER_SIZE + sizeof(Ifx_Fifo) + 8];
static uint8 s_asclin1_rx_buf[ASC1_RX_BUFFER_SIZE + sizeof(Ifx_Fifo) + 8];

static uint8 rxdata[32];
static Ifx_SizeT rxlength;
static uint8 s_asclin1_is_received = 0;
/***********************************************************************/
/*Function*/
/***********************************************************************/
void Asclin1_Printf(pchar format,...)
{
    char  message[USB_UART_MAX_PRINT_SIZE + 1];
    Ifx_SizeT count;
    va_list   args;
    va_start(args, format);
    vsprintf((char *)message, format, args);
    va_end(args);
    count = (Ifx_SizeT)strlen(message);
    IFX_ASSERT(IFX_VERBOSE_LEVEL_ERROR, count < STDIF_DPIPE_MAX_PRINT_SIZE);

    char *ch_ptr = message;
    char ch = *ch_ptr;
    while(ch != 0) {
        if(ch == '\n') {
            IfxAsclin_Asc_blockingWrite(&s_asclin1, '\r');
            IfxAsclin_Asc_blockingWrite(&s_asclin1, '\n');
        } else {
            IfxAsclin_Asc_blockingWrite(&s_asclin1, ch);
        }
        ch_ptr++;
        ch = *ch_ptr;
    }

    //return
}

void Asclin1_Write(uint8 data)
{
    IfxAsclin_Asc_blockingWrite(&s_asclin1, data);
}

uint8 Asclin1_Is_Received(){
    return s_asclin1_is_received;
}

void Reset_Rxflag_ASC1(void)
{
    s_asclin1_is_received=0;
}

uint8* Asclin1_Read(){
    if(s_asclin1_is_received > 0)
    {
        int length = 0;
        uint8 data = IfxAsclin_Asc_blockingRead(&s_asclin1);
        while(1)
        {
           if(data == 0xff)
           {
               break;
           }

           rxdata[length++] = data;
           data = IfxAsclin_Asc_blockingRead(&s_asclin1);
        }

        s_asclin1_is_received = 0;


        return rxdata;
    }
    return -1;
}

void Asclin1_Init()
{
    /* ======================= init_asclin2 ======================= */
    IfxAsclin_Asc_Config asc_conf;

    /* Set default configurations */
    IfxAsclin_Asc_initModuleConfig(&asc_conf, &MODULE_ASCLIN1); /* Initialize the structure with default values      */

    /* Set the desired baud rate */
    asc_conf.baudrate.baudrate = ASC1_BAUDRATE;                                   /* Set the baud rate in bit/s       */
    asc_conf.baudrate.oversampling = IfxAsclin_OversamplingFactor_16;            /* Set the oversampling factor      */

    /* Configure the sampling mode */
    asc_conf.bitTiming.medianFilter = IfxAsclin_SamplesPerBit_three;             /* Set the number of samples per bit*/
    asc_conf.bitTiming.samplePointPosition = IfxAsclin_SamplePointPosition_8;    /* Set the first sample position    */

    /* ISR priorities and interrupt target */
    asc_conf.interrupt.txPriority = ISR_PRIORITY_ASCLIN1_TX;  /* Set the interrupt priority for TX events             */
    asc_conf.interrupt.rxPriority = ISR_PRIORITY_ASCLIN1_RX;  /* Set the interrupt priority for RX events             */
    asc_conf.interrupt.erPriority = ISR_PRIORITY_ASCLIN1_ER;  /* Set the interrupt priority for Error events          */
    asc_conf.interrupt.typeOfService = IfxSrc_Tos_cpu0;

    /* Pin configuration */
    const IfxAsclin_Asc_Pins pins = {
          .cts        = NULL_PTR,                         /* CTS pin not used                                     */
          .ctsMode    = IfxPort_InputMode_pullUp,
          .rx         = &_M_ASCLIN1_RX_IN ,        /* Select the pin for RX connected to the USB port      */
          .rxMode     = IfxPort_InputMode_pullUp,         /* RX pin                                               */
          .rts        = NULL_PTR,                         /* RTS pin not used                                     */
          .rtsMode    = IfxPort_OutputMode_pushPull,
          .tx         = &_M_ASCLIN1_TX_OUT,         /* Select the pin for TX connected to the USB port      */
          .txMode     = IfxPort_OutputMode_pushPull,      /* TX pin                                               */
          .pinDriver  = IfxPort_PadDriver_cmosAutomotiveSpeed1
    };
    asc_conf.pins = &pins;

    /* FIFO buffers configuration */
    asc_conf.txBuffer = s_asclin1_tx_buf;                      /* Set the transmission buffer                          */
    asc_conf.txBufferSize = ASC1_TX_BUFFER_SIZE;              /* Set the transmission buffer size                     */
    asc_conf.rxBuffer = s_asclin1_rx_buf;                      /* Set the receiving buffer                             */
    asc_conf.rxBufferSize = ASC1_RX_BUFFER_SIZE;              /* Set the receiving buffer size                        */

    /* Init ASCLIN module */
    IfxAsclin_Asc_initModule(&s_asclin1, &asc_conf);          /* Initialize the module with the given configuration   */

}
IFX_INTERRUPT(asclin1_tx_ISR, 0, ISR_PRIORITY_ASCLIN1_TX);
void asclin1_tx_ISR(void)
{
    IfxAsclin_Asc_isrTransmit(&s_asclin1);
    // 이게 없으면 첫 문자만 보내지고 그 이후에는 전송이 안됨.
}

IFX_INTERRUPT(asclin1_rx_ISR, 0, ISR_PRIORITY_ASCLIN1_RX);
void asclin1_rx_ISR(void)
{
    IfxAsclin_Asc_isrReceive(&s_asclin1);
    s_asclin1_is_received = 1;
}

IFX_INTERRUPT(asclin1_err_ISR, 0, ISR_PRIORITY_ASCLIN1_ER);
void asclin1_err_ISR(void)
{
    ;
}



