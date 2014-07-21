/******************************************************************************/
// Free implementation of Bullfrog's Dungeon Keeper strategy game.
/******************************************************************************/
/** @file frontmenu_net.c
 *     GUI menus for network support.
 * @par Purpose:
 *     Functions to show and maintain network screens.
 * @par Comment:
 *     None.
 * @author   KeeperFX Team
 * @date     05 Jan 2009 - 09 Oct 2010
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "frontmenu_net.h"
#include "globals.h"
#include "bflib_basics.h"

#include "bflib_datetm.h"
#include "bflib_guibtns.h"
#include "bflib_video.h"
#include "bflib_vidraw.h"
#include "bflib_sprite.h"
#include "bflib_sprfnt.h"

#include "front_network.h"
#include "gui_frontbtns.h"
#include "gui_draw.h"
#include "frontend.h"
#include "front_landview.h"
#include "net_game.h"
#include "kjm_input.h"
#include "game_merge.h"
#include "game_legacy.h"
#include "keeperfx.hpp"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/
DLLIMPORT void _DK_frontnet_start_input(void);
DLLIMPORT void _DK_frontnet_serial_reset(void);
DLLIMPORT void _DK_frontnet_modem_reset(void);
DLLIMPORT void _DK_frontnet_draw_services_scroll_tab(struct GuiButton *gbtn);
DLLIMPORT void _DK_frontnet_draw_service_button(struct GuiButton *gbtn);
DLLIMPORT void _DK_frontnet_service_maintain(struct GuiButton *gbtn);
DLLIMPORT void _DK_frontnet_service_up_maintain(struct GuiButton *gbtn);
DLLIMPORT void _DK_frontnet_service_down_maintain(struct GuiButton *gbtn);
DLLIMPORT void _DK_frontnet_service_up(struct GuiButton *gbtn);
DLLIMPORT void _DK_frontnet_service_down(struct GuiButton *gbtn);
DLLIMPORT void _DK_frontnet_service_select(struct GuiButton *gbtn);
DLLIMPORT void _DK_frontnet_session_set_player_name(struct GuiButton *gbtn);
DLLIMPORT void _DK_frontnet_draw_text_bar(struct GuiButton *gbtn);
DLLIMPORT void _DK_frontnet_session_up(struct GuiButton *gbtn);
DLLIMPORT void _DK_frontnet_session_up_maintain(struct GuiButton *gbtn);
DLLIMPORT void _DK_frontnet_session_down(struct GuiButton *gbtn);
DLLIMPORT void _DK_frontnet_session_down_maintain(struct GuiButton *gbtn);
DLLIMPORT void _DK_frontnet_session_maintain(struct GuiButton *gbtn);
DLLIMPORT void _DK_frontnet_draw_sessions_scroll_tab(struct GuiButton *gbtn);
DLLIMPORT void _DK_frontnet_draw_session_selected(struct GuiButton *gbtn);
DLLIMPORT void _DK_frontnet_session_select(struct GuiButton *gbtn);
DLLIMPORT void _DK_frontnet_draw_session_button(struct GuiButton *gbtn);
DLLIMPORT void _DK_frontnet_players_up(struct GuiButton *gbtn);
DLLIMPORT void _DK_frontnet_players_up_maintain(struct GuiButton *gbtn);
DLLIMPORT void _DK_frontnet_players_down(struct GuiButton *gbtn);
DLLIMPORT void _DK_frontnet_players_down_maintain(struct GuiButton *gbtn);
DLLIMPORT void _DK_frontnet_draw_players_scroll_tab(struct GuiButton *gbtn);
DLLIMPORT void _DK_frontnet_draw_net_session_players(struct GuiButton *gbtn);
DLLIMPORT void _DK_frontnet_session_join(struct GuiButton *gbtn);
DLLIMPORT void _DK_frontnet_session_create(struct GuiButton *gbtn);
DLLIMPORT void _DK_frontnet_return_to_main_menu(struct GuiButton *gbtn);
DLLIMPORT void _DK_frontnet_join_game_maintain(struct GuiButton *gbtn);
DLLIMPORT void _DK_frontnet_draw_alliance_box_tab(struct GuiButton *gbtn);
DLLIMPORT void _DK_frontnet_draw_net_start_players(struct GuiButton *gbtn);
DLLIMPORT void _DK_frontnet_select_alliance(struct GuiButton *gbtn);
DLLIMPORT void _DK_frontnet_draw_alliance_grid(struct GuiButton *gbtn);
DLLIMPORT void _DK_frontnet_draw_alliance_button(struct GuiButton *gbtn);
DLLIMPORT void _DK_frontnet_maintain_alliance(struct GuiButton *gbtn);
DLLIMPORT void _DK_frontnet_messages_up(struct GuiButton *gbtn);
DLLIMPORT void _DK_frontnet_messages_up_maintain(struct GuiButton *gbtn);
DLLIMPORT void _DK_frontnet_messages_down(struct GuiButton *gbtn);
DLLIMPORT void _DK_frontnet_messages_down_maintain(struct GuiButton *gbtn);
DLLIMPORT void _DK_frontnet_draw_bottom_scroll_box_tab(struct GuiButton *gbtn);
DLLIMPORT void _DK_frontnet_draw_messages_scroll_tab(struct GuiButton *gbtn);
DLLIMPORT void _DK_frontnet_draw_current_message(struct GuiButton *gbtn);
DLLIMPORT void _DK_frontnet_draw_messages(struct GuiButton *gbtn);
DLLIMPORT void _DK_frontnet_start_game_maintain(struct GuiButton *gbtn);
DLLIMPORT void _DK_frontnet_return_to_session_menu(struct GuiButton *gbtn);
DLLIMPORT void _DK_frontnet_draw_small_scroll_box_tab(struct GuiButton *gbtn);
DLLIMPORT void _DK_frontnet_draw_small_scroll_box(struct GuiButton *gbtn);
DLLIMPORT void _DK_frontnet_comport_up(struct GuiButton *gbtn);
DLLIMPORT void _DK_frontnet_comport_up_maintain(struct GuiButton *gbtn);
DLLIMPORT void _DK_frontnet_comport_down(struct GuiButton *gbtn);
DLLIMPORT void _DK_frontnet_comport_down_maintain(struct GuiButton *gbtn);
DLLIMPORT void _DK_frontnet_draw_comport_scroll_tab(struct GuiButton *gbtn);
DLLIMPORT void _DK_frontnet_draw_comport_selected(struct GuiButton *gbtn);
DLLIMPORT void _DK_frontnet_comport_select(struct GuiButton *gbtn);
DLLIMPORT void _DK_frontnet_draw_comport_button(struct GuiButton *gbtn);
DLLIMPORT void _DK_frontnet_comport_select_maintain(struct GuiButton *gbtn);
DLLIMPORT void _DK_frontnet_speed_up(struct GuiButton *gbtn);
DLLIMPORT void _DK_frontnet_speed_up_maintain(struct GuiButton *gbtn);
DLLIMPORT void _DK_frontnet_speed_down(struct GuiButton *gbtn);
DLLIMPORT void _DK_frontnet_speed_down_maintain(struct GuiButton *gbtn);
DLLIMPORT void _DK_frontnet_draw_speed_scroll_tab(struct GuiButton *gbtn);
DLLIMPORT void _DK_frontnet_draw_speed_selected(struct GuiButton *gbtn);
DLLIMPORT void _DK_frontnet_speed_select(struct GuiButton *gbtn);
DLLIMPORT void _DK_frontnet_draw_speed_button(struct GuiButton *gbtn);
DLLIMPORT void _DK_frontnet_speed_select_maintain(struct GuiButton *gbtn);
DLLIMPORT void _DK_frontnet_draw_text_cont_bar(struct GuiButton *gbtn);
DLLIMPORT void _DK_frontnet_net_set_modem_init(struct GuiButton *gbtn);
DLLIMPORT void _DK_frontnet_net_set_modem_hangup(struct GuiButton *gbtn);
DLLIMPORT void _DK_frontnet_net_set_modem_dial(struct GuiButton *gbtn);
DLLIMPORT void _DK_frontnet_net_set_phone_number(struct GuiButton *gbtn);
DLLIMPORT void _DK_frontnet_net_modem_start(struct GuiButton *gbtn);
DLLIMPORT void _DK_frontnet_net_modem_start_maintain(struct GuiButton *gbtn);
DLLIMPORT void _DK_frontnet_net_set_modem_answer(struct GuiButton *gbtn);
DLLIMPORT void _DK_frontnet_net_serial_start(struct GuiButton *gbtn);
DLLIMPORT void _DK_frontnet_net_serial_start_maintain(struct GuiButton *gbtn);
/******************************************************************************/
#ifdef __cplusplus
}
#endif
/******************************************************************************/
const char *get_net_speed_text(int id)
{
    static const char *net_speed[] = {
       "9600",
      "14400",
      "19200",
      "28800",
      "38400",
      "57600",
     "115200",
       "ISDN",
    };
    const int limit = sizeof(net_speed)/sizeof(*net_speed) - 1;
    if (id < 0)
      id = 0;
    if (id > limit)
      id = limit;
    return net_speed[id];
}

