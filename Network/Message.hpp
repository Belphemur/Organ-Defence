/*
 * File: message.hpp
 * Author: neo
 *
 * Created on 10 février 2010, 12:45
 *
 * This file contains everything related to the message types and contents
 * sent across the network.
 */
#ifndef _MESSAGE_HPP
#define _MESSAGE_HPP

#include <cstddef>
#include <vector>
#include <climits>

//------------
// SOCKETS ID
//------------

// Every socket is used for a certain type of message. They are stored in an
// array of socket descriptors.
// For readability reasons, we define constants that represents the index
// of a certain socket (= a certain message type)

// Note : Port = Message + DEFAULT_PORT

#define DEFAULT_PORT 1515
const int MESSAGES_START = 30;

enum Message {
	MSG_NOP = MESSAGES_START,
    PLAYER_READY, // Sent when the player joins the game
    CLIENT_LEFT,
    PLAYER_MOVE, // Sent during game when player moves
    PLAYER_NAME,
	PLAYER_ID,
    UNIT_DATA,// Sent during the game, when a unit must be modified
    UNIT_DESTROY,// Sent during the game, when a unit is destroyed
    MOUSE_CLICK, // Sent during game when a player does a mouse click
    BUILD_TOWER, // Sent during game when a player builds
    CHANGE_WEAPON, // Sent during game when a player switches weapon
	NEW_WEAPON,    // Sent during game when a player buys a weapon
    GAME_OVER,
    ORGAN_TYPE,
    ORGAN_HP,
    PLAYER_HP,
    PLAYER_MONEY,
	UPDATE_KILLS,
	NEW_WAVE,
	SCORE
};

enum Direction {
	DIR_RIGHT = 0,
	DIR_UP,
	DIR_LEFT,
	DIR_DOWN,
	DIR_NOY,
	DIR_NOX
};

enum SocketStatus
{
	Done,        
	Disconnected
};

namespace sf
{
    // 8 bits integer types
    #if UCHAR_MAX == 0xFF
        typedef signed   char Int8;
        typedef unsigned char Uint8;
    #else
        #error No 8 bits integer type for this platform
    #endif

    // 16 bits integer types
    #if USHRT_MAX == 0xFFFF
        typedef signed   short Int16;
        typedef unsigned short Uint16;
    #elif UINT_MAX == 0xFFFF
        typedef signed   int Int16;
        typedef unsigned int Uint16;
    #elif ULONG_MAX == 0xFFFF
        typedef signed   long Int16;
        typedef unsigned long Uint16;
    #else
        #error No 16 bits integer type for this platform
    #endif

    // 32 bits integer types
    #if USHRT_MAX == 0xFFFFFFFF
        typedef signed   short Int32;
        typedef unsigned short Uint32;
    #elif UINT_MAX == 0xFFFFFFFF
        typedef signed   int Int32;
        typedef unsigned int Uint32;
    #elif ULONG_MAX == 0xFFFFFFFF
        typedef signed   long Int32;
        typedef unsigned long Uint32;
    #else
        #error No 32 bits integer type for this platform
    #endif
    
} // namespace sf

#endif
