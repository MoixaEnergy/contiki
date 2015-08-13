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

#include "contiki-conf.h"

#include "dev/leds.h"

#include "p32xxxx.h"

/* PORT E */
#define LEDS_SCHM_1                     (1 << 0)
#define LEDS_SCHM_2                     (1 << 1)
#define LEDS_SCHM_3                     (1 << 2)
#define LEDS_SCHM_4                     (1 << 3)
#define LEDS_SCHM_5                     (1 << 4)
#define LEDS_SCHM_6                     (1 << 5)
#define LEDS_SCHM_7                     (1 << 6)
#define LEDS_SCHM_8                     (1 << 7)
/* PORT C */
#define LEDS_SCHM_9                     (1 << 1)
#define LEDS_SCHM_10                    (1 << 2)

#define pp_concat3(a,b,c)               a ## b ## c

#define led_1_on                        (LATECLR = LEDS_SCHM_1)
#define led_1_off                       (LATESET = LEDS_SCHM_1)
#define led_1_toggle                    (LATEINV = LEDS_SCHM_1)
#define led_2_on                        (LATESET = LEDS_SCHM_2)
#define led_2_off                       (LATECLR = LEDS_SCHM_2)
#define led_2_toggle                    (LATEINV = LEDS_SCHM_2)
#define led_3_on                        (LATESET = LEDS_SCHM_3)
#define led_3_off                       (LATECLR = LEDS_SCHM_3)
#define led_3_toggle                    (LATEINV = LEDS_SCHM_3)
#define led_4_on                        (LATESET = LEDS_SCHM_4)
#define led_4_off                       (LATECLR = LEDS_SCHM_4)
#define led_4_toggle                    (LATEINV = LEDS_SCHM_4)
#define led_5_on                        (LATESET = LEDS_SCHM_5)
#define led_5_off                       (LATECLR = LEDS_SCHM_5)
#define led_5_toggle                    (LATEINV = LEDS_SCHM_5)
#define led_6_on                        (LATESET = LEDS_SCHM_6)
#define led_6_off                       (LATECLR = LEDS_SCHM_6)
#define led_6_toggle                    (LATEINV = LEDS_SCHM_6)
#define led_7_on                        (LATESET = LEDS_SCHM_7)
#define led_7_off                       (LATECLR = LEDS_SCHM_7)
#define led_7_toggle                    (LATEINV = LEDS_SCHM_7)
#define led_8_on                        (LATESET = LEDS_SCHM_8)
#define led_8_off                       (LATECLR = LEDS_SCHM_8)
#define led_8_toggle                    (LATEINV = LEDS_SCHM_8)
#define led_9_on                        (LATCSET = LEDS_SCHM_9)
#define led_9_off                       (LATCCLR = LEDS_SCHM_9)
#define led_9_toggle                    (LATCINV = LEDS_SCHM_9)
#define led_10_on                       (LATCSET = LEDS_SCHM_10)
#define led_10_off                      (LATCCLR = LEDS_SCHM_10)
#define led_10_toggle                   (LATCINV = LEDS_SCHM_10)
#define led_on(n)                       pp_concat3(led_,n,_on)
#define led_off(n)                      pp_concat3(led_,n,_off)
#define led_toggle(n)                   pp_concat3(led_,n,_toggle)

/*---------------------------------------------------------------------------*/
void
leds_arch_init(void)
{
	int i;

	TRISECLR =
		LEDS_SCHM_1 | LEDS_SCHM_2 | LEDS_SCHM_3 | LEDS_SCHM_4 |
		LEDS_SCHM_5 | LEDS_SCHM_6 | LEDS_SCHM_7 | LEDS_SCHM_8;
	TRISCCLR =
		LEDS_SCHM_9 | LEDS_SCHM_10;

	led_1_off; led_2_off; led_3_off; led_4_off; led_5_off;
	led_6_off; led_7_off; led_8_off; led_9_off; led_10_off;
}
/*---------------------------------------------------------------------------*/
unsigned char
leds_arch_get(void)
{
	unsigned char progress_leds = 0;

	if (PORTE & LEDS_SCHM_2) {
		progress_leds = 1;
		if (PORTE & LEDS_SCHM_3) {
			progress_leds = 2;
			if (PORTE & LEDS_SCHM_4) {
				progress_leds = 3;
				if (PORTE & LEDS_SCHM_5) {
					progress_leds = 4;
				}
			}
		}
	}
	return
		(PORTE & LEDS_SCHM_1  ? 0 : (1 << 0)) |
		(PORTE & LEDS_SCHM_6  ? 0 : (1 << 1)) |
		(PORTE & LEDS_SCHM_7  ? 0 : (1 << 2)) |
		(PORTE & LEDS_SCHM_8  ? 0 : (1 << 3)) |
		(PORTC & LEDS_SCHM_9  ? 0 : (1 << 4)) |
		(PORTC & LEDS_SCHM_10 ? 0 : (1 << 5)) |
		(progress_leds << 6);
}
/*---------------------------------------------------------------------------*/
void
leds_arch_set(unsigned char leds)
{
	unsigned char progress_leds;

	progress_leds = leds >> 6;
	if (leds & LEDS_BATTERY) {
		led_2_on;
		if (progress_leds > 0) {
			led_3_on;
			if (progress_leds > 1) {
				led_4_on;
				if (progress_leds > 2)
					led_5_on;
				else
					led_5_off;
			}
			else {
				led_4_off; led_5_off;
			}
		}
		else {
			led_3_off; led_4_off; led_5_off;
		}
	}
	else {
		led_2_off; led_3_off; led_4_off; led_5_off;
	}

	if (leds & LEDS_BYPASS)
		led_1_on;
	else
		led_1_off;
	if (leds & LEDS_PV)
		led_6_on;
	else
		led_6_off;
	if (leds & LEDS_MAINS)
		led_7_on;
	else
		led_7_off;
	if (leds & LEDS_BATTERY)
		led_8_on;
	else
		led_8_off;
	if (leds & LEDS_POWER)
		led_9_on;
	else
		led_9_off;
	if (leds & LEDS_WIFI)
		led_10_on;
	else
		led_10_off;
}
/*---------------------------------------------------------------------------*/

/** @} */
