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
 * Description : uni_resource_prefetch.h
 * Author      : chenxiaosong@unisound.com
 * Date        : 2020.04.01
 *
 **************************************************************************/
#ifndef SDK_CLOUD_MC_INC_UNI_RESOURCE_PREFETCH_H_
#define SDK_CLOUD_MC_INC_UNI_RESOURCE_PREFETCH_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "uni_iot.h"

typedef enum {
  RES_DEVICE,
  RES_STRATEGY, 
  RES_NEWS, 
  RES_AD,
  RES_QR_CODE,
  RES_COMMUNITY,
  RES_MAX
} ResType;

typedef int (*ResNotify) (ResType type);

Result ResPrefetchInit(void);
void   ResPrefetchFinal(void);
void   ResPrefetchRegisterNotify(ResNotify);

#ifdef __cplusplus
}
#endif
#endif  //  SDK_CLOUD_MC_INC_UNI_RESOURCE_PREFETCH_H_