//###########################################################################
// FILE:   udma.h
// TITLE:  Prototypes and macros for the uDMA controller.
//###########################################################################
// $TI Release: F28M35x Support Library v202 $
// $Release Date: Tue Apr  8 12:35:30 CDT 2014 $
//###########################################################################

#ifndef __UDMA_H__
#define __UDMA_H__

//*****************************************************************************
// If building with a C++ compiler, make all of the definitions in this header
// have a C binding.
//*****************************************************************************
#ifdef __cplusplus
extern "C"
{
#endif

//*****************************************************************************
//! \addtogroup udma_api
//! @{
//*****************************************************************************

//*****************************************************************************
// A structure that defines an entry in the channel control table.  These
// fields are used by the uDMA controller and normally it is not necessary for
// software to directly read or write fields in the table.
//*****************************************************************************
typedef struct
{
        // The ending source address of the data transfer.
        volatile void *pvSrcEndAddr;

        // The ending destination address of the data transfer.
        volatile void *pvDstEndAddr;

        // The channel control mode.
        volatile unsigned long ulControl;

        // An unused location.
        volatile unsigned long ulSpare;
}
tDMAControlTable;

//*****************************************************************************
//! A helper macro for building scatter-gather task table entries.
//!
//! \param ulTransferCount is the count of items to transfer for this task.
//! \param ulItemSize is the bit size of the items to transfer for this task.
//! \param ulSrcIncrement is the bit size increment for source data.
//! \param pvSrcAddr is the starting address of the data to transfer.
//! \param ulDstIncrement is the bit size increment for destination data.
//! \param pvDstAddr is the starting address of the destination data.
//! \param ulArbSize is the arbitration size to use for the transfer task.
//! \param ulMode is the transfer mode for this task.
//!
//! This macro is intended to be used to help populate a table of uDMA tasks
//! for a scatter-gather transfer.  This macro will calculate the values for
//! the fields of a task structure entry based on the input parameters.
//!
//! There are specific requirements for the values of each parameter.  No
//! checking is done so it is up to the caller to ensure that correct values
//! are used for the parameters.
//!
//! The \e ulTransferCount parameter is the number of items that will be
//! transferred by this task.  It must be in the range 1-1024.
//!
//! The \e ulItemSize parameter is the bit size of the transfer data.  It must
//! be one of \b UDMA_SIZE_8, \b UDMA_SIZE_16, or \b UDMA_SIZE_32.
//!
//! The \e ulSrcIncrement parameter is the increment size for the source data.
//! It must be one of \b UDMA_SRC_INC_8, \b UDMA_SRC_INC_16,
//! \b UDMA_SRC_INC_32, or \b UDMA_SRC_INC_NONE.
//!
//! The \e pvSrcAddr parameter is a void pointer to the beginning of the source
//! data.
//!
//! The \e ulDstIncrement parameter is the increment size for the destination
//! data.  It must be one of \b UDMA_DST_INC_8, \b UDMA_DST_INC_16,
//! \b UDMA_DST_INC_32, or \b UDMA_DST_INC_NONE.
//!
//! The \e pvDstAddr parameter is a void pointer to the beginning of the
//! location where the data will be transferred.
//!
//! The \e ulArbSize parameter is the arbitration size for the transfer, and
//! must be one of \b UDMA_ARB_1, \b UDMA_ARB_2, \b UDMA_ARB_4, and so on
//! up to \b UDMA_ARB_1024.  This is used to select the arbitration size in
//! powers of 2, from 1 to 1024.
//!
//! The \e ulMode parameter is the mode to use for this transfer task.  It
//! must be one of \b UDMA_MODE_BASIC, \b UDMA_MODE_AUTO,
//! \b UDMA_MODE_MEM_SCATTER_GATHER, or \b UDMA_MODE_PER_SCATTER_GATHER.  Note
//! that normally all tasks will be one of the scatter-gather modes while the
//! last task is a task list will be AUTO or BASIC.
//!
//! This macro is intended to be used to initialize individual entries of
//! a structure of tDMAControlTable type, like this:
//!
//! \verbatim
//!     tDMAControlTable MyTaskList[] =
//!     {
//!         uDMATaskStructEntry(Task1Count, UDMA_SIZE_8,
//!                             UDMA_SRC_INC_8, MySourceBuf,
//!                             UDMA_DST_INC_8, MyDestBuf,
//!                             UDMA_ARB_8, UDMA_MODE_MEM_SCATTER_GATHER),
//!         uDMATaskStructEntry(Task2Count, ... ),
//!     }
//! \endverbatim
//!
//! \return Nothing; this is not a function.
//*****************************************************************************
#define uDMATaskStructEntry(ulTransferCount,                               \
                            ulItemSize,                                    \
                            ulSrcIncrement,                                \
                            pvSrcAddr,                                     \
                            ulDstIncrement,                                \
                            pvDstAddr,                                     \
                            ulArbSize,                                     \
                            ulMode)                                        \
    {                                                                      \
        (((ulSrcIncrement) == UDMA_SRC_INC_NONE) ? (pvSrcAddr) :           \
         ((void *)(&((unsigned char *)(pvSrcAddr))[((ulTransferCount) <<   \
                                                    ((ulSrcIncrement) >>   \
                                                     26)) - 1]))),         \
        (((ulDstIncrement) == UDMA_DST_INC_NONE) ? (pvDstAddr) :           \
         ((void *)(&((unsigned char *)(pvDstAddr))[((ulTransferCount) <<   \
                                                    ((ulDstIncrement) >>   \
                                                     30)) - 1]))),         \
        (ulSrcIncrement) | (ulDstIncrement) | (ulItemSize) | (ulArbSize) | \
        (((ulTransferCount) - 1) << 4) |                                   \
        ((((ulMode) == UDMA_MODE_MEM_SCATTER_GATHER) ||                    \
          ((ulMode) == UDMA_MODE_PER_SCATTER_GATHER)) ?                    \
         (ulMode) | UDMA_MODE_ALT_SELECT : (ulMode)), 0                    \
    }