const char *get_net_comport_text(int id)
{
    static const char *net_comport[] = {
        "COM1",
        "COM2",
        "COM3",
        "COM4",
        "COM5",
        "COM6",
        "COM7",
        "COM8",
    };
    const int limit = sizeof(net_comport)/sizeof(*net_comport) - 1;
    if (id < 0)
      id = 0;
    if (id > limit)
      id = limit;
    return net_comport[id];
}

long frontnet_number_of_players_in_session(void)
{
    long i,nplyr;
    nplyr = 0;
    for (i=0; i < NET_PLAYERS_COUNT; i++)
    {
      if (network_player_active(i))
        nplyr++;
    }
    return nplyr;
}

void frontnet_session_up_maintain(struct GuiButton *gbtn)
{
    //_DK_frontnet_session_up_maintain(gbtn);
    gbtn->flags ^= (gbtn->flags ^ 8 * (net_session_scroll_offset != 0)) & 8;
}

void frontnet_session_down_maintain(struct GuiButton *gbtn)
{
    //_DK_frontnet_session_down_maintain(gbtn);
    gbtn->flags ^= (gbtn->flags ^ 8 * (net_number_of_sessions - 1 > net_session_scroll_offset)) & 8;
}

void frontnet_session_maintain(struct GuiButton *gbtn)
{
    //_DK_frontnet_session_maintain(gbtn);
    gbtn->flags ^= (gbtn->flags ^ 8 * (net_session_scroll_offset + (long)gbtn->content - 45 < net_number_of_sessions)) & 8;
}

void frontnet_players_up_maintain(struct GuiButton *gbtn)
{
    //_DK_frontnet_players_up_maintain(gbtn);
    gbtn->flags ^= (gbtn->flags ^ 8 * (net_player_scroll_offset != 0)) & 8;
}

void frontnet_players_down_maintain(struct GuiButton *gbtn)
{
    //_DK_frontnet_players_down_maintain(gbtn);
    gbtn->flags ^= (gbtn->flags ^ 8 * (net_number_of_enum_players - 1 > net_player_scroll_offset)) & 8;
}

void frontnet_join_game_maintain(struct GuiButton *gbtn)
{
    //_DK_frontnet_join_game_maintain(gbtn);
    if (net_service_index_selected == 1)
    {
      if ( net_session_index_active != -1 && net_config_info.str_join[0] )
        gbtn->flags |= 0x08;
      else
        gbtn->flags &= ~0x08;
    }
    else
    {
      gbtn->flags ^= (gbtn->flags ^ 8 * (net_session_index_active != -1)) & 8;
    }
}

