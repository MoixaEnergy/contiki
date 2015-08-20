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

#define BUTTON_POLL_PERIOD  (CLOCK_SECOND / 5)
#define BUTTON_RESET_DELAY  (CLOCK_SECOND * 8)

typedef enum {
	ButtonOverride = 0,
	ButtonReset,
	ButtonMode,
	ButtonJoin
} Button;

typedef union {
	uint8_t all;

	struct {
		uint32_t override : 1;
		uint32_t reset : 1;
		uint32_t mode : 1;
		uint32_t join : 1;
		uint32_t bit4 : 1;
		uint32_t bit5 : 1;
		uint32_t bit6 : 1;
		uint32_t bit7 : 1;
	};
} ButtonState;

static ButtonState buttonState;
static struct ctimer pollTimer;
static struct ctimer resetTimer;

/*
 * The override button is latching. A pointer to this variable is sent alongside
 * a button transition event and determines whether this is an On or Off
 * transiton.
 */
static uint32_t overrideLatch = 0;

/*
 * This function is called if and only if the reset button is depressed for
 * BUTTON_RESET_DELAY seconds. Only then it broadcasts the reset message.
 */
static void
bcast_reset(void* data)
{
	process_post(PROCESS_BROADCAST, ButtonResetEvent, NULL);
}

/*
 * Self-restarting poll timer callback function.
 */
static void
poll(void* data)
{
	uint32_t v;

	v = PORTDbits.RD11;
	if (v != buttonState.override) {
		buttonState.override = overrideLatch = v;
		process_post(PROCESS_BROADCAST,
			     ButtonOverrideEvent, &overrideLatch);

		// Also possible but not informative enough:
                // sensors_changed(&button_sensor);
	}
	v = PORTGbits.RG12;
	if (v != buttonState.reset) {
		buttonState.reset = v;
		if (v)
			ctimer_set(&resetTimer, BUTTON_RESET_DELAY,
				   bcast_reset, NULL);
		else
			ctimer_stop(&resetTimer);
	}
	v = PORTGbits.RG13;
	if (v != buttonState.mode) {
		buttonState.mode = v;
		if (v)
			process_post(PROCESS_BROADCAST, ButtonModeEvent, NULL);
	}
	v = PORTDbits.RD8;
	if (v != buttonState.join) {
		buttonState.join = v;
		if (v)
			process_post(PROCESS_BROADCAST, ButtonJoinEvent, NULL);
	}
	ctimer_reset(&pollTimer);
}

/*
 * Setup function that configures the necessary IO and initiates button polling.
 */
static int
config(int type, int value)
{
	switch (type) {
	case SENSORS_HW_INIT:
		TRISDSET = BIT(8) | BIT(11);
		TRISGSET = BIT(12) | BIT(13);
		buttonState.all = 0;
		return 1;
	case SENSORS_ACTIVE:
		ctimer_set(&pollTimer, BUTTON_POLL_PERIOD, poll, NULL);
		return 1;
	default:
		break;
	}
	return 0;
}

SENSORS_SENSOR(button_sensor, BUTTON_SENSOR, NULL, config, NULL);

/** @} */