//*****************************************************************************
// Close the Doxygen group.
//! @}
//*****************************************************************************

//*****************************************************************************
// Flags that can be passed to uDMAChannelAttributeEnable(),
// uDMAChannelAttributeDisable(), and returned from uDMAChannelAttributeGet().
//*****************************************************************************
#define UDMA_ATTR_USEBURST              0x00000001
#define UDMA_ATTR_ALTSELECT             0x00000002
#define UDMA_ATTR_HIGH_PRIORITY         0x00000004
#define UDMA_ATTR_REQMASK               0x00000008
#define UDMA_ATTR_ALL                   0x0000000F

//*****************************************************************************
// DMA control modes that can be passed to uDMAModeSet() and returned
// uDMAModeGet().
//*****************************************************************************
#define UDMA_MODE_STOP                  0x00000000
#define UDMA_MODE_BASIC                 0x00000001
#define UDMA_MODE_AUTO                  0x00000002
#define UDMA_MODE_PINGPONG              0x00000003
#define UDMA_MODE_MEM_SCATTER_GATHER    0x00000004
#define UDMA_MODE_PER_SCATTER_GATHER    0x00000006
#define UDMA_MODE_ALT_SELECT            0x00000001

//*****************************************************************************
// Channel configuration values that can be passed to uDMAControlSet().
//*****************************************************************************
#define UDMA_DST_INC_8                  0x00000000
#define UDMA_DST_INC_16                 0x40000000
#define UDMA_DST_INC_32                 0x80000000
#define UDMA_DST_INC_NONE               0xc0000000
#define UDMA_SRC_INC_8                  0x00000000
#define UDMA_SRC_INC_16                 0x04000000
#define UDMA_SRC_INC_32                 0x08000000
#define UDMA_SRC_INC_NONE               0x0c000000
#define UDMA_SIZE_8                     0x00000000
#define UDMA_SIZE_16                    0x11000000
#define UDMA_SIZE_32                    0x22000000
#define UDMA_ARB_1                      0x00000000
#define UDMA_ARB_2                      0x00004000
#define UDMA_ARB_4                      0x00008000
#define UDMA_ARB_8                      0x0000c000
#define UDMA_ARB_16                     0x00010000
#define UDMA_ARB_32                     0x00014000
#define UDMA_ARB_64                     0x00018000
#define UDMA_ARB_128                    0x0001c000
#define UDMA_ARB_256                    0x00020000
#define UDMA_ARB_512                    0x00024000
#define UDMA_ARB_1024                   0x00028000
#define UDMA_NEXT_USEBURST              0x00000008

