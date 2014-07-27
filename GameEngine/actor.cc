#include "actor.hh"
#include "weapon.hh"
#include <algorithm>
using std::min;
using std::max;
#include "monster.hh"
#include "tower.hh"

Actor:: ~Actor(){
   delete currentWeapon;
}


bool Actor::canShoot() const{
   return currentWeapon && currentWeapon->canShoot();
}

void Actor::shoot(double angle, bool ignoreReload){
	if (ignoreReload or canShoot()){
	  int d = getR() + 10 + 1;	// assumes proj. radius of 10 :S, todo: use real projectile radius
	  int x = getX() + d * cos(angle);
	  int y = getY() + d * sin(angle);
	  currentWeapon->shoot(angle, x, y);
   }
}

bool Actor::tick(){
   if (poisonAmount){
	  --poisonAmount;
	  if (!(poisonAmount % POISON_FREQ)) 
		 hurt(1);
   }
   if (slowness)
	  --slowness;
   move();
   return true;
}

void Actor::hurt(int damageAmount){
   if (isAlive()){
	  health -= damageAmount;
	  if (health <= 0)
		 die();
	  else if (health > maxHealth)
		 health = maxHealth;
	  healthHasChanged = true;
   }
}

void Actor::setHealth(int health){
   this->health = health;
   healthHasChanged = true;
}

int Actor::getHealthIfChanged(){
   if (healthHasChanged){
	  healthHasChanged = false;
	  return getHealth();
   }
   return -1;
}

void Actor::poison(int poisonAmount){
   this->poisonAmount += poisonAmount;
}

void Actor::slow(int slowingAmount){
   setDx(0);
   setDy(0);
   slowness += slowingAmount;
}

void Actor::move(){
   double tx = getX() + getDx();
   double ty = getY() + getDy();
   if (inGameArea() && !inGameArea(tx, ty, getR())){
	  setDy(0);
	  setDx(0);
   }
   TwoDObject::move();
}

void Actor::move(double dx, double dy){
   if (slowness > 0){
	  dx /= 2;
	  dy /= 2;
   }
   double tx = getX() + dx;
   double ty = getY() + dy;
   if (inGameArea() && !inGameArea(tx, ty, getR())){
	  setDy(0);
	  setDx(0);
   }
   else{
	  setDx(dx);
	  setDy(dy);
   }
}

//===================================================================

void ActorFactory::create(string name, double x, double y){
   if (!name.empty()){
	  string actor = name.substr(0, name.length() - 2); // todo: write clean version :P
	  string last = name.substr(name.length() - 1);
	  if (last == "t")
		 TowerFactory::create(actor, x, y);
	  else if (last == "m")
		 MonsterFactory::create(actor, x, y);
	  else
		 throw std::runtime_error("unknown actor type: " + name);
   }
}
