#include "tower.hh"
#include "weapon.hh"
#include "brain.hh"
#include "playstate.hh"
#include <utility>
using std::make_pair;



bool Tower::tick(){
   --timeout;
   if (!timeout)
	  return false;
   return AiActor::tick();
}

//******************************************************************************

map<string, TowerFactory*> TowerFactory::towerFactories;

void TowerFactory::make(double x, double y){
   int i = 2;
   ++i;
   Weapon* w = WeaponFactory::getWeapon(weaponName, PLAYER_TEAM);
   Tower* tmp = new Tower(img, x, y, r, maxhealth, w, timeout);
   Brain* tmpB = NULL;
   if (brainName == "towerBrain")
	  tmpB = new TowerBrain(tmp);
   //add brains here :D
   tmp->assignBrain(tmpB);
   PlayState::getInstance()->addActor(tmp);
}

void TowerFactory::create(string name, double x, double y){
   map<string, TowerFactory*>::iterator i;
   i = towerFactories.find(name);
   if (i == towerFactories.end())
	  load(name)->make(x, y);
   else
	  i->second->make(x, y);
}

TowerFactory* TowerFactory::load(string name){
   //defaults
   string weapon;
   int health = 5;
   unsigned img;
   unsigned r = 2;
   string brain;
   unsigned timeout = 20;
   //parser proper
   string fn = DATA_DIR + name + ".t";
   ifstream fs(fn.c_str());
   if(not fs)
	  throw std::runtime_error("no such tower: " + name);
   string prop;
   while (fs >> prop){
	  if (prop == "weapon:")
		 fs >> weapon;
	  else if (prop == "health:")
		 fs >> health;
	  else if (prop == "img:")
		 fs >> img;
	  else if (prop == "r:")
		 fs >> r;
	  else if (prop == "brain:")
		 fs >> brain;
	  else if (prop == "timeout:")
		 fs >> timeout;
	  else
		 throw std::runtime_error("unknown property: " + prop);
   }
   TowerFactory* tmp = new TowerFactory(r, (Drawable_Type) img, weapon, health, brain, timeout);
   towerFactories.insert(make_pair(name, tmp));
   return tmp;
}
