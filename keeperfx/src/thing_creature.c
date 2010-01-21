/******************************************************************************/
// Free implementation of Bullfrog's Dungeon Keeper strategy game.
/******************************************************************************/
/** @file thing_creature.c
 *     Creatures related functions.
 * @par Purpose:
 *     Functions for support of creatures as things.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     17 Mar 2009 - 10 May 2009
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "thing_creature.h"
#include "globals.h"

#include "bflib_memory.h"
#include "bflib_math.h"

#include "engine_lenses.h"
#include "config_creature.h"
#include "creature_states.h"
#include "thing_effects.h"
#include "lens_mist.h"
#include "keeperfx.hpp"
#include "frontend.h"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/
unsigned short creature_graphics[][22] = {
  {   0,   0,   0,   0,   0,   0,   0,   0,   0,  0,   0,
      0,   0,   0,   0,   0,   0,   0,  0,   0,   0,   0,},
  { 426, 424, 424, 428,   0,   0,   0,  0, 430, 436, 442,
    438, 440, 444,  52, 432, 434, 946, 20, 164, 178,  20,},
  { 404, 402, 402, 406,   0,   0,   0,  0, 408, 414, 420,
    416, 418, 422,  62, 410, 412, 946, 21, 165, 180,  21,},
  { 382, 380, 380, 384,   0,   0,   0,  0, 386, 392, 398,
    394, 396, 400,  54, 388, 390, 946, 15, 168, 182,  24,},
  { 206, 204, 204, 208,   0,   0,   0,  0, 210, 216, 222,
    218, 220, 224,  48, 212, 214, 946, 15, 172, 184,  28,},
  { 360, 358, 358, 362,   0,   0,   0,  0, 364, 370, 376,
    372, 374, 378,  48, 366, 368, 946, 22, 166, 186,  22,},
  { 228, 226, 226, 230,   0,   0,   0,  0, 232, 238, 244,
    240, 242, 246,  60, 234, 236, 946, 18, 162, 188,  18,},
  { 162, 160, 160, 164,   0,   0,   0,  0, 166, 172, 178,
    174, 176, 180,  48, 168, 170, 946, 18, 214, 190,  18,},
  { 338, 336, 336, 340,   0,   0,   0,350, 342, 348, 354,
    350, 352, 356,  48, 344, 346, 946, 22, 167, 192,  23,},
  { 316, 314, 314, 318,   0,   0,   0,  0, 320, 326, 332,
    328, 330, 334,  48, 322, 324, 946, 26, 170, 194,  26,},
  { 294, 292, 292, 296,   0,   0,   0,  0, 298, 304, 310,
    306, 308, 312,  48, 300, 302, 946, 15, 169, 196,  25,},
  { 272, 270, 270, 274,   0,   0,   0,  0, 276, 282, 288,
    284, 286, 290,  58, 278, 280, 946, 15, 161, 198,  17,},
  { 250, 248, 248, 252,   0,   0,   0,  0, 254, 260, 266,
    262, 264, 268,  64, 256, 258, 946, 19, 163, 200,  19,},
  {  26,  24,  24,  28,   0,   0,   0,  0,  30,  36,  42,
     38,  40,  44,  48,  32,  34, 946, 15, 171, 202,  27,},
  { 754, 752, 752, 756, 766,   0,   0,  0, 758, 764, 770,
    766, 768, 772,  48, 760, 762, 946,  1, 145, 204,   1,},
  { 732, 730, 730, 734,   0,   0,   0,  0, 736, 742, 748,
    744, 746, 750,  92, 738, 740, 946,  2, 146, 206,   2,},
  { 710, 708, 708, 712,   0,   0,   0,  0, 714, 720, 726,
    722, 724, 728,  90, 716, 718, 946,  3, 147, 208,   3,},
  { 688, 686, 686, 690,   0,   0,   0,  0, 692, 698, 704,
    700, 702, 706,  88, 694, 696, 946,  4, 148, 210,   4,},
  { 666, 664, 664, 668,   0,   0,   0,  0, 670, 676, 682,
    678, 680, 684,  86, 672, 674, 946,  5, 149, 212,   5,},
  { 644, 644, 644, 646,   0,   0,   0,  0, 648, 654, 660,
    656, 658, 662,  84, 650, 652, 946,  6, 150, 214,   6,},
  { 624, 622, 622, 626,   0,   0,   0,  0, 628, 634, 640,
    636, 638, 642,  82, 630, 632, 946,  7, 151, 216,   7,},
  { 602, 600, 600, 604,   0,   0,   0,  0, 606, 612, 618,
    614, 616, 620,  80, 608, 610, 946,  8, 152, 218,   8,},
  { 580, 578, 578, 582,   0,   0,   0,  0, 584, 590, 596,
    592, 594, 598,  78, 586, 588, 946,  9, 153, 220,   9,},
  { 556, 554, 566, 558, 558, 568, 562,564, 560, 574, 576,
    564, 556, 556,  56, 570, 572, 946, 10, 154, 222,  10,},
  { 534, 532, 532, 536,   0,   0,   0,  0, 538, 544, 550,
    546, 548, 552,  76, 540, 542, 946, 11, 155, 224,  11,},
  { 512, 510, 510, 514,   0,   0,   0,  0, 516, 522, 528,
    524, 526, 530,  50, 518, 520, 946, 12, 156, 226,  12,},
  { 490, 488, 488, 492,   0,   0,   0,  0, 494, 500, 506,
    502, 504, 508,  74, 496, 498, 946, 13, 157, 228,  13,},
  {   2,   0,   0,   4,  22,   0,   0,  0,   6,  12,  18,
     14,  16,  20,  48,   8,  10, 946, 15, 159, 230,  15,},
  { 470, 468, 468, 472,   0,   0,   0,  0, 474, 480, 486,
    482, 484, 470,  48, 476, 478, 946, 16, 160, 232,  16,},
  { 448, 446, 446, 450,   0,   0,   0,  0, 452, 458, 464,
    460, 462, 466,  68, 454, 456, 946, 14, 158, 234,  14,},
  { 184, 182, 182, 186,   0,   0,   0,  0, 188, 194, 200,
    196, 198, 202,  66, 190, 192, 946, 19, 173, 496,  29,},
  { 980, 980, 980, 980, 980,   0,   0,  0, 980, 980, 980,
    980, 980, 980, 980, 980, 980, 980,  0,   0,   0,   0,},
};

struct CreaturePickedUpOffset creature_picked_up_offset[] = {
  {  0,   0,  0,  0},
  {  6, 122,  0,  0},
  { 38, 134,  0,  0},
  {  0,  82,  0,  0},
  { -1,  69,  0,  0},
  { 14,  44,  0,  0},
  {  8,  64,  0,  0},
  { 14,  76,  0,  0},
  { 12,  50,  0,  0},
  {  6,  74,  0,  0},
  { 10,  90,  0,  0},
  {  8, 116,  0,  0},
  { 10, 102,  0,  0},
  {  4, 104,  0,  0},
  {  4, 128,  0,  0},
  { -5,  54,  0,  0},
  {  4,  96,  0,  0},
  { 14, 120,  0,  0},
  {  0,  50,  0,  0},
  { 14,  68,  0,  0},
  { -6, 126,  0,  0},
  { -8,  84,  0,  0},
  { -8,  76,  0,  0},
  { -2,  46,  0,  0},
  { 22,  60,  0,  0},
  {  0,  70,  0,  0},
  {  2,  44,  0,  0},
  {-12,  80,  0,  0},
  { -8,  60,  0,  0},
  {  0,  74,  0,  0},
  {  5, 121,  0,  0},
  {  0,   0,  0,  0},
};

int creature_swap_idx[CREATURE_TYPES_COUNT];

/******************************************************************************/
DLLIMPORT long _DK_remove_all_traces_of_combat(struct Thing *thing);
DLLIMPORT void _DK_cause_creature_death(struct Thing *thing, unsigned char a2);
DLLIMPORT void _DK_apply_spell_effect_to_thing(struct Thing *thing, long spell_idx, long spell_lev);
DLLIMPORT void _DK_creature_cast_spell_at_thing(struct Thing *caster, struct Thing *target, long a3, long a4);
DLLIMPORT void _DK_creature_cast_spell(struct Thing *caster, long a2, long a3, long a4, long a5);
DLLIMPORT void _DK_set_first_creature(struct Thing *thing);
DLLIMPORT void _DK_remove_first_creature(struct Thing *thing);
DLLIMPORT struct Thing *_DK_get_creature_near(unsigned short pos_x, unsigned short pos_y);
DLLIMPORT struct Thing *_DK_get_creature_near_with_filter(unsigned short pos_x, unsigned short pos_y, Thing_Filter filter, long a4);
DLLIMPORT struct Thing *_DK_get_creature_near_for_controlling(unsigned char a1, long a2, long a3);
DLLIMPORT long _DK_remove_creature_from_group(struct Thing *thing);
DLLIMPORT long _DK_add_creature_to_group_as_leader(struct Thing *thing1, struct Thing *thing2);
DLLIMPORT void _DK_anger_apply_anger_to_creature(struct Thing *thing, long anger, long a2, long a3);
DLLIMPORT long _DK_creature_available_for_combat_this_turn(struct Thing *thing);
DLLIMPORT long _DK_creature_look_for_combat(struct Thing *thing);
DLLIMPORT struct Thing *_DK_get_enemy_dungeon_heart_creature_can_see(struct Thing *thing);
DLLIMPORT long _DK_set_creature_object_combat(struct Thing *crthing, struct Thing *obthing);
DLLIMPORT void _DK_set_creature_door_combat(struct Thing *crthing, struct Thing *obthing);
DLLIMPORT void _DK_food_eaten_by_creature(struct Thing *crthing, struct Thing *obthing);
DLLIMPORT void _DK_creature_fire_shot(struct Thing *firing,struct  Thing *target, unsigned short a1, char a2, unsigned char a3);
DLLIMPORT unsigned long _DK_control_creature_as_controller(struct PlayerInfo *player, struct Thing *thing);
DLLIMPORT unsigned long _DK_control_creature_as_passenger(struct PlayerInfo *player, struct Thing *thing);
DLLIMPORT void _DK_load_swipe_graphic_for_creature(struct Thing *thing);
DLLIMPORT unsigned short _DK_find_next_annoyed_creature(unsigned char a1, unsigned short a2);
DLLIMPORT long _DK_creature_instance_has_reset(struct Thing *thing, long a2);
DLLIMPORT long _DK_get_human_controlled_creature_target(struct Thing *thing, long a2);
DLLIMPORT void _DK_set_creature_instance(struct Thing *thing, long a1, long a2, long a3, struct Coord3d *pos);
DLLIMPORT void _DK_draw_creature_view(struct Thing *thing);
DLLIMPORT void _DK_process_creature_standing_on_corpses_at(struct Thing *thing, struct Coord3d *pos);
DLLIMPORT short _DK_kill_creature(struct Thing *thing, struct Thing *tngrp, char a1, unsigned char a2, unsigned char a3, unsigned char a4);
DLLIMPORT void _DK_process_creature_instance(struct Thing *thing);
DLLIMPORT void _DK_update_creature_count(struct Thing *thing);
DLLIMPORT long _DK_process_creature_state(struct Thing *thing);
DLLIMPORT long _DK_move_creature(struct Thing *thing);
DLLIMPORT void _DK_init_creature_level(struct Thing *thing, long nlev);
DLLIMPORT long _DK_check_for_first_person_barrack_party(struct Thing *thing);
/******************************************************************************/
TbBool thing_can_be_controlled_as_controller(struct Thing *thing)
{
  if (thing->class_id == TCls_Creature)
    return true;
  if (thing->class_id == TCls_DeadCreature)
    return true;
  return false;
}

