#include "SceneManager.hpp"
#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include "../sdl_include.hpp"


#include <stdio.h>
#include <stdlib.h>


#include "../Network/ClientNetworkManager.hpp"
#include "../Network/Message.hpp"
#include "../Network/Packet.hpp"
#include "../GameEngine/DrawableType.hh"
#include "../GameEngine/consts.hh"
#include "Logger.hpp"
#include "Entity.hpp"
#include "ImageLoader.hpp"
#include "Timer.hpp"

SceneManager* SceneManager::instance_ = NULL;

SceneManager* SceneManager::getInstance() {
	if(!instance_)
		instance_ = new SceneManager();
		
	return instance_;
}

void SceneManager::killInstance() {
	instance_->drop();
	delete instance_;
}

SceneManager::SceneManager()
{
	Logger* logger =  Logger::getInstance();
	screen_ =  SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_FULLSCREEN); //screen resolution, color depth, HardWare/Software usage, Double/Single  buffer
	imageLoader_ = new ImageLoader();
	textColor_.r = 0;
	textColor_.g = 0;
	textColor_.b = 0;
	waveMsgTick_=0;
	equipedWeapon_=0;
	playerWeapons[0]=true;
	for(uint8_t i = 1; i<6;i++)
		playerWeapons[i]=false;
	fontB_ = TTF_OpenFont( "images/fonts/Arial_Bold.ttf", 16 ); 
	if(fontB_ == NULL)
		logger->log("Arial_Bold.ttf was not loaded");
	else
		logger->log("Arial_Bold.ttf loaded");
		
	font_ = TTF_OpenFont( "images/fonts/Arial.ttf", 16 ); 
	if(font_ == NULL)
		logger->log("Arial.ttf was not loaded");
	else
		logger->log("Arial.ttf loaded");
	gameOver_ = false;
	organHP_ = 0;
	playerADN_ = 0;
	playerHP_ = 0;
	myPid = 0;
	playerKills_=0;
}

SceneManager::~SceneManager(void)
{
}

void SceneManager::removeUnit()
{
	ClientNetworkManager* client = ClientNetworkManager::getInstance();	
	sf::Packet package;
	client->receive(package);
	sf::Int32 ID;
	package >> ID;
	mIt=unitMap_.find(ID); //finding the unit to delete by ID
	if(mIt == unitMap_.end())
	{
		std::cerr<< "Unit ("<<ID<<") to be deleted was not found!!!" << std::endl;
	}
	else
	{
		if((*mIt).second.name_ == "Player")
		{
			(*mIt).second.image_ = "dead";
			playerNameMap_.erase(ID);
			playerNameMap_.insert(std::pair<int, SDL_Surface*>(ID, TTF_RenderText_Solid( font_, "NOOB", textColor_ )));
		}
		else
			unitMap_.erase(mIt);  //removing it from UnitDeque
	}
}

void SceneManager::modifyUnit()
{
	ClientNetworkManager* client = ClientNetworkManager::getInstance();	
	sf::Packet packet, tmp;
	tmp.Clear();
	Entity entity;
	client->receive(packet);
	sf::Uint32 id; sf::Uint8 typeNet;
	sf::Int16 x; sf::Int16 y;
	sf::Int16 dx; sf::Int16 dy;
	
	tmp=packet;
	packet>>id;
    mIt=unitMap_.find(id);	//finding the unit to be modified
    packet>>typeNet>>x>>y>>dx>>dy;
	if(unitMap_.end() == mIt)	//unit was not found, should be created
	{
                entity = assembleEntity(tmp);
                unitMap_[id]=entity;
	}
	else
	{
        
        Drawable_Type type=static_cast<Drawable_Type>(typeNet);
        (*mIt).second.x_ = x;		//modifying position and speed
		(*mIt).second.y_ = y;
		(*mIt).second.Vx_ = dx;
		(*mIt).second.Vy_ = dy;
		if(type==TYPE_PLAYER_DEAD)
		{
			(*mIt).second.image_ = (*mIt).second.prefix_+"dead";
			playerHP_ = 0;
		}
		else if ((*mIt).second.image_==(*mIt).second.prefix_+"dead")
		{
			(*mIt).second.image_ =  (*mIt).second.prefix_+"bas";
		}
		else if(type==TYPE_DEAD_ORGAN)
		{
			(*mIt).second.image_ +="_dead";
		}
	}
}

