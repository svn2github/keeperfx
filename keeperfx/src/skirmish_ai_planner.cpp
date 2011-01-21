/******************************************************************************/
// Free implementation of Bullfrog's Dungeon Keeper strategy game.
/******************************************************************************/
/** @file skirmish_ai_planner.cpp
 *     Planner/state tree explorer for new AI. Highly experimental, I don't yet
 *     know how it will perform. ;-)
 *     Is .cpp although use of C++ is minimized and can be replaced later.
 * @par Purpose:
 *     Experimental computer player intended to play multiplayer maps better.
 * @par Comment:
 *     None.
 * @author   KeeperFX Team
 * @date     2011
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/

#include "skirmish_ai_planner.h"

#include "config_creature.h"
#include "config_terrain.h"
#include "keeperfx.hpp"

#include <assert.h>
#include <memory.h>
#include <stdlib.h>

//start of C++ dependencies
#include <algorithm>
#include <set>
//end of C++ dependencies

#define MAX_KEEPERS         4

#define PRIORITY_CHANGE_TIME_PER_CREATURE   5 //TODO AI: relate this param to action time to move a creature

#define FLAG_DUNGEON_FORTIFIED  0x1 //dungeon is unbreakable to anyone else
#define FLAG_HEART_ACCESSIBLE   0x2 //from our player
#define FLAG_BATTLE_ACCESSIBLE  0x4 //we can try to provoke a battle because we are in immediate contact
#define FLAG_LIQUID_BLOCKED     0x8 //their dungeon is unreachable from us until we get bridge

//for some reason neither min or max were defined despite including headers that define them...
#ifndef min
#define min(a, b) (a < b? a : b)
#endif

#ifndef max
#define max(a, b) (a > b? a : b)
#endif

struct NodePlayerState
{
    unsigned gold; //in treasury chamber and off map
    unsigned spells_researched; //bitmask; rationale for keeping /player is that there
        //is only one instance - thus we can easily conclude effect of stealing enemy spells
    unsigned creature_level_sum; //for calculating an average combat power /non-worker
        //storing individual levels is too much
    unsigned char creatures[CREATURE_TYPES_COUNT];
    unsigned char flags;
};

struct NodeState
{
    int time;
    struct NodePlayerState players[MAX_KEEPERS];
    unsigned char pool[CREATURE_TYPES_COUNT];
    unsigned rooms_built; //bitmask; for planning player (at least one room of kind built)
    unsigned rooms_available; //bitmask; for planning player (room kind is researched)
    unsigned char creature_prio;
};

struct Node
{
    struct Node * next; //to keep track of nodes allocated

    int id; //required because we can't use pointer as final compare key (order must be deterministic on multiple machines)
    struct Node * parent;
    struct SAI_PlanDecision decision;
    struct NodeState state;
};

struct PlayerEnvironment
{
    unsigned spells_researchable; //bitmask; so we can use bool ops with spells_researched
};

struct Environment
{
    struct PlayerEnvironment players[MAX_KEEPERS];
    unsigned rooms_researchable; //bitmask; so we can use bool ops with rooms_built/rooms_available bitmasks
};

static bool search_compare(const Node * lhs, const Node * rhs);

typedef bool (*NodeCompareFunc)(const Node * lhs, const Node * rhs);
typedef std::set<Node *, NodeCompareFunc> NodeSet;

struct Planner
{
    int my_plyr_idx; //for convenience when looking through pointer
    int next_node_id;
    enum SAI_PlanType plan_type;
    struct Environment env; //precompiled variables not changing on search (for efficiency)
    struct Node * root;

    //rewrite using some C library later if C++ dependency becomes an issue
    NodeSet open;

    struct Node * freelist_head;
    struct Node * freelist_tail;
    struct Node * livelist_head;
    struct Node * livelist_tail;

    Planner(int plyr_idx, NodeCompareFunc cmp) : my_plyr_idx(plyr_idx),
        next_node_id(0), env(), open(cmp),
        freelist_head(), freelist_tail(), livelist_head() {
    }
};

static struct Planner planners[MAX_KEEPERS] = {
    Planner(0, search_compare),
    Planner(1, search_compare),
    Planner(2, search_compare),
    Planner(3, search_compare),
};

static struct Planner * planner;

static const char plan_names[][32] = {
    "Wait",
    "Take Room",
    "Build Room",
    "Hide",
    "Launch Assault",
    "Launch Armageddon",
    "Prioritize"
};


static int calc_attack_power(const struct NodeState * state, int plyr_idx)
{
    //TODO AI: add some kind of way to judge creature types
    //TODO AI: add effect of power spells and access to gold to fuel those spells
    return state->players[plyr_idx].creature_level_sum;
}

static int calc_attack_balance(const struct NodeState * state)
{
    int i;
    int my_attack;
    int best_enemy_attack;

    my_attack = calc_attack_power(state, planner->my_plyr_idx);

    best_enemy_attack = 0;
    for (i = 0; i < MAX_KEEPERS; ++i) {
        if (i != planner->my_plyr_idx && player_exists(get_player(i))) {
            best_enemy_attack = max(best_enemy_attack, calc_attack_power(state, i));
        }
    }

    return my_attack - best_enemy_attack;
}

static int node_score(const struct Node * node)
{
    int score;
    score = 0;

    score += calc_attack_balance(&node->state) * 100;

    return score;
}

static int node_heuristic(const struct Node * node)
{
    int score;
    score = 0;

    score -= node->state.time;

    if (node->decision.type == SAI_PLAN_WAIT) {
        score -= 1; //give preference to non-wait nodes
    }

    return score;
}

static bool search_compare(const Node * lhs, const Node * rhs)
{
    int diff;

    diff = node_score(lhs) + node_heuristic(lhs) -
        (node_score(rhs) + node_heuristic(rhs));
    if (diff != 0) {
        return diff < 0;
    }

    return (lhs->id - rhs->id) < 0; //for multi-machine determinism
}

static bool score_compare(const Node * lhs, const Node * rhs)
{
    int diff;
    diff = node_score(lhs) - node_score(rhs);
    if (diff != 0) {
        return diff < 0;
    }

    return (lhs->id - rhs->id) < 0;
}

void state_time_simulation(struct NodeState * node, int time)
{
}

static int is_room_usable(struct Room * room)
{
    return 1; //TODO AI: implement. should check capacity or some such
}

static int count_workers(struct NodePlayerState * state, int plyr_idx)
{
    return state->creatures[get_players_special_digger_breed(plyr_idx)];
}

static int count_non_workers(struct NodePlayerState * state, int plyr_idx)
{
    int count, i;
    int worker_breed;

    worker_breed = get_players_special_digger_breed(plyr_idx);
    count = 0;

    for (i = 0; i < CREATURE_TYPES_COUNT; ++i) {
        if (i != worker_breed) {
            count += state->creatures[i];
        }
    }

    return count;
}

static void prepare_creature_pool_state(struct NodeState * state)
{
    int i;

    for (i = 0; i < CREATURE_TYPES_COUNT; ++i) {
        state->pool[i] = (unsigned char) min(0xFF, game.pool.crtr_kind[i]);
    }
}

static void prepare_planning_player_state(struct NodeState * state)
{
    int i, j;
    struct PlayerInfo * plyr;
    struct Dungeon * dungeon;
    struct Room * room;

    plyr = get_player(planner->my_plyr_idx);
    dungeon = get_players_dungeon(plyr);

    //look up rooms
    for (i = 0; i < ROOM_TYPES_COUNT; ++i) {
        for (j = dungeon->room_kind[i]; j != 0; j = room->next_of_owner) {
            room = room_get(i);
            if (room_is_invalid(room)) {
                continue;
            }

            if (is_room_usable(room)) {
                state->rooms_built |= 1 << i;
                break;
            }
        }

        if (is_room_available(planner->my_plyr_idx, i)) {
            state->rooms_available |= 1 << i;
        }
    }
}

static void prepare_player_environment(struct PlayerEnvironment * state, int plyr_idx)
{
    int i;
    struct PlayerInfo * plyr;
    struct Dungeon * dungeon;

    plyr = get_player(plyr_idx);
    dungeon = get_players_dungeon(plyr);

    //look up spells
    for (i = 0; i < KEEPER_SPELLS_COUNT; ++i) {
        if (dungeon->magic_resrchable[i]) {
            state->spells_researchable |= 1 << i;
        }
    }
}

static void prepare_player_state(struct NodePlayerState * state, int plyr_idx)
{
    int i;
    struct PlayerInfo * plyr;
    struct Dungeon * dungeon;
    struct CreatureControl * cctrl;
    struct Thing * thing;

    plyr = get_player(plyr_idx);
    dungeon = get_players_dungeon(plyr);

    //state->gold = dungeon-> //TODO AI: reverse draw_gold_total or check_map_for_gold to find out how to calc total gold

    //look up creatures
    for (i = dungeon->creatr_list_start; i != 0; i = cctrl->next_in_group) {
        thing = thing_get(i);
        cctrl = creature_control_get_from_thing(thing);
        if (thing_is_invalid(thing) || creature_control_invalid(cctrl)) {
            continue;
        }

        assert(thing->model >= 0);
        assert(thing->model < CREATURE_TYPES_COUNT);

        state->creatures[thing->model] += 1;

        if (get_players_special_digger_breed(plyr_idx) != thing->model) {
            state->creature_level_sum += cctrl->explevel;
        }
    }

    //look up spells
    for (i = 0; i < KEEPER_SPELLS_COUNT; ++i) {
        if (is_power_available(plyr_idx, i)) {
            state->spells_researched |= 1 << i;
        }
    }
}

static void prepare_environment(struct Environment * state)
{
    int i;
    struct PlayerInfo * plyr;
    struct Dungeon * dungeon;

    plyr = get_player(planner->my_plyr_idx);
    dungeon = get_players_dungeon(plyr);

    //look up rooms
    for (i = 0; i < ROOM_TYPES_COUNT; ++i) {
        if (dungeon->room_resrchable[i]) {
            state->rooms_researchable |= 1 << i;
        }
    }

    //all players
    for (i = 0; i < MAX_KEEPERS; ++i) {
        if (player_exists(get_player(i))) {
            prepare_player_environment(&state->players[i], i);
        }
    }
}

static int estimate_room_build_time(int kind)
{
    //TODO AI: interface with other parts of AI instead to give an more realistic value
    return 1000; //40 s AFAIK
}

static int estimate_room_research_time(int kind, struct NodeState * state)
{
    //TODO AI: check from research values in config, our existing creatures, and research costs
    return 1000;
}

static Node * new_node(enum SAI_PlanDecisionType type, struct Node * parent, int time)
{
    struct Node * node;

    if (planner->freelist_head) {
        node = planner->freelist_head;
        planner->freelist_head = node->next;

        if (!planner->freelist_head) {
            planner->freelist_tail = NULL;
        }
    }
    else {
        node = (Node *) malloc(sizeof(*node));
    }

    node->next = planner->livelist_head;
    planner->livelist_head = node;
    if (!planner->livelist_tail) {
        planner->livelist_tail = node;
    }

    node->decision.type = type;
    node->id = planner->next_node_id++;
    node->parent = parent;

    if (parent) {
        memcpy(&node->state, &parent->state, sizeof(node->state));

        if (time > 0) {
            state_time_simulation(&node->state, time);
        }
    }
    else {
        memset(&node->state, 0, sizeof(node->state));
    }

    return node;
}

/**
 * Compiles the game state to a plan state and sets it as the root.
 */
