/******************************************************************************/
// Free implementation of Bullfrog's Dungeon Keeper strategy game.
/******************************************************************************/
/** @file ariadne_regions.h
 *     Header file for ariadne_regions.c.
 * @par Purpose:
 *     Regions array for Ariande system support functions.
 * @par Comment:
 *     Just a header file - #defines, typedefs, function prototypes etc.
 * @author   Tomasz Lis
 * @date     11 Mar 2010 - 22 Jul 2010
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#ifndef DK_ARIADNE_REGIONS_H
#define DK_ARIADNE_REGIONS_H

#include "bflib_basics.h"
#include "globals.h"

#define REGIONS_COUNT 300

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/
#ifdef __cplusplus
#pragma pack(1)
#endif

struct RegionT { // sizeof = 3
  unsigned short field_0;
  unsigned char field_2;
};

#ifdef __cplusplus
#pragma pack()
#endif
/******************************************************************************/
extern struct RegionT bad_region;
#define INVALID_REGION &bad_region;
/******************************************************************************/
TbBool regions_connected(long tree_reg1, long tree_reg2);
#define region_set(ntri, nreg) region_set_f(ntri, nreg, __func__)
void region_set_f(long ntri, unsigned long nreg, const char *func_name);

/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
