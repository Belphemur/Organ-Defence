#include "bang.hh"
#include "projectile.hh"
#include "effect.hh"
#include "actorEffect.hh"
#include "playstate.hh"



BangFactory* BangFactory::getBangFactory(string name){
   if (!name.empty()){
	  string bang = name.substr(0, name.length() - 2); // todo: write clean version :P
	  string last = name.substr(name.length() - 1);
	  if (last == "p")
		 return ProjectileFactory::getProjectileFactory(bang);
	  else if (last == "e")
		 return EffectFactory::getEffectFactory(name);
	  else if (last == "m" || last == "t")
		 return ActorEffectFactory::getBangFactory(name);
	  else
		 throw std::runtime_error("bangFactory::getBF: unknown kind:" + name);
   }
   return NULL;
}

void BangFactory::create(string name, double x, double y, double angle, int team){
   PlayState::getInstance()->addBang(getBangFactory(name)->make(x, y, angle, team));
}
