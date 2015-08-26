/*
 * Contiki PIC32 Port project
 *
 * Copyright (c) 2012,
 *  Scuola Superiore Sant'Anna (http://www.sssup.it) and
 *  Consorzio Nazionale Interuniversitario per le Telecomunicazioni
 *  (http://www.cnit.it).
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 */

/**
 * \addtogroup cpu
 * @{ */

/**
 * \defgroup pic32 PIC32 Contiki Port
 *
 * @{
 */

/**
 * \file   pic32.h
 * \brief  PIC32MX initialization routines
 * \author Giovanni Pellerano <giovanni.pellerano@evilaliv3.org>
 * \date   2012-03-23
 */

#ifndef INCLUDE_PIC32_PIC32_H_
#define INCLUDE_PIC32_PIC32_H_

/*
 * PIC32MX795F512L - Specific Functions
 *
 * All the functions in this part of the file are specific for the
 * pic32mx795f512l that is characterized by registers' name that differ from
 * the 3xx and 4xx families of the pic32mx.
 */

/*  Initial Configuration
 *
 *  Main Clock       -> SYSCLK = (Crystal_Freq / FPLLIDIV) * FPLLMUL / FPLLODIV
 *  Peripheral Clock -> PBCLK  = SYSCLK / FPBDIV
 *
 *  SYSCLK = 80MHz, PBCLK = 80MHz
*/

#include <p32xxxx.h>

#pragma config FNOSC    = PRIPLL       // Oscillator Selection
#pragma config FPLLIDIV = DIV_2        // PLL Input Divider (PIC32 Starter Kit: use divide by 2 only)
#pragma config FPLLMUL  = MUL_20       // PLL Multiplier
#pragma config FPLLODIV = DIV_1        // PLL Output Divider
#pragma config FPBDIV   = DIV_1        // Peripheral Clock divisor
#pragma config FWDTEN   = OFF          // Watchdog Timer can be enabled by software
#pragma config OSCIOFNC = OFF          // CLKO Enable
#pragma config POSCMOD  = HS           // Primary Oscillator
#pragma config FSOSCEN  = OFF          // Secondary Oscillator Enable (KLO was off)
#pragma config CP       = OFF          // Code Protect
#pragma config BWP      = OFF          // Boot Flash Write Protect
#pragma config PWP      = OFF          // Program Flash Write Protect
#pragma config ICESEL   = ICS_PGx2     // ICE/ICD Comm Channel Select

#if CONTIKI_TARGET_MASLOW
  #ifdef USB_A0_SILICON_WORK_AROUND
    #pragma config UPLLEN   = OFF       // USB PLL Enabled (A0 bit inverted)
  #else
    #pragma config UPLLEN   = ON        // USB PLL Enabled
  #endif
  #pragma config WDTPS     = PS65536    // Watchdog Timer Postscale
  #pragma config FCKSM     = CSDCMD     // Clock Switching & Fail Safe Clock Monitor
  #pragma config IESO      = ON         // Internal/External Switch-over
  #pragma config DEBUG     = OFF        // 4-wire JTAG and Trace disabled
  #pragma config FUSBIDIO  = OFF        // use as IO
  #pragma config FVBUSONIO = OFF        // use as IO
#elif CONTIKI_TARGET_SEEDEYE
  #pragma config WDTPS     = PS32768    // Watchdog Timer Postscale
  #pragma config FCKSM     = CSDCMD     // Clock Switching & Fail Safe Clock Monitor
  #pragma config IESO      = OFF        // Internal/External Switch-over
  #pragma config DEBUG     = OFF        // Debugger Disabled for Starter Kit
  #pragma config FSRSSEL   = PRIORITY_7
  /* external PHY in RMII/default configuration */
  #pragma config FMIIEN    = OFF
  #pragma config FETHIO    = ON
#else
  #error __FILE__: No suitable platform chosen
#endif // CONTIKI_TARGET_MASLOW

void pic32_init(void);

#endif /* INCLUDE_PIC32_PIC32_H_ */

/** @} */
/** @} */
