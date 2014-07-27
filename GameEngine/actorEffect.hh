#ifndef _ACTOREFFECT_H_
#define _ACTOREFFECT_H_

#include "effect.hh"

class ActorEffect : public Effect{

	string actorName;
public:
	ActorEffect(double x, double y, unsigned r, Drawable_Type img, int timeout, int team, string actorName): 
	Effect(x, y, r, img, timeout, team), actorName(actorName) {};
	bool tick();
};


class ActorEffectFactory : public EffectFactory{
   static map<string, ActorEffectFactory*> actorEffectFactories;
   string actorName;
   static EffectFactory* load(string);
public:
   ActorEffectFactory(string actorName) :
	  EffectFactory(0, 0, TYPE_INVISIBLE), actorName(actorName) {}
   static ActorEffectFactory* getBangFactory(string name);
   Bang* make(double x, double y, double angle, int team);
};


#endif /* _ACTOREFFECT_H_ */
