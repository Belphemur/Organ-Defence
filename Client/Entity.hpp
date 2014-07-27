#ifndef _ENTITY_H
#define _ENTITY_H

#include <string>
#include <iostream>

#include "../sdl_include.hpp"

class Entity
{
public:
	Entity(void){};
	Entity(std::string name, std::string image, int id, float x, float y, int vX = 0, int vY = 0, int frameMax = 0, bool animation = false, bool rectangle = true, std::string prefix=""): name_(name), image_(image), id_(id), x_(x), y_(y), Vx_(vX), Vy_(vY), frameMax_(frameMax), frame_(0), animation_(animation), rectangle_(rectangle),prefix_(prefix)
	{}
	
	virtual void advance(Uint32 deltaTicks)
	{
		x_ += Vx_ * ( deltaTicks / 1000.f );
		y_ += Vy_ * ( deltaTicks / 1000.f );
		frame_ += (deltaTicks / 100.f);
		if(name_ == "Player")
		{
			if(Vy_ == 0 && Vx_ == 0)
			{
				animation_ = false;
			}
			else
			{ 
				animation_ = true;
			}


			if(animation_ == true)
			{
				if(Vx_ > 0 && Vx_ > Vy_)
					image_ = prefix_+"droite";
				if(Vx_ < 0 && Vx_ < Vy_)
					image_ = prefix_+"gauche";
				if(Vy_ > 0 && Vy_ > Vx_)
					image_ = prefix_+"bas";
				if(Vy_ < 0 && Vy_ < Vx_)
					image_ = prefix_+"haut";
				if(Vx_ > 0 && Vy_ > 0)
					image_ = prefix_+"bas-droite";
				if(Vx_ < 0 && Vy_ > 0)
					image_ = prefix_+"bas-gauche";
				if(Vx_ < 0 && Vy_ < 0)
					image_ = prefix_+"haut-gauche";
				if(Vx_ > 0 && Vy_ < 0)
					image_ = prefix_+"haut-droite";
			}
		}
		if(animation_ == true)
		{
			if(frame_ > frameMax_)
				frame_ = 0;
		}
		else
		{
			frame_ = 0;
		}	
	}

	SDL_Rect getPosition()
	{
		SDL_Rect offset;
		offset.x = x_;
		offset.y = y_;
		return offset;
	}

	virtual ~Entity(void){};

	std::string name_;			// todo: shouldn't all this be private?
	std::string image_;	
	int id_;
	float x_,y_;
	int Vx_, Vy_;
	int frameMax_;
	float frame_;
	bool animation_;
	bool rectangle_;
	std::string prefix_; 
};
#endif
