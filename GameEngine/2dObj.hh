#ifndef _2DOBJ_H_
#define _2DOBJ_H_

#include "consts.hh"
#include <cmath>
#include <stdint.h>
#include <iostream>
#include "DrawableType.hh"

struct Drawable{
   unsigned int uid;
   int dx, dy;
   double x, y;
   Drawable_Type type;
};

/**
 *  Represents a 2D Object drawn on the map 
 */

class TwoDObject{
   /** 
	* Id 
	*/
   static unsigned int currentUid;
   /**
    * Radius 
    */
   unsigned r;
   /** 
    * image id
    */
   Drawable_Type type;
   /**
    * Object ID
    */
   unsigned int uid;
   bool modified;
   bool sent;
   double x, y;
   /**
    * Speed vector 
    */
   double dx, dy;
public:							//note: nationalized for unit tests
   bool inGameArea() const {
	  return inGameArea(x, y, r);}
   static bool inGameArea(int x, int y, int r){
	  return ((r <= x) && ((MAP_WIDTH - r) >= x) && (r <= y) && ((MAP_HEIGHT - r) >= y));}
public:
  TwoDObject(double x, double y, unsigned r, double dx=0, double dy=0, Drawable_Type type=TYPE_INVISIBLE) : r(r), type(type), uid(++currentUid), modified(true), sent(false), x(x), y(y), dx(dx), dy(dy) {}
   /* Check that 2 objects don't collide */
   bool collide(const TwoDObject *p) const { return collide(*p); }
   bool collide(const TwoDObject &other) const {
	  double dx = x - other.x;
	  double dy = y - other.y;
	  double distSqrd = dx*dx + dy*dy;
	  return distSqrd < (r + other.r) * (r + other.r);
   }
   void move() {
	    x += dx;
	    y += dy;
	}
   void moveTo(double x, double y) {this->x = x; this->y = y; modified = true;}
   bool isVisible() const {return (type != TYPE_INVISIBLE);}
   uint8_t getImg() const {return type;}
   double getX() const {return x;}
   double getY() const {return y;}
   unsigned getR() const {return r;}
   double dist(TwoDObject* o) const {
	  return sqrt ((x - o->x)*(x - o->x) + (y - o->y)*(y - o->y));
   }
   bool hasBeenModified() const {return modified;}
   Drawable getDrawable() {
	  modified = false;
	  sent = true;
	  Drawable tmp;
	  tmp.uid = uid;
	  tmp.x = x; tmp.y = y;
	  tmp.dx = dx * TICKS_PER_SECONDS; tmp.dy = dy * TICKS_PER_SECONDS;
	  tmp.type = type;
	  return tmp;
   }
   double getDx() const {return dx;}
   double getDy() const {return dy;}
   void setDx(double dx_) { if(dx_ != dx) modified = true; dx = dx_;}
   void setDy(double dy_) { if(dy_ != dy) modified = true; dy = dy_;}
   void setType(Drawable_Type type) {if (type != this->type) modified = true; this->type = type;}
   unsigned getUid() const {return uid;}
   virtual bool tick() {return true;};		// note: not abstract for unit tests
   virtual ~TwoDObject();
};

#endif /* _2DOBJ_H_ */