void SceneManager::updateOrganHealth()
{ //received new organ HP, updating the data
	ClientNetworkManager* client = ClientNetworkManager::getInstance();	
	sf::Packet package;
	client->receive(package);
	sf::Int16 HP;
	package >> HP;
	organHP_ = HP;	
}

void SceneManager::updatePlayerHealth()
{ //received new player HP, updating the data
	ClientNetworkManager* client = ClientNetworkManager::getInstance();	
	sf::Packet package;
	client->receive(package);
	sf::Int16 HP;
	package >> HP;
	playerHP_ = HP;
}

void SceneManager::updatePlayerMoney()
{//received new player money data
	ClientNetworkManager* client = ClientNetworkManager::getInstance();	
	sf::Packet package;
	client->receive(package);
	sf::Int16 ADN;
	package >> ADN;
	playerADN_ = ADN;
}

void SceneManager::updatePlayerKills()
{//received new player kills count
	ClientNetworkManager* client = ClientNetworkManager::getInstance();	
	sf::Packet package;
	client->receive(package);
	sf::Int16 kills;
	package >> kills;
	playerKills_ = kills;
}
void SceneManager::updatePlayerId()
{//received the players id
	ClientNetworkManager* client = ClientNetworkManager::getInstance();	
	sf::Packet package;
	client->receive(package);
	sf::Uint32 id;
	package >> id;
	char buffer[30];
	sprintf(buffer,"Id received : %d",(int)id);
	Logger::getInstance()->log(buffer);
	
	myPid = id;
}
void SceneManager::loadImages()
{
	std::ifstream imageslist("data/images", ios::in);
	
	while(!imageslist.eof()) {
		std::string filename, filepath;
		imageslist >> filename >> filepath;
		
		imageLoader_->loadImage(filename, filepath);
	}
	
	imageslist.close();
} 

void SceneManager::draw()
{
	SDL_Rect offset;
	offset.x = - getCameraX();
	offset.y = - getCameraY();
	SDL_BlitSurface( imageLoader_->getImage("background"), NULL, screen_, &offset);
	if(unitMap_.size() < 2)
	{ //waiting for the connection image
		offset.x = (SCREEN_WIDTH - imageLoader_->getImage("titlescreen")->w) /2;
		SDL_BlitSurface( imageLoader_->getImage("titlescreen"), NULL, screen_, &offset );
	}
	else
		for(mIt = unitMap_.begin(); mIt != unitMap_.end(); mIt++)
		{
			offset = (*mIt).second.getPosition();
			SDL_Rect anim;
			int imageSize = imageLoader_->getImage((*mIt).second.image_)->h;
			//calculating animation coordinates
			anim.x=(int)((*mIt).second.frame_) * imageSize;
			anim.y=0;
			
			//looking if the image is square or not
			if((*mIt).second.rectangle_ == false) 
				anim.w=imageLoader_->getImage((*mIt).second.image_)->w;
			else
				anim.w=imageLoader_->getImage((*mIt).second.image_)->h;
				
			anim.h=imageLoader_->getImage((*mIt).second.image_)->h;

			offset.x -=anim.w/2;
			offset.y -=anim.h/2;

			offset.x -= getCameraX();
			offset.y -= getCameraY();
			
			//blitting
			SDL_BlitSurface( imageLoader_->getImage((*mIt).second.image_), &anim, screen_, &offset );
			//special cases:
			if((*mIt).second.name_ == "Player")
			{
				std::map<int, SDL_Surface*>::iterator playerName;
				//should put the name
				playerName = playerNameMap_.find((*mIt).second.id_);

				offset.x -= (playerName->second->w - anim.w) / 2;
				offset.y -= 20;
				SDL_BlitSurface( playerName->second,NULL, screen_, &offset );
			}
			if((*mIt).second.name_ == "organ")
			{
				//drawing the health bar
				offset.x = (SCREEN_WIDTH - imageLoader_->getImage("healthbar200")->w) /2;
				offset.y = 10;
				SDL_BlitSurface(imageLoader_->getImage("border"),NULL, screen_, &offset );
				anim.w = (getOrganHealth()  * imageLoader_->getImage("healthbar200")->w) / ORGAN_HEALTH ;
				offset.y += 1;
				SDL_BlitSurface(imageLoader_->getImage("healthbar200"),&anim, screen_, &offset );
			}
		}
		if(gameOver_ == true)
		{
			//drawing the game over screen
			offset.x = (SCREEN_WIDTH - imageLoader_->getImage("gameover")->w)/2 ;
			offset.y = (SCREEN_HEIGHT - imageLoader_->getImage("gameover")->h)/2 - 150;
			SDL_BlitSurface( imageLoader_->getImage("gameover"),NULL, screen_, &offset );			
			
			SDL_Surface * text = NULL;
			text = TTF_RenderText_Solid( fontB_, scoreMsg_.c_str(), textColor_ );
			offset.x=40;
			offset.y=40;
			SDL_BlitSurface(text, NULL, screen_, &offset );
			SDL_FreeSurface(text);
		}
	showPlayerRelatedData();
	SDL_Flip( screen_ );
}