//*****************************************************************************
// Channel numbers to be passed to API functions that require a channel number
// ID.
//*****************************************************************************
#define UDMA_CHANNEL_USBEP1RX           0
#define UDMA_CHANNEL_USBEP1TX           1
#define UDMA_CHANNEL_USBEP2RX           2
#define UDMA_CHANNEL_USBEP2TX           3
#define UDMA_CHANNEL_USBEP3RX           4
#define UDMA_CHANNEL_USBEP3TX           5
#define UDMA_CHANNEL_ETH0RX             6
#define UDMA_CHANNEL_ETH0TX             7
#define UDMA_CHANNEL_UART0RX            8
#define UDMA_CHANNEL_UART0TX            9
#define UDMA_CHANNEL_SSI0RX             10
#define UDMA_CHANNEL_SSI0TX             11
#define UDMA_CHANNEL_SW_12              12
#define UDMA_CHANNEL_SW_13              13
#define UDMA_CHANNEL_TMR0A              18
#define UDMA_CHANNEL_TMR0B              19
#define UDMA_CHANNEL_TMR1A              20
#define UDMA_CHANNEL_TMR1B              21
#define UDMA_CHANNEL_UART1RX            22
#define UDMA_CHANNEL_UART1TX            23
#define UDMA_CHANNEL_SSI1RX             24
#define UDMA_CHANNEL_SSI1TX             25
#define UDMA_CHANNEL_SW_26              26
#define UDMA_CHANNEL_SW_27              27
#define UDMA_CHANNEL_SW_30              30

//Included for compatability with Stellaris examples
#define UDMA_CHANNEL_SW                 UDMA_CHANNEL_SW_12

//*****************************************************************************
// Flags to be OR'd with the channel ID to indicate if the primary or alternate
// control structure should be used.
//*****************************************************************************
#define UDMA_PRI_SELECT                 0x00000000
#define UDMA_ALT_SELECT                 0x00000020

//*****************************************************************************
// uDMA interrupt sources, to be passed to uDMAIntRegister() and
// uDMAIntUnregister().
//*****************************************************************************
#define UDMA_INT_SW                     62
#define UDMA_INT_ERR                    63

//*****************************************************************************
// Channel numbers to be passed to API functions that require a channel number
// ID.  These are for secondary peripheral assignments.
//*****************************************************************************
#define UDMA_SEC_CHANNEL_UART2RX_0      0
#define UDMA_SEC_CHANNEL_UART2TX_1      1
#define UDMA_SEC_CHANNEL_TMR3A          2
#define UDMA_SEC_CHANNEL_TMR3B          3
#define UDMA_SEC_CHANNEL_TMR2A_4        4
#define UDMA_SEC_CHANNEL_TMR2B_5        5
#define UDMA_SEC_CHANNEL_TMR2A_6        6
#define UDMA_SEC_CHANNEL_TMR2B_7        7
#define UDMA_SEC_CHANNEL_UART1RX        8
#define UDMA_SEC_CHANNEL_UART1TX        9
#define UDMA_SEC_CHANNEL_SSI1RX         10
#define UDMA_SEC_CHANNEL_SSI1TX         11
#define UDMA_SEC_CHANNEL_UART2RX_12     12
#define UDMA_SEC_CHANNEL_UART2TX_13     13
#define UDMA_SEC_CHANNEL_TMR2A_14       14
#define UDMA_SEC_CHANNEL_TMR2B_15       15
#define UDMA_SEC_CHANNEL_TMR1A          18
#define UDMA_SEC_CHANNEL_TMR1B          19
#define UDMA_SEC_CHANNEL_EPI0RX         20
#define UDMA_SEC_CHANNEL_EPI0TX         21
#define UDMA_SEC_CHANNEL_SW_22          22
#define UDMA_SEC_CHANNEL_SW_23          23
#define UDMA_SEC_CHANNEL_SW_28          28
#define UDMA_SEC_CHANNEL_SW_29          29
#define UDMA_SEC_CHANNEL_SW_30          30



//*****************************************************************************
// Channel numbers to be passed to API functions that require a channel number
// ID.  These are for third peripheral assignments.
//*****************************************************************************
#define UDMA_THRD_CHANNEL_SSI2RX        12
#define UDMA_THRD_CHANNEL_SSI2TX        13
#define UDMA_THRD_CHANNEL_SSI3RX        14
#define UDMA_THRD_CHANNEL_SSI3TX        15
#define UDMA_THRD_CHANNEL_UART3RX       16
#define UDMA_THRD_CHANNEL_UART3TX       17
#define UDMA_THRD_CHANNEL_UART4RX       18
#define UDMA_THRD_CHANNEL_UART4TX       19


