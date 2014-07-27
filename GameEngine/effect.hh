#ifndef _EFFECT_H_
#define _EFFECT_H_

#include <string>
using std::string;
#include <list>
using std::list;
#include <map>
using std::map;
using std::pair;
#include <fstream>
using std::ifstream;
#include <stdexcept>


#include "bang.hh"

class PlayState;

class Effect : public Bang{
   int visibleTimeout;
protected:
   bool active;
public:
   Effect(double x, double y, unsigned r, Drawable_Type img, int timeout, int team) :
	  Bang(x, y, r, 0, 0, img, team), visibleTimeout(timeout), active(true) {}
   bool tick();
};

class EffectFactory : public BangFactory{
protected:
   unsigned timeout;
public:
   EffectFactory(unsigned r, unsigned timeout, Drawable_Type img) : 
	  BangFactory(r, img), timeout(timeout) {}
   static EffectFactory* getEffectFactory(string name);
};

#endif /* _EFFECT_H_ */
