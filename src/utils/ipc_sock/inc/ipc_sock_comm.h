/**************************************************************************
 * Copyright (C) 2017-2017  Unisound
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 **************************************************************************
 *
 * Description : ipc_sock_comm.h
 * Author      : guozhenkun@unisound.com
 * Date        : 2018.12.20
 *
 **************************************************************************/
#ifndef UTILS_IPCSOCK_INC_IPC_SOCK_COMM_H_
#define UTILS_IPCSOCK_INC_IPC_SOCK_COMM_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "uni_iot.h"
#include "list_head.h"
#include "uni_msg.h"
#define IPC_SOCK_CODE_OK         (0)
#define IPC_SOCK_CODE_MALLOC     (1)
#define IPC_SOCK_CODE_VERIFY     (2)
#define IPC_SOCK_CODE_DISCONN    (3)

int IpcSockReadMsg(int sockfd, IpcMsg **msg);
int IpcSockWriteMsg(int sockfd, IpcMsg *msg);
#ifdef __cplusplus
}
#endif
#endif