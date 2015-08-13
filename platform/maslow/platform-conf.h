/*
 * Maslow
 *
 * Copyright (c) 2015, Moixa Technology
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
 * \file   -
 * \brief  -
 * \author -
 * \date   -
 */

#ifndef PLATFORM_CONF_H_
#define PLATFORM_CONF_H_

#define UART_DEBUG_BAUDRATE                     115200

#define PLATFORM_HAS_BATTERY                    0
#define PLATFORM_HAS_BUTTON                     0
#define PLATFORM_HAS_LEDS                       1
#define PLATFORM_HAS_RADIO                      0

#define CLOCK_CONF_SECOND                       1024

#define LEDS_BYPASS                     (1 << 0)
#define LEDS_PV                         (1 << 1)
#define LEDS_MAINS                      (1 << 2)
#define LEDS_BATTERY                    (1 << 3)
#define LEDS_POWER                      (1 << 4)
#define LEDS_WIFI                       (1 << 5)

/* LED name mapping complying with Contiki naming convention */
#define LEDS_CONF_RED                   LEDS_BYPASS
#define LEDS_CONF_YELLOW                LEDS_BATTERY
#define LEDS_CONF_GREEN                 LEDS_POWER
#define LEDS_CONF_BLUE                  LEDS_WIFI

/* LEDs from 2 to 5 are represented as a bitmask on the 2 highest bits */
#define LEDS_CONF_ALL                   0xFF

#endif /* PLATFORM_CONF_H_ */
