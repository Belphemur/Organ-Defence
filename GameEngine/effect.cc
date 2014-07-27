#include "effect.hh"
#include "actor.hh"
#include "playstate.hh"
#include "hurtEffect.hh"
#include "actorEffect.hh"


bool Effect::tick(){
   --visibleTimeout;
   return visibleTimeout > 0;
}

//******************************************************************************

EffectFactory* EffectFactory::getEffectFactory(string name){
   if (!name.empty()){
	  string effectName = name.substr(0, name.length() - 2); // todo: write clean version :P
	  string last = name.substr(name.length() - 1);
	  if (last == "e")
		 return HurtEffectFactory::getHurtEffectFactory(effectName);
// 	  else if (last == "a")
// 		 return ActorEffectFactory::getActorEffectFactory(effectName);
	  else
		 throw std::runtime_error("unknown effect type: " + name);
   }
   return NULL;
}
