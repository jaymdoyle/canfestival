/*
This file is part of CanFestival, a library implementing CanOpen Stack.

Copyright (C): Edouard TISSERANT and Francis DUPIN
AVR Port: Andreas GLAUSER and Peter CHRISTEN

See COPYING file for copyrights details.

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

//#define DEBUG_WAR_CONSOLE_ON
//#define DEBUG_ERR_CONSOLE_ON


#include "canfestival.h"
#include <dev/can/can.h>
#include <fcntl.h>
#include <bsp.h>
#include <unistd.h>
#include <rtems/libio.h>



int translate_baud_rate(char* optarg);

volatile unsigned char msg_received = 0;
int can = 5;
CAN_HANDLE canOpen_driver(s_BOARD * board)
/******************************************************************************
Initialize the hardware to receive CAN messages and start the timer for the
CANopen stack.
INPUT	bitrate		bitrate in kilobit
OUTPUT can handle
******************************************************************************/
{

  char name[20];
  sprintf(name, "/dev/can%s", board->busname);
  can = open(name, O_RDWR | O_NONBLOCK, 0640);

  // Configure the filter to accept all messages
  can_filter f = {
      .number = 0,
      .filter = can_filter_stdid(0x2),
      .mask   = 0,

  };

  int numFilters = ioctl(can, CAN_GET_NUM_FILTERS);
  if (numFilters) {
    ioctl(can, CAN_SET_FILTER, &f);
  }

  ioctl(can, CAN_SET_BAUDRATE, translate_baud_rate(board->baudrate));
  return can;
}

int canClose_driver(CAN_HANDLE fd) {
  if (fd)
    close(fd);
  return 0;
}

unsigned char canSend_driver(CAN_HANDLE fd, Message const *m)
/******************************************************************************
The driver send a CAN message passed from the CANopen stack
INPUT	CAN_PORT is not used (only 1 avaiable)
	Message *m pointer to message to send
OUTPUT	1 if  hardware -> CAN frame
******************************************************************************/
{
  can_msg m2;
  m2.id = m->cob_id;
  m2.len = m->len;
  memcpy(m2.data, m->data, m->len);
  int status = write(fd, &m2, sizeof(m2));
  return 1;
}

unsigned char canReceive_driver(CAN_HANDLE fd, Message *m)
/******************************************************************************
The driver pass a received CAN message to the stack
INPUT	Message *m pointer to received CAN message
OUTPUT	1 if a message received
******************************************************************************/
{
  can_msg rx_m;

  int status = read(can, &rx_m, sizeof(rx_m));
  if (status > 0) {
    size_t i;
    m->len = rx_m.len;
    m->cob_id = rx_m.id;
    memcpy(m->data, rx_m.data, m->len);
    return 1;
  }
  else {
    return 0;
  }

}

/***************************************************************************/
unsigned char canChangeBaudRate_driver( CAN_HANDLE fd, char* baud)
{

  ioctl(fd, CAN_SET_BAUDRATE, baud);
	return 0;
}

int translate_baud_rate(char* optarg){
	if(!strcmp( optarg, "1M")) return 1000000;
	if(!strcmp( optarg, "500K")) return 500000;
	if(!strcmp( optarg, "250K")) return 250000;
	if(!strcmp( optarg, "125K")) return 125000;
	if(!strcmp( optarg, "100K")) return 100000;
	if(!strcmp( optarg, "50K")) return 50000;
	if(!strcmp( optarg, "20K")) return 20000;
	if(!strcmp( optarg, "none")) return 0;
	return 0x0000;
}

