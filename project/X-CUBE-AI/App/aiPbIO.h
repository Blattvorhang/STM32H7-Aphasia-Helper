/**
 ******************************************************************************
 * @file    aiPbIO.h
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

#ifndef _AI_PB_IO_H_
#define _AI_PB_IO_H_

#include <pb.h>

int pb_io_stream_init(void);

void pb_io_flush_ostream(void);
void pb_io_flush_istream(void);

pb_ostream_t pb_io_ostream(int fd);
pb_istream_t pb_io_istream(int fd);

#endif /* _AI_PB_IO_H_ */