static void insert_root_node(void)
{
    struct Node * root;
    int i;

    root = new_node(SAI_PLAN_WAIT, NULL, 0);

    prepare_creature_pool_state(&root->state);
    prepare_planning_player_state(&root->state);

    for (i = 0; i < MAX_KEEPERS; ++i) {
        if (player_exists(get_player(i))) {
            prepare_player_state(&root->state.players[i], i);
        }
    }

    planner->open.insert(root);
    planner->root = root;
}

static void insert_wait_node(struct Node * parent)
{
    struct Node * wait;

    wait = new_node(SAI_PLAN_WAIT, parent, 0);

    planner->open.insert(wait);
}

static void insert_build_room_node(struct Node * parent, int kind)
{
    struct Node * build;
    int time;

    time = estimate_room_build_time(kind);
    if (parent->decision.type == SAI_PLAN_WAIT) {
        time += estimate_room_research_time(kind, &parent->state);
    }

    build = new_node(SAI_PLAN_BUILD_ROOM, parent, time);
    build->decision.param.kind = kind;
    build->state.rooms_built |= (1 << kind);

    planner->open.insert(build);
}

static void insert_creature_prioritize_node(struct Node * parent,
    enum SAI_CreaturePriority cp)
{
    struct Node * prio;
    int time;
    int nbr;

