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
 * \addtogroup Maslow platform
 *
 * @{
 */

/**
 * \file   -
 * \brief  -
 * \author -
 * \date   -
 */

#include "p32xxxx.h"

/* These are 6 status LEDs (except for the battery progress LEDs from left to
 * right on the Maslow panel. The Contiki core is limited to a maximum of 8 LEDs
 * due to the byte-size bitmask state of all the LEDs. */
typedef enum {
	LedBypass,
	LedPV,
	LedMains,
	LedBattery,
	LedPower,
	LedWiFi
} Led;

/* LED name mapping complying with Contiki naming convention */
#define LEDS_CONF_RED     LedBypass
#define LEDS_CONF_YELLOW  LedBattery
#define LEDS_CONF_GREEN   LedPower
#define LEDS_CONF_BLUE    LedWiFi

/* LEDs not including the progress bar elements */
#define LEDS_CONF_ALL     0x3F

/* Progress bar made of single LEDs */
void leds_progress_init(void);
void leds_progress_set(uint8_t progress);
