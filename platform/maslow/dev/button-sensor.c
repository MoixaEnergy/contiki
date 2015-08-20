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

#include <dev/button-sensor.h>

#include <p32xxxx.h>

#include <pic32_irq.h>

#define BUTTON_POLL_PERIOD  (CLOCK_SECOND / 3)

typedef enum {
	ButtonJoin = 0,
	ButtonFuse,
	ButtonMode,
	ButtonOverride
} Button;

typedef union {
	uint8_t all;

	struct {
		uint32_t join : 1;
		uint32_t fuse : 1;
		uint32_t mode : 1;
		uint32_t override : 1;
		uint32_t bit4 : 1;
		uint32_t bit5 : 1;
		uint32_t bit6 : 1;
		uint32_t bit7 : 1;
	};
} ButtonState;

static ButtonState buttonState;
static struct ctimer pollTimer;

process_event_t buttonJoinEvent;
process_event_t buttonFuseEvent;
process_event_t buttonModeEvent;
process_event_t buttonOverrideEvent;

/*
static int
value(int type)
{
	switch (type) {
	case ButtonMode:
		return PORTGbits.RG13;
	default:
		break;
	}
	return -1;
}
*/
/*---------------------------------------------------------------------------*/
static void
poll(void *data)
{
	uint32_t v;

	v = PORTDbits.RD8;
	if (v != buttonState.join) {
		buttonState.join = v;
		if (v)
			process_post(PROCESS_BROADCAST, buttonJoinEvent, NULL);
		/*
		 * Also possible but not informative enough:
                 * sensors_changed(&button_sensor);
		 */
	}
	v = PORTGbits.RG12;
	if (v != buttonState.fuse) {
		buttonState.fuse = v;
		if (v)
			process_post(PROCESS_BROADCAST, buttonFuseEvent, NULL);
	}
	v = PORTGbits.RG13;
	if (v != buttonState.mode) {
		buttonState.mode = v;
		if (v)
			process_post(PROCESS_BROADCAST, buttonModeEvent, NULL);
	}
	ctimer_reset(&pollTimer);
}
/*---------------------------------------------------------------------------*/
static int
config(int type, int value)
{
	switch (type) {
	case SENSORS_HW_INIT:
		TRISDSET = BIT(8);
		TRISGSET = BIT(12) | BIT(13);
		buttonState.all = 0;
		return 1;
	case SENSORS_ACTIVE:
//		buttonState.mode = value(ButtonMode);
		ctimer_set(&pollTimer, BUTTON_POLL_PERIOD, poll, NULL);
		return 1;
	default:
		break;
	}
	return 0;
}
/*---------------------------------------------------------------------------*/
SENSORS_SENSOR(button_sensor, BUTTON_SENSOR, NULL, config, NULL);
/*---------------------------------------------------------------------------*/

/** @} */
