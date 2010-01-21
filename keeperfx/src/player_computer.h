/******************************************************************************/
// Free implementation of Bullfrog's Dungeon Keeper strategy game.
/******************************************************************************/
/** @file player_computer.h
 *     Header file for player_computer.cpp.
 *     Note that this file is a C header, while its code is CPP.
 * @par Purpose:
 *     Computer player definitions and activities.
 * @par Comment:
 *     Just a header file - #defines, typedefs, function prototypes etc.
 * @author   Tomasz Lis
 * @date     10 Mar 2009 - 20 Mar 2009
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/

#ifndef DK_PLYR_COMPUT_H
#define DK_PLYR_COMPUT_H

#include "bflib_basics.h"
#include "globals.h"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/
#define COMPUTER_TASKS_COUNT        100
#define COMPUTER_PROCESSES_COUNT     20
#define COMPUTER_CHECKS_COUNT        15
#define COMPUTER_EVENTS_COUNT        12
#define COMPUTER_PROCESS_LISTS_COUNT 14

#define COMPUTER_CHECKS_TYPES_COUNT  51
#define COMPUTER_EVENTS_TYPES_COUNT  31

#ifdef __cplusplus
#pragma pack(1)
#endif

struct Computer2;
struct ComputerProcess;
struct ComputerCheck;
struct ComputerEvent;
struct Event;

typedef unsigned char ComputerType;
typedef char ComputerName[LINEMSG_SIZE];

typedef long (*Comp_Process_Func)(struct Computer2 *comp, struct ComputerProcess *process);
typedef long (*Comp_Check_Func)(struct Computer2 *comp, struct ComputerCheck * check);
typedef long (*Comp_Event_Func)(struct Computer2 *comp, struct ComputerEvent *cevent,struct Event *event);
typedef long (*Comp_EvntTest_Func)(struct Computer2 *comp, struct ComputerEvent *cevent);

struct Comp_Check_Func_ListItem {
  const char *name;
  Comp_Check_Func func;
};

struct ComputerProcess { // sizeof = 72
  char *name;
  unsigned long field_4;
  unsigned long field_8;
  unsigned long field_C;
  unsigned long field_10;
  unsigned long field_14;
  Comp_Process_Func func_check;
  Comp_Process_Func func_setup;
  Comp_Process_Func func_task;
  Comp_Process_Func func_complete;
  Comp_Process_Func func_pause;
  struct ComputerProcess *parent;
  unsigned long field_30;
  unsigned long field_34;
  unsigned long field_38;
  unsigned long field_3C;
  unsigned long field_40;
  unsigned long field_44;
};

struct ComputerCheck { // sizeof = 32
  char *name;
  unsigned long field_4;
  unsigned long field_8;
  Comp_Check_Func func_check;
  long param1;
  long param2;
  long param3;
  long param4;
};

struct ComputerEvent { // sizeof = 44
  char *name;
  unsigned long field_4;
  unsigned long field_8;
  Comp_Event_Func func_event;
  Comp_EvntTest_Func func_test;
  long field_14;
  struct ComputerProcess *process;
  long param1;
  long param2;
  long param3;
  long param4;
};

struct ComputerProcessTypes { // sizeof = 1124
  char *name;
  long field_4;
  long field_8;
  long field_C;
  long field_10;
  long field_14;
  long field_18;
  long field_1C;
  struct ComputerProcess *processes[COMPUTER_PROCESSES_COUNT];
  struct ComputerCheck checks[COMPUTER_CHECKS_COUNT];
  struct ComputerEvent events[COMPUTER_EVENTS_COUNT];
  long field_460;
};

struct ValidRooms { // sizeof = 8
  long rkind;
  struct ComputerProcess *process;
};

struct ComputerProcessMnemonic {
  char name[16];
  struct ComputerProcess *process;
};

struct ComputerCheckMnemonic {
  char name[16];
  struct ComputerCheck *check;
};

struct ComputerEventMnemonic {
  char name[16];
  struct ComputerEvent *event;
};

struct ComputerTask { // sizeof = 148
  unsigned char field_0[140];
  unsigned short field_8C;
  unsigned char field_8E[5];
  unsigned char field_93;
};

struct Comp2_UnkStr1 { // sizeof = 394
  unsigned char field_0[6];
  unsigned long field_6;
  unsigned char field_A[380];
  unsigned long field_186;
};

struct Computer2 { // sizeof = 5322
  long field_0;
  unsigned long field_4;
  unsigned long field_8;
  unsigned long field_C;
  unsigned long field_10;
  unsigned long field_14;
  unsigned long field_18;
  unsigned long field_1C;
  unsigned long field_20;
  void *field_24;
  unsigned long model;
  unsigned long field_2C;
  unsigned long field_30;
  unsigned long field_34;
  struct ComputerProcess processes[COMPUTER_PROCESSES_COUNT+1];
  struct ComputerCheck checks[COMPUTER_CHECKS_COUNT];
  struct ComputerEvent events[COMPUTER_EVENTS_COUNT];
  struct Comp2_UnkStr1 unkarr_A10[5];
  unsigned char field_11C2[446];
  unsigned char field_1380[128];
  unsigned char field_1400[196];
  short field_14C4;
  long field_14C6;
};

#ifdef __cplusplus
#pragma pack()
#endif

/******************************************************************************/
extern unsigned short computer_types[];
/******************************************************************************/
DLLIMPORT struct ComputerProcessTypes _DK_ComputerProcessLists[14];
//#define ComputerProcessLists _DK_ComputerProcessLists
/******************************************************************************/
void shut_down_process(struct Computer2 *comp, struct ComputerProcess *process);
void reset_process(struct Computer2 *comp, struct ComputerProcess *process);
/******************************************************************************/
long set_next_process(struct Computer2 *comp);
void computer_check_events(struct Computer2 *comp);
long process_checks(struct Computer2 *comp);
long process_tasks(struct Computer2 *comp);
long get_computer_money_less_cost(struct Computer2 *comp);
/******************************************************************************/
void setup_a_computer_player(unsigned short plyridx, long comp_model);
void process_computer_players2(void);
short load_computer_player_config(void);
void setup_computer_players2(void);
/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif