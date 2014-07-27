#include "projectile.hh"
#include "playstate.hh"
#include "util.hh"
#include <cstdlib>

bool Projectile::tick(){
   if (--timeout <= 0)
	  expire();
   if (friction){
	  setDx(getDx() * friction);
	  setDy(getDy() * friction);
   }
   move();
   if (!hitBang.empty()){
	  list<Actor*> actors = PlayState::getInstance()->getActorList();
	  for (list<Actor*>::iterator a = actors.begin(); a != actors.end(); ++a)
		 if (collide(*a) && team != (*a)->getTeam()){
			hit();
			break;
		 }
   }
   return alive;
}

void Projectile::explode(string bang, unsigned amount){
   if (!bang.empty()){
	  for (unsigned i = 0; i < amount; ++i){
		 double angle = (rand() % 1000) / 1000.0 * 2 * 3.14;
		 BangFactory::create(bang, getX(), getY(), angle, team);
	  }
   }
}

void Projectile::expire(){
   explode(expireBang, expireAmount);
   alive = false;
}

void Projectile::hit(){
   explode(hitBang, hitAmount);
   if (removeOnHit)
	  alive = false;
}

//******************************************************************************

map<string, ProjectileFactory*> ProjectileFactory::projectileFactories;

void ProjectileFactory::create(string name, double x, double y, double angle, int team){
   PlayState::getInstance()->addBang(getProjectileFactory(name)->make(x, y, angle, team));
}

Bang* ProjectileFactory::make(double x, double y, double angle, int team){
   double speed = randBetween(minSpeed, maxSpeed);
   double dx = speed * cos(angle);
   double dy = speed * sin(angle);
   unsigned timeout = randBetween(minTimeout, maxTimeout);
   return new Projectile(x, y, r, img, team, dx, dy, timeout, hitBang, expireBang, hitAmount, expireAmount, friction, removeOnHit);
}

ProjectileFactory* ProjectileFactory::getProjectileFactory(string name){
   map<string, ProjectileFactory*>::iterator i;
   i = projectileFactories.find(name);
   if (i == projectileFactories.end())
	  return load(name);
   return i->second;
}

ProjectileFactory* ProjectileFactory::load(string name){
   unsigned r = 2;
   int img = TYPE_INVISIBLE;
   double minSpeed = 100;
   double maxSpeed = 0;
   unsigned minTimeout = 100, maxTimeout = 0;
   string hitBangName, expireBangName;
   unsigned hitAmount = 1, expireAmount = 1;
   double friction = 0;
   bool removeOnHit = true;

   string fn = DATA_DIR + name + ".p";
   ifstream fs(fn.c_str());
   if (!fs)
	  throw std::runtime_error("no such projectile: " + name);
   //parser goes here
   string prop;
   while (fs >> prop){
	  if (prop == "r:")
		 fs >> r;
	  else if (prop == "img:")
		 fs >> img;
	  else if (prop == "speed:")
		 fs >> minSpeed;
	  else if (prop == "maxSpeed:")
		 fs >> maxSpeed;
	  else if (prop == "timeout:")
		 fs >> minTimeout;
	  else if (prop == "maxTimeout:")
		 fs >> maxTimeout;
	  else if (prop == "onHit:")
		 fs >> hitBangName;
	  else if (prop == "onExpire:")
		 fs >> expireBangName;
	  else if (prop == "hitAmount:")
		 fs >> hitAmount;
	  else if (prop == "expireAmount:")
		 fs >> expireAmount;
	  else if (prop == "friction:")
		 fs >> friction;
	  else if (prop == "remainOnHit!")
		 removeOnHit = false;
	  else
		 throw std::runtime_error("unknown property in '" + name + "' : " + prop);
   }
   
   maxSpeed = maxSpeed ? maxSpeed : minSpeed;
   maxTimeout = maxTimeout >= minTimeout ? maxTimeout : minTimeout;
   ProjectileFactory * pf = new ProjectileFactory((Drawable_Type) img, r, minSpeed, maxSpeed, minTimeout, maxTimeout, hitBangName, expireBangName,  hitAmount, expireAmount, friction, removeOnHit);
   projectileFactories.insert(pair<string, ProjectileFactory*>(name, pf));
   return pf;
}
