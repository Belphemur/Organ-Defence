#include "EventListener.hpp"
#include <iostream>
#include "../sdl_include.hpp"
#include "../Network/ClientNetworkManager.hpp"

EventListener* EventListener::instance_ = NULL;

EventListener* EventListener::getInstance() {
	if(!instance_)
		instance_ = new EventListener();
		
	return instance_;
}

void EventListener::killInstance() {
	delete instance_;
}

EventListener::EventListener()
{
		for (int i = 0; i <= SDLK_LAST; i++)
		{
			keyStates_[i] = UP; // all keys are UP
		}
		for (int i = 0; i <= 2; i++)
		{
			mouse_.mouseButtonStates_[i] = UP;	//all mouse buttons are UP
		}
		
		mouse_.x_ = 0;	//mouse position
		mouse_.y_ = 0;

		quit_ = false;	//no quit at the beginning

}

void EventListener::startEventProcess()
{
	SDL_Event event;
	while(SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_KEYDOWN:
			if (keyStates_[event.key.keysym.sym] != DOWN)
			{
				keyStates_[event.key.keysym.sym] = PRESSED; // Set to Pressed
				
				if(event.key.keysym.sym == SDLK_ESCAPE) 	//quit if esc was pressed
				{
					quit_=true;
					break;
				}
			}
			break;

		case SDL_KEYUP:
			if (keyStates_[event.key.keysym.sym] != UP)
			{
				keyStates_[event.key.keysym.sym] = RELEASED; // Set to Released
			}
			break;
									
		case SDL_MOUSEMOTION:
			mouse_.x_=event.motion.x;
			mouse_.y_=event.motion.y;
			break;

		case SDL_MOUSEBUTTONDOWN:
			if(mouse_.mouseButtonStates_[event.button.button-1] != DOWN)
			{
				mouse_.mouseButtonStates_[event.button.button-1] = PRESSED;
			}
			break;

		case SDL_MOUSEBUTTONUP:
			if(mouse_.mouseButtonStates_[event.button.button-1] != UP)
			{
				mouse_.mouseButtonStates_[event.button.button-1] = RELEASED;
			}
			break;

		case SDL_QUIT:
			quit_ = true;
			break;

		default:
			break;
		}
	}
}


bool EventListener::keyPressed(SDLKey keycode)
{
	if (keyStates_[keycode] == PRESSED)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool EventListener::keyDown(SDLKey keyCode)
{
	if (keyStates_[keyCode] == PRESSED || keyStates_[keyCode] == DOWN)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool EventListener::keyReleased(SDLKey keyCode)
{
	if (keyStates_[keyCode] == RELEASED)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool EventListener::keyUp(SDLKey keyCode)
{
	if (keyStates_[keyCode] == RELEASED || keyStates_[keyCode] == UP)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool EventListener::mouseButtonPressed(Uint8 buttonCode)
{
	if (mouse_.mouseButtonStates_[buttonCode] == PRESSED)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool EventListener::mouseButtonDown(Uint8 buttonCode)
{
	if (mouse_.mouseButtonStates_[buttonCode] == PRESSED || mouse_.mouseButtonStates_[buttonCode] == DOWN)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool EventListener::mouseButtonUp(Uint8 buttonCode)
{
	if (mouse_.mouseButtonStates_[buttonCode] == RELEASED || mouse_.mouseButtonStates_[buttonCode] == UP)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool EventListener::mouseButtonReleased(Uint8 buttonCode)
{
	if (mouse_.mouseButtonStates_[buttonCode] == RELEASED)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void EventListener::endEventProcess()
{
	for (int i = 0; i <= SDLK_LAST; i++)
	{
		if(keyStates_[i]==PRESSED)
			keyStates_[i]=DOWN;

		if(keyStates_[i]==RELEASED)
			keyStates_[i]=UP;
	}

	for (int i = 0; i <= 2; i++)
	{
		if(mouse_.mouseButtonStates_[i]==PRESSED)
			mouse_.mouseButtonStates_[i]=DOWN;

		if(mouse_.mouseButtonStates_[i]==RELEASED)
			mouse_.mouseButtonStates_[i]=UP;
	}	
}

bool EventListener::isToQuit()
{
	return quit_;
}

int EventListener::getMouseX() const
{
	return mouse_.x_;
}

int EventListener::getMouseY() const
{
	return mouse_.y_;
}
