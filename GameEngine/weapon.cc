#include "weapon.hh"
#include "playstate.hh"
#include "util.hh"
#include <cstdlib>
#include "bang.hh"


bool Weapon::canShoot() const{
   return (PlayState::getInstance()->getCurrentFrame() - lastShotFrame) > static_cast<int>(reloadTime);
}

void Weapon::shoot(double angle, double x, double y){
   PlayState* ps = PlayState::getInstance();
   lastShotFrame = ps->getCurrentFrame();
   unsigned num = randBetween(amountMin, amountMax);
   for (unsigned i = 0; i < num; ++i) {
	  double ang = angle;
	  if (spread)
		 ang += (rand() % 1000 / 1000.0 * spread) - spread / 2;
	  BangFactory::create(projectileName, x, y, ang, team);
   }
}

//******************************************************************************

map<string, WeaponFactory*> WeaponFactory::weapons;
map<string, BangFactory*> WeaponFactory::bangs;

Weapon* WeaponFactory::getWeapon(string name, int team){
   if (name.empty())
	  return NULL;
   return getFactory(name)->make(team);
}

unsigned WeaponFactory::getWeaponPrice(string name){
   return getFactory(name)->price;
}

WeaponFactory* WeaponFactory::getFactory(string name){
   map<string, WeaponFactory*>::iterator i = weapons.find(name);
   if (i == weapons.end())
	  return load(name);
   return i->second;
}

Weapon* WeaponFactory::make(int team){
   if (healing)
	  team = !team;
   return new Weapon(reloadTime, projectileName, amountMin, amountMax, spread, team);
}

WeaponFactory* WeaponFactory::load(string name){
   //defaults
   unsigned reload = 30;
   double spread = 0;
   unsigned minAmount = 1, maxAmount = 1;
   string projName;
   bool healing = false;
   unsigned price = 10;
   //parser proper
   string fn = DATA_DIR + name + ".w";
   ifstream fs(fn.c_str());
   if(not fs)
	  throw std::runtime_error("no such weapon: " + name);
   string prop;
   while (fs >> prop){
	  if (prop == "reload:")
		 fs >> reload;
	  else if (prop == "spread:")
		 fs >> spread;
	  else if (prop == "minAmount:")
		 fs >> minAmount;
	  else if (prop == "maxAmount:")
		 fs >> maxAmount;
	  else if (prop == "shoots:")
		 fs >> projName;
	  else if (prop == "healing!")
		 healing = true;
	  else if (prop == "price:")
		 fs >> price;
	  else
		 throw std::runtime_error("unkown prop in " + fn + ": " + prop);
   }
   maxAmount = maxAmount >= minAmount ? maxAmount : minAmount;
   WeaponFactory* w = new WeaponFactory(reload, projName, minAmount, maxAmount, spread, healing, price);
   weapons.insert(pair<string, WeaponFactory*>(name, w));
   return w;
}
