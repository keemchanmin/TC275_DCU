#include <Asclin2.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

#include "IfxAsclin_Asc.h"
#include "IfxPort.h"
#include "IfxPort_PinMap.h"

#define USB_UART_MAX_PRINT_SIZE (255)

#define ASC2_TX_BUFFER_SIZE          256                                     /* Define the TX buffer size in byte    */
#define ASC2_RX_BUFFER_SIZE          256                                     /* Define the RX buffer size in byte    */
#define ASC2_BAUDRATE                9600                                  /* Define the UART baud rate            */





/* USB - ASCLIN1 */
#define ISR_PRIORITY_ASCLIN2_TX     0x31    /* Priority for interrupt ASCLIN3 Transmit  */
#define ISR_PRIORITY_ASCLIN2_RX     0x30    /* Priority for interrupt ASCLIN3 Receive   */
#define ISR_PRIORITY_ASCLIN2_ER     0x32    /* Priority for interrupt ASCLIN3 Errors    */
/***********************************************************************/
/*Typedef*/
/***********************************************************************/

/***********************************************************************/
/*Static Function Prototype*/
/***********************************************************************/

/***********************************************************************/
/*Variable*/
/***********************************************************************/
static IfxAsclin_Asc s_asclin2;
static uint8 s_asclin2_tx_buf[ASC2_TX_BUFFER_SIZE + sizeof(Ifx_Fifo) + 8];
static uint8 s_asclin2_rx_buf[ASC2_RX_BUFFER_SIZE + sizeof(Ifx_Fifo) + 8];

/***********************************************************************/
/*Function*/
/***********************************************************************/

void Send_Bytes(uint8* data, uint16 length) {
    for(uint16 i = 0; i < length; i++) {
        IfxAsclin_Asc_blockingWrite(&s_asclin2, data[i]);
        IfxAsclin_Asc_isrTransmit(&s_asclin2);
    }
    IfxAsclin_Asc_isrTransmit(&s_asclin2);
}
void Asclin2_Printf(pchar format,...)
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
            IfxAsclin_Asc_blockingWrite(&s_asclin2, '\r');
            IfxAsclin_Asc_blockingWrite(&s_asclin2, '\n');
        } else {
            IfxAsclin_Asc_blockingWrite(&s_asclin2, ch);
        }
        ch_ptr++;
        ch = *ch_ptr;
    }

    //return
}

void Asclin2_Init() {

    /* ======================= init_asclin2 ======================= */
    IfxAsclin_Asc_Config asc_conf;

    /* Set default configurations */
    IfxAsclin_Asc_initModuleConfig(&asc_conf, &MODULE_ASCLIN2); /* Initialize the structure with default values      */

    /* Set the desired baud rate */
    asc_conf.baudrate.baudrate = ASC2_BAUDRATE;                                   /* Set the baud rate in bit/s       */
    asc_conf.baudrate.oversampling = IfxAsclin_OversamplingFactor_16;            /* Set the oversampling factor      */

    /* Configure the sampling mode */
    asc_conf.bitTiming.medianFilter = IfxAsclin_SamplesPerBit_three;             /* Set the number of samples per bit*/
    asc_conf.bitTiming.samplePointPosition = IfxAsclin_SamplePointPosition_8;    /* Set the first sample position    */

    /* ISR priorities and interrupt target */
    asc_conf.interrupt.txPriority = ISR_PRIORITY_ASCLIN2_TX;  /* Set the interrupt priority for TX events             */
    asc_conf.interrupt.rxPriority = ISR_PRIORITY_ASCLIN2_RX;  /* Set the interrupt priority for RX events             */
    asc_conf.interrupt.erPriority = ISR_PRIORITY_ASCLIN2_ER;  /* Set the interrupt priority for Error events          */
    asc_conf.interrupt.typeOfService = IfxSrc_Tos_cpu0;

    /* Pin configuration */
    const IfxAsclin_Asc_Pins pins = {
          .cts        = NULL_PTR,                         /* CTS pin not used                                     */
          .ctsMode    = IfxPort_InputMode_pullUp,
          .rx         = &IfxAsclin2_RXE_P33_8_IN ,        /* Select the pin for RX connected to the USB port      */
          .rxMode     = IfxPort_InputMode_pullUp,         /* RX pin                                               */
          .rts        = NULL_PTR,                         /* RTS pin not used                                     */
          .rtsMode    = IfxPort_OutputMode_pushPull,
          .tx         = &IfxAsclin2_TX_P33_9_OUT,         /* Select the pin for TX connected to the USB port      */
          .txMode     = IfxPort_OutputMode_pushPull,      /* TX pin                                               */
          .pinDriver  = IfxPort_PadDriver_cmosAutomotiveSpeed1
    };
    asc_conf.pins = &pins;

    /* FIFO buffers configuration */
    asc_conf.txBuffer = s_asclin2_tx_buf;                      /* Set the transmission buffer                          */
    asc_conf.txBufferSize = ASC2_TX_BUFFER_SIZE;              /* Set the transmission buffer size                     */
    asc_conf.rxBuffer = s_asclin2_rx_buf;                      /* Set the receiving buffer                             */
    asc_conf.rxBufferSize = ASC2_RX_BUFFER_SIZE;              /* Set the receiving buffer size                        */

    /* Init ASCLIN module */
    IfxAsclin_Asc_initModule(&s_asclin2, &asc_conf);          /* Initialize the module with the given configuration   */

}

IFX_INTERRUPT(asclin2_tx_ISR, 0, ISR_PRIORITY_ASCLIN2_TX);
void asclin2_tx_ISR(void)
{
    IfxAsclin_Asc_isrTransmit(&s_asclin2);
    // 이게 없으면 첫 문자만 보내지고 그 이후에는 전송이 안됨.
}

IFX_INTERRUPT(asclin2_rx_ISR, 0, ISR_PRIORITY_ASCLIN2_RX);
void asclin2_rx_ISR(void)
{
    //IfxPort_togglePin(IfxPort_P10_1.port, IfxPort_P10_1.pinIndex);
    //for(int i=0;i<2000000;i++){

    //}
    IfxAsclin_Asc_isrReceive(&s_asclin2);
}

IFX_INTERRUPT(asclin1_err_ISR, 0, ISR_PRIORITY_ASCLIN2_ER);
void asclin2_err_ISR(void)
{
    ;
}



