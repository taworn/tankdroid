/**
 * @file common.cxx
 * @desc Common game objects.
 */
#ifndef GAME_COMMON_HXX
#define GAME_COMMON_HXX

#define STAGE_COUNT 4
#define EAGLE_HP 8
#define ITEM_DROP_RATE 3
#define ITEM_GUN_TIME 10000
#ifdef __ANDROID__
#define TANK_HERO_START_HP 15
#else
#define TANK_HERO_START_HP 10
#endif

#include "movable.hxx"
#include "tank.hxx"
#include "tankhero.hxx"
#include "bullet.hxx"
#include "item.hxx"
#include "map.hxx"
#include "arena.hxx"

#endif // GAME_COMMON_HXX

