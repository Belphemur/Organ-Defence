#include "brain.hh"
#include "actor.hh"
#include "playstate.hh"
#include "player.hh"
#include "util.hh"
#include <cstdlib>
#include "weapon.hh"


class Player;

void Brain::moveTowards(Actor* a){
   moveTowards(a->getX(), a->getY());
}

void Brain::moveTowards(double x, double y){
   double dx = x - body->getX();
   double dy = y - body->getY();
   double dist = sqrt((dx*dx) +(dy*dy));
   dx /= dist;
   dy /= dist;
   dx *= body->getSpeed();
   dy *= body->getSpeed();
   body->move(dx, dy);
}

Actor* Brain::getCloseTarget(int targetTeam, int maxDist){
   Actor* target = NULL;
   int bestDist = maxDist;
   list<Actor*> actors = PlayState::getInstance()->getActorList();
   for (list<Actor*>::iterator i = actors.begin(); i != actors.end(); ++i){
	  if ((*i)->getTeam() == targetTeam && (*i) != body){
		 if (body->dist(*i) < bestDist){
			bestDist = body->dist(*i);
			target = *i;
		 }
	  }
   }
   return target;
}

Player* Brain::getRandomPlayer(){
   list<Player*> players = PlayState::getInstance()->getPlayers();
   list<Player*>::iterator j = players.begin();
   for (int i = 0; i < randBetween(0,players.size()-1); ++i) 
	  ++j;
   return *j;
}

void Brain::shoot(Actor* a){
   if (a)
	  body->shoot(atan2(a->getY() - body->getY(), a->getX() - body->getX()));
}

bool Brain::canShoot() const{
   return body->canShoot() && (rand()%3); // extra salt so all the monsters won't shoot at the exact same time
}

//******************************************************************************

void OrbiterBrain::doSomething(){
   PlayState* ps = PlayState::getInstance();
   Actor* a = ps->getOrgan();
   if (!arrived && body->dist(a) > orbitDistLow)
	  moveTowards(a);
   else if (!arrived){
	  arrived = true;
   }
   else{
	  if (body->dist(a) > orbitDistHigh)
		 arrived = false;
	  double dx = a->getX() - body->getX();
	  double dy = a->getY() - body->getY();
	  moveTowards(body->getX() - dy, body->getY() + dx);
	  if (canShoot())
		 shoot(a);
   }
}

//******************************************************************************

void WandererBrain::newTarget(){
   targetX = randBetween(50, MAP_WIDTH - 50);
   targetY = randBetween(50, MAP_HEIGHT - 50);
}

void WandererBrain::doSomething(){
   if (abs(static_cast<int>(body->getX() - targetX)) < 10 && abs(static_cast<int>(body->getY() - targetY)) < 10)
	  newTarget();
   moveTowards(targetX, targetY);
   if (canShoot())
	  shoot(getCloseTarget(!body->getWeapon()->getTeam()));
}

//******************************************************************************

void StalkerBrain::doSomething(){
   list<Player*> players = PlayState::getInstance()->getPlayers();
   if (nbPlayers != players.size()){
	  nbPlayers = players.size();
	  target = getRandomPlayer();
   }
   if (!target->isAlive())
	  target = getRandomPlayer();
   if (body->dist(target) > maxDist)
	  moveTowards(target->getX() + xOffset, target->getY() + yOffset);
   else
	  body->move(0, 0);
   if(canShoot())
	  shoot(target);
}

//******************************************************************************

void TowerBrain::doSomething(){
   if (canShoot())
      shoot(getCloseTarget(!body->getWeapon()->getTeam()));
}

//******************************************************************************

void FakePlayerBrain::doSomething(){
   /*if (!(--moveCount)){
	  moveCount = randBetween(20, 80);
	  int d = rand() % 6;
	  int dir = DIR_RIGHT;
	  if (d == 0)
		 dir = DIR_UP;
	  else if (d == 1)
		 dir = DIR_LEFT;
	  else if (d == 2)
		 dir = DIR_DOWN;
	  else if (d == 3)
		 dir = DIR_NOX;
	  else if (d == 4)
		 dir = DIR_NOY;
	  static_cast<Player*>(body)->move(dir);			// note: works because only aiplayers use this brain
	   */
		
		if (!(--moveCount)){
		moveCount = randBetween(20, 80);
		double oX = PlayState::getInstance()->getOrgan()->getX();
		double oY = PlayState::getInstance()->getOrgan()->getY();
		int test = rand() % 2;
		if (test == 0)
		moveTowards(oX, body->getY());
		else
		moveTowards(body->getX(), oY);
		
		

		
	
   }
   if (canShoot())
	  shoot(getCloseTarget(!body->getWeapon()->getTeam(), 500));
}
