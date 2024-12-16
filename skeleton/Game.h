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
#include "SpringForceGenerator.h"
#include "ExplosionForceGenerator.h"
using namespace physx;

//textos que se van a usar durante el juego
extern std::string title_text;
extern std::string next_text;
extern std::string intro_text;
extern std::string intro_text2;
extern std::string input_text;
extern std::string points_text;
extern std::string lives_text;
extern std::string intro_text3;
extern std::string intro_text4;
extern std::string respawn_text;
extern std::string lost_text;
extern std::string win_text;
extern std::string exit_text;

//constantes globales
constexpr double SPAWN_DIANA = 1.5;		//tiempo entre que spawnea una diana y la siguiente	
constexpr int DIANA_POINTS = 5;			//puntos que da si se acierta una diana
constexpr int TOTAL_DIANAS = 10;		//numero total de dianas que spawnean en el nivel
//posiciones maximas y minimas de spawn de dianas
constexpr double MAX_Z = 100.0;
constexpr double MIN_Z = -100.0;
constexpr double MAX_Y = 100.0;
constexpr double MIN_Y = 10.0;

constexpr double COOLDOWN = 0.5;		//tiempo entre disparo y disparo
constexpr double RESPAWN_TIME = 3.0;	//tiempo que tarda en volver a empezar el nivel

class Game {
private:
	Camera* cam = GetCamera();			//camara del juego

	PxPhysics* gPhysics = NULL;
	PxMaterial* gMaterial = NULL;
	PxScene* gScene = NULL;

	//nombre de las escenas del juego
	enum Scenes {
		START = 0,
		INTRO,
		SNOW,
		INTRO2,
		LEVEL1,
		LEVEL2,
		LEVEL3,
		RESPAWN,
		WIN,
		LOST,
		TOTAL
	};

	Scenes current;									//escena actual
	Scenes last_scene;								//escena anterior

	//listas de items, particulas, sistemas, solidos rigidos dinamicos/estaticos
	std::list<RenderItem*> items;
	std::list<Particle*> particles;
	std::list<ParticlesSystem*> systems;
	std::list<RigidSolid*> rigid_solids;
	std::list<RigidStatic*> rigid_statics;
	std::list<RigidSolidSystem*> rigid_systems;

	std::random_device rd;
	std::mt19937 mt;

	Diana* diana = nullptr;			
	RigidSolid* player = nullptr;					//jugador es un solido rigido

	double dianaElapsedTime;						//gestiona el tiempo para el spawn de dianas

	bool canJump = false;							//para saber si el jugador puede saltar

	double cooldownTime;							//gestiona el tiempo para el cooldown de disparo
	bool can_shoot = true;							//para saber si el jugador puede disparar

	double respawnTime;								//gestiona el tiempo para el respawn del nivel
	bool dying = false;								//para saber si el jugador esta muriendo
	bool wait_restart = false;						//para saber si el jugador esta esperando para el respawn

	bool next_level = false;						//gestiona si el jugador pasa de nivel

	int cont_dianas = 0;							//contador de dianas que llevan spawneadas
	int points = 0;									//contador de puntos obtenidos en el nivel de dianas
	int lives = 0;									//contador de vidas restantes del jugador
public:
	Game(PxPhysics* gPhysics, PxMaterial* gMaterial, PxScene* gScene);
	~Game();

	void update(double t);
	void keyPress(unsigned char key);
	void handleMouse(int button, int state, int x, int y);
	void onCollision(physx::PxActor* actor1, physx::PxActor* actor2);
private:
	//creacion de las diferentes escenas
	void titleScene();
	void introScene();
	void snowScene();
	void intro2Scene();
	void level1Scene();
	void level2Scene();
	void level3Scene();
	void respawnScene();
	void lostScene();
	void winScene();
	//gestiona cambio de escena
	void changeScene();	
	//limpia la escena
	void clearScene();	
	//cambia la diana de posicion en pantalla
	void setDiana();
	//crea el jugador
	void createPlayer();
	//crea muelles
	void createDock(const Vector3& pos, double k, const Vector4& color4);
};

#endif