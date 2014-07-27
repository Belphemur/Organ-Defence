#ifndef _BANG_H_
#define _BANG_H_

#include <string>
using std::string;

#include "2dObj.hh"

class Bang : public TwoDObject {
protected:
   int team;
public:
   Bang(double x, double y, unsigned r, double dx, double dy, Drawable_Type img, int team) : TwoDObject(x, y, r, dx, dy, img), team(team) {}
   virtual bool tick() = 0;
};

class BangFactory{
protected:
   unsigned r;
   Drawable_Type img;
   
   virtual Bang* make(double x, double y, double angle, int team) = 0;
   static BangFactory* getBangFactory(string name);
public:
   BangFactory(unsigned r, Drawable_Type img) : r(r), img(img) {}
   unsigned getR() const {return r;}
   static void create(string name, double x, double y, double angle, int team);
};

#endif /* _BANG_H_ */
