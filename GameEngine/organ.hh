#ifndef _ORGAN_H_
#define _ORGAN_H_

#include "actor.hh"
#include "consts.hh"
#include <cstdlib>

class PlayState;

class Organ : public Actor{
   void die();
public:
   Organ() : Actor(TYPE_ORGAN_HEART, MAP_WIDTH / 2, MAP_HEIGHT / 2, ORGAN_R, PLAYER_TEAM, ORGAN_HEALTH, 0){
	  if(!(rand() % 3))
		 setType(TYPE_ORGAN_LIVER);
	  if(!(rand() % 3))
		 setType(TYPE_ORGAN_LUNG);
   } 
   bool tick();
};

#endif /* _ORGAN_H_ */
