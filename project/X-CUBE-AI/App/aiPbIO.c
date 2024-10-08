/**
 ******************************************************************************
 * @file    aiPbIO.c
 * @author  MCD/AIS Team
 * @brief   Low Level ProtoBuffer IO functions for COM stack
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2019,2021 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software is licensed under terms that can be found in the LICENSE file in
 * the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

/*
 * Description:
 *
 * Low-Level IO functions to port the nano PB stack
 *
 * History:
 *  - v1.0 - initial version. Based on a split of the original aiTestUtility.c
 *           (v1.4) file previous (aiValidation v3.2)
 */

/* System headers */
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <string.h>

#include <aiPbIO.h>

#include <aiTestUtility.h>

#include <pb.h>
#include <pb_encode.h>
#include <pb_decode.h>
#include <stm32msg.pb.h>


#define _PACKET_PAYLOAD_IN_SIZE (EnumLowLevelIO_IO_IN_PACKET_SIZE)

static struct o_packet {
  uint8_t pw;
  uint8_t payload[_PACKET_PAYLOAD_IN_SIZE];
} o_packet;

static bool write_packet(void) {
  return ioRawWriteBuffer((uint8_t *)&o_packet, _PACKET_PAYLOAD_IN_SIZE + 1);
}

void pb_io_flush_ostream(void)
{
  o_packet.pw |= (1 << 7); /* Indicate last packet */
  write_packet();
  o_packet.pw = 0;
}

static bool write_callback(pb_ostream_t *stream, const uint8_t *buf,
    size_t count)
{
  bool res = true;
  uint8_t *pr = (uint8_t *)buf;

  UNUSED(stream);

  while (count) {
    for (; o_packet.pw < _PACKET_PAYLOAD_IN_SIZE && count; o_packet.pw++) {
      o_packet.payload[o_packet.pw] = *pr;
      pr++;
      count--;
    }
    if (o_packet.pw == _PACKET_PAYLOAD_IN_SIZE) {
      res = write_packet();
      o_packet.pw = 0;
    }
  }
  return res;
}


#define _PACKET_PAYLOAD_OUT_SIZE (EnumLowLevelIO_IO_OUT_PACKET_SIZE)

static struct i_packet {
  uint8_t pr;
  uint8_t payload[_PACKET_PAYLOAD_OUT_SIZE];
} i_packet;

static int i_ridx = 0;

static bool read_packet(void) {
  bool res = ioRawReadBuffer((uint8_t *)&i_packet,
      _PACKET_PAYLOAD_OUT_SIZE + 1);
  i_ridx = 0;
  return res;
}

void pb_io_flush_istream(void)
{
  i_packet.pr = 0xFF;
  i_ridx = 0;
}

static bool read_callback(pb_istream_t *stream, uint8_t *buf, size_t count)
{
  bool res = true;
  uint8_t *pw = (uint8_t *)buf;

  UNUSED(stream);

  if (count == 0)
    return true;

  if (i_packet.pr == 0xFF)
    res = read_packet();

  if (res == false)
    return res;

  while (count) {
    for (; i_packet.pr > 0 && count; i_packet.pr--) {
      *pw = i_packet.payload[i_ridx];
      pw++;
      count--;
      i_ridx++;
    }
    if (count && i_packet.pr == 0) {
      uint8_t sync = 0xAA;
      ioRawWriteBuffer(&sync, 1);
      read_packet();
    }
  }

  return res;
}

pb_ostream_t pb_io_ostream(int fd)
{
#ifndef PB_NO_ERRMSG
  pb_ostream_t stream = {&write_callback, (void*)(intptr_t)fd, SIZE_MAX, 0, NULL};
#else
  pb_ostream_t stream = {&write_callback, (void*)(intptr_t)fd, SIZE_MAX, 0};
#endif
  return stream;
}

pb_istream_t pb_io_istream(int fd)
{
#ifndef PB_NO_ERRMSG
  pb_istream_t stream = {&read_callback, (void*)(intptr_t)fd, SIZE_MAX, NULL};
#else
  pb_istream_t stream = {&read_callback, (void*)(intptr_t)fd, SIZE_MAX};
#endif
  return stream;
}

int pb_io_stream_init(void)
{
  ioRawDisableLLWrite();
  return 0;
}