void frontnet_maintain_alliance(struct GuiButton *gbtn)
{
    //_DK_frontnet_maintain_alliance(gbtn);
    long plyr_idx1, plyr_idx2;
    plyr_idx1 = gbtn->field_1B;
    plyr_idx2 = (long)gbtn->content - 74;
    if ( plyr_idx2 >= net_number_of_enum_players || net_number_of_enum_players <= plyr_idx1 || plyr_idx2 == plyr_idx1 )
      gbtn->flags &= ~0x08;
    else
      gbtn->flags |= 0x08;
}

void frontnet_messages_up_maintain(struct GuiButton *gbtn)
{
    //_DK_frontnet_messages_up_maintain(gbtn);
    gbtn->flags ^= (gbtn->flags ^ 8 * (net_message_scroll_offset != 0)) & 8;
}

void frontnet_messages_down_maintain(struct GuiButton *gbtn)
{
    //_DK_frontnet_messages_down_maintain(gbtn);
    gbtn->flags ^= (gbtn->flags ^ 8 * (net_number_of_messages - 1 > net_message_scroll_offset)) & 8;
}

void frontnet_start_game_maintain(struct GuiButton *gbtn)
{
    //_DK_frontnet_start_game_maintain(gbtn);
    gbtn->flags ^= (gbtn->flags ^ 8 * (net_number_of_enum_players > 1)) & 8;
}

void frontnet_comport_up_maintain(struct GuiButton *gbtn)
{
    //_DK_frontnet_comport_up_maintain(gbtn);
    gbtn->flags ^= (gbtn->flags ^ 8 * (net_comport_scroll_offset != 0)) & 8;
}

void frontnet_comport_down_maintain(struct GuiButton *gbtn)
{
    //_DK_frontnet_comport_down_maintain(gbtn);
    gbtn->flags ^= (gbtn->flags ^ 8 * (number_of_comports - 1 > net_comport_scroll_offset)) & 8;
}

void frontnet_comport_select_maintain(struct GuiButton *gbtn)
{
    //_DK_frontnet_comport_select_maintain(gbtn);
    gbtn->flags ^= (gbtn->flags ^ 8 * (net_comport_scroll_offset + (long)gbtn->content - 45 < number_of_comports)) & 8;
}

void frontnet_speed_up_maintain(struct GuiButton *gbtn)
{
    //_DK_frontnet_speed_up_maintain(gbtn);
    gbtn->flags ^= (gbtn->flags ^ 8 * (net_speed_scroll_offset != 0)) & 8;
}

void frontnet_speed_down_maintain(struct GuiButton *gbtn)
{
    //_DK_frontnet_speed_down_maintain(gbtn);
    gbtn->flags ^= (gbtn->flags ^ 8 * (number_of_speeds - 1 > net_speed_scroll_offset)) & 8;
}

void frontnet_speed_select_maintain(struct GuiButton *gbtn)
{
    //_DK_frontnet_speed_select_maintain(gbtn);
    gbtn->flags ^= (gbtn->flags ^ 8 * (net_speed_scroll_offset + (long)gbtn->content - 47 < number_of_speeds)) & 8;
}

void frontnet_net_modem_start_maintain(struct GuiButton *gbtn)
{
    //_DK_frontnet_net_modem_start_maintain(gbtn);
    if ((net_comport_index_active == -1) || (net_speed_index_active == -1))
      gbtn->flags &= ~0x08;
    else
      gbtn->flags |= 0x08;
}

void frontnet_net_serial_start_maintain(struct GuiButton *gbtn)
{
  if ((net_comport_index_active == -1) || (net_speed_index_active == -1))
    gbtn->flags &= ~0x08;
  else
    gbtn->flags |= 0x08;
}

void frontnet_serial_reset(void)
{
    //_DK_frontnet_serial_reset();
    net_write_config_file();
}

void frontnet_modem_reset(void)
{
    //_DK_frontnet_modem_reset();
    net_write_config_file();
}

void frontnet_start_input(void)
{
    //_DK_frontnet_start_input();
    if (lbInkey & 0x80)
    {
        lbInkey = 0;
        return;
    }
    if (lbInkey != 0)
    {
        unsigned short asckey;
        asckey = key_to_ascii(lbInkey, KMod_SHIFT);
        if ((lbInkey == 14) || (lbInkey == 28) || (frontend_font_char_width(1,asckey) > 0))
        {
            struct ScreenPacket *nspck;
            nspck = &net_screen_packet[my_player_number];
            if ((nspck->field_4 & 0xF8) == 0)
            {
              nspck->field_4 = (nspck->field_4 & 7) | 0x40;
              nspck->param1 = lbInkey;
              if ((lbKeyOn[42] == 0) && (lbKeyOn[54] == 0))
              {
                  nspck->param2 = 0;
                  lbInkey = 0;
                  return;
              }
              nspck->param2 = 1;
            }
        }
        lbInkey = 0;
    }
}

void frontnet_draw_services_scroll_tab(struct GuiButton *gbtn)
{
    frontend_draw_scroll_tab(gbtn, net_service_scroll_offset, 0, net_number_of_services);
}

void frontnet_session_set_player_name(struct GuiButton *gbtn)
{
    //_DK_frontnet_session_set_player_name(gbtn);
    strcpy(net_player_name, tmp_net_player_name);
    strcpy(net_config_info.str_u2, tmp_net_player_name);
    net_write_config_file();
}

void frontnet_draw_text_bar(struct GuiButton *gbtn)
{
    //_DK_frontnet_draw_text_bar(gbtn);
    struct TbSprite *spr;
    int i;
    long pos_x, pos_y;
    pos_x = gbtn->scr_pos_x;
    pos_y = gbtn->scr_pos_y;
    int units_per_px;
    units_per_px = simple_frontend_sprite_height_units_per_px(gbtn, 72);
    spr = &frontend_sprite[71];
    LbSpriteDrawResized(pos_x, pos_y, units_per_px, spr);
    pos_x += spr->SWidth * units_per_px / 16;
    spr = &frontend_sprite[72];
    for (i=0; i < 4; i++)
    {
        LbSpriteDrawResized(pos_x, pos_y, units_per_px, spr);
        pos_x += spr->SWidth * units_per_px / 16;
    }
    spr = &frontend_sprite[73];
    LbSpriteDrawResized(pos_x, pos_y, units_per_px, spr);
}

