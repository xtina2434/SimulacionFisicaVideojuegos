#ifndef GAME_H
#define GAME_H

#include "core.hpp"
#include "RenderUtils.hpp"
#include <ctype.h>
#include <PxPhysicsAPI.h>
#include <iostream>
#include <list>


#include "ParticlesSystem.h"
using namespace physx;

extern std::string title_text;
extern std::string next_text;
extern std::string intro_text;
extern std::string intro_text2;
extern std::string intro_text3;
class Game {
public:
	Game(PxPhysics* gPhysics, PxMaterial* gMaterial, PxScene* gScene);
	~Game();

	void update(double t);
	void keyPress(unsigned char key);
	

private:
	PxPhysics* gPhysics = NULL;
	PxMaterial* gMaterial = NULL;
	PxScene* gScene = NULL;


	void titleScene();
	void introScene();
	void snowScene();
	void changeScene();
	void clearScene();

	std::vector<std::string> texts_scene;
	
	enum Scenes {
		START = 0,
		INTRO,
		SNOW,
		TOTAL
	};
	Scenes current;

	std::list<ParticlesSystem*> systems;
};

#endif