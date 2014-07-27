#include "actor.hh"
#include "actorEffect.hh"

bool ActorEffect::tick(){
   if (active){
	  ActorFactory::create(actorName, getX(), getY());
	  active = false;
   }
   return Effect::tick();
}


//******************************************************************************


map<string, ActorEffectFactory*> ActorEffectFactory::actorEffectFactories;

ActorEffectFactory* ActorEffectFactory::getBangFactory(string name){ // todo: replace factories in proj by string and rm this
   map<string, ActorEffectFactory*>::iterator i;
   i = actorEffectFactories.find(name);
   if (i == actorEffectFactories.end()){
	  ActorEffectFactory* tmp = new ActorEffectFactory(name);
	  actorEffectFactories.insert(make_pair(name, tmp));
	  return tmp;
   }
   return i->second;
}

Bang* ActorEffectFactory::make(double x, double y, double angle, int team){
   return new ActorEffect(x, y, r, img, timeout, team, actorName);
}

