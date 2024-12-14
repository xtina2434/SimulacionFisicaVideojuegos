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
#include "RigidSolid.h"
#include "RigidStatic.h"
#include "RigidSolidSystem.h"
using namespace physx;

extern std::string title_text;
extern std::string next_text;
extern std::string intro_text;
extern std::string intro_text2;
extern std::string points_text;
extern std::string lives_text;
extern std::string intro_text3;
extern std::string intro_text4;

constexpr double SPAWN_DIANA = 1.5;
constexpr double MAX_Z = 100.0;
constexpr double MIN_Z = -100.0;
constexpr double MAX_Y = 100.0;
constexpr double MIN_Y = 10.0;

constexpr double COOLDOWN = 0.5;
constexpr double RESPAWN = 3.0;

constexpr int DIANA_POINTS = 5;
constexpr int TOTAL_DIANAS = 1;
class Game {
private:
	Camera* cam = GetCamera();

	PxPhysics* gPhysics = NULL;
	PxMaterial* gMaterial = NULL;
	PxScene* gScene = NULL;

	enum Scenes {
		START = 0,
		INTRO,
		SNOW,
		INTRO2,
		LEVEL1,
		LEVEL2,
		RESPAWN,
		LOST,
		TOTAL
	};
	Scenes current;
	Scenes last_scene;
	std::list<Particle*> particles;
	std::list<ParticlesSystem*> systems;
	std::list<RigidSolid*> rigid_solids;
	std::list<RigidStatic*> rigid_statics;
	std::list<RigidSolidSystem*> rigid_systems;

	double elapsedTime;

	std::random_device rd;
	std::mt19937 mt;

	Diana* diana = nullptr;
	RigidSolid* player = nullptr;

	bool canJump = false;

	double cooldownTime;
	bool can_shoot = true;

	double respawnTime;
	bool dying = false;

	bool next_level = false;

	int cont_dianas = 0;
	int points = 0;
	int lives = 0;
public:
	Game(PxPhysics* gPhysics, PxMaterial* gMaterial, PxScene* gScene);
	~Game();

	void update(double t);
	void keyPress(unsigned char key);
	void handleMouse(int button, int state, int x, int y);
	void onCollision(physx::PxActor* actor1, physx::PxActor* actor2);
private:
	void titleScene();
	void introScene();
	void snowScene();
	void intro2Scene();
	void level1();
	void level2();
	void level3();
	void respawn();
	void lost();
	void win();

	void changeScene();
	void clearScene();


	void setDiana();
	void createPlayer();
};

#endif