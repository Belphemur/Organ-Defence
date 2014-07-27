#ifndef _EVENTLISTENER_H
#define _EVENTLISTENER_H

#include "SDL/SDL.h"

enum keyStatesENUM {UP, DOWN, PRESSED, RELEASED};


struct MouseData
{
	int x_;
	int y_;
	keyStatesENUM mouseButtonStates_[6];
};

class EventListener
{
public:
	static EventListener* getInstance();
	static void killInstance();
	/**
     * Looking for input events and updating state vectors
     */
	void startEventProcess();
	/**
     * Finishing moves on state vectors at the end of main loop
     */
	void endEventProcess();
	/**
     * If player desires to quit the game
     * @return true if player pressed ESC
     */
	bool isToQuit();
	/**
     * Specified button state
     * @return true if this button was just pressed
     */
	bool keyPressed(SDLKey);
	/**
     * Specified button state
     * @return true if this button is pressed
     */
	bool keyDown(SDLKey);
	/**
     * Specified button state
     * @return true if this button was just released
     */
	bool keyReleased(SDLKey);
	/**
     * Specified button state
     * @return true if this button is released
     */
	bool keyUp(SDLKey);
	/**
     * Specified mouse button state
     * @return true if this mouse button was just pressed
     */
	bool mouseButtonPressed(Uint8);
	/**
     * Specified mouse button state
     * @return true if this mouse button is pressed
     */
	bool mouseButtonDown(Uint8);
	/**
     * Specified mouse button state
     * @return true if this mouse button was just released
     */
	bool mouseButtonReleased(Uint8);
	/**
     * Specified mouse button state
     * @return true if this mouse button is released
     */
	bool mouseButtonUp(Uint8);
	/**
     * Mouse position
     * @return mouse X coordinate
     */
	int getMouseX() const;
	/**
     * Mouse position
     * @return mouse Y coordinate
     */
	int getMouseY() const;

private:
	static EventListener* instance_;
	EventListener(void);
	keyStatesENUM keyStates_[SDLK_LAST];
	bool quit_;
	MouseData mouse_;
	
};


#endif


