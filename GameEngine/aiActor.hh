#ifndef _AIMONSTER_H_
#define _AIMONSTER_H_

#include "actor.hh"

class Brain;


class AiActor : public Actor{
   Brain* brain;
public:
   AiActor(Drawable_Type img, double x, double y, unsigned r, int team, short health, Weapon* w, double speed) :
	  Actor(img, x, y, r, team, health, speed), brain(NULL) {
	  setWeapon(w);
   }
   bool tick();
   void assignBrain(Brain* b) {brain = b;}
   virtual ~AiActor();
};

#endif /* _AIMONSTER_H_ */