TbBool thing_can_be_controlled_as_passenger(struct Thing *thing)
{
  if (thing->class_id == TCls_Creature)
    return true;
  if (thing->class_id == TCls_DeadCreature)
    return true;
  if ((thing->class_id == TCls_Object) && (thing->model == 10))
    return true;
  return false;
}

long check_for_first_person_barrack_party(struct Thing *thing)
{
  return _DK_check_for_first_person_barrack_party(thing);
}

TbBool control_creature_as_controller(struct PlayerInfo *player, struct Thing *thing)
{
  struct CreatureStats *crstat;
  struct CreatureControl *cctrl;
  struct InitLight ilght;
  struct Camera *cam;
  //return _DK_control_creature_as_controller(player, thing);
  if ( (thing->owner != player->field_2B) || !thing_can_be_controlled_as_controller(thing) )
  {
    if (!control_creature_as_passenger(player, thing))
      return false;
    player->acamera->mappos.z.val += game.creature_stats[23].eye_height;
    return true;
  }
  cctrl = creature_control_get_from_thing(thing);
  cctrl->field_2D = 0;
  cctrl->field_2F = 0;
  cctrl->field_31 = 0;
  if (is_my_player(player))
  {
    toggle_status_menu(0);
    turn_off_roaming_menus();
  }
  cam = player->acamera;
  player->field_2F = thing->index;
  player->field_31 = thing->field_9;
  if (cam != NULL)
    player->field_4B5 = cam->field_6;
  thing->field_0 |= 0x20u;
  thing->field_4F |= 0x01;
  set_start_state(thing);
  set_player_mode(player, 2);
  if (thing->class_id == TCls_Creature)
  {
    cctrl->max_speed = calculate_correct_creature_maxspeed(thing);
    check_for_first_person_barrack_party(thing);
    if ((cctrl->field_7A & 0xFFF) != 0)
      make_group_member_leader(thing);
  }
  memset(&ilght, 0, sizeof(struct InitLight));
  ilght.mappos.x.val = thing->mappos.x.val;
  ilght.mappos.y.val = thing->mappos.y.val;
  ilght.mappos.z.val = thing->mappos.z.val;
  ilght.field_3 = 1;
  ilght.field_2 = 36;
  ilght.field_0 = 2560;
  ilght.field_11 = 1;
  thing->field_62 = light_create_light(&ilght);
  light_set_light_never_cache(thing->field_62);
  if (thing->field_62 == 0)
    ERRORLOG("Cannot allocate light to new controlled thing");
  if (is_my_player_number(thing->owner))
  {
    if (thing->class_id == TCls_Creature)
    {
      crstat = creature_stats_get_from_thing(thing);
      setup_eye_lens(crstat->eye_effect);
    }
  }
  return true;
}

