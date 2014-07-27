#ifndef _MONSTER_H_
#define _MONSTER_H_

#include "aiActor.hh"
#include "consts.hh"
#include <stdexcept>
#include <map>
using std::map;
using std::pair;
#include <string>
using std::string;

class Weapon;
class Brain;
class PlayState;

class Monster : public AiActor{
   int value;
public:
   Monster(Drawable_Type img, double x, double y, unsigned r, short health, Weapon* w, double speed, int value) : 
	  AiActor(img, x, y, r, MONSTER_TEAM, health, w, speed), value(value) {}
   virtual void die();
   virtual ~Monster() {}
   
};

class MonsterFactory : public ActorFactory{
   int value;
   static map<string, MonsterFactory*> monsterFactories;
   static MonsterFactory* load(string);
   void make(double x, double y);
   MonsterFactory(unsigned r, Drawable_Type img, int team, string weaponName, int health, double speed, string brainName, int value) :
	  ActorFactory(r, img, team, weaponName, health, speed, brainName), value(value) {}
public:
   static void create(string, double x, double y);
};

#endif /* _MONSTER_H_ */
