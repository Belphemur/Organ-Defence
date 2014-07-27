#ifndef __C_INITS_HPP_
#define __C_INITS_HPP_

#include <string>

std::string getHostname(int argc, char** argv);
std::string getPlayername(int argc, char** argv);

void initSDL();
void initTTF();
void initClientNetworkManager();
void initEventListener();
void initSceneManager();
void initController();

#endif
