#ifndef _ACTOR_H_
#define _ACTOR_H_

#include <list>
using std::list;
#include "2dObj.hh"
#include <string>
using std::string;

class Weapon;

class Actor : public TwoDObject{
protected:
   int team;
   Weapon* currentWeapon;
   short health;
   short maxHealth;
   int slowness;
   int poisonAmount;
   double speed;
   bool healthHasChanged;
   virtual void die() {}
   virtual void setHealth(int);

public:
   Actor(Drawable_Type img, double x, double y, unsigned r, int team, short health, double speed) : TwoDObject(x, y, r, 0, 0, img), team(team), currentWeapon(NULL), health(health), maxHealth(health), slowness(0), poisonAmount(0), speed(speed), healthHasChanged(true) {}
   virtual void setWeapon(Weapon* w) {currentWeapon = w;}
   virtual void shoot(double angle, bool ignoreReload = false);
   virtual bool tick();
   virtual void hurt(int damageAmount);
   virtual void slow(int slowingAmount);
   virtual void poison(int poisonAmount);
   virtual int getTeam() const {return team;}
   virtual void move(double dx, double dy);
   virtual void move();
   virtual double getSpeed() const { return speed;}
   virtual int getHealth() const { return health;}
   virtual bool isAlive() const {return health > 0;}
   virtual int getHealthIfChanged();
   virtual bool canShoot() const;
   virtual ~Actor();
   virtual Weapon* getWeapon() const {return currentWeapon;}
};

class ActorFactory{
protected:
   unsigned r;
   Drawable_Type img;
   int team;
   string weaponName;
   short maxhealth;
   double speed;
   string brainName;
public:
   ActorFactory(unsigned r, Drawable_Type img, int team, string weaponName, short health, double speed, string brainName) : r(r), img(img), team(team), weaponName(weaponName), maxhealth(health), speed(speed), brainName(brainName) {}
   static void create(string name, double x, double y);
};


#endif /* _ACTOR_H_ */
