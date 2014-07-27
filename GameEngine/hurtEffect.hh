#ifndef _HURTEFFECT_H_
#define _HURTEFFECT_H_

#include "effect.hh"

class HurtEffect :public Effect{
   int damage;
   int poisonAmount;
   int slowingAmount;
public:
   HurtEffect(double x, double y, unsigned r, Drawable_Type img, int d, int p, int s, int timeout, int team) :
	  Effect(x, y, r, img, timeout, team), damage(d), poisonAmount(p), slowingAmount(s) {}
   bool tick();
};


class HurtEffectFactory : public EffectFactory{
   static map<string, HurtEffectFactory*> hurtEffectFactories;
   int damage, poisonAmount, slowingAmount;
   static HurtEffectFactory* load(string);
public:
   HurtEffectFactory(unsigned r, unsigned timeout, int d, int p, int s, Drawable_Type img) :
	  EffectFactory(r, timeout, img), damage(d), poisonAmount(p), slowingAmount(s) {}
   static HurtEffectFactory* getHurtEffectFactory(string name);
   Bang* make(double x, double y, double angle, int team);
};

#endif /* _HURTEFFECT_H_ */
