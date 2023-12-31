//###########################################################################
// FILE:   timer.h
// TITLE:  Prototypes for the timer module
//###########################################################################
// $TI Release: F28M35x Support Library v202 $
// $Release Date: Tue Apr  8 12:35:30 CDT 2014 $
//###########################################################################

#ifndef __TIMER_H__
#define __TIMER_H__

//*****************************************************************************
// If building with a C++ compiler, make all of the definitions in this header
// have a C binding.
//*****************************************************************************
#ifdef __cplusplus
extern "C"
{
#endif

//*****************************************************************************
// Values that can be passed to TimerConfigure as the ulConfig parameter.
//*****************************************************************************
#define TIMER_CFG_32_BIT_OS        0x00000021   // 32-bit one-shot timer
#define TIMER_CFG_32_BIT_OS_UP     0x00000031   // 32-bit one-shot up-count
                                                // timer
#define TIMER_CFG_32_BIT_PER       0x00000022   // 32-bit periodic timer
#define TIMER_CFG_32_BIT_PER_UP    0x00000032   // 32-bit periodic up-count
                                                // timer
#define TIMER_CFG_32_RTC           0x01000000   // 32-bit RTC timer
#define TIMER_CFG_16_BIT_PAIR      0x04000000   // Two 16-bit timers
#define TIMER_CFG_A_ONE_SHOT       0x00000021   // Timer A one-shot timer
#define TIMER_CFG_A_ONE_SHOT_UP    0x00000031   // Timer A one-shot up-count
                                                // timer
#define TIMER_CFG_A_PERIODIC       0x00000022   // Timer A periodic timer
#define TIMER_CFG_A_PERIODIC_UP    0x00000032   // Timer A periodic up-count
                                                // timer
#define TIMER_CFG_A_CAP_COUNT      0x00000003   // Timer A event counter
#define TIMER_CFG_A_CAP_TIME       0x00000007   // Timer A event timer
#define TIMER_CFG_A_PWM            0x0000000A   // Timer A PWM output
#define TIMER_CFG_B_ONE_SHOT       0x00002100   // Timer B one-shot timer
#define TIMER_CFG_B_ONE_SHOT_UP    0x00003100   // Timer B one-shot up-count
                                                // timer
#define TIMER_CFG_B_PERIODIC       0x00002200   // Timer B periodic timer
#define TIMER_CFG_B_PERIODIC_UP    0x00003200   // Timer B periodic up-count
                                                // timer
#define TIMER_CFG_B_CAP_COUNT      0x00000300   // Timer B event counter
#define TIMER_CFG_B_CAP_TIME       0x00000700   // Timer B event timer
#define TIMER_CFG_B_PWM            0x00000A00   // Timer B PWM output

//*****************************************************************************
// Values that can be passed to TimerIntEnable, TimerIntDisable, and
// TimerIntClear as the ulIntFlags parameter, and returned from TimerIntStatus.
//*****************************************************************************
#define TIMER_TIMB_MATCH           0x00000800   // TimerB match interrupt
#define TIMER_CAPB_EVENT           0x00000400   // CaptureB event interrupt
#define TIMER_CAPB_MATCH           0x00000200   // CaptureB match interrupt
#define TIMER_TIMB_TIMEOUT         0x00000100   // TimerB time out interrupt
#define TIMER_TIMA_MATCH           0x00000010   // TimerA match interrupt
#define TIMER_RTC_MATCH            0x00000008   // RTC interrupt mask
#define TIMER_CAPA_EVENT           0x00000004   // CaptureA event interrupt
#define TIMER_CAPA_MATCH           0x00000002   // CaptureA match interrupt
#define TIMER_TIMA_TIMEOUT         0x00000001   // TimerA time out interrupt

//*****************************************************************************
// Values that can be passed to TimerControlEvent as the ulEvent parameter.
//*****************************************************************************
#define TIMER_EVENT_POS_EDGE       0x00000000   // Count positive edges
#define TIMER_EVENT_NEG_EDGE       0x00000404   // Count negative edges
#define TIMER_EVENT_BOTH_EDGES     0x00000C0C   // Count both edges

//*****************************************************************************
// Values that can be passed to most of the timer APIs as the ulTimer
// parameter.
//*****************************************************************************
#define TIMER_A                    0x000000ff   // Timer A
#define TIMER_B                    0x0000ff00   // Timer B
#define TIMER_BOTH                 0x0000ffff   // Timer Both

//*****************************************************************************
// Prototypes for the APIs.
//*****************************************************************************
extern void TimerEnable(unsigned long ulBase, unsigned long ulTimer);
extern void TimerDisable(unsigned long ulBase, unsigned long ulTimer);
extern void TimerConfigure(unsigned long ulBase, unsigned long ulConfig);
extern void TimerControlLevel(unsigned long ulBase, unsigned long ulTimer,
                              tBoolean bInvert);
extern void TimerControlTrigger(unsigned long ulBase, unsigned long ulTimer,
                                tBoolean bEnable);
extern void TimerControlEvent(unsigned long ulBase, unsigned long ulTimer,
                              unsigned long ulEvent);
extern void TimerControlStall(unsigned long ulBase, unsigned long ulTimer,
                              tBoolean bStall);
extern void TimerControlWaitOnTrigger(unsigned long ulBase,
                                      unsigned long ulTimer,
                                      tBoolean      bWait);
extern void TimerRTCEnable(unsigned long ulBase);
extern void TimerRTCDisable(unsigned long ulBase);
extern void TimerPrescaleSet(unsigned long ulBase, unsigned long ulTimer,
                             unsigned long ulValue);
extern unsigned long TimerPrescaleGet(unsigned long ulBase,
                                      unsigned long ulTimer);
extern void TimerPrescaleMatchSet(unsigned long ulBase, unsigned long ulTimer,
                                  unsigned long ulValue);
extern unsigned long TimerPrescaleMatchGet(unsigned long ulBase,
                                           unsigned long ulTimer);
extern void TimerLoadSet(unsigned long ulBase, unsigned long ulTimer,
                         unsigned long ulValue);
extern unsigned long TimerLoadGet(unsigned long ulBase, unsigned long ulTimer);
extern unsigned long TimerValueGet(unsigned long ulBase,
                                   unsigned long ulTimer);
extern void TimerMatchSet(unsigned long ulBase, unsigned long ulTimer,
                          unsigned long ulValue);
extern unsigned long TimerMatchGet(unsigned long ulBase,
                                   unsigned long ulTimer);
extern void TimerIntRegister(unsigned long ulBase, unsigned long ulTimer,
                             void (*pfnHandler)(void));
extern void TimerIntUnregister(unsigned long ulBase, unsigned long ulTimer);
extern void TimerIntEnable(unsigned long ulBase, unsigned long ulIntFlags);
extern void TimerIntDisable(unsigned long ulBase, unsigned long ulIntFlags);
extern unsigned long TimerIntStatus(unsigned long ulBase, tBoolean bMasked);
extern void TimerIntClear(unsigned long ulBase, unsigned long ulIntFlags);

//*****************************************************************************
// Mark the end of the C bindings section for C++ compilers.
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif // __TIMER_H__



