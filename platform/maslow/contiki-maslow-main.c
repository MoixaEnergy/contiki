/*
 * Maslow
 *
 * Copyright (c) 2012, Moixa Technology
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
 * \addtogroup platform
 * @{ */

/**
 * \defgroup Maslow platform
 *
 * @{
 */

/**
 * \file   -
 * \brief  -
 * \author -
 * \date   -
 */

#include <contiki.h>
#include <clock.h>
#include <lib/random.h>
//#include "dev/button-sensor.h"
//#include "dev/battery-sensor.h"
#include "dev/leds-arch.h"
#include "dev/leds.h"
#include <dev/watchdog.h>

#include <pic32.h>
#include <pic32_clock.h>

//#include <debug-uart.h>

#include <stdio.h>
#include <string.h>

#define DEBUG 1
#if DEBUG
  #include <stdio.h>
  #define PRINTF(...) printf(__VA_ARGS__)
#else
  #define PRINTF(...)
#endif

//SENSORS(&button_sensor);

/*---------------------------------------------------------------------------*/
static void
print_processes(struct process *const processes[])
{
	PRINTF("Starting:\n");

	while (*processes != NULL) {
		PRINTF(" '%s'\n", (*processes)->name);
		processes++;
	}

	PRINTF("\n");
}

/*---------------------------------------------------------------------------*/
int
main(int argc, char **argv)
{
	int32_t r;

	pic32_init();
	watchdog_init();
	leds_init();
	leds_progress_init();

	clock_init();

//	dbg_setup_uart(UART_DEBUG_BAUDRATE);

//	PRINTF("CPU Clock: %uMhz\n", pic32_clock_get_system_clock() / 1000000);
//	PRINTF("Peripheral Clock: %uMhz\n", pic32_clock_get_peripheral_clock() / 1000000);

//	random_init(4321);
	process_init();
	process_start(&etimer_process, NULL);
	ctimer_init();
	rtimer_init();
	asm volatile("ei");  // enable interrupts

//	process_start(&sensors_process, NULL);
//	SENSORS_ACTIVATE(battery_sensor);

	leds_on(LEDS_ALL);
//	leds_progress_set(4);

	/* Starting autostarting process */
//	print_processes(autostart_processes);
	autostart_start(autostart_processes);

//	PRINTF("Processes running\n");

	leds_off(LEDS_ALL);

	watchdog_start();

	/*
	 * This is the scheduler loop.
	 */
	while (1) {

		do {
			/* Reset watchdog. */
			watchdog_periodic();
			r = process_run();
		} while (r > 0);

		watchdog_stop();
		/* low-power mode start */
		asm volatile("wait");
		/* low-power mode end */
		watchdog_start();
	}

	return 0;
}
/*---------------------------------------------------------------------------*/

/** @} */
/** @} */
