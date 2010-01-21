/******************************************************************************/
// Free implementation of Bullfrog's Dungeon Keeper strategy game.
/******************************************************************************/
/** @file slab_data.c
 *     Map Slabs support functions.
 * @par Purpose:
 *     Definitions and functions to maintain map slabs.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     25 Apr 2009 - 12 May 2009
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "slab_data.h"
#include "globals.h"

#include "player_instances.h"
#include "keeperfx.hpp"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/
struct SlabMap bad_slabmap_block;
/******************************************************************************/
/******************************************************************************/
/*
 * Slab number - stores both X and Y coords in one number.
 */
unsigned long get_slab_number(long slb_x, long slb_y)
{
  if (slb_x > map_tiles_x) slb_x = map_tiles_x;
  if (slb_y > map_tiles_y) slb_y = map_tiles_y;
  if (slb_x < 0)  slb_x = 0;
  if (slb_y < 0) slb_y = 0;
  return slb_y*(map_tiles_x) + slb_x;
}

/*
 * Decodes X coordinate from slab number.
 */
long slb_num_decode_x(unsigned long slb_num)
{
  return slb_num % (map_tiles_x);
}

/*
 * Decodes Y coordinate from slab number.
 */
long slb_num_decode_y(unsigned long slb_num)
{
  return (slb_num/(map_tiles_x))%map_tiles_y;
}

struct SlabMap *get_slabmap_direct(long slab_num)
{
  if ((slab_num < 0) || (slab_num >= map_tiles_x*map_tiles_y))
      return INVALID_SLABMAP_BLOCK;
  return &game.slabmap[slab_num];
}

struct SlabMap *get_slabmap_block(long slab_x, long slab_y)
{
  if ((slab_x < 0) || (slab_x >= map_tiles_x))
      return INVALID_SLABMAP_BLOCK;
  if ((slab_y < 0) || (slab_y >= map_tiles_y))
      return INVALID_SLABMAP_BLOCK;
  return &game.slabmap[slab_y*(map_tiles_x) + slab_x];
}

struct SlabMap *get_slabmap_for_subtile(long stl_x, long stl_y)
{
  if ((stl_x < 0) || (stl_x >= map_subtiles_x))
      return INVALID_SLABMAP_BLOCK;
  if ((stl_y < 0) || (stl_y >= map_subtiles_y))
      return INVALID_SLABMAP_BLOCK;
  return &game.slabmap[map_to_slab[stl_y]*(map_tiles_x) + map_to_slab[stl_x]];
}

TbBool slabmap_block_invalid(struct SlabMap *slb)
{
  if (slb == NULL)
    return true;
  if (slb == INVALID_SLABMAP_BLOCK)
    return true;
  return (slb < &game.slabmap[0]);
}

long slabmap_owner(struct SlabMap *slb)
{
  return slb->field_5 & 0x07;
}

void clear_slabs(void)
{
  struct SlabMap *slb;
  unsigned long x,y;
  for (y=0; y < map_tiles_y; y++)
    for (x=0; x < map_tiles_x; x++)
    {
      slb = &game.slabmap[y*map_tiles_x + x];
      memset(slb, 0, sizeof(struct SlabMap));
      slb->slab = SlbT_ROCK;
    }
}

void reveal_whole_map(struct PlayerInfo *player)
{
  clear_dig_for_map_rect(player->field_2B,0,map_tiles_x,0,map_tiles_y);
  reveal_map_rect(player->field_2B,1,map_subtiles_x,1,map_subtiles_y);
  pannel_map_update(0, 0, map_subtiles_x+1, map_subtiles_y+1);
}
/******************************************************************************/
#ifdef __cplusplus
}
#endif