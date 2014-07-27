#ifndef _WEAPON_H_
#define _WEAPON_H_

#include <string>
using std::string;
#include <map>
using std::map;
using std::pair;
#include <fstream>
using std::ifstream;
#include <stdexcept>

class PlayState;
class BangFactory;

class Weapon{
   unsigned reloadTime;
   string projectileName;
   unsigned amountMin;
   unsigned amountMax;
   double spread;
   long lastShotFrame;
   int team;

public:
   Weapon(unsigned reloadTime, string proj, unsigned amountMin, unsigned amountMax, double spread, int team) :
	  reloadTime(reloadTime), projectileName(proj), amountMin(amountMin), amountMax(amountMax), spread(spread), lastShotFrame(-9999), team(team) {}
   void shoot(double angle, double x, double y);
   bool canShoot() const;
   int getTeam() const {return team;}
};

class WeaponFactory{
   static map<string, WeaponFactory*> weapons;
   static map<string, BangFactory*> bangs;
   static WeaponFactory* load(string name);
   Weapon* make(int team);
   unsigned reloadTime;
   string projectileName;
   unsigned amountMin, amountMax;
   double spread;
   bool healing;
   unsigned price;

   static WeaponFactory* getFactory(string name);
public:
   WeaponFactory(unsigned reloadTime, string proj, unsigned amountMin, unsigned amountMax, double spread, bool healing, unsigned price) :
	  reloadTime(reloadTime), projectileName(proj), amountMin(amountMin),
	  amountMax(amountMax), spread(spread), healing(healing), price(price) {}
   static Weapon* getWeapon(string name, int team);
   static unsigned getWeaponPrice(string name);
};

#endif /* _WEAPON_H_ */