TbBool control_creature_as_passenger(struct PlayerInfo *player, struct Thing *thing)
{
  struct Camera *cam;
  //return _DK_control_creature_as_passenger(player, thing);
  if (thing->owner != player->field_2B)
  {
    ERRORLOG("Player %d cannot control as passenger thing owned by player %d",(int)player->field_2B,(int)thing->owner);
    return false;
  }
  if (!thing_can_be_controlled_as_passenger(thing))
  {
    ERRORLOG("Thing of class %d and model %d can't be controlled as passenger",
        (int)thing->class_id,(int)thing->model);
    return false;
  }
  if (is_my_player(player))
  {
    toggle_status_menu(0);
    turn_off_roaming_menus();
  }
  cam = player->acamera;
  player->field_2F = thing->index;
  player->field_31 = thing->field_9;
  if (cam != NULL)
    player->field_4B5 = cam->field_6;
  set_player_mode(player, 3);
  thing->field_4F |= 0x01;
  return true;
}

void load_swipe_graphic_for_creature(struct Thing *thing)
{
  _DK_load_swipe_graphic_for_creature(thing);
}

long creature_available_for_combat_this_turn(struct Thing *thing)
{
  return _DK_creature_available_for_combat_this_turn(thing);
}

long creature_look_for_combat(struct Thing *thing)
{
  return _DK_creature_look_for_combat(thing);
}

struct Thing *get_enemy_dungeon_heart_creature_can_see(struct Thing *thing)
{
  return _DK_get_enemy_dungeon_heart_creature_can_see(thing);
}

long set_creature_object_combat(struct Thing *crthing, struct Thing *obthing)
{
  return _DK_set_creature_object_combat(crthing, obthing);
}

void set_creature_door_combat(struct Thing *crthing, struct Thing *obthing)
{
  _DK_set_creature_door_combat(crthing, obthing);
}

void food_eaten_by_creature(struct Thing *crthing, struct Thing *obthing)
{
  _DK_food_eaten_by_creature(crthing, obthing);
}

void anger_apply_anger_to_creature(struct Thing *thing, long anger, long a2, long a3)
{
  _DK_anger_apply_anger_to_creature(thing, anger, a2, a3);
}

void apply_spell_effect_to_thing(struct Thing *thing, long spell_idx, long spell_lev)
{
  _DK_apply_spell_effect_to_thing(thing, spell_idx, spell_lev);
}

short creature_take_wage_from_gold_pile(struct Thing *crthing,struct Thing *obthing)
{
  struct CreatureStats *crstat;
  struct CreatureControl *cctrl;
  long i;
  crstat = creature_stats_get_from_thing(crthing);
  cctrl = creature_control_get_from_thing(crthing);
  if (obthing->long_13 <= 0)
  {
    ERRORLOG("GoldPile had no gold so was deleted.");
    delete_thing_structure(obthing, 0);
    return false;
  }
  if (crthing->long_13 < crstat->gold_hold)
  {
    if (obthing->long_13+crthing->long_13 > crstat->gold_hold)
    {
      i = crstat->gold_hold-crthing->long_13;
      crthing->long_13 += i;
      obthing->long_13 -= i;
    } else
    {
      crthing->long_13 += obthing->long_13;
      delete_thing_structure(obthing, 0);
    }
  }
  anger_apply_anger_to_creature(crthing, crstat->annoy_got_wage, 1, 1);
  return true;
}

void creature_cast_spell_at_thing(struct Thing *caster, struct Thing *target, long spl_idx, long a4)
{
  const struct SpellInfo *spinfo;
  long i;
  if ((caster->field_0 & 0x20) != 0)
  {
    if (target->class_id == TCls_Object)
      i = 1;
    else
      i = 2;
  } else
  {
    if (target->class_id == TCls_Object)
      i = 3;
    else
    if (target->owner == caster->owner)
      i = 2;
    else
      i = 4;
  }
  spinfo = get_magic_info(spl_idx);
  if (magic_info_is_invalid(spinfo))
  {
    ERRORLOG("Thing owned by player %d tried to cast invalid spell %ld",(int)caster->owner,spl_idx);
    return;
  }
  creature_fire_shot(caster, target, spinfo->field_1, a4, i);
}

void creature_cast_spell(struct Thing *caster, long spl_idx, long a3, long trg_x, long trg_y)
{
  const struct SpellInfo *spinfo;
  struct CreatureControl *cctrl;
  struct CreatureStats *crstat;
  struct Thing *efthing;
  long i,k;
  //_DK_creature_cast_spell(caster, spl_idx, a3, trg_x, trg_y);
  spinfo = get_magic_info(spl_idx);
  cctrl = creature_control_get_from_thing(caster);
  crstat = creature_stats_get_from_thing(caster);
  if (creature_control_invalid(cctrl))
  {
    ERRORLOG("Invalid creature tried to cast spell %ld",spl_idx);
    return;
  }
  if (spl_idx == 10) // Teleport
  {
    cctrl->teleport_x = trg_x;
    cctrl->teleport_y = trg_y;
  }
  // Check if the spell can be fired as a shot
  if (spinfo->field_1)
  {
    if ((caster->field_0 & 0x20) != 0)
      i = 1;
    else
      i = 4;
    creature_fire_shot(caster, 0, spinfo->field_1, a3, i);
  } else
  // Check if the spell can be self-casted
  if (spinfo->field_2)
  {
    i = (long)spinfo->field_6;
    if (i > 0)
      thing_play_sample(caster, i, 100, 0, 3, 0, 4, 256);
    i = cctrl->explevel;
    // Make sure the creature level isn't larger than max spell level
    if (i > SPELL_MAX_LEVEL)
      i = SPELL_MAX_LEVEL;
    apply_spell_effect_to_thing(caster, spl_idx, i);
  }
  // Check if the spell has an effect associated
  if (spinfo->cast_effect != 0)
  {
    efthing = create_effect(&caster->mappos, spinfo->cast_effect, caster->owner);
    if (!thing_is_invalid(efthing))
    {
      if (spinfo->cast_effect == 14)
        efthing->byte_13.f3 = 3;
    }
  }
  // If the spell has area_range, then make area damage
  if (spinfo->area_range > 0)
  {
    i = cctrl->explevel;
    // This damage is computed directly, not selected from array, so it
    // don't have to be limited as others... but let's limit it anyway
    if (i > SPELL_MAX_LEVEL)
      i = SPELL_MAX_LEVEL;
    k = compute_creature_max_range(spinfo->area_range, i);
    i = compute_creature_attack_damage(spinfo->area_damage, crstat->luck, i);
    explosion_affecting_area(caster, &caster->mappos, k, i, spinfo->area_hit_type);
  }
}

void process_creature_instance(struct Thing *thing)
{
  struct CreatureControl *cctrl;
  struct InstanceInfo *inst_inf;
  cctrl = creature_control_get_from_thing(thing);
  //_DK_process_creature_instance(thing);
  if (cctrl->field_D2)
  {
    cctrl->field_D4++;
    if (cctrl->field_D6 == cctrl->field_D4)
    {
       inst_inf = &instance_info[cctrl->field_D2];
      if (inst_inf->func_cb != NULL)
        inst_inf->func_cb(thing, &inst_inf->field_22);
    }
    if (cctrl->field_D8 == cctrl->field_D4)
    {
      if ( cctrl->field_D3 )
      {
        cctrl->field_D4--;
        cctrl->field_D3 = 0;
        return;
      }
      cctrl->field_DE[cctrl->field_D2] = game.play_gameturn;
      cctrl->field_D2 = 0;
    }
    cctrl->field_D3 = 0;
  }
}

void update_creature_count(struct Thing *thing)
{
  _DK_update_creature_count(thing);
}

struct Thing *find_gold_pile_or_chicken_laying_on_mapblk(struct Map *mapblk)
{
  struct Thing *thing;
  struct Room *room;
  unsigned long k;
  long i;
  k = 0;
  i = get_mapwho_thing_index(mapblk);
  while (i != 0)
  {
    thing = thing_get(i);
    if (thing_is_invalid(thing))
    {
      WARNLOG("Jump out of things array");
      break;
    }
    i = thing->field_2;
    if (thing->class_id == TCls_Object)
    {
      if ((thing->model == 43) && thing_touching_floor(thing))
        return thing;
      if (thing->model == 10)
      {
        room = get_room_thing_is_on(thing);
        if (room_is_invalid(room))
          return thing;
        if ((room->kind != RoK_GARDEN) && (room->kind != RoK_TORTURE) && (room->kind != RoK_PRISON))
          return thing;
      }
    }
    k++;
    if (k > THINGS_COUNT)
    {
      ERRORLOG("Infinite loop detected when sweeping things list");
      break;
    }
  }
  return INVALID_THING;
}

struct Thing *find_interesting_object_laying_around_thing(struct Thing *crthing)
{
  struct Thing *thing;
  struct Map *mapblk;
  long stl_x,stl_y;
  long k;
  for (k=0; k < AROUND_TILES_COUNT; k++)
  {
    stl_x = crthing->mappos.x.stl.num + around[k].delta_x;
    stl_y = crthing->mappos.y.stl.num + around[k].delta_y;
    mapblk = get_map_block_at(stl_x,stl_y);
    if (!map_block_invalid(mapblk))
    {
      if ((mapblk->flags & 0x10) == 0)
      {
        thing = find_gold_pile_or_chicken_laying_on_mapblk(mapblk);
        if (!thing_is_invalid(thing))
          return thing;
      }
    }
  }
  return INVALID_THING;
}

