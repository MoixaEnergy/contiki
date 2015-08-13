/*
 * Example for the Maslow Board
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
 * \addtogroup -
 *
 * @{
 */

/**
 * \file   -
 * \brief  -
 * \author -
 * \date   -
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "contiki.h"
#include "sys/rtimer.h"
#include "dev/leds.h"

#include <p32xxxx.h>

PROCESS(test_leds, "Test LEDs");
AUTOSTART_PROCESSES(&test_leds);

struct fader {
	struct rtimer rt;
	struct pt pt;
	uint8_t led;
	rtimer_clock_t ontime, offtime;
	int fade_rate;
};

static char
fader(struct rtimer *t, void *ptr)
{
	struct fader *f = ptr;

	PT_BEGIN(&f->pt);

	while(1) {
		leds_on(f->led);
		rtimer_set(t, RTIMER_TIME(t) + f->ontime, 1,
			   (rtimer_callback_t)fader, ptr);

		PT_YIELD(&f->pt);

		leds_off(f->led);
		rtimer_set(t, RTIMER_TIME(t) + f->offtime, 1,
			   (rtimer_callback_t)fader, ptr);

		f->ontime += f->fade_rate;
		f->offtime -= f->fade_rate;
		if(f->offtime <= 4 || f->offtime >= 100) {
			f->fade_rate = -f->fade_rate;
		}

		PT_YIELD(&f->pt);
	}

	PT_END(&f->pt);
}

static void
init_fader(struct fader *f, uint8_t led, int rate)
{
	f->led = led;
	f->fade_rate = rate;
	f->ontime = 4;
	f->offtime = 100;
	PT_INIT(&f->pt);
	rtimer_set(&f->rt, RTIMER_NOW() + led, 1, (rtimer_callback_t)fader, f);
}

PROCESS_THREAD(test_leds, ev, data)
{
	int i;
	static struct fader fader_bypass;

	PROCESS_BEGIN();

	rtimer_init();
	init_fader(&fader_bypass, LEDS_BYPASS, 4);

	for (i = 0; i < 10; i++)
		leds_blink();

	leds_on(1);

	while (1)
		PROCESS_WAIT_EVENT();

	PROCESS_END();
}

/** @} */