    nbr = count_non_workers(&parent->state.players[planner->my_plyr_idx],
            planner->my_plyr_idx);
    time = PRIORITY_CHANGE_TIME_PER_CREATURE * nbr;

    prio = new_node(SAI_PLAN_CREATURE_PRIORITIZE, parent, time);
    prio->decision.param.cp = cp;

    planner->open.insert(prio);
}

static void visit_node(struct Node * node)
{
    int i;
    int can_build;
    int can_wait_to_research;

    //AIDBG(6, "Visiting node of type %i", node->decision.type);

    //Generate children:

    //1) we can always wait if we haven't waited (a wait node following a wait node has no purpose)
    if (node->decision.type != SAI_PLAN_WAIT) {
        insert_wait_node(node);
    }

    //2) room build nodes
    can_wait_to_research = node->decision.type == SAI_PLAN_WAIT &&
        node->state.rooms_built & (1 << RoK_LIBRARY)?
            planner->env.rooms_researchable : 0;
    can_build = ~node->state.rooms_built & (node->state.rooms_available | can_wait_to_research);
    for (i = 0; i < ROOM_TYPES_COUNT; ++i) {
        if (can_build & (1 << i)) {
            insert_build_room_node(node, i);
        }
    }

    //3 setting creature work priorities
    if (node->decision.type != SAI_PLAN_CREATURE_PRIORITIZE) {
        if (node->state.creature_prio != SAI_CP_SAVE_MONEY) {
            insert_creature_prioritize_node(node, SAI_CP_SAVE_MONEY);
        }
        if (node->state.creature_prio != SAI_CP_TRAIN &&
                node->state.rooms_built & (1 << RoK_TRAINING)) {
            insert_creature_prioritize_node(node, SAI_CP_TRAIN);
        }
        if (node->state.creature_prio != SAI_CP_RESEARCH &&
                node->state.rooms_built & (1 << RoK_LIBRARY)) {
            insert_creature_prioritize_node(node, SAI_CP_RESEARCH);
        }
        if (node->state.creature_prio != SAI_CP_MANUFACTURE &&
                node->state.rooms_built & (1 << RoK_WORKSHOP)) {
            insert_creature_prioritize_node(node, SAI_CP_MANUFACTURE);
        }
        if (node->state.creature_prio != SAI_CP_SCAVENGE &&
                node->state.rooms_built & (1 << RoK_SCAVENGER)) {
            insert_creature_prioritize_node(node, SAI_CP_SCAVENGE);
        }
    }
}