long process_creature_state(struct Thing *thing)
{
  struct CreatureControl *cctrl;
  struct CreatureStats *crstat;
  struct StateInfo *stati;
  struct Thing *tgthing;
  long x,y;
  long k;
  SYNCDBG(18,"Starting");
//TODO: rework! (causes hang if out of things)
  //return _DK_process_creature_state(thing);

  cctrl = creature_control_get_from_thing(thing);
  process_person_moods_and_needs(thing);
  if (creature_available_for_combat_this_turn(thing))
  {
    if (!creature_look_for_combat(thing))
    {
      if ((!cctrl->field_3) && (thing->model != 23))
      {
        tgthing = get_enemy_dungeon_heart_creature_can_see(thing);
        if (!thing_is_invalid(tgthing))
          set_creature_object_combat(thing, tgthing);
      }
    }
  }
  if ((cctrl->field_3 & 0x10) == 0)
  {
    if ((cctrl->field_1D0) && ((cctrl->flgfield_0 & 0x0200) == 0))
    {
        if ( can_change_from_state_to(thing, thing->field_7, 86) )
        {
          x = stl_num_decode_x(cctrl->field_1D0);
          y = stl_num_decode_y(cctrl->field_1D0);
          tgthing = get_door_for_position(x,y);
          if (!thing_is_invalid(tgthing))
          {
            if (thing->owner != tgthing->owner)
              set_creature_door_combat(thing, tgthing);
          }
        }
    }
  }
  cctrl->field_1D0 = 0;
  if ((cctrl->field_7A & 0xFFF) != 0)
  {
    if (!creature_is_group_leader(thing))
      process_obey_leader(thing);
  }
  if ((thing->field_7 < 1) || (thing->field_7 >= 145))
  {
    ERRORLOG("Creature has illegal state[1], T%d, S%d, TCS%d", (int)thing->index, (int)thing->field_7, (int)thing->field_8);
    set_start_state(thing);
  }
  if (((thing->field_25 & 0x20) == 0) && (thing->model != 23))
  {
    tgthing = find_interesting_object_laying_around_thing(thing);
    if (!thing_is_invalid(tgthing))
    {
      if (tgthing->model == 43)
      {
        crstat = creature_stats_get_from_thing(thing);
        if (tgthing->long_13 > 0)
        {
          if (thing->long_13 < crstat->gold_hold)
          {
            if (crstat->gold_hold < tgthing->long_13 + thing->long_13)
            {
              k = crstat->gold_hold - thing->long_13;
              thing->long_13 += k;
              tgthing->long_13 -= k;
            } else
            {
              thing->long_13 += tgthing->long_13;
              delete_thing_structure(tgthing, 0);
            }
          }
        } else
        {
          ERRORLOG("GoldPile with no gold!");
          delete_thing_structure(tgthing, 0);
        }
        anger_apply_anger_to_creature(thing, crstat->annoy_got_wage, 1, 1);
      } else
      if (tgthing->model == 10)
      {
        if (!is_thing_passenger_controlled(tgthing))
          food_eaten_by_creature(tgthing, thing);
      }
    }
  }
  SYNCDBG(18,"Executing state %d",(int)thing->field_7);
  stati = get_thing_state_info(thing);
  if (stati->ofsfield_0 == NULL)
    return false;
  if (stati->ofsfield_0(thing) != -1)
    return false;
  SYNCDBG(18,"Finished");
  return true;
}

TbBool update_dead_creatures_list(struct Dungeon *dungeon, struct Thing *thing)
{
  struct CreatureStorage *cstore;
  struct CreatureControl *cctrl;
  long i;
  SYNCDBG(18,"Starting");
  cctrl = creature_control_get_from_thing(thing);
  if ((dungeon == NULL) || creature_control_invalid(cctrl))
  {
    WARNLOG("Invalid victim or dungeon");
    return false;
  }
  // Check if the creature of same type is in list
  i = dungeon->dead_creatures_count-1;
  while (i >= 0)
  {
    cstore = &dungeon->dead_creatures[i];
    if ((cstore->model == thing->model) && (cstore->explevel == cctrl->explevel))
    {
      // This creature is already in list
      SYNCDBG(18,"Already in list");
      return false;
    }
    i--;
  }
  // Find a slot for the new creature
  if (dungeon->dead_creatures_count < DEAD_CREATURES_MAX_COUNT)
  {
    i = dungeon->dead_creatures_count;
    dungeon->dead_creatures_count++;
  } else
  {
    i = dungeon->dead_creature_idx;
    dungeon->dead_creature_idx++;
    if (dungeon->dead_creature_idx >= DEAD_CREATURES_MAX_COUNT)
      dungeon->dead_creature_idx = 0;
  }
  cstore = &dungeon->dead_creatures[i];
  cstore->model = thing->model;
  cstore->explevel = cctrl->explevel;
  SYNCDBG(19,"Finished");
  return true;
}

/*
 * Increases proper kills counter for given player's dungeon.
 */
TbBool inc_player_kills_counter(long killer_idx, struct Thing *victim)
{
  struct Dungeon *killer_dungeon;
  killer_dungeon = &(game.dungeon[killer_idx%DUNGEONS_COUNT]);
  if (victim->owner == killer_idx)
    killer_dungeon->lvstats.friendly_kills++;
  else
    killer_dungeon->battles_won++;
  return true;
}

/*
 * Increases kills counters when victim is being killed by killer.
 * Note that killer may be invalid - in this case def_plyr_idx identifies the killer.
 */
TbBool update_kills_counters(struct Thing *victim, struct Thing *killer, char def_plyr_idx, unsigned char a5)
{
  struct CreatureControl *cctrl;
  cctrl = creature_control_get_from_thing(victim);
  if (a5)
  {
    if (!thing_is_invalid(killer))
    {
      return inc_player_kills_counter(killer->owner, victim);
    }
    if ((def_plyr_idx != -1) && (game.field_14E497 != def_plyr_idx))
    {
      return inc_player_kills_counter(def_plyr_idx, victim);
    }
  }
  if ((cctrl->field_1D2 != -1) && (game.field_14E497 != cctrl->field_1D2))
  {
    return inc_player_kills_counter(cctrl->field_1D2, victim);
  }
  return false;
}

long move_creature(struct Thing *thing)
{
  return _DK_move_creature(thing);
}

void cause_creature_death(struct Thing *thing, unsigned char a2)
{
  _DK_cause_creature_death(thing, a2);
}

long remove_all_traces_of_combat(struct Thing *thing)
{
  return _DK_remove_all_traces_of_combat(thing);
}

void prepare_to_controlled_creature_death(struct Thing *thing)
{
  struct PlayerInfo *player;
  player = &(game.players[thing->owner%PLAYERS_COUNT]);
  leave_creature_as_controller(player, thing);
  player->field_43E = 0;
  if (player->field_2B == thing->owner)
    setup_eye_lens(0);
  set_camera_zoom(player->acamera, player->field_4B6);
  if (player->field_2B == thing->owner)
  {
    turn_off_all_window_menus();
    turn_off_menu(31);
    turn_off_menu(35);
    turn_off_menu(32);
    turn_on_main_panel_menu();
    set_flag_byte(&game.numfield_C,0x40,(game.numfield_C & 0x20) != 0);
  }
  light_turn_light_on(player->field_460);
}

