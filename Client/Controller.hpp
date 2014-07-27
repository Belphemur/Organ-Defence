#ifndef _CONTROLLER_H
#define _CONTROLLER_H

#include <iostream>
#include "EventListener.hpp"
#include "../Network/ClientNetworkManager.hpp"

class Controller 
{
	private:
		Controller(void);
		static Controller* instance_;
	public:
		static Controller* getInstance();
		static void killInstance();
		/**
		* Listen Keyboard input
		*/
		void ListenKeyboard(EventListener*, ClientNetworkManager*);
		/**
		* Listen mouse input
		*/
		void ListenMouse(EventListener*, ClientNetworkManager*);
		/**
		* Listen for the class choice
		*/
		int ListenClass(EventListener*);
};

#endif
