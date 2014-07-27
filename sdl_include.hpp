#ifndef _SDL_INCLUDE_HPP
#ifdef __APPLE__
#include <SDL/SDL.h>
#include <SDL_image/SDL_image.h>
#include <SDL_ttf/SDL_ttf.h>
#else
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#endif

#define _T std::cout << __FILE__ << "::" << __LINE__ << std::endl;
#endif