void frontnet_session_up(struct GuiButton *gbtn)
{
    //_DK_frontnet_session_up(gbtn);
    if (net_session_scroll_offset > 0)
      net_session_scroll_offset--;
}

void frontnet_session_down(struct GuiButton *gbtn)
{
    //_DK_frontnet_session_down(gbtn);
    if (net_session_scroll_offset < net_number_of_sessions - 1)
      net_session_scroll_offset++;
}

void frontnet_draw_sessions_scroll_tab(struct GuiButton *gbtn)
{
    frontend_draw_scroll_tab(gbtn, net_session_scroll_offset, 0, net_number_of_sessions);
}

void frontnet_players_up(struct GuiButton *gbtn)
{
    //_DK_frontnet_players_up(gbtn);
    if (net_player_scroll_offset > 0)
      net_player_scroll_offset--;
}

void frontnet_players_down(struct GuiButton *gbtn)
{
    //_DK_frontnet_players_down(gbtn);
    if (net_player_scroll_offset < net_number_of_enum_players - 1)
      net_player_scroll_offset++;
}

void frontnet_draw_players_scroll_tab(struct GuiButton *gbtn)
{
    frontend_draw_scroll_tab(gbtn, net_player_scroll_offset, 0, net_number_of_enum_players);
}

void frontnet_draw_net_session_players(struct GuiButton *gbtn)
{
    //_DK_frontnet_draw_net_session_players(gbtn);
    int i;
    i = frontend_button_caption_font(gbtn, 0);
    lbDisplay.DrawFlags = 0;
    LbTextSetFont(frontend_font[i]);
    int height;
    height = 0;
    long netplyr_idx;
    int shift_y;
    netplyr_idx = net_player_scroll_offset;
    for (shift_y=0; shift_y < gbtn->height; shift_y += height, netplyr_idx++)
    {
        const char *text;
        text = net_player[netplyr_idx].name;
        if (netplyr_idx >= net_number_of_enum_players)
            break;
        const struct TbSprite *spr;
        spr = &frontend_sprite[21+netplyr_idx];
        height = LbTextLineHeight();
        i = height - spr->SHeight;
        LbSpriteDraw(gbtn->scr_pos_x, gbtn->scr_pos_y + shift_y + abs(i)/2, spr);
        LbTextSetWindow(gbtn->scr_pos_x, shift_y + gbtn->scr_pos_y, gbtn->width - spr->SWidth, height);
        LbTextDraw(spr->SWidth, 0, text);
    }
}

void frontnet_session_add(struct GuiButton *gbtn)
{
    //TODO NET When clicked, it could either just display a modal text field (if that's possible), or a completely new screen which simply has a text field (for IP address) and OK/Cancel buttons.
}

void frontnet_session_join(struct GuiButton *gbtn)
{
    long plyr_num;
    plyr_num = network_session_join();
    if (plyr_num < 0)
        return;
    frontend_set_player_number(plyr_num);
    frontend_set_state(FeSt_NET_START);
}

void frontnet_return_to_main_menu(struct GuiButton *gbtn)
{
  if ( LbNetwork_Stop() )
  {
    ERRORLOG("LbNetwork_Stop() failed");
    return;
  }
  frontend_set_state(FeSt_MAIN_MENU);
}

void frontnet_draw_alliance_box_tab(struct GuiButton *gbtn)
{
    //_DK_frontnet_draw_alliance_box_tab(gbtn);
    const struct TbSprite *spr;
    int pos_x, pos_y;

    pos_x = gbtn->scr_pos_x;
    pos_y = gbtn->scr_pos_y;
    spr = &frontend_sprite[74];
    LbSpriteDraw(pos_x, pos_y, spr);
    pos_x += spr->SWidth;
    spr = &frontend_sprite[77];
    LbSpriteDraw(pos_x, pos_y, spr);
    pos_x += spr->SWidth;
    spr = &frontend_sprite[76];
    LbSpriteDraw(pos_x, pos_y, spr);

    pos_y += 5;
    pos_x = gbtn->scr_pos_x;
    spr = &frontend_sprite[74];
    pos_x += spr->SWidth - 1;
    if (net_number_of_enum_players > 0)
    {
        spr = &frontend_sprite[21];
        LbSpriteDraw(pos_x, pos_y, spr);
        pos_x += spr->SWidth;
    }
    if (net_number_of_enum_players > 1)
    {
        spr = &frontend_sprite[22];
        LbSpriteDraw(pos_x, pos_y, spr);
        pos_x += spr->SWidth;
    }
    if (net_number_of_enum_players > 2)
    {
        spr = &frontend_sprite[23];
        LbSpriteDraw(pos_x, pos_y, spr);
        pos_x += spr->SWidth;
    }
    if (net_number_of_enum_players > 3)
    {
        spr = &frontend_sprite[24];
        LbSpriteDraw(pos_x, pos_y, spr);
    }
}

void frontnet_draw_net_start_players(struct GuiButton *gbtn)
{
    //_DK_frontnet_draw_net_start_players(gbtn);
    int i;
    i = frontend_button_caption_font(gbtn, 0);
    lbDisplay.DrawFlags = 0;
    LbTextSetFont(frontend_font[i]);
    int height;
    height = 0;
    long netplyr_idx;
    int shift_y;
    netplyr_idx = net_player_scroll_offset;
    for (shift_y=0; shift_y < gbtn->height; shift_y += height, netplyr_idx++)
    {
        const char *text;
        text = net_player[netplyr_idx].name;
        if (netplyr_idx >= net_number_of_enum_players)
            break;
        const struct TbSprite *spr;

        long subplyr_idx;
        for (subplyr_idx = 0; subplyr_idx < net_number_of_enum_players; subplyr_idx++)
        {
            if (subplyr_idx >= NET_PLAYERS_COUNT)
                break;
            if (net_player_info[subplyr_idx].active)
            {
                if (subplyr_idx == netplyr_idx)
                    break;
            }
        }
        spr = &frontend_sprite[21+netplyr_idx];
        height = LbTextLineHeight();
        i = height - spr->SHeight;
        LbSpriteDraw(gbtn->scr_pos_x, gbtn->scr_pos_y + shift_y + abs(i)/2, spr);
        LbTextSetWindow(gbtn->scr_pos_x, gbtn->scr_pos_y + shift_y, gbtn->width - spr->SWidth, height);
        LbTextDraw(spr->SWidth, 0, text);
    }
}

