#ifndef GAME_H
#define GAME_H

#include "core.hpp"
#include "RenderUtils.hpp"
#include <ctype.h>
#include <PxPhysicsAPI.h>
#include <iostream>
#include <list>
#include <random>

#include "ParticlesSystem.h"
#include "Particle.h"
#include "Diana.h"
using namespace physx;

extern std::string title_text;
extern std::string next_text;
extern std::string intro_text;
extern std::string intro_text2;

constexpr double SPAWN_DIANA = 1.5;
constexpr double MAX_Z = 100.0;
constexpr double MIN_Z = -100.0;
constexpr double MAX_Y = 100.0;
constexpr double MIN_Y = 10.0;

constexpr double COOLDOWN = 0.5;
class Game {
private:
	PxPhysics* gPhysics = NULL;
	PxMaterial* gMaterial = NULL;
	PxScene* gScene = NULL;

	enum Scenes {
		START = 0,
		INTRO,
		SNOW,
		TOTAL
	};
	Scenes current;

	std::list<Particle*> particles;
	std::list<ParticlesSystem*> systems;

	double elapsedTime;
	std::random_device rd;
	std::mt19937 mt;
	Diana* diana = nullptr;

	double cooldownTime;
	bool can_shoot = true;

public:
	Game(PxPhysics* gPhysics, PxMaterial* gMaterial, PxScene* gScene);
	~Game();

	void update(double t);
	void keyPress(unsigned char key);
	void handleMouse(int button, int state, int x, int y);

private:
	void titleScene();
	void introScene();
	void snowScene();

	void changeScene();
	void clearScene();


	void setDiana();
	
};

#endif