static const char * decision_param_string(struct SAI_PlanDecision * decision)
{
    switch (decision->type) {
    case SAI_PLAN_TAKE_ROOM:
    case SAI_PLAN_BUILD_ROOM:
        return room_code_name(decision->param.kind);
    case SAI_PLAN_CREATURE_PRIORITIZE:
        switch (decision->param.kind) {
        case SAI_CP_SAVE_MONEY:
            return "Saving Money";
        case SAI_CP_TRAIN:
            return "Training";
        case SAI_CP_RESEARCH:
            return "Research";
        case SAI_CP_MANUFACTURE:
            return "Manufacturing";
        case SAI_CP_SCAVENGE:
            return "Scavenging";
        }
    default:
        return "(N/A)";
    }
}

static int eval_least_score_on_path(const Node * leaf)
{
    int least;

    least = INT_MAX;

    do {
        least = min(least, node_score(leaf));
        leaf = leaf->parent;
    } while (leaf);

    return least;
}

static void set_planning_player(int plyr)
{
    assert(plyr >= 0);
    assert(plyr < MAX_KEEPERS);
    planner = &planners[plyr];
}

void SAI_begin_plan(int plyr, enum SAI_PlanType type)
{
    AIDBG(3, "Starting");
    set_planning_player(plyr);

    assert(planner->open.empty());
    assert(planner->livelist_head == NULL);

    planner->next_node_id = 0;
    planner->plan_type = type;
    memset(&planner->env, 0, sizeof(planner->env));
    prepare_environment(&planner->env);

    insert_root_node();
}