void frontnet_select_alliance(struct GuiButton *gbtn)
{
    //_DK_frontnet_select_alliance(gbtn);
    struct PlayerInfo *myplyr;
    myplyr = get_my_player();
    int plyr1_idx, plyr2_idx;
    plyr1_idx = (long)gbtn->content - 74;
    plyr2_idx = gbtn->field_1B;
    if ( plyr1_idx == myplyr->id_number || plyr2_idx == myplyr->id_number )
    {
        struct ScreenPacket *nspck;
        nspck = &net_screen_packet[my_player_number];
        if ((nspck->field_4 & 0xF8) == 0)
        {
            nspck->field_4 = (nspck->field_4 & 7) | 0x20;
            nspck->param1 = plyr1_idx;
            nspck->param2 = plyr2_idx;
        }
    }
}

void frontnet_draw_alliance_grid(struct GuiButton *gbtn)
{
    //_DK_frontnet_draw_alliance_grid(gbtn);
    int pos_x, pos_y;
    pos_y = gbtn->scr_pos_y;
    struct TbSprite *spr;
    int netplyr_idx;

    pos_x = gbtn->scr_pos_x;
    spr = &frontend_sprite[83];
    for (netplyr_idx=0; netplyr_idx < NET_PLAYERS_COUNT; netplyr_idx++)
    {
        LbSpriteDraw(pos_x / pixel_size, pos_y / pixel_size, spr);
        pos_x += spr->SWidth;
    }
    pos_y += spr->SHeight;

    pos_x = gbtn->scr_pos_x;
    spr = &frontend_sprite[84];
    for (netplyr_idx=0; netplyr_idx < NET_PLAYERS_COUNT; netplyr_idx++)
    {
        LbSpriteDraw(pos_x / pixel_size, pos_y / pixel_size, spr);
        pos_x += spr->SWidth;
    }
    pos_y += spr->SHeight;

    pos_x = gbtn->scr_pos_x;
    spr = &frontend_sprite[85];
    for (netplyr_idx=0; netplyr_idx < NET_PLAYERS_COUNT; netplyr_idx++)
    {
        LbSpriteDraw(pos_x / pixel_size, pos_y / pixel_size, spr);
        pos_x += spr->SWidth;
    }
    pos_y += spr->SHeight;
}

void frontnet_draw_alliance_button(struct GuiButton *gbtn)
{
    //_DK_frontnet_draw_alliance_button(gbtn);
    int plyr1_idx, plyr2_idx;
    struct TbSprite *spr;
    plyr2_idx = gbtn->field_1B;
    plyr1_idx = (long)gbtn->content - 74;
    if ((plyr1_idx == plyr2_idx) || (frontend_alliances & alliance_grid[plyr1_idx][plyr2_idx]))
      spr = &frontend_sprite[87];
    else
      spr = &frontend_sprite[84];
    LbSpriteDraw(gbtn->scr_pos_x, gbtn->scr_pos_y, spr);
}

void frontnet_messages_up(struct GuiButton *gbtn)
{
    //_DK_frontnet_messages_up(gbtn);
    if (net_message_scroll_offset > 0)
      net_message_scroll_offset--;
}

void frontnet_messages_down(struct GuiButton *gbtn)
{
    //_DK_frontnet_messages_down(gbtn);
    if (net_message_scroll_offset < net_number_of_messages - 1)
      net_message_scroll_offset++;
}

void frontnet_draw_bottom_scroll_box_tab(struct GuiButton *gbtn)
{
    //_DK_frontnet_draw_bottom_scroll_box_tab(gbtn);
    long pos_x, pos_y;
    struct TbSprite *spr;
    pos_x = gbtn->scr_pos_x;
    pos_y = gbtn->scr_pos_y;
    lbDisplay.DrawFlags = Lb_SPRITE_FLIP_VERTIC;
    spr = &frontend_sprite[74];
    LbSpriteDraw(pos_x, pos_y, spr);
    pos_x += spr->SWidth;
    spr = &frontend_sprite[75];
    LbSpriteDraw(pos_x, pos_y, spr);
    pos_x += spr->SWidth;
    LbSpriteDraw(pos_x, pos_y, spr);
    pos_x += spr->SWidth;
    spr = &frontend_sprite[76];
    LbSpriteDraw(pos_x, pos_y, spr);
    lbDisplay.DrawFlags = 0;
}

void frontnet_draw_messages_scroll_tab(struct GuiButton *gbtn)
{
    frontend_draw_scroll_tab(gbtn, net_message_scroll_offset, 0, net_number_of_messages);
}

void frontnet_draw_scroll_selection_box(struct GuiButton *gbtn, long font_idx, const char *text)
{
    struct TbSprite * sprite;
    int draw_x;
    int i;
    unsigned char height;
    sprite = &frontend_sprite[55];
    draw_x = gbtn->scr_pos_x;
    for (i = 6; i > 0; --i)
    {
      LbSpriteDraw(draw_x, gbtn->scr_pos_y, sprite);
      draw_x += sprite->SWidth;
      ++sprite;
    }

    if (text != NULL)
    {
      lbDisplay.DrawFlags = 0;
      LbTextSetFont(frontend_font[font_idx]);
      height = LbTextHeight(text);
      LbTextSetWindow(gbtn->scr_pos_x + 13, gbtn->scr_pos_y, gbtn->width - 26, height);
      LbTextDraw(0, 0, text);
    }
}

