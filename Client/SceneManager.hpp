#ifndef _SCENEMANAGER_H
#define _SCENEMANAGER_H
#include <list>

#include <map>
#include <string>
#include "../sdl_include.hpp"

#include "Entity.hpp"
#include "ImageLoader.hpp"
#include "Logger.hpp"
#include "../Network/ClientNetworkManager.hpp"
#include "../Network/Message.hpp"
#include "../Network/Packet.hpp"

class SceneManager
{
public:
	static SceneManager* getInstance();
	static void killInstance();
	/**
     * Loading all images
     */
	virtual void loadImages();
	/**
     * Main drawing method to draw all units
     */
	virtual void draw();
	/**
     * Receiving data from the server, and moving all units in space
     */
	virtual void update(int);
	/**
     * Setting the player name
     */
	virtual void setPlayerName(std::string);
	/**
     * Get camera X coordinate
     */
    virtual int getCameraX();
   	/**
     * Get camera Y coordinate
     */
    virtual int getCameraY();
	/**
     * Drawing the class choice image
     */
    virtual void drawClassChoice();
	/**
     * Setting the player class when the choice is done
     */
	virtual void setPlayerClass(int pc);
	
private:
	static SceneManager* instance_;
	ImageLoader* imageLoader_;
    std::map<int, Entity> unitMap_;
    std::map<int, Entity>::iterator mIt;
	int lastId_;
	SDL_Surface* screen_;
	std::map<int, SDL_Surface*> playerNameMap_;
	TTF_Font *font_;
	TTF_Font *fontB_;
	SDL_Color textColor_;
	std::string name_; 
	sf::Int16 organHP_;	
	bool gameOver_;
	sf::Int16 playerHP_, playerADN_, playerKills_;
	sf::Uint32 myPid;
    std::string waveMsg_;
    int waveMsgTick_;
	int playerClass_;
    bool playerWeapons[6];
    uint8_t equipedWeapon_;
    std::list<std::string> classWeapons_;
    std::string scoreMsg_;


	SceneManager(void);
	~SceneManager(void);
	/**
     * Destroying all units
     */
	virtual void drop();
	/**
     * Assemble the correct unit
     * @return Entity ready to be inserted
     */
	virtual Entity assembleEntity(sf::Packet);
	/**
     * Removing unit 
     */
	virtual void removeUnit();
	/**
     * Modifying or creating a unit
     */
	virtual void modifyUnit();
	/**
     * Setting all players names
     */
	virtual void addPlayerName();
	/**
     * Update Organs HP points
     */
	virtual void updateOrganHealth();
	/**
     * Update Players HP points
     */
	virtual void updatePlayerHealth();
	 /**
     * Update Players ADN points
     */
	virtual void updatePlayerMoney();
	/**
     * Update Players kill count
     */
	virtual void updatePlayerKills();
	/**
     * Message with a new wave direction
     */
	virtual void updateWaveMessage();
	/**
     * Message with a new equiped weapon
     */
	virtual void updateEquipedWeapon();
	/**
     * Message with the player id
     */
	virtual void updatePlayerId();
	/**
     * Message with the new score
     */
	virtual void updateScore();
	/**
     * Drawing HP, kills, weapons, time data for organ and player
     */
	virtual void showPlayerRelatedData();
	/**
     * Get Organ HP points
     */
	virtual sf::Int16 getOrganHealth() {return organHP_ > 0 ? organHP_ : 0;};
	/**
     * Link on the players Entity
     */
    virtual Entity* myPlayer();
};
#endif