TbBool kill_creature(struct Thing *thing, struct Thing *killertng, char killer_plyr_idx,
      unsigned char a4, TbBool died_in_battle, unsigned char a6)
{
  struct CreatureControl *cctrl;
  struct CreatureControl *cctrlgrp;
  struct CreatureStats *crstat;
  struct Dungeon *dungeon;
  struct Dungeon *killerdngn;
  long i,k;
  SYNCDBG(18,"Starting");
  //return _DK_kill_creature(thing, killertng, killer_plyr_idx, a4, died_in_battle, a6);
  dungeon = NULL;
  cctrl = creature_control_get_from_thing(thing);
  cleanup_current_thing_state(thing);
  remove_all_traces_of_combat(thing);
  if ((cctrl->field_7A & 0xFFF) != 0)
    remove_creature_from_group(thing);
  if (thing->owner != game.field_14E497)
    dungeon = &(game.dungeon[thing->owner%DUNGEONS_COUNT]);
  if (!thing_is_invalid(killertng) && (killertng->owner == game.field_14E497))
    died_in_battle = 0;
  if (killer_plyr_idx == game.field_14E497)
    died_in_battle = 0;
  remove_events_thing_is_attached_to(thing);
  if (dungeon != NULL)
  {
    update_dead_creatures_list(dungeon, thing);
    if (died_in_battle)
    {
      dungeon->battles_lost++;
    }
  }

  if (!creature_control_invalid(cctrl))
  {
    if ((cctrl->field_AC & 0x04) != 0)
    {
      cctrl->field_AC &= 0xFB;
      for (i=0; i < 3; i++)
      {
        k = cctrl->field_2B3[i];
        if (k != 0)
        {
          thing = thing_get(k);
          delete_thing_structure(thing, 0);
          cctrl->field_2B3[i] = 0;
        }
      }
    }
    if ((cctrl->field_AD & 0x01) != 0)
    {
      cctrl->field_AD &= 0xFE;
      for (i=0; i < 3; i++)
      {
        k = cctrl->field_2B9[i];
        if (k != 0)
        {
          thing = thing_get(k);
          delete_thing_structure(thing, 0);
          cctrl->field_2B9[i] = 0;
        }
      }
    }
  }
  update_kills_counters(thing, killertng, killer_plyr_idx, died_in_battle);
  if (thing_is_invalid(killertng) || (killertng->owner == game.field_14E497) || (killer_plyr_idx == game.field_14E497) || (dungeon == NULL))
  {
    if ((a4) && ((thing->field_0 & 0x20) != 0))
    {
      prepare_to_controlled_creature_death(thing);
    }
    cause_creature_death(thing, a4);
    return true;
  }
  // Now we are sure that killertng and dungeon pointers are correct
  if (thing->owner == killertng->owner)
  {
    if ((get_creature_model_flags(thing) & MF_IsDiptera) && (get_creature_model_flags(killertng) & MF_IsArachnid))
    {
      dungeon->lvstats.flies_killed_by_spiders++;
    }
  }
  cctrlgrp = creature_control_get_from_thing(killertng);
  if (!creature_control_invalid(cctrlgrp))
    cctrlgrp->field_C2++;
  if (is_my_player_number(thing->owner))
  {
    output_message(11, 40, 1);
  } else
  if (is_my_player_number(killertng->owner))
  {
    output_message(12, 40, 1);
  }
  if (game.field_14E496 == killertng->owner)
  {
    killerdngn = &(game.dungeon[killertng->owner%DUNGEONS_COUNT]);
    if ((killerdngn->creature_tendencies & 0x01) != 0)
      ERRORLOG("How can hero have tend to imprison");
  }
  crstat = creature_stats_get_from_thing(killertng);
  anger_apply_anger_to_creature(killertng, crstat->annoy_win_battle, 4, 1);
  if (!creature_control_invalid(cctrlgrp) && died_in_battle)
    cctrlgrp->field_8C[14]++;
  if (dungeon != NULL)
    dungeon->hates_player[killertng->owner] += game.fight_hate_kill_value;
  SYNCDBG(18,"Almost finished");
  killerdngn = &(game.dungeon[killertng->owner%DUNGEONS_COUNT]);
  if ((a6) || (killerdngn->room_kind[4] == 0)
    || (killerdngn->creature_tendencies & 0x01) == 0)
  {
    if (a4 == 0)
    {
      cause_creature_death(thing, a4);
      return true;
    }
  }
  if (a4)
  {
    if ((thing->field_0 & 0x20) != 0)
      prepare_to_controlled_creature_death(thing);
    cause_creature_death(thing, a4);
    return true;
  }
  clear_creature_instance(thing);
  thing->field_7 = 67;
  cctrl = creature_control_get_from_thing(thing);
  cctrl->flgfield_0 |= 0x0400;
  cctrl->flgfield_0 |= 0x0200;
  cctrl->field_280 = 2000;
  thing->health = 1;
  return true;
}

void process_creature_standing_on_corpses_at(struct Thing *thing, struct Coord3d *pos)
{
  _DK_process_creature_standing_on_corpses_at(thing, pos);
}

/**
 * Calculates damage made by a creature by hand (using strength).
 */
long calculate_melee_damage(struct Thing *thing)
{
  struct CreatureControl *cctrl;
  struct CreatureStats *crstat;
  cctrl = creature_control_get_from_thing(thing);
  crstat = creature_stats_get_from_thing(thing);
  return compute_creature_attack_damage(crstat->strength, crstat->luck, cctrl->explevel);
  /* TODO: check if we really have to restrict it that much
  if (damage >= 256)
    damage = 256;
  return damage;*/
}

/**
 * Calculates damage made by a creature using specific shot type.
 */
long calculate_shot_damage(struct Thing *thing,long shot_kind)
{
  struct CreatureControl *cctrl;
  struct CreatureStats *crstat;
  struct ShotStats *shotstat;
  shotstat = &shot_stats[shot_kind];
  cctrl = creature_control_get_from_thing(thing);
  crstat = creature_stats_get_from_thing(thing);
  return compute_creature_attack_damage(shotstat->damage, crstat->luck, cctrl->explevel);
}

