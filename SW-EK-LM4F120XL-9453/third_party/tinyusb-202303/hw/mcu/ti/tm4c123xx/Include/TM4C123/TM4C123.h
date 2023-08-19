/**************************************************************************//**
 * @file     TM4C123.h
 * @brief    CMSIS Device System Header File for
 *           Texas Instruments Tiva TM4C123 Device Series
 * @version  V12591
 * @date     12. February 2014
 *
 * @note
 *                                                             modified by Keil
 ******************************************************************************/

#ifndef TM4C123_H
#define TM4C123_H

#if   defined(TM4C1230C3PM)
  #include "TM4C1230C3PM.h"

#elif defined(TM4C123GH6PM)
  #include "TM4C123GH6PM.h"

#else
#error "TM4C123.h: TM4C123 Device NOT specified"
#endif

#endif /* TM4C123_H */
