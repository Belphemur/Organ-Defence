#ifndef _CONSTS_H_
#define _CONSTS_H_
#include <climits>
#include <string>

const std::string DATA_DIR = "data/";

const int PLAYER_FIGHTER = 0;
const int PLAYER_HEALER = 1;
const int PLAYER_BUILDER = 2;

const int MONSTER_TEAM = 0;
const int PLAYER_TEAM = 1;

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 800;

const double MAP_WIDTH = 2000;
const double MAP_HEIGHT = 1500;

const unsigned PLAYER_R = 30;
const unsigned PLAYER_HEALTH = 100;
const double TICKS_PER_SECONDS = 20;
const int RESPAWN_TIMEOUT = 100;
const unsigned DEATH_PENALTY = 50;

const int POISON_FREQ = 10;		// poisoned actors will be hurt once in P_F frames
const int MONSTER_DISP = 100; 	// displacement between monsters in a new wave

const unsigned ORGAN_R = 90;
const int ORGAN_HEALTH = 1600;

const double ORBIT_DIST_L = 250;
const double ORBIT_DIST_H = 400;

const int MAX_PLAYERS = 4;

#endif /* _CONSTS_H_ */