void creature_fire_shot(struct Thing *firing,struct  Thing *target, unsigned short shot_kind, char a2, unsigned char a3)
{
  struct CreatureControl *cctrl;
  struct CreatureStats *crstat;
  struct ShotStats *shotstat;
  struct Coord3d pos1;
  struct Coord3d pos2;
  struct ComponentVector cvect;
  struct Thing *shot;
  struct Thing *tmptng;
  short angle_xy,angle_yz;
  long damage;
  long target_idx,i;
  TbBool flag1;
  //_DK_creature_fire_shot(firing,target, a1, a2, a3); return;
  cctrl = creature_control_get_from_thing(firing);
  crstat = creature_stats_get_from_thing(firing);
  shotstat = &shot_stats[shot_kind];
  flag1 = false;
  // Prepare source position
  pos1.x.val = firing->mappos.x.val;
  pos1.y.val = firing->mappos.y.val;
  pos1.z.val = firing->mappos.z.val;
  pos1.x.val += (cctrl->field_2C1 * LbSinL(firing->field_52+512) >> 16);
  pos1.y.val -= (cctrl->field_2C1 * LbCosL(firing->field_52+512) >> 8) >> 8;
  pos1.x.val += (cctrl->field_2C3 * LbSinL(firing->field_52) >> 16);
  pos1.y.val -= (cctrl->field_2C3 * LbCosL(firing->field_52) >> 8) >> 8;
  pos1.z.val += (cctrl->field_2C5);
  // Compute launch angles
  if (thing_is_invalid(target))
  {
    angle_xy = firing->field_52;
    angle_yz = firing->field_54;
  } else
  {
    pos2.x.val = target->mappos.x.val;
    pos2.y.val = target->mappos.y.val;
    pos2.z.val = target->mappos.z.val;
    pos2.z.val += (target->field_58 >> 1);
    if (( shotstat->field_48 ) && (target->class_id != 9))
    {
      flag1 = true;
      pos1.z.val = pos2.z.val;
    }
    angle_xy = get_angle_xy_to(&pos1, &pos2);
    angle_yz = get_angle_yz_to(&pos1, &pos2);
  }
  // Compute shot damage
  if ( shotstat->field_48 )
  {
    damage = calculate_melee_damage(firing);
  } else
  {
    damage = calculate_shot_damage(firing,shot_kind);
  }
  shot = NULL;
  target_idx = 0;
  // Set target index for navigating shots
  if (shot_kind == 6)
  {
    if (!thing_is_invalid(target))
      target_idx = target->index;
  }
  switch ( shot_kind )
  {
    case 4:
    case 12:
      if ((thing_is_invalid(target)) || (get_2d_distance(&firing->mappos, &pos2) > 5120))
      {
          project_point_to_wall_on_angle(&pos1, &pos2, firing->field_52, firing->field_54, 256, 20);
      }
      shot = create_thing(&pos2, 2, shot_kind, firing->owner, -1);
      if (thing_is_invalid(shot))
        return;
      if (shot_kind == 12)
        draw_lightning(&pos1, &pos2, 96, 93);
      else
        draw_lightning(&pos1, &pos2, 96, 60);
      shot->health = shotstat->health;
      *(short *)&shot->byte_13.h = shotstat->damage;
      shot->field_1D = firing->index;
      break;
    case 7:
      if ((thing_is_invalid(target)) || (get_2d_distance(&firing->mappos, &pos2) > 768))
        project_point_to_wall_on_angle(&pos1, &pos2, firing->field_52, firing->field_54, 256, 4);
      shot = create_thing(&pos2, 2, shot_kind, firing->owner, -1);
      if (thing_is_invalid(shot))
        return;
      draw_flame_breath(&pos1, &pos2, 96, 2);
      shot->health = shotstat->health;
      *(short *)&shot->byte_13.h = shotstat->damage;
      shot->field_1D = firing->index;
      break;
    case 13:
      for (i=0; i < 32; i++)
      {
        tmptng = create_thing(&pos1, 2, shot_kind, firing->owner, -1);
        if (thing_is_invalid(tmptng))
          break;
        shot = tmptng;
        shot->byte_13.f3 = a3;
        shot->field_52 = (angle_xy + ACTION_RANDOM(101) - 50) & 0x7FF;
        shot->field_54 = (angle_yz + ACTION_RANDOM(101) - 50) & 0x7FF;
        angles_to_vector(shot->field_52, shot->field_54, shotstat->speed, &cvect);
        shot->pos_32.x.val += cvect.x;
        shot->pos_32.y.val += cvect.y;
        shot->pos_32.z.val += cvect.z;
        shot->field_1 |= 0x04;
        *(short *)&shot->byte_13.h = damage;
        shot->health = shotstat->health;
        shot->field_1D = firing->index;
      }
      break;
    default:
      shot = create_thing(&pos1, 2, shot_kind, firing->owner, -1);
      if (thing_is_invalid(shot))
        return;
      shot->field_52 = angle_xy;
      shot->field_54 = angle_yz;
      angles_to_vector(shot->field_52, shot->field_54, shotstat->speed, &cvect);
      shot->pos_32.x.val += cvect.x;
      shot->pos_32.y.val += cvect.y;
      shot->pos_32.z.val += cvect.z;
      shot->field_1 |= 0x04;
      *(short *)&shot->byte_13.h = damage;
      shot->health = shotstat->health;
      shot->field_1D = firing->index;
      shot->word_17 = target_idx;
      shot->byte_13.l = compute_creature_max_dexterity(crstat->dexterity,cctrl->explevel);
      break;
  }
  if (!thing_is_invalid(shot))
  {
#if (BFDEBUG_LEVEL > 0)
    damage = *(short *)&shot->byte_13.h;
    // Special debug code that shows amount of damage the shot will make
    if ((start_params.debug_flags & DFlg_ShotsDamage) != 0)
        create_price_effect(&pos1, my_player_number, damage);
    if ((damage < 0) || (damage > 2000))
    {
      WARNLOG("Shot of type %d carries %d damage",(int)shot_kind,(int)damage);
    }
#endif
    shot->byte_13.f3 = a3;
    if (shotstat->firing_sound > 0)
    {
      thing_play_sample(firing, shotstat->firing_sound + UNSYNC_RANDOM(shotstat->firing_sound_variants),
          100, 0, 3, 0, 3, 256);
    }
    if (shotstat->shot_sound > 0)
    {
      thing_play_sample(shot, shotstat->shot_sound, 100, 0, 3, 0, shotstat->field_20, 256);
    }
    set_flag_byte(&shot->field_25,0x10,flag1);
  }
}

void set_creature_level(struct Thing *thing, long nlvl)
{
  //_DK_set_creature_level(thing, nlvl); return;
  struct CreatureStats *crstat;
  struct CreatureControl *cctrl;
  struct Dungeon *dungeon;
  long old_max_health,max_health;
  int i,k;
  crstat = creature_stats_get_from_thing(thing);
  cctrl = creature_control_get_from_thing(thing);
  if (creature_control_invalid(cctrl))
    return;
  if (nlvl > CREATURE_MAX_LEVEL-1)
    nlvl = CREATURE_MAX_LEVEL-1;
  if (nlvl < 0)
    nlvl = 0;
  old_max_health = compute_creature_max_health(crstat->health,cctrl->explevel);
  cctrl->explevel = nlvl;
  max_health = compute_creature_max_health(crstat->health,cctrl->explevel);
  cctrl->max_health = max_health;
  if (cctrl->field_AD & 0x02)
    thing->field_46 = 300;
  else
    thing->field_46 = saturate_set_signed( 300 + (300*cctrl->explevel) / 20, 16);
  thing->health = saturate_set_signed( (thing->health*max_health)/old_max_health, 16);
  for (i=0; i < 10; i++)
  {
    k = crstat->instance_spell[i];
    if (k > 0)
    {
      if (crstat->instance_level[i] <= cctrl->explevel+1)
        cctrl->instances[k] = 1;
    }
  }
  if (game.field_14E497 != thing->owner)
  {
    dungeon = &(game.dungeon[thing->owner%DUNGEONS_COUNT]);
    dungeon->field_EA8 += game.creature_scores[thing->model%CREATURE_TYPES_COUNT].value[cctrl->explevel%CREATURE_MAX_LEVEL];
  }
}

