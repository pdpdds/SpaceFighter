#pragma once

/////////////////////////////////////////////////////////////////////////////////////////
//Resource
/////////////////////////////////////////////////////////////////////////////////////////
#define STAR_SAIL 1
#define EXPLSION_EFFECT_1 2
#define HERO_PLANE			3
#define EXPLSION_EFFECT_2 4
#define EXPLSION_EFFECT_3 5
#define MISAILE_1 6
#define MISAILE_2 7
#define PLANE_1	  8
#define PLANE_2 9
#define PLANE_3 10
#define PLANE_4 11
#define ITEM_BOX 12


/////////////////////////////////////////////////////////////////////////////////////////
//Weapon Identifer
/////////////////////////////////////////////////////////////////////////////////////////
#define WEAPON_DEFAULT 0
#define WEAPON_TWO  1
#define WEAPON_THREE 3
#define WEAPON_FIREWORK 2
#define WEAPON_INDUCE  4//유도 미사일

/////////////////////////////////////////////////////////////////////////////////////////
//Object Type
/////////////////////////////////////////////////////////////////////////////////////////
#define OBJECT_PLAYER  0
#define OBJECT_ENEMY_GENERAL 1
#define OBJECT_BOMB 2
#define OBJECT_BOSS 3
#define OBJECT_TRIGGER 4
#define OBJECT_WEAPON  5
#define OBJECT_ALL  6

/////////////////////////////////////////////////////////////////////////////////////////
//SF Message
/////////////////////////////////////////////////////////////////////////////////////////
#define MSG_PLANE_DIE 1
#define MSG_PLAYER_DIE 2
#define MSG_PLAYER_LIVE 3


/////////////////////////////////////////////////////////////////////////////////////////
//SF GameState
/////////////////////////////////////////////////////////////////////////////////////////

enum eSFGameState
{
	SF_GAMESTATE_INIT = 0,
	SF_GAMESTATE_INTRO,
	SF_GAMESTATE_TITLE,
	SF_GAMESTATE_PROLOGUE,
	SF_GAMESTATE_PLAY,
	SF_GAMESTATE_ENDING,
	SF_GAMESTATE_OPTION,
};

enum eSFPlayerState
{
	SF_PLAYERSTATE_INIT = 0,
	SF_PLAYERSTATE_LIVE,
	SF_PLAYERSTATE_DIE,
};
