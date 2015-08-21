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
#include "dev/button-sensor.h"
#include <pic32_clock.h>

#include <p32xxxx.h>
#include <math.h>

PROCESS(leds_progress, "Progress LEDs");
PROCESS(led_fader, "LED fader");
PROCESS(buzzer, "PWM buzzer");
PROCESS(button_handler, "Button handler");

AUTOSTART_PROCESSES(&leds_progress, &led_fader, &button_handler);

/*
 * Progress LEDs
 */
static struct etimer timer;
static uint8_t progress;

PROCESS_THREAD(leds_progress, ev, data)
{
	PROCESS_BEGIN();

	progress = 0;
	etimer_set(&timer, CLOCK_SECOND / 9);
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
struct pwm {
	struct etimer cycle_timer;
	struct etimer on_timer;
	clock_time_t on_time;
	int change_rate;
};

inline void
pwm_init(struct pwm* p, clock_time_t cycle_time, int change_rate)
{
	p->change_rate = change_rate;
	p->on_time = 1;
	etimer_set(&p->cycle_timer, cycle_time);
}

inline void
pwm_cycle(struct pwm* p)
{
	p->on_time += p->change_rate;
	if ((p->change_rate > 0 &&
	     p->on_time >
	     p->cycle_timer.timer.interval - p->change_rate) ||
	    (p->change_rate < 0 &&
	     p->on_time < -p->change_rate)) {
		p->change_rate = -p->change_rate;
	}
}

PROCESS_THREAD(led_fader, ev, data)
{
	static struct pwm p;

	PROCESS_BEGIN();

	pwm_init(&p, CLOCK_SECOND / 42, CLOCK_SECOND / 1050);

	while (1) {
		etimer_set(&p.on_timer, p.on_time);
		leds_on(BIT(LedBypass));

		PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&p.on_timer));
		leds_off(BIT(LedBypass));

		pwm_cycle(&p);

		PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&p.cycle_timer));
		etimer_reset(&p.cycle_timer);
	}

	PROCESS_END();
}

/*
 * Buzzer synth
 */
PROCESS_THREAD(buzzer, ev, data)
{
	static struct pwm p;

	PROCESS_BEGIN();

	pwm_init(&p, CLOCK_SECOND / 1000, CLOCK_SECOND / 10000);

	while (1) {
		etimer_set(&p.on_timer, p.on_time);
		buzzer_on();

		PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&p.on_timer));
		buzzer_off();

		pwm_cycle(&p);

		PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&p.cycle_timer));
		etimer_reset(&p.cycle_timer);
	}

	PROCESS_END();
}

PROCESS_THREAD(button_handler, ev, data)
{
	static uint32_t ev_data = 0;

	PROCESS_BEGIN();

	while (1) {
		PROCESS_WAIT_EVENT();
		if (data)
			ev_data = *(uint32_t*) data;
		else
			ev_data = 0;
		printf("button_handler: event %xh, data %xh\n", ev, ev_data);

		switch (ev) {
		case ButtonOverrideEvent:
			leds_toggle(BIT(LedPV));
			break;
		case ButtonResetEvent:
			leds_toggle(BIT(LedMains));
			break;
		case ButtonModeEvent:
			leds_toggle(BIT(LedPower));
			if (!process_is_running(&buzzer))
				process_start(&buzzer, NULL);
			else
				process_exit(&buzzer);
			break;
		case ButtonJoinEvent:
			leds_toggle(BIT(LedWiFi));
			break;
		default:
			break;
		}
	}

	PROCESS_END();
}

/** @} */