void init_creature_level(struct Thing *thing, long nlev)
{
  _DK_init_creature_level(thing,nlev);
}

long creature_instance_has_reset(struct Thing *thing, long a2)
{
  return _DK_creature_instance_has_reset(thing, a2);
}

long get_human_controlled_creature_target(struct Thing *thing, long a2)
{
  return _DK_get_human_controlled_creature_target(thing, a2);
}

void set_creature_instance(struct Thing *thing, long a1, long a2, long a3, struct Coord3d *pos)
{
  _DK_set_creature_instance(thing, a1, a2, a3, pos);
}

unsigned short find_next_annoyed_creature(unsigned char a1, unsigned short a2)
{
  return _DK_find_next_annoyed_creature(a1, a2);
}

void draw_creature_view(struct Thing *thing)
{
  struct TbGraphicsWindow grwnd;
  struct PlayerInfo *player;
  long grscr_w,grscr_h;
  unsigned char *wscr_cp;
  unsigned char *scrmem;
  //_DK_draw_creature_view(thing); return;

  // If no eye lens required - just draw on the screen, directly
  player = &(game.players[my_player_number%PLAYERS_COUNT]);
  if (((game.flags_cd & MFlg_EyeLensReady) == 0) || (eye_lens_memory == NULL))
  {
    engine(&player->cameras[1]);
    return;
  }
  if ((game.numfield_1B == 0) || (game.numfield_1B == 13) || (game.numfield_1B >= 14))
  {
    engine(&player->cameras[1]);
    return;
  }

  //TODO: Temporary hack, until rewritten CMistFade is not completely used
  if ((game.numfield_1B >= 4) && (game.numfield_1B <= 12))
  {
    _DK_draw_creature_view(thing);
    return;
  }
  scrmem = eye_lens_spare_screen_memory;
  // Store previous graphics settings
  wscr_cp = lbDisplay.WScreen;
  grscr_w = lbDisplay.GraphicsScreenWidth;
  grscr_h = lbDisplay.GraphicsScreenHeight;
  LbScreenStoreGraphicsWindow(&grwnd);
  // Prepare new settings
  LbMemorySet(scrmem, 0, eye_lens_width*eye_lens_height*sizeof(TbPixel));

  lbDisplay.WScreen = scrmem;
  lbDisplay.GraphicsScreenHeight = eye_lens_height;
  lbDisplay.GraphicsScreenWidth = eye_lens_width;
  LbScreenSetGraphicsWindow(0, 0, MyScreenWidth/pixel_size, MyScreenHeight/pixel_size);
  // Draw on our buffer
  setup_engine_window(0, 0, MyScreenWidth, MyScreenHeight);
  engine(&player->cameras[1]);
  // Restore original graphics settings
  lbDisplay.WScreen = wscr_cp;
  lbDisplay.GraphicsScreenWidth = grscr_w;
  lbDisplay.GraphicsScreenHeight = grscr_h;
  LbScreenLoadGraphicsWindow(&grwnd);
  // Draw the buffer on real screen
  setup_engine_window(0, 0, MyScreenWidth, MyScreenHeight);
  switch (game.numfield_1B)
  {
  case 1:
  case 2:
      draw_lens(lbDisplay.WScreen, scrmem, eye_lens_memory,
            MyScreenWidth/pixel_size, MyScreenHeight/pixel_size, lbDisplay.GraphicsScreenWidth);
      break;
  case 3:
      flyeye_blitsec(scrmem, lbDisplay.WScreen, MyScreenWidth/pixel_size,
            lbDisplay.GraphicsScreenWidth, 1, MyScreenHeight/pixel_size);
      break;
  case 4:
  case 5:
  case 6:
  case 7:
  case 8:
  case 9:
  case 10:
  case 11:
  case 12:
	  draw_mist(lbDisplay.WScreen, lbDisplay.GraphicsScreenWidth, scrmem,
          MyScreenWidth/pixel_size, MyScreenWidth/pixel_size, MyScreenHeight/pixel_size);
      break;
  default:
      ERRORLOG("Invalid lens mode.");
      break;
  }
}

struct Thing *get_creature_near(unsigned short pos_x, unsigned short pos_y)
{
  return _DK_get_creature_near(pos_x, pos_y);
}

struct Thing *get_creature_near_with_filter(unsigned short pos_x, unsigned short pos_y, Thing_Filter filter, long a4)
{
  return _DK_get_creature_near_with_filter(pos_x, pos_y, filter, a4);
}

struct Thing *get_creature_near_for_controlling(unsigned char a1, long a2, long a3)
{
  return _DK_get_creature_near_for_controlling(a1, a2, a3);
}

long remove_creature_from_group(struct Thing *thing)
{
  return _DK_remove_creature_from_group(thing);
}

long add_creature_to_group_as_leader(struct Thing *thing1, struct Thing *thing2)
{
  return _DK_add_creature_to_group_as_leader(thing1, thing2);
}

void set_first_creature(struct Thing *thing)
{
  _DK_set_first_creature(thing);
}

void remove_first_creature(struct Thing *thing)
{
  _DK_remove_first_creature(thing);
}

TbBool thing_is_creature(const struct Thing *thing)
{
  if (thing_is_invalid(thing))
    return false;
  if (thing->class_id != TCls_Creature)
    return false;
  return true;
}

TbBool thing_is_creature_special_digger(const struct Thing *thing)
{
  if (thing_is_invalid(thing))
    return false;
  if (thing->class_id != TCls_Creature)
    return false;
  return ((get_creature_model_flags(thing) & MF_IsSpecDigger) != 0);
}

/******************************************************************************/
#ifdef __cplusplus
}
#endif