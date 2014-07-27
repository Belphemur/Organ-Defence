#ifndef _BRAIN_H_
#define _BRAIN_H_

#include "consts.hh"
#include <cstdlib>
#include "util.hh"


class PlayState;
class Actor;
class Player;

class Brain{
protected:
   Actor *body;
   void moveTowards(double x, double y);
   void moveTowards(Actor*);
   Actor* getCloseTarget(int targetTeam, int maxDist=MAP_WIDTH);
   void shoot(Actor*);
   bool canShoot() const;
   Player* getRandomPlayer();
public:
   Brain(Actor *me) : body(me) {}
   virtual void doSomething() = 0;
};

class OrbiterBrain : public Brain{
   double orbitDistLow, orbitDistHigh;
   bool arrived;
public:
   OrbiterBrain(Actor *a) :
	  Brain(a), orbitDistLow(ORBIT_DIST_L + rand() % static_cast<int>(ORBIT_DIST_H - ORBIT_DIST_L)), orbitDistHigh(ORBIT_DIST_H), arrived(false) {}
   void doSomething();
};

class WandererBrain : public Brain{
   int targetX, targetY;
   void newTarget();
public:
   WandererBrain(Actor *a) : Brain(a) {
	  newTarget();}
   void doSomething();
};

class StalkerBrain : public Brain{
   unsigned nbPlayers;
   Actor* target;
   int maxDist;
   int xOffset, yOffset;
public:
   StalkerBrain(Actor *a) : Brain(a), nbPlayers(0), target(NULL), maxDist(randBetween(150, 250)), xOffset(randBetween(-50, 50)), yOffset(randBetween(-50, 50)) {}
   void doSomething();
};

class TowerBrain : public Brain{
public:
   TowerBrain(Actor *a) : Brain(a) {}
   void doSomething();
};

class FakePlayerBrain : public Brain{
   int moveCount;
public:
   FakePlayerBrain(Actor *a) : Brain(a), moveCount(1) {}
   void doSomething();
};

#endif /* _BRAIN_H_ */