void SceneManager::drop()
{
	unitMap_.clear();
	TTF_CloseFont(font_);
	TTF_CloseFont(fontB_);
	imageLoader_->drop();
}

void SceneManager::update(int ticks)
{
	ClientNetworkManager* client = ClientNetworkManager::getInstance();	
	Logger* logger =  Logger::getInstance();
	Message header = static_cast<Message>(client->receive<sf::Uint8>());
	while(header != 0)
	{
		if(header==UNIT_DATA)
			modifyUnit();
		else if(header==UNIT_DESTROY)	
			removeUnit();
		else if(header==ORGAN_HP)
			updateOrganHealth();
		else if (header == PLAYER_HP)
		    updatePlayerHealth();
		else if(header == PLAYER_MONEY)
			updatePlayerMoney();
		else if(header == UPDATE_KILLS)
			updatePlayerKills();
		else if(header == NEW_WAVE)
			updateWaveMessage();
		else if(header == CHANGE_WEAPON)
			updateEquipedWeapon();
		else if(header==PLAYER_ID)
			updatePlayerId();
		else if(header==PLAYER_NAME)
		{
			logger->log("Received PLAYER_NAME, setting player name");	
			addPlayerName();	
		}
		else if(header==GAME_OVER)
		{
			logger->log("Received GAME_OVER, you lose !");
			gameOver_ = true;
			AbsTimer* absTimer_ = AbsTimer::getInstance();
			absTimer_->pause();
		}
		else if(header == SCORE)
			updateScore();
		else
		{}
		usleep(1);
		header = static_cast<Message>(client->receive<sf::Uint8>());
	}
	for(mIt = unitMap_.begin(); mIt != unitMap_.end(); mIt++)
		(*mIt).second.advance(ticks);	//each unit advance in space
}
Entity SceneManager::assembleEntity(sf::Packet packet)
{ 
	AbsTimer* absTimer_ = AbsTimer::getInstance();
	//getting all data from the packet
	sf::Uint32 id; sf::Uint8 typeNet;
	sf::Int16 x; sf::Int16 y;
    sf::Int16 dx; sf::Int16 dy;
	packet>>id>>typeNet>>x>>y>>dx>>dy;	
	Drawable_Type type=static_cast<Drawable_Type>(typeNet);
	Entity entity;
	//creating the correct Entity with correct parameters
	switch (type)
	{	
		
		case TYPE_PLAYER_HEALER:
			entity=Entity("Player","heal_bas",id,x,y,dx,dy,4,false,true,"heal_");
		break;
		case TYPE_PLAYER_FIGHTER:
			entity=Entity("Player","bas",id,x,y,dx,dy,4,false);
		break;
		case TYPE_PLAYER_BUILDER:
			entity=Entity("Player","builder_bas",id,x,y,dx,dy,4,false,true,"builder_");
		break;
		case TYPE_ORGAN_HEART:		
			entity = Entity("organ", "coeur", id, x,y);
			absTimer_->start();
		break;
		case TYPE_ORGAN_LIVER:		
			entity = Entity("organ", "foie", id, x,y);
			absTimer_->start();
		break;
		case TYPE_ORGAN_LUNG:		
			entity = Entity("organ", "poumon", id, x,y);
			absTimer_->start();
		break;
		case TYPE_BULLET:
			entity = Entity("bullet", "bullet", id, x,y, dx,dy);
		break;
		case TYPE_SHOTGUN_BULLET:
			entity = Entity("shotgun_bullet", "shotgun_bullet", id, x,y, dx,dy);
		break;
		case TYPE_MINE:
			entity = Entity("mine", "mine", id, x,y, dx,dy);
		break;
    	case TYPE_BLACK_BULLET:
		    entity = Entity("blackBullet", "blackBullet", id, x, y, dx, dy);
		break;
    	case TYPE_GREEN_BULLET:
		    entity = Entity("greenBullet", "greenBullet", id, x, y, dx, dy);
		break;
    	case TYPE_PURPLE_BULLET:
		    entity = Entity("purpleBullet", "purpleBullet", id, x, y, dx, dy);
		break;
    	case TYPE_GRENADE:
		    entity = Entity("grenade", "grenade", id, x, y, dx, dy);
		break;
    	case TYPE_LAVA_BALL:
		    entity = Entity("lavaball", "lavaball", id, x, y, dx, dy);
		break;
		case TYPE_GREEN_MONSTER:
			entity = Entity("virus1a", "virus1a", id, x,y, dx,dy, 4, true);
		break;
		case TYPE_BLUE_MONSTER:
			entity = Entity("virus2", "virus2", id, x,y, dx,dy);
		break;
		case TYPE_UGLY_MONSTER:
			entity = Entity("virus3", "virus3", id, x,y, dx,dy);
		break;
	    case TYPE_HUGE_MONSTER:
			entity = Entity("virus4", "virus4", id, x,y, dx,dy);
		break;
	    case TYPE_RED_MONSTER:
			entity = Entity("virus5", "virus5", id, x,y, dx,dy);
		break;
		case TYPE_TOWER_HURT:
			entity = Entity("tower-hurt", "tower-hurt", id, x,y, dx,dy);
		break;
		case TYPE_TOWER_HEAL:
			entity = Entity("tower-heal", "tower-heal", id, x,y, dx,dy);
		break;
		case TYPE_TOWER_SLOW:
			entity = Entity("tower-slow", "tower-slow", id, x,y, dx,dy);
		break;
		case TYPE_TOWER_POISON:
			entity = Entity("tower-poison", "tower-poison", id, x,y, dx,dy);
		break;
		case TYPE_TOWER_SAND:
			entity = Entity("tower-sand", "tower-sand", id, x,y, dx,dy);
		break;
		case TYPE_EXPLOSION:
			entity = Entity("explosion", "explosion", id, x,y, dx,dy,8, true);
		break;
	    case TYPE_EGG:
		   entity = Entity("egg", "egg", id, x, y, dx, dy, 1, false, false);
		break;
		case TYPE_HEALING_EXPLOSION:
			entity = Entity("healing", "healing", id, x,y, dx,dy,5, true);
		break;
		case TYPE_SHRAPNEL:
			entity = Entity("shrapnel", "shrapnel", id, x,y, dx,dy);
		break;
	    case TYPE_HEALING_BULLET:
			entity = Entity("healing_bullet", "healing_bullet", id, x,y, dx,dy);
		break;
		default:
			throw std::runtime_error("unknown img type...");
	}
	return entity;
}