void SAI_process_plan(int plyr, int node_budget)
{
    struct Node * node;
    int node_count_at_start;

    AIDBG(3, "Starting");
    set_planning_player(plyr);
    node_count_at_start = planner->next_node_id;

    while (!planner->open.empty() &&
            planner->next_node_id - node_count_at_start < node_budget) {
        node = *planner->open.begin();
        planner->open.erase(planner->open.begin());
        //node does not get lost here, it is still on planner livelist
        visit_node(node);
    }
}

void SAI_end_plan(int plyr, struct SAI_PlanDecision ** decisions, int * num_decisions)
{
    struct Node * node;
    struct Node * best_leaf;
    NodeSet::iterator it;
    int i;
    int score;
    int best_score;

    AIDBG(3, "Starting, %i nodes will be considered", planner->next_node_id);
    set_planning_player(plyr);

    *num_decisions = 0;
    *decisions = NULL;
    best_leaf = NULL;
    assert(!planner->open.empty());

    //find best leaf (according to type of plan)
    NodeSet score_order(score_compare);
    score_order.insert(planner->open.begin(), planner->open.end());

    if (planner->plan_type == SAI_PLAN_MOST_REWARDING) {
        AIDBG(4, "Looking for most rewarding plan", planner->open.size());
        best_leaf = *score_order.begin();
    }
    else if (planner->plan_type == SAI_PLAN_LEAST_RISKY) {
        AIDBG(4, "Looking for least risky plan", planner->open.size());
        best_leaf = *score_order.begin();
        best_score = eval_least_score_on_path(best_leaf);

        for (it = ++score_order.begin(); it != score_order.end(); ++it) {
            node = *it;

            if (node_score(node) <= best_score) {
                break; //we can't get a better score now (recall the set is ordered to lower scores)
            }

            score = eval_least_score_on_path(node);
            if (score > best_score) {
                best_leaf = node;
                best_score = score;
            }
        }
    }
    else {
        ERRORLOG("Bad plan type %i", (int) planner->plan_type);
    }

    if (best_leaf) {
        //find number of decisions for best decision path
        AIDBG(4, "Extracting plan");
        for (node = best_leaf; node != planner->root; node = node->parent) {
            *num_decisions += 1;
        }

        //extract best decision path
        *decisions = (struct SAI_PlanDecision *) malloc(sizeof(**decisions) * *num_decisions);
        for (i = *num_decisions - 1, node = best_leaf; i >= 0; --i) {
            memcpy(*decisions + i, &node->decision, sizeof(node->decision));
            node = node->parent;
        }

        //debug print path
        AIDBG(5, "First 100 decisions of plan (or less):");
        for (i = 0; i < *num_decisions && i < 100; ++i) {
            AIDBG(5, "Plan decision: %s %s", plan_names[(*decisions)[i].type],
                        decision_param_string(*decisions + i));
        }
    }


    planner->open.clear();

    //transfer nodes to freelist
    if (planner->freelist_tail) {
        planner->freelist_tail->next = planner->livelist_head;
    }
    else {
        planner->freelist_head = planner->livelist_head;
        planner->freelist_tail = planner->livelist_tail;
    }

    planner->livelist_head = NULL;
    planner->livelist_tail = NULL;
}

void SAI_destroy_plan(int plyr)
{
    struct Node * node;

    AIDBG(3, "Starting");
    set_planning_player(plyr);

    planner->open.clear();

    //destroy nodes
    while (planner->freelist_head) {
        node = planner->freelist_head;
        planner->freelist_head = planner->freelist_head->next;
        free(node);
    }
    while (planner->livelist_head) {
        node = planner->livelist_head;
        planner->livelist_head = planner->livelist_head->next;
        free(node);
    }
    planner->freelist_tail = NULL;
    planner->livelist_tail = NULL;
}