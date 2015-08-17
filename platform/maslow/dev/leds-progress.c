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

void
leds_progress_init(void)
{
	TRISCCLR = BIT(1) | BIT(2);
	TRISECLR = BIT(6) | BIT(7);

	LATCSET  = BIT(1) | BIT(2);
	LATESET  = BIT(6) | BIT(7);
}

void
leds_progress_set(uint8_t progress)
{
	switch (progress) {
	case 0:
		LATCbits.LATC1 = 1;
		LATEbits.LATE7 = 1;
		LATEbits.LATE6 = 1;
		LATCbits.LATC2 = 1;
		break;
	case 1:
		LATCbits.LATC1 = 0;
		LATEbits.LATE7 = 1;
		LATEbits.LATE6 = 1;
		LATCbits.LATC2 = 1;
		break;
	case 2:
		LATCbits.LATC1 = 0;
		LATEbits.LATE7 = 0;
		LATEbits.LATE6 = 1;
		LATCbits.LATC2 = 1;
		break;
	case 3:
		LATCbits.LATC1 = 0;
		LATEbits.LATE7 = 0;
		LATEbits.LATE6 = 0;
		LATCbits.LATC2 = 1;
		break;
	case 4:
	default:
		LATCbits.LATC1 = 0;
		LATEbits.LATE7 = 0;
		LATEbits.LATE6 = 0;
		LATCbits.LATC2 = 0;
		break;
	}
}