void SceneManager::addPlayerName()
{
	ClientNetworkManager* client = ClientNetworkManager::getInstance();	
	sf::Uint32 id;
	std::string name;
	sf::Packet packet, tmp;
	tmp.Clear();
	//receiving the packet
	client->receive(packet);
	packet >> id >> name;
	//creating the storing the SDL_Surface with the player name on it
	if(myPid==id)
		playerNameMap_.insert(std::pair<int, SDL_Surface*>(id, TTF_RenderText_Solid( fontB_, name.c_str(), textColor_ )));
	else
		playerNameMap_.insert(std::pair<int, SDL_Surface*>(id, TTF_RenderText_Solid( font_, name.c_str(), textColor_ )));
}

void SceneManager::setPlayerName(std::string name)
{
	name_ = name;
}

void SceneManager::showPlayerRelatedData()
{
	SDL_Rect offset;
	SDL_Surface * text = NULL;
	char buffer [30];
	//weapons dock
	offset.x = (SCREEN_WIDTH - imageLoader_->getImage("dock")->w)/2;
	offset.y = SCREEN_HEIGHT-imageLoader_->getImage("dock")->h;
	SDL_BlitSurface(imageLoader_->getImage("dock"),NULL, screen_, &offset );
	//selected weapon
	offset.x=(SCREEN_WIDTH - imageLoader_->getImage("dock")->w-381) + (63 * equipedWeapon_ + 1);
	offset.y+=16;
	SDL_BlitSurface(imageLoader_->getImage("selected"),NULL, screen_, &offset );
	offset.y+=4;
	int cpt=1;
	//all weapons
	for(std::list<std::string>::iterator i= classWeapons_.begin();i!=classWeapons_.end();i++)
	{
		offset.x=((SCREEN_WIDTH - imageLoader_->getImage("dock")->w)-420) + (63*cpt) - 16;
		SDL_BlitSurface(imageLoader_->getImage(*i),NULL, screen_, &offset );
		if(! playerWeapons[cpt-1])
		{
			offset.x=((SCREEN_WIDTH - imageLoader_->getImage("dock")->w)/2) + (63*cpt) - 16;
			SDL_BlitSurface(imageLoader_->getImage("lock"),NULL, screen_, &offset );
		}
		cpt++;
	}
	//ADN image
	offset.x = SCREEN_WIDTH - imageLoader_->getImage("ADN")->w - 50 ;
	offset.y = SCREEN_HEIGHT - imageLoader_->getImage("ADN")->h;
	SDL_BlitSurface(imageLoader_->getImage("ADN"),NULL, screen_, &offset );	
	//HP image
	offset.x = 0 ;
	offset.y = SCREEN_HEIGHT - imageLoader_->getImage("HP")->h;
	SDL_BlitSurface(imageLoader_->getImage("HP"),NULL, screen_, &offset );		
	//ADN points
	offset.x = SCREEN_WIDTH - 50;
	offset.y = SCREEN_HEIGHT - 30;
	sprintf (buffer, "%d", playerADN_);
	text = TTF_RenderText_Solid( fontB_, buffer, textColor_ );
	SDL_BlitSurface(text, NULL, screen_, &offset );
	SDL_FreeSurface(text);
	//HP points
	offset.x = 50 ;
	offset.y = SCREEN_HEIGHT - 30;
	sprintf (buffer, "%d", playerHP_);
	text = TTF_RenderText_Solid( fontB_, buffer, textColor_ ); 
	SDL_BlitSurface(text, NULL, screen_, &offset );
	SDL_FreeSurface(text);
	//Showing game time
	AbsTimer* absTimer_ = AbsTimer::getInstance();
	offset.x = SCREEN_WIDTH - 185 ;
	offset.y = 10;
	int time= (absTimer_->getTicks()/1000);
	int min = time/60;
	if(min==0)
		sprintf (buffer, "Temps de Jeu: %d sec", time);
	else
	{
		sprintf (buffer, "Temps de Jeu: %d m %d s",min ,time%60);
	}
	text = TTF_RenderText_Solid( fontB_, buffer, textColor_ );
	SDL_BlitSurface(text, NULL, screen_, &offset );
	SDL_FreeSurface(text);
	//Kill count
	offset.x=0;
	offset.y=10;
	sprintf (buffer, "Victimes: %d", playerKills_);
	text = TTF_RenderText_Solid( fontB_, buffer, textColor_ );
	SDL_BlitSurface(text ,NULL, screen_, &offset );
	SDL_FreeSurface(text);
	//Wave approache message 
	if(absTimer_->getTicks()-waveMsgTick_ < 4000)
	{
		text=TTF_RenderText_Solid( fontB_, waveMsg_.c_str(), textColor_) ;
		offset.x = (SCREEN_WIDTH/2)-250;
		offset.y = 30;
		SDL_BlitSurface(text,NULL, screen_, &offset );
		SDL_FreeSurface(text);
	}
}

