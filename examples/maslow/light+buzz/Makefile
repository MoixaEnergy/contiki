ifndef TARGET
TARGET=maslow
endif

PIC32_MODEL ?= 32MX575F512L
TOOLCHAIN   ?= /opt/microchip/xc32/v1.40
CONTIKI_WITH_IPV4 = 0
CONTIKI_WITH_IPV6 = 0
CONTIKI_WITH_RIME = 0
CONTIKI_WITH_RPL  = 0

APPS = serial-shell

all: light+buzz

CONTIKI=../../../

include $(CONTIKI)/Makefile.include