//*****************************************************************************
// uDMA Channel 0-7 default peripheral selection masks, to be passed to
// uDMAChannel0_7SelectDefMapping().
//*****************************************************************************
#define UDMA_CHAN0_DEF_USBEP1RX_M       0x0000000F
#define UDMA_CHAN1_DEF_USBEP1TX_M       0x000000F0
#define UDMA_CHAN2_DEF_USBEP2RX_M       0x00000F00
#define UDMA_CHAN3_DEF_USBEP2TX_M       0x0000F000
#define UDMA_CHAN4_DEF_USBEP3RX_M       0x000F0000
#define UDMA_CHAN5_DEF_USBEP3TX_M       0x00F00000
#define UDMA_CHAN6_DEF_ETH0RX_M         0x0F000000
#define UDMA_CHAN7_DEF_ETH0TX_M         0xF0000000


//*****************************************************************************
// uDMA Channel 0-7 second/third peripheral selections, to be passed to
// uDMAChannel0_7SelectAltMapping(). Any options not listed are Reserved.
//*****************************************************************************
#define UDMA_CHAN0_SEC_UART2RX          0x00000001
#define UDMA_CHAN1_SEC_UART2TX          0x00000010
#define UDMA_CHAN2_SEC_TMR3A            0x00000100
#define UDMA_CHAN3_SEC_TMR3B            0x00001000
#define UDMA_CHAN4_SEC_TMR2A            0x00010000
#define UDMA_CHAN5_SEC_TMR2B            0x00100000
#define UDMA_CHAN6_SEC_TMR2A            0x01000000
#define UDMA_CHAN7_SEC_TMR2B            0x10000000

//*****************************************************************************
// uDMA Channel 8-15 default peripheral selection masks, to be passed to
// uDMAChannel8_15SelectDefMapping().
//*****************************************************************************
#define UDMA_CHAN8_DEF_UART0RX_M        0x0000000F
#define UDMA_CHAN9_DEF_UART0TX_M        0x000000F0
#define UDMA_CHAN10_DEF_SSI0RX_M        0x00000F00
#define UDMA_CHAN11_DEF_SSI0TX_M        0x0000F000
#define UDMA_CHAN12_DEF_SW_M            0x000F0000
#define UDMA_CHAN13_DEF_SW_M            0x00F00000
#define UDMA_CHAN14_DEF_RSVD_M          0x0F000000
#define UDMA_CHAN15_DEF_RSVD_M          0xF0000000

//*****************************************************************************
// uDMA Channel 8-15 second/third peripheral selections, to be passed to
// uDMAChannel8_15SelectAltMapping(). Any options not listed are Reserved.
//*****************************************************************************
#define UDMA_CHAN8_SEC_UART1RX          0x00000001
#define UDMA_CHAN9_SEC_UART1TX          0x00000010
#define UDMA_CHAN10_SEC_SSI1RX          0x00000100
#define UDMA_CHAN11_SEC_SSI1TX          0x00001000
#define UDMA_CHAN12_SEC_UART2RX         0x00010000
#define UDMA_CHAN12_THRD_SSI2RX         0x00020000
#define UDMA_CHAN13_SEC_UART2TX         0x00100000
#define UDMA_CHAN13_THRD_SSI2TX         0x00200000
#define UDMA_CHAN14_SEC_TMR2A           0x01000000
#define UDMA_CHAN14_THRD_SSI3RX         0x02000000
#define UDMA_CHAN15_SEC_TMR2B           0x10000000
#define UDMA_CHAN15_THRD_SSI3TX         0x20000000

//*****************************************************************************
// uDMA Channel 16-23 default peripheral selection masks, to be passed to
// uDMAChannel16_23SelectDefMapping().
//*****************************************************************************
#define UDMA_CHAN16_DEF_RSVD_M          0x0000000F
#define UDMA_CHAN17_DEF_RSVD_M          0x000000F0
#define UDMA_CHAN18_DEF_TMR0A_M         0x00000F00
#define UDMA_CHAN19_DEF_TMR0B_M         0x0000F000
#define UDMA_CHAN20_DEF_TMR1A_M         0x000F0000
#define UDMA_CHAN21_DEF_TMR1B_M         0x00F00000
#define UDMA_CHAN22_DEF_UART1RX_M       0x0F000000
#define UDMA_CHAN23_DEF_UART1TX_M       0xF0000000

