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
from os.path import join as opj

CC = "arm-rtems4.11-gcc"
PROG_CFLAGS = "-qrtems -B/other/rtems/bsps/arm-rtems4.11/lib/ -B/other/rtems/bsps/arm-rtems4.11/stm32f4x/lib/ --specs bsp_specs -march=armv7-m -mthumb -O0 -g -mfloat-abi=hard -mfpu=fpv4-sp-d16  -DNOT_USE_DYNAMIC_LOADING"
EXE_CFLAGS = " "
OS_NAME = "Linux"
ARCH_NAME = "arm"
PREFIX = "/other/rtems/bsps"
TARGET = "rtems"
CAN_DRIVER = "can_rtems"
TIMERS_DRIVER = "timers_rtems"


def getCFLAGS(Cpth):
	ipth = opj(Cpth, "include")
	return PROG_CFLAGS + ' -I"' + ipth  + '" -I"' + opj(ipth, TARGET)  + '" -I"' + opj(ipth, CAN_DRIVER)  + '" -I"' + opj(ipth,TIMERS_DRIVER) + '"'

def getLDFLAGS(Cpth):
    return EXE_CFLAGS + ' "' + opj(Cpth,"src","libcanfestival.a") + '" "' + opj(Cpth,"drivers", TARGET, "libcanfestival_%s.a"%TARGET)+ '"'

# herafter is appended DLL_LIST declaration

DLL_LIST=['can_rtems']