void frontnet_draw_current_message(struct GuiButton *gbtn)
{
    static TbClockMSec last_time = 0;
    static TbBool print_with_cursor = 1;

    struct PlayerInfo *player;
    int font_idx;
    char text[2048];
    //_DK_frontnet_draw_current_message(gbtn);

    // Blink cursor - switch state every 100ms
    if (LbTimerClock() >= last_time + 100)
    {
        print_with_cursor = !print_with_cursor;
        last_time = LbTimerClock();
    }

    // Get player
    player = get_my_player();
    if (player_invalid(player)) {
        return;
    }

    // Prepare text buffer and font
    snprintf(text, sizeof(text), "%s%s", player->mp_message_text, print_with_cursor?"_":"");
    font_idx = frontend_button_caption_font(gbtn, 0);
    // And draw the message
    frontnet_draw_scroll_selection_box(gbtn, font_idx, text);
}

void frontnet_draw_messages(struct GuiButton *gbtn)
{
  //_DK_frontnet_draw_messages(gbtn);

  int y;
  int font_idx;
  struct NetMessage *message_ptr;
  int num_active;
  unsigned char font_height;
  struct TbSprite *player_sprite;
  long long height_diff;
  int scroll_offset;
  int i;

  y = 0;
  scroll_offset = net_message_scroll_offset;
  font_idx = frontend_button_caption_font(gbtn, 0);
  lbDisplay.DrawFlags = 0;
  LbTextSetFont(frontend_font[font_idx]);
  if ( gbtn->height )
  {
    message_ptr = &net_message[net_message_scroll_offset];
    do
    {
      if ( scroll_offset >= net_number_of_messages )
        break;
      num_active = 0;
      for (i = message_ptr->plyr_idx; i > 0; --i)
      {
        if ( net_player_info[i].active)
          ++num_active;
      }

      player_sprite = &frontend_sprite[num_active + 21];
      font_height = LbTextLineHeight();

      height_diff = font_height - player_sprite->SHeight;
      LbSpriteDraw(gbtn->scr_pos_x, y + gbtn->scr_pos_y + (((unsigned)height_diff - (unsigned) (height_diff >> 32)) >> 1), player_sprite);

      LbTextSetWindow(gbtn->scr_pos_x, y + gbtn->scr_pos_y, gbtn->width, font_height);
      LbTextDraw(player_sprite->SWidth, 0, message_ptr->text);

      ++message_ptr;
      y += font_height;
      ++scroll_offset;
    }
    while ( y < gbtn->height );
  }
}

void frontnet_return_to_session_menu(struct GuiButton *gbtn)
{
    if ( LbNetwork_Stop() )
    {
        ERRORLOG("LbNetwork_Stop() failed");
        return;
    }
    if ( setup_network_service(net_service_index_selected) )
        frontend_set_state(FeSt_NET_SESSION);
    else
        frontend_set_state(FeSt_MAIN_MENU);
}

void frontnet_draw_small_scroll_box_tab(struct GuiButton *gbtn)
{
    //_DK_frontnet_draw_small_scroll_box_tab(gbtn);
    long pos_x, pos_y;
    struct TbSprite *spr;
    pos_x = gbtn->scr_pos_x;
    pos_y = gbtn->scr_pos_y;
    spr = &frontend_sprite[74];
    LbSpriteDraw(pos_x, pos_y, spr);
    pos_x += spr->SWidth;
    spr = &frontend_sprite[75];
    LbSpriteDraw(pos_x, pos_y, spr);
    pos_x += spr->SWidth;
    spr = &frontend_sprite[77];
    LbSpriteDraw(pos_x, pos_y, spr);
    pos_x += spr->SWidth;
    spr = &frontend_sprite[76];
    LbSpriteDraw(pos_x, pos_y, spr);
}

void frontnet_draw_small_scroll_selection_box(struct GuiButton *gbtn, long font_idx, const char *text)
{
    struct TbSprite *spr;
    int pos_x, pos_y;
    pos_x = gbtn->scr_pos_x;
    pos_y = gbtn->scr_pos_y;
    spr = &frontend_sprite[55];
    LbSpriteDraw(pos_x, pos_y, spr);
    pos_x += spr->SWidth;
    spr = &frontend_sprite[56];
    LbSpriteDraw(pos_x, pos_y, spr);
    pos_x += spr->SWidth;
    LbSpriteDraw(pos_x, pos_y, spr);
    pos_x += spr->SWidth;
    spr = &frontend_sprite[60];
    LbSpriteDraw(pos_x, pos_y, spr);
    if (text != NULL)
    {
        lbDisplay.DrawFlags = 0;
        LbTextSetFont(frontend_font[font_idx]);
        int height;
        height = LbTextLineHeight();
        LbTextSetWindow(gbtn->scr_pos_x + 13, gbtn->scr_pos_y, gbtn->width - 26, height);
        LbTextDraw(0, 0, text);
    }
}