void SceneManager::updateWaveMessage() 
{
	ClientNetworkManager* client = ClientNetworkManager::getInstance();	
	sf::Packet package;
	//receiving the package
	client->receive(package);
	sf::Uint8 nb ;
	package >>nb;
	//updating the message
	waveMsg_ = "Attention : nouvelle vague de virus en approche";
	for(sf::Uint8 i = 0; i<nb; i++)
	{
		std::string position;
		package >> position;
		if(position == "NW")
			waveMsg_ += " par le Nord-Ouest";
		else if(position == "NE")
			waveMsg_ += " par le Nord-Est";
		else if(position == "SE")
			waveMsg_ += " par le Sud-Est";
		else if(position == "SW")
			waveMsg_ += " par le Sud-Ouest";
	}
		
	waveMsgTick_=AbsTimer::getInstance()->getTicks();
}

void SceneManager::updateScore() 
{
	ClientNetworkManager* client = ClientNetworkManager::getInstance();	
	sf::Packet package;
	//receiving the package
	client->receive(package);
	sf::Uint16 score;
	package >> score;
	score++;
	char buffer[30];
	//new score message
	sprintf (buffer, "Votre Score: %d points", (int)score);
	scoreMsg_=buffer;
	std::cout<<scoreMsg_<<std::endl;
}

