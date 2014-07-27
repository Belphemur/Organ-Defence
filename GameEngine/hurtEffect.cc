#include "hurtEffect.hh"
#include <utility>
#include "playstate.hh"
#include "actor.hh"


using std::make_pair;

bool HurtEffect::tick(){
   PlayState* ps = PlayState::getInstance();
   if (active){
	  for (list<Actor*>::iterator a = ps->getActorList().begin(); a != ps->getActorList().end(); ++a)
		 if (collide(*a) && team != (*a)->getTeam()){
			if (damage)
			   (*a)->hurt(damage);
			if (slowingAmount)
			   (*a)->slow(slowingAmount);
			if (poisonAmount)
			   (*a)->poison(poisonAmount);
		 }
	  active = false;
   }
   return Effect::tick();
}


//******************************************************************************


map<string, HurtEffectFactory*> HurtEffectFactory::hurtEffectFactories;

HurtEffectFactory* HurtEffectFactory::getHurtEffectFactory(string name){
   map<string, HurtEffectFactory*>::iterator i;
   i = hurtEffectFactories.find(name);
   if (i == hurtEffectFactories.end())
	  return load(name);
   return i->second;
}

HurtEffectFactory* HurtEffectFactory::load(string name){
   //defaults
   unsigned r = 2;
   int img = 0;
   unsigned timeout = 18;
   int damage = 0, poisonAmount = 0, slowingAmount = 0;

   string fn = DATA_DIR + name + ".e";
   ifstream fs(fn.c_str());
   if (!fs)
	  throw std::runtime_error("no such effect: " + name);
   string prop;
   while (fs >> prop){
	  if (prop == "r:")
		 fs >> r;
	  else if (prop == "img:")
		 fs >> img;
	  else if (prop == "timeout:")
		 fs >> timeout;
	  else if (prop == "damage:")
		 fs >> damage;
	  else if (prop == "poison:")
		 fs >> poisonAmount;
	  else if (prop == "slowing:")
		 fs >> slowingAmount;
	  else
		 throw std::runtime_error("unknown prop in effect " + name + ": " + prop);
   }
   HurtEffectFactory * ef = new HurtEffectFactory(r, timeout, damage, poisonAmount, slowingAmount, (Drawable_Type) img);
   hurtEffectFactories.insert(make_pair(name, ef));
   return ef;
}

Bang* HurtEffectFactory::make(double x, double y, double angle, int team){
   return new HurtEffect(x, y, r, img, damage, poisonAmount, slowingAmount, timeout, team);
}