void frontnet_draw_small_scroll_box(struct GuiButton *gbtn)
{
    //_DK_frontnet_draw_small_scroll_box(gbtn); return;
    long pos_x, pos_y;
    struct TbSprite *spr;
    pos_x = gbtn->scr_pos_x;
    pos_y = gbtn->scr_pos_y;
    int btn_type;
    int len;
    btn_type = (long)gbtn->content;
    if (btn_type == 24) {
        len = 2;
    } else
    if (btn_type == 25) {
        len = 3;
    } else
    if (btn_type == 26) {
        len = 7;
    } else {
        ERRORLOG("Unknown button type %d",(int)btn_type);
        return;
    }
    spr = &frontend_sprite[25];
    LbSpriteDraw(pos_x, pos_y, spr);
    pos_x += spr->SWidth;
    spr++;
    LbSpriteDraw(pos_x, pos_y, spr);
    pos_x += spr->SWidth;
    spr++;
    LbSpriteDraw(pos_x, pos_y, spr);
    pos_x += spr->SWidth;
    spr += 3;
    LbSpriteDraw(pos_x, pos_y, spr);
    pos_x += spr->SWidth;
    spr++;
    LbSpriteDraw(pos_x, pos_y, spr);

    int dlen;
    dlen = 3;
    spr = &frontend_sprite[25];
    pos_y += spr->SHeight;
    for ( ; len > 0; len -= dlen)
    {
      pos_x = gbtn->scr_pos_x;
      int spr_idx;
      if (len < 3)
          spr_idx = 33;
      else
          spr_idx = 40;
      spr = &frontend_sprite[spr_idx];
      LbSpriteDraw(pos_x, pos_y, spr);
      pos_x += spr->SWidth;
      spr++;
      LbSpriteDraw(pos_x, pos_y, spr);
      pos_x += spr->SWidth;
      spr++;
      LbSpriteDraw(pos_x, pos_y, spr);
      pos_x += spr->SWidth;
      spr += 3;
      LbSpriteDraw(pos_x, pos_y, spr);
      pos_x += spr->SWidth;
      if (len < 3)
          spr_idx = 39;
      else
          spr_idx = 46;
      spr = &frontend_sprite[spr_idx];
      LbSpriteDraw(pos_x, pos_y, spr);
      pos_y += spr->SHeight;
      if (len < 3)
          dlen = 1;
      else
          dlen = 3;
    }

    pos_x = gbtn->scr_pos_x;
    spr = &frontend_sprite[47];
    pos_y = gbtn->scr_pos_y + gbtn->height - spr->SHeight;
    LbSpriteDraw(pos_x, pos_y, spr);
    pos_x += spr->SWidth;
    spr++;
    LbSpriteDraw(pos_x, pos_y, spr);
    pos_x += spr->SWidth;
    spr++;
    LbSpriteDraw(pos_x, pos_y, spr);
    pos_x += spr->SWidth;
    spr += 3;
    LbSpriteDraw(pos_x, pos_y, spr);
    pos_x += spr->SWidth;
    spr++;
    LbSpriteDraw(pos_x, pos_y, spr);
}

void frontnet_comport_up(struct GuiButton *gbtn)
{
    //_DK_frontnet_comport_up(gbtn);
    if (net_comport_scroll_offset > 0)
      net_comport_scroll_offset--;
}

void frontnet_comport_down(struct GuiButton *gbtn)
{
    //_DK_frontnet_comport_down(gbtn);
    if (net_comport_scroll_offset < number_of_comports - 1)
      net_comport_scroll_offset++;
}

void frontnet_draw_comport_scroll_tab(struct GuiButton *gbtn)
{
    frontend_draw_scroll_tab(gbtn, net_comport_scroll_offset, 0, number_of_comports);
}

void frontnet_draw_comport_selected(struct GuiButton *gbtn)
{
    //_DK_frontnet_draw_comport_selected(gbtn);
    if (net_comport_index_active == -1)
        frontnet_draw_small_scroll_selection_box(gbtn, frontend_button_caption_font(gbtn, 0), 0);
    else
        frontnet_draw_small_scroll_selection_box(gbtn, frontend_button_caption_font(gbtn, 0), get_net_comport_text(net_comport_index_active));
}

void frontnet_comport_select(struct GuiButton *gbtn)
{
    //_DK_frontnet_comport_select(gbtn);
    gbtn->flags ^= (gbtn->flags ^ 8 * (net_comport_scroll_offset + (long)gbtn->content - 45 < number_of_comports)) & 8;
}

void frontnet_draw_comport_button(struct GuiButton *gbtn)
{
    //_DK_frontnet_draw_comport_button(gbtn);
    int i;
    int febtn_idx;
    febtn_idx = (long)gbtn->content;
    i = net_comport_scroll_offset + febtn_idx - 45;
    if (i < number_of_comports)
    {
        int font_idx;
        font_idx = frontend_button_caption_font(gbtn,frontend_mouse_over_button);
        LbTextSetFont(frontend_font[font_idx]);
        lbDisplay.DrawFlags = 0;
        const char *text;
        text = get_net_comport_text(i);
        LbTextSetWindow(gbtn->scr_pos_x, gbtn->scr_pos_y, gbtn->width, LbTextHeight(text));
        LbTextDraw(0, 0, text);
    }
}

void frontnet_speed_up(struct GuiButton *gbtn)
{
    //_DK_frontnet_speed_up(gbtn);
    if (net_speed_scroll_offset > 0)
      net_speed_scroll_offset--;
}

void frontnet_speed_down(struct GuiButton *gbtn)
{
    //_DK_frontnet_speed_down(gbtn);
    if (net_speed_scroll_offset < number_of_speeds - 1)
      net_speed_scroll_offset++;
}

void frontnet_draw_speed_scroll_tab(struct GuiButton *gbtn)
{
    frontend_draw_scroll_tab(gbtn, net_speed_scroll_offset, 0, number_of_speeds);
}

void frontnet_draw_speed_selected(struct GuiButton *gbtn)
{
    //_DK_frontnet_draw_speed_selected(gbtn);
    // Select font to draw
    if (net_speed_index_active == -1)
        frontnet_draw_small_scroll_selection_box(gbtn, frontend_button_caption_font(gbtn, 0), 0);
    else
        frontnet_draw_small_scroll_selection_box(gbtn, frontend_button_caption_font(gbtn, 0), get_net_speed_text(net_speed_index_active));
}

void frontnet_speed_select(struct GuiButton *gbtn)
{
    //_DK_frontnet_speed_select(gbtn);
    gbtn->flags ^= (gbtn->flags ^ 8 * (net_speed_scroll_offset + (long)gbtn->content - 47 < number_of_speeds)) & 8;
}

void frontnet_draw_speed_button(struct GuiButton *gbtn)
{
    //_DK_frontnet_draw_speed_button(gbtn);
    int i;
    int febtn_idx;
    febtn_idx = (long)gbtn->content;
    i = net_speed_scroll_offset + febtn_idx - 47;
    if (i < number_of_speeds)
    {
        // Select font to draw
        int font_idx;
        font_idx = frontend_button_caption_font(gbtn,frontend_mouse_over_button);
        LbTextSetFont(frontend_font[font_idx]);
        lbDisplay.DrawFlags = 0;
        const char *text;
        text = get_net_speed_text(i);
        LbTextSetWindow(gbtn->scr_pos_x, gbtn->scr_pos_y, gbtn->width, LbTextHeight(text));
        LbTextDraw(0, 0, text);
    }
}

