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
#include "leds-arch.h"

#include "p32xxxx.h"

/*---------------------------------------------------------------------------*/
void
leds_arch_init(void)
{
	TRISECLR = BIT(0) | BIT(1) | BIT(2) | BIT(3) | BIT(4) | BIT(5);
	LATESET  = BIT(0) | BIT(1) | BIT(2) | BIT(3) | BIT(4) | BIT(5);
}
/*---------------------------------------------------------------------------*/
uint8_t
leds_arch_get(void)
{
	return
		(PORTEbits.RE3 ? 0 : BIT(LedBypass))  |
		(PORTEbits.RE1 ? 0 : BIT(LedPV))      |
		(PORTEbits.RE0 ? 0 : BIT(LedMains))   |
		(PORTEbits.RE2 ? 0 : BIT(LedBattery)) |
		(PORTEbits.RE5 ? 0 : BIT(LedPower))   |
		(PORTEbits.RE4 ? 0 : BIT(LedWiFi));
}
/*---------------------------------------------------------------------------*/
void
leds_arch_set(uint8_t leds)
{
	LATEbits.LATE3 = (leds & BIT(LedBypass))  ? 0 : 1;
        LATEbits.LATE1 = (leds & BIT(LedPV))      ? 0 : 1;
	LATEbits.LATE0 = (leds & BIT(LedMains))   ? 0 : 1;
	LATEbits.LATE2 = (leds & BIT(LedBattery)) ? 0 : 1;
	LATEbits.LATE5 = (leds & BIT(LedPower))   ? 0 : 1;
	LATEbits.LATE4 = (leds & BIT(LedWiFi))    ? 0 : 1;
}
/*---------------------------------------------------------------------------*/

/** @} */
