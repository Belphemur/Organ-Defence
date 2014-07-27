#ifndef _IMAGELOADER_H
#define _IMAGELOADER_H

#include <string>
#include <map>
#include "../sdl_include.hpp"


#include "Logger.hpp"

class ImageLoader
{
public:
	ImageLoader(void){};
	/**
     * Image loading and optimization
     */
	virtual void loadImage(std::string name, std::string filepath)
	{
		Logger* logger =  Logger::getInstance();
		filepath += name;
		filepath += ".png";										//get the file path
		SDL_Surface* loadedImage = NULL; 
		SDL_Surface* optimizedImage = NULL; 
		loadedImage = IMG_Load( filepath.c_str() ); 
		if( loadedImage != NULL )							
		{ 
			optimizedImage = SDL_DisplayFormatAlpha( loadedImage ); //optimizing the image
			SDL_FreeSurface( loadedImage );						//deleting the old image
			logger->log(filepath+" loaded");
			if(name != "background")
				SDL_SetColorKey( optimizedImage, SDL_SRCCOLORKEY, SDL_MapRGB( optimizedImage->format, 255, 143, 143 ) );
				
			mapWithSurfaces_.insert(std::pair<std::string, SDL_Surface*>(name,optimizedImage));	//inserting optimized image into the map
		}
		else
		{
			logger->log(filepath+" could not be found");
			logger->log( IMG_GetError());		//SDL_image error
		}
	}
	/**
     * Getting the image
     * @return SDL_Surface with the image
     */
	virtual SDL_Surface *getImage(std::string name)
	{
		if(mapWithSurfaces_[name] != NULL)
			return mapWithSurfaces_[name];
		Logger* logger =  Logger::getInstance();
		logger->log(name+" could not be drawn because the image was not loaded!");
		return NULL;
		
	}
	/**
     * Destroying all images
     */
	virtual void drop()
	{	
		Logger* logger =  Logger::getInstance();
		for ( mapIterator_ = mapWithSurfaces_.begin() ; mapIterator_ != mapWithSurfaces_.end(); mapIterator_++ )
		{
			SDL_FreeSurface( mapIterator_->second );			//deleting the images for SDL
			logger->log(mapIterator_->first+" surface deleted");
		}
		mapWithSurfaces_.clear();										//clearing the map
		logger->log("Image Loader map is cleared");
	}
	virtual ~ImageLoader(void){};
private:
	std::map<std::string, SDL_Surface*> mapWithSurfaces_;
	std::map<std::string, SDL_Surface*>::iterator mapIterator_;


};

#endif
