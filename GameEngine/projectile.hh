#ifndef _PROJECTILE_H_
#define _PROJECTILE_H_

#include <cmath>
#include <list>
using std::list;
#include <map>
using std::map;
using std::pair;
#include <fstream>
using std::ifstream;
#include <stdexcept>

#include "bang.hh"
#include "effect.hh"
#include "actor.hh"

class Projectile : public Bang{
protected:
   unsigned timeout;
   string hitBang, expireBang;
   unsigned hitAmount, expireAmount;
   bool alive;
   double friction;
   bool removeOnHit;
   
   void hit();
   void expire();
   void explode(string, unsigned amount);
public:
   Projectile(double x, double y, unsigned r, Drawable_Type img, int team, double dx, double dy, unsigned timeout, string hitBang, string expBang, unsigned hitAmount, unsigned expireAmount, double friction, bool removeOnHit) : 
	  Bang(x, y, r, dx, dy, img, team), timeout(timeout), hitBang(hitBang), expireBang(expBang), hitAmount(hitAmount), expireAmount(expireAmount), alive(true), friction(friction), removeOnHit(removeOnHit)
	  {}
   /*!
	 \return true unless projectile must be destroyed.
   */
   bool tick();
};
   
class ProjectileFactory: public BangFactory{
   static map<string, ProjectileFactory*> projectileFactories;
   double minSpeed;
   double maxSpeed;
   unsigned minTimeout, maxTimeout;
   string hitBang, expireBang;
   unsigned hitAmount, expireAmount;
   double friction;
   bool removeOnHit;
   static ProjectileFactory* load(string name);
   Bang* make(double x, double y, double angle, int team);
public:
   static ProjectileFactory* getProjectileFactory(string name);
   ProjectileFactory(Drawable_Type img, unsigned r, double speed, double maxSpeed, unsigned minTimeout, unsigned maxTimeout, string hitProj, string expireProj, unsigned hitAmount, unsigned expireAmount, double friction, bool removeOnHit) : BangFactory(r, img), minSpeed(speed), maxSpeed(maxSpeed), minTimeout(minTimeout), maxTimeout(maxTimeout), hitBang(hitProj), expireBang(expireProj), hitAmount(hitAmount), expireAmount(expireAmount), friction(friction), removeOnHit(removeOnHit) {}
   static void create(string name, double x, double y, double angle, int team);
};

#endif /* _PROJECTILE_H_ */