Entity* SceneManager::myPlayer()
{
   std::map<int, Entity>::iterator i = unitMap_.find(myPid);
   if (i == unitMap_.end())
	  return NULL;
   return &(i->second);
}

int SceneManager::getCameraX()
{
   if (!myPlayer())
	  return 0;
   int x = myPlayer()->x_ - SCREEN_WIDTH/2;
   x = max(x, 0);
   x = min(x, static_cast<int>(MAP_WIDTH - SCREEN_WIDTH));
   return x;
}

int SceneManager::getCameraY()
{
   if (!myPlayer())
	  return 0;
   int y = myPlayer()->y_ - SCREEN_HEIGHT/2;
   y = max(y, 0);
   y = min(y, static_cast<int>(MAP_HEIGHT - SCREEN_HEIGHT));
   return y;
}

void SceneManager::drawClassChoice()
{
	SDL_Rect offset;
	offset.x =0;
	offset.y =0;
	SDL_BlitSurface( imageLoader_->getImage("backgroundChoixClass"), NULL, screen_, &offset);
	offset.x=((SCREEN_WIDTH- imageLoader_->getImage("classes")->w)/2);
	offset.y=((SCREEN_HEIGHT - imageLoader_->getImage("classes")->h)/2);
	SDL_BlitSurface( imageLoader_->getImage("classes"), NULL, screen_, &offset);
	SDL_Flip( screen_ );
	
}

void SceneManager::updateEquipedWeapon()
{
	ClientNetworkManager* client = ClientNetworkManager::getInstance();	
	sf::Packet package;
	client->receive(package);
	sf::Uint8 weapon;
	package>>weapon;
	equipedWeapon_ = weapon;
	if(!playerWeapons[weapon])
		playerWeapons[weapon]=true;
	
}
void SceneManager::setPlayerClass(int pc)
{
	//looking for the players choice
	std::string name, prop;
	playerClass_ = pc;
	if(pc==PLAYER_HEALER)
		name="healer";
	else if (pc==PLAYER_BUILDER)
		name="builder";
	else
		name="fighter";
	//loading according data
	string fn = DATA_DIR + name + ".class";
	ifstream fs(fn.c_str());
	while (fs >> prop)
	{
		if (prop == "weaponlist:")
		{
			fs >> prop;
			while (prop != "endlist")
			{
				classWeapons_.push_back(prop);
				fs >> prop;
			}
		}
	}  
}
