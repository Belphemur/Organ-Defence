#ifndef _TOWER_H_
#define _TOWER_H_

#include "aiActor.hh"
#include <map>
using std::map;

class Tower : public AiActor{
   unsigned timeout;
public:
   Tower(Drawable_Type img, double x, double y, unsigned r, short health, Weapon* w, int timeout) :
	  AiActor(img, x, y, r, PLAYER_TEAM, health, w, 0), timeout(timeout) {}
   virtual ~Tower() {}
   bool tick();
};


class TowerFactory : public ActorFactory{
   static map<string, TowerFactory*> towerFactories;
   unsigned timeout;
   void make(double x, double y);
   static TowerFactory* load(string name);
public:
   TowerFactory(unsigned r, Drawable_Type img, string weaponName, short health, string brainName, int timeout) :
	  ActorFactory(r, img, PLAYER_TEAM, weaponName, health, 0, brainName), timeout(timeout) {}
   static void create(string name, double x, double y);
};

#endif /* _TOWER_H_ */