void frontnet_draw_text_cont_bar(struct GuiButton *gbtn)
{
    //_DK_frontnet_draw_text_cont_bar(gbtn);
    int pos_x, pos_y;
    pos_y = gbtn->scr_pos_y;
    struct TbSprite *spr;
    int netplyr_idx;
    pos_x = gbtn->scr_pos_x;

    spr = &frontend_sprite[80];
    LbSpriteDraw(pos_x, pos_y, spr);
    pos_x += spr->SWidth;

    spr = &frontend_sprite[81];
    for (netplyr_idx=0; netplyr_idx < NET_PLAYERS_COUNT; netplyr_idx++)
    {
        LbSpriteDraw(pos_x / pixel_size, pos_y / pixel_size, spr);
        pos_x += spr->SWidth;
    }

    spr = &frontend_sprite[82];
    LbSpriteDraw(pos_x, pos_y, spr);
}

void frontnet_net_set_modem_init(struct GuiButton *gbtn)
{
    //_DK_frontnet_net_set_modem_init(gbtn);
    strcpy(net_config_info.str_atz, tmp_net_modem_init);
}

void frontnet_net_set_modem_hangup(struct GuiButton *gbtn)
{
    //_DK_frontnet_net_set_modem_hangup(gbtn);
    strcpy(net_config_info.str_ath, tmp_net_modem_hangup);
}

void frontnet_net_set_modem_dial(struct GuiButton *gbtn)
{
    //_DK_frontnet_net_set_modem_dial(gbtn);
    strcpy(net_config_info.str_atdt, tmp_net_modem_dial);
}

void frontnet_net_set_phone_number(struct GuiButton *gbtn)
{
    //_DK_frontnet_net_set_phone_number(gbtn);
    strcpy(net_config_info.str_join, tmp_net_phone_number);
}

void frontnet_net_modem_start(struct GuiButton *gbtn)
{
    //_DK_frontnet_net_modem_start(gbtn);
    if ((net_comport_index_active == -1) || (net_speed_index_active == -1))
        gbtn->flags &= ~0x08;
    else
        gbtn->flags |= 0x08;
}

void frontnet_net_set_modem_answer(struct GuiButton *gbtn)
{
    //_DK_frontnet_net_set_modem_answer(gbtn);
    strcpy(net_config_info.str_ats, tmp_net_modem_answer);
}

void frontnet_net_serial_start(struct GuiButton *gbtn)
{
    const char *net_speed_text;
    net_serial_data.field_0 = net_config_info.numfield_0;
    net_speed_text = get_net_speed_text(net_config_info.numfield_9);
    if (strcmp(net_speed_text, "ISDN") != 0)
    {
        net_serial_data.numfield_4 = atoi(net_speed_text);
    } else
    {
        ERRORLOG("ISDN not supported by Serial");
    }
  net_serial_data.field_8 = net_config_info.numfield_1[(unsigned char)net_config_info.numfield_0];
  net_serial_data.str_dial = NULL;
  net_serial_data.str_phone = NULL;
  net_serial_data.str_hang = NULL;
  net_serial_data.str_answr = NULL;
  setup_network_service(0);
}

void frontnet_service_up_maintain(struct GuiButton *gbtn)
{
    set_flag_byte(&gbtn->flags, LbBtnF_Unknown08, (net_service_scroll_offset != 0));
}

void frontnet_service_down_maintain(struct GuiButton *gbtn)
{
    set_flag_byte(&gbtn->flags, LbBtnF_Unknown08, (net_number_of_services-1 > net_service_scroll_offset));
}

void frontnet_service_up(struct GuiButton *gbtn)
{
    if (net_service_scroll_offset > 0)
      net_service_scroll_offset--;
}

void frontnet_service_down(struct GuiButton *gbtn)
{
    if ( net_number_of_services-1 > net_service_scroll_offset )
      net_service_scroll_offset++;
}

void frontnet_service_maintain(struct GuiButton *gbtn)
{
    set_flag_byte(&gbtn->flags, LbBtnF_Unknown08, (net_service_scroll_offset+(long)gbtn->content-45 < net_number_of_services));
}

void frontnet_draw_service_button(struct GuiButton *gbtn)
{
  int srvidx;
  long febtn_idx;
  // Find and verify selected network service
  febtn_idx = (long)(gbtn->content);
  srvidx = febtn_idx + net_service_scroll_offset - 45;
  if (srvidx >= net_number_of_services)
    return;
  // Select font to draw
  int font_idx;
  font_idx = frontend_button_caption_font(gbtn,frontend_mouse_over_button);
  LbTextSetFont(frontend_font[font_idx]);
  int h;
  lbDisplay.DrawFlags = Lb_TEXT_HALIGN_LEFT;
  // Set drawing window and draw the text
  int units_per_px;
  units_per_px = gbtn->height * 16 / 22;
  h = LbTextLineHeight()*units_per_px/16;
  LbTextSetWindow(gbtn->scr_pos_x, gbtn->scr_pos_y, gbtn->width, h);
  LbTextDrawResized(0, 0, units_per_px, net_service[srvidx]);
}

void frontnet_service_select(struct GuiButton *gbtn)
{
  int srvidx;
  srvidx = (long)(gbtn->content) + net_service_scroll_offset - 45;
  if ( ((game.system_flags & GSF_AllowOnePlayer) != 0)
     && (srvidx+1 >= net_number_of_services) )
  {
      fe_network_active = 0;
      frontend_set_state(FeSt_NETLAND_VIEW);
  } else
  if (srvidx <= 0)
  {
      frontend_set_state(FeSt_NET_SERIAL);
  } else
  {
      setup_network_service(srvidx);
  }
}

/******************************************************************************/