//*****************************************************************************
// uDMA Channel 16-23 second/third peripheral selections, to be passed to
// uDMAChannel16_23SelectAltMapping(). Any options not listed are Reserved.
//*****************************************************************************
#define UDMA_CHAN16_THRD_UART3RX        0x00000002
#define UDMA_CHAN17_THRD_UART3TX        0x00000020
#define UDMA_CHAN18_SEC_TMR1A           0x00000100
#define UDMA_CHAN18_THRD_UART4RX        0x00000200
#define UDMA_CHAN19_SEC_TMR1B           0x00001000
#define UDMA_CHAN19_THRD_UART4TX        0x00002000
#define UDMA_CHAN20_SEC_EPI0RX          0x00010000
#define UDMA_CHAN21_SEC_EPI0TX          0x00100000
#define UDMA_CHAN22_SEC_SW              0x01000000
#define UDMA_CHAN23_SEC_SW              0x10000000

//*****************************************************************************
// uDMA Channel 24-31 default peripheral selections, to be passed to
// uDMAChannel24_31SelectDefMapping().
//*****************************************************************************
#define UDMA_CHAN24_DEF_SSI1RX_M        0x0000000F
#define UDMA_CHAN25_DEF_SSI1TX_M        0x000000F0
#define UDMA_CHAN26_DEF_SW_M            0x00000F00
#define UDMA_CHAN27_DEF_SW_M            0x0000F000
#define UDMA_CHAN28_DEF_RSVD_M          0x000F0000
#define UDMA_CHAN29_DEF_RSVD_M          0x00F00000
#define UDMA_CHAN30_DEF_SW_M            0x0F000000
#define UDMA_CHAN31_DEF_RSVD_M          0xF0000000

//*****************************************************************************
// uDMA Channel 24-31 second/third peripheral selections, to be passed to
// uDMAChannel24_31SelectAltMapping(). Any options not listed are Reserved.
//*****************************************************************************
#define UDMA_CHAN28_SEC_SW              0x00010000
#define UDMA_CHAN29_SEC_SW              0x00100000
#define UDMA_CHAN30_SEC_SW              0x01000000


//*****************************************************************************
// API Function prototypes
//*****************************************************************************
extern void uDMAEnable(void);
extern void uDMADisable(void);
extern unsigned long uDMAErrorStatusGet(void);
extern void uDMAErrorStatusClear(void);
extern void uDMAChannelEnable(unsigned long ulChannelNum);
extern void uDMAChannelDisable(unsigned long ulChannelNum);
extern tBoolean uDMAChannelIsEnabled(unsigned long ulChannelNum);
extern void uDMAControlBaseSet(void *pControlTable);
extern void *uDMAControlBaseGet(void);
extern void *uDMAControlAlternateBaseGet(void);
extern void uDMAChannelRequest(unsigned long ulChannelNum);
extern void uDMAChannelAttributeEnable(unsigned long ulChannelNum,
                                       unsigned long ulAttr);
extern void uDMAChannelAttributeDisable(unsigned long ulChannelNum,
                                        unsigned long ulAttr);
extern unsigned long uDMAChannelAttributeGet(unsigned long ulChannelNum);
extern void uDMAChannelControlSet(unsigned long ulChannelStructIndex,
                                  unsigned long ulControl);
extern void uDMAChannelTransferSet(unsigned long ulChannelStructIndex,
                                   unsigned long ulMode, void *pvSrcAddr,
                                   void *pvDstAddr,
                                   unsigned long ulTransferSize);
extern void uDMAChannelScatterGatherSet(unsigned long ulChannelNum,
                                        unsigned ulTaskCount, void *pvTaskList,
                                        unsigned long ulIsPeriphSG);
extern unsigned long uDMAChannelSizeGet(unsigned long ulChannelStructIndex);
extern unsigned long uDMAChannelModeGet(unsigned long ulChannelStructIndex);
extern void uDMAIntRegister(unsigned long ulIntChannel,
                            void (*pfnHandler)(void));
extern void uDMAIntUnregister(unsigned long ulIntChannel);
extern void uDMAChannel0_7SelectDefault(unsigned long ulDefPeriphs);
extern void uDMAChannel8_15SelectDefault(unsigned long ulDefPeriphs);
extern void uDMAChannel16_23SelectDefault(unsigned long ulDefPeriphs);
extern void uDMAChannel24_31SelectDefault(unsigned long ulDefPeriphs);
extern void uDMAChannel0_7SelectAltMapping (unsigned long ulAltPeriphs);
extern void uDMAChannel8_15SelectAltMapping (unsigned long ulAltPeriphs);
extern void uDMAChannel16_23SelectAltMapping (unsigned long ulAltPeriphs);
extern void uDMAChannel24_31SelectAltMapping (unsigned long ulAltPeriphs);

//*****************************************************************************
// Mark the end of the C bindings section for C++ compilers.
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif //  __UDMA_H__



