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
#include <sys/clock.h>
#include <lib/random.h>
#include <dev/watchdog.h>
#include <dev/serial-line.h>
#include <net/linkaddr.h>
#include <pic32.h>
#include <pic32_clock.h>
#include <pic32_uart.h>
#include <debug-uart.h>
#include <lpm.h>
#include "dev/button-sensor.h"
//#include "dev/battery-sensor.h"
#include "dev/leds-arch.h"
#include "dev/leds.h"
#include "dev/buzzer.h"
#include "usb/usb-serial.h"

#include <string.h>

#define DEBUG 1
#if DEBUG
  #include <stdio.h>
  #define PRINTF(...) printf(__VA_ARGS__)
#else
  #define PRINTF(...)
#endif

linkaddr_t linkaddr_node_addr = {{0, 0, 0, 0, 0, 0}};

SENSORS(&button_sensor);

/*---------------------------------------------------------------------------*/
static void print_processes(struct process *const processes[])
{
	PRINTF("Starting:\n");

	while (*processes != NULL) {
		PRINTF(" '%s'\n", (*processes)->name);
		processes++;
	}

	PRINTF("\n");
}

int main(int argc, char **argv)
{
	int32_t r;

	process_init(); // run before any function that starts a process
	pic32_init();
	watchdog_init();
	leds_init();
	leds_progress_init();
	buzzer_init();

	clock_init();
	rtimer_init();
	ctimer_init();

	leds_on(LEDS_ALL);

        /* Serial line init part 2/3: set up the UART port. */
	uart_console_init(UART_BAUDRATE);

//	usb_serial_init();
//	usb_serial_set_input(serial_line_input_byte);

        /* Serial line init part 3/3: start the OS process. */
	serial_line_init();

	asm volatile("ei");  // enable interrupts

	PRINTF("CPU Clock: %uMhz\n",
	       pic32_clock_get_system_clock() / 1000000);
	PRINTF("Peripheral Clock: %uMhz\n",
	       pic32_clock_get_peripheral_clock() / 1000000);

	random_init(4321);
	process_start(&etimer_process, NULL);
	process_start(&sensors_process, NULL);
	SENSORS_ACTIVATE(button_sensor);

	/* Starting autostarting process */
	print_processes(autostart_processes);
	autostart_start(autostart_processes);

	leds_off(LEDS_ALL);
	watchdog_start();
	PRINTF("Starting the main scheduler loop\n");

	/*
	 * This is the scheduler loop.
	 */
	while (1) {

		do {
			/* Reset watchdog. */
			watchdog_periodic();
			r = process_run();
		} while (r > 0);

#if LPM_MODE > LPM_MODE_NONE
		watchdog_stop();
		/* low-power mode start */
		asm volatile("wait");
		/* low-power mode end */
		watchdog_start();
#endif // LPM_MODE
	}

	return 0;
}
/*---------------------------------------------------------------------------*/

/** @} */
/** @} */
