#include "monster.hh"
#include "brain.hh"
#include "playstate.hh"
#include "weapon.hh"

void Monster::die(){
   PlayState* ps = PlayState::getInstance();
   ps->giveMoney(value);
   ps->addKill();
}


//******************************************************************************

map<string, MonsterFactory*> MonsterFactory::monsterFactories;

void MonsterFactory::make(double x, double y){
   Monster* tmp = new Monster(img, x, y, r, maxhealth, WeaponFactory::getWeapon(weaponName, MONSTER_TEAM), speed, value);
   Brain* tmpB;
   if (brainName == "orbiterBrain")
	  tmpB = new OrbiterBrain(tmp);
   else if(brainName == "wandererBrain")
	  tmpB = new WandererBrain(tmp);
   else if(brainName == "stalkerBrain")
	  tmpB = new StalkerBrain(tmp);
   //add brains here :D
   else
	  throw std::runtime_error("oh noes, a brainless monster: " + brainName);
   tmp->assignBrain(tmpB);
   PlayState::getInstance()->addActor(tmp);
}

void MonsterFactory::create(string name, double x, double y){
   map<string, MonsterFactory*>::iterator i;
   i = monsterFactories.find(name);
   if (i == monsterFactories.end())
	  load(name)->make(x, y);
   else
	  i->second->make(x, y);
}

MonsterFactory* MonsterFactory::load(string name){
   //defaults
   string weapon;
   double speed = 1;
   int health = 5;
   unsigned img;
   unsigned r = 2;
   string brain;
   int value = 1;
   //parser proper
   string fn = DATA_DIR + name + ".m";
   ifstream fs(fn.c_str());
   if(not fs)
	  throw std::runtime_error("no such monster: " + name);
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
	  else if (prop == "value:")
		 fs >> value;
	  else if (prop == "speed:")
		 fs >> speed;
	  else
		 throw std::runtime_error("unknown property: " + prop);
   }
   MonsterFactory* tmp = new MonsterFactory(r, (Drawable_Type) img, MONSTER_TEAM, weapon, health, speed, brain, value);
   monsterFactories.insert(pair<string, MonsterFactory*>(name, tmp));
   return tmp;
}
