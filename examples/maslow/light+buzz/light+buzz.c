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
#include "dev/leds-arch.h"
#include "dev/leds.h"
#include "dev/buzzer.h"
#include <pic32_clock.h>

#include <p32xxxx.h>
#include <math.h>

PROCESS(leds_progress, "Progress LEDs");
PROCESS(led_fader, "LED fader");
PROCESS(buzzer, "PWM buzzer");

AUTOSTART_PROCESSES(&leds_progress, &led_fader, &buzzer);

/*
 * Progress LEDs
 */
static struct etimer timer;
static uint8_t progress;

PROCESS_THREAD(leds_progress, ev, data)
{
	PROCESS_BEGIN();

	progress = 0;
	etimer_set(&timer, CLOCK_SECOND / 5);
	while (1) {
		PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
		leds_progress_set(abs(progress % 8 - 4));
		progress++;
		etimer_reset(&timer);
	}

	PROCESS_END();
}

/*
 * LED fader
 */
struct fader {
	uint8_t led;
	struct etimer cycle_timer;
	struct etimer on_timer;
	clock_time_t on_time;
	int change_rate;
};

inline void
init_fader(struct fader *f, uint8_t led,
	   clock_time_t cycle_time, int change_rate)
{
	f->led = led;
	f->change_rate = change_rate;
	f->on_time = 1;
	etimer_set(&f->cycle_timer, cycle_time);
}


PROCESS_THREAD(led_fader, ev, data)
{
	static struct fader red;

	PROCESS_BEGIN();

	init_fader(&red, BIT(LedBypass), CLOCK_SECOND / 50, CLOCK_SECOND / 2000);

	while (1) {
		leds_on(red.led);
		etimer_set(&red.on_timer, red.on_time);

		PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&red.on_timer));
		leds_off(red.led);

		red.on_time += red.change_rate;
		if ((red.change_rate > 0 &&
		     red.on_time >
		     red.cycle_timer.timer.interval - red.change_rate) ||
		    (red.change_rate < 0 &&
		     red.on_time < -red.change_rate)) {
			red.change_rate = -red.change_rate;
		}

		PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&red.cycle_timer));
		etimer_reset(&red.cycle_timer);
	}

	PROCESS_END();
}

/*
 * Buzzer synth
 */
struct beep {
	struct etimer cycle_timer;
	struct etimer on_timer;
	clock_time_t on_time;
	int change_rate;
};

inline void
init_beep(struct beep *f, clock_time_t cycle_time, int change_rate)
{
	f->change_rate = change_rate;
	f->on_time = 1;
	etimer_set(&f->cycle_timer, cycle_time);
}


PROCESS_THREAD(buzzer, ev, data)
{
	static struct beep b;

	PROCESS_BEGIN();

	init_beep(&b, CLOCK_SECOND / 1000, CLOCK_SECOND / 10000 /* 100 µs */);

	while (1) {
		buzzer_on();
		etimer_set(&b.on_timer, b.on_time);

		PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&b.on_timer));
		buzzer_off();

		b.on_time += b.change_rate;
		if ((b.change_rate > 0 &&
		     b.on_time >=
		     b.cycle_timer.timer.interval - b.change_rate) ||
		    (b.change_rate < 0 &&
		     b.on_time < -b.change_rate)) {
			b.change_rate = -b.change_rate;
		}

		PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&b.cycle_timer));
		etimer_reset(&b.cycle_timer);
	}

	PROCESS_END();
}

/** @} */
