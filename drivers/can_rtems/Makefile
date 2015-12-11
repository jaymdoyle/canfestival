#! gmake

#
# Copyright (C) 2006 Laurent Bessard
# 
# This file is part of canfestival, a library implementing the canopen
# stack
# 
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
# 
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
# 
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
# 

CC = arm-rtems4.11-gcc
OPT_CFLAGS = -O2
CFLAGS = $(OPT_CFLAGS)
PROG_CFLAGS = -qrtems -B/other/rtems/bsps/arm-rtems4.11/lib/ -B/other/rtems/bsps/arm-rtems4.11/stm32f4x/lib/ --specs bsp_specs -march=armv7-m -mthumb -O0 -g -mfloat-abi=hard -mfpu=fpv4-sp-d16  -DNOT_USE_DYNAMIC_LOADING
PREFIX = /other/rtems/bsps
TARGET = rtems
CAN_DRIVER = can_rtems
TIMERS_DRIVER = timers_rtems
ENABLE_DLL_DRIVERS=0

INCLUDES = -I../../include -I../../include/$(TARGET) -I../../include/$(CAN_DRIVER)

OBJS = $(CAN_DRIVER).o

ifeq ($(ENABLE_DLL_DRIVERS),1)
CFLAGS += -fPIC
DRIVER = libcanfestival_$(CAN_DRIVER).so
else
DRIVER = $(OBJS)
endif

TARGET_SOFILES = $(DESTDIR)$(PREFIX)/lib/$(DRIVER)

all: driver

driver: $(DRIVER)

%.o: %.c
	$(CC) $(CFLAGS) $(PROG_CFLAGS) ${PROGDEFINES} $(INCLUDES) -o $@ -c $<

libcanfestival_$(CAN_DRIVER).so: $(OBJS)
	$(CC) -shared -Wl,-soname,libcanfestival_$(CAN_DRIVER).so $(CAN_DLL_CFLAGS) -o $@ $<

install: libcanfestival_$(CAN_DRIVER).so
	mkdir -p $(DESTDIR)$(PREFIX)/lib/
	cp $< $(DESTDIR)$(PREFIX)/lib/
	
uninstall:
	rm -f $(TARGET_HFILES)

clean:
	rm -f $(OBJS) libcanfestival_$(CAN_DRIVER).so

mrproper: clean
