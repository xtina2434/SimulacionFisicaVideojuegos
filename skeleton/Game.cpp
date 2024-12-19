#include "Game.h"

Game::Game(PxPhysics* _gPhysics, PxMaterial* _gMaterial, PxScene* _gScene) :
	gPhysics(_gPhysics), gMaterial(_gMaterial), gScene(_gScene), mt(rd())
{
	//empieza en la escena del titulo
	current = START;
	titleScene();
}
Game::~Game()
{
	for (auto it = systems.begin(); it != systems.end(); ++it) {
		if (*it != nullptr) {
			delete* it;
		}
	}
	systems.clear();
	for (auto it = rigid_systems.begin(); it != rigid_systems.end(); ++it) {
		if (*it != nullptr) {
			delete* it;
		}
	}
	rigid_systems.clear();
	for (auto it = particles.begin(); it != particles.end(); ++it) {
		if (*it != nullptr) {
			delete* it;
		}
	}
	particles.clear();
	for (auto it = rigid_solids.begin(); it != rigid_solids.end(); ++it) {
		if (*it != nullptr) {
			delete* it;
		}
	}
	rigid_solids.clear();
	for (auto it = rigid_statics.begin(); it != rigid_statics.end(); ++it) {
		if (*it != nullptr) {
			delete* it;
		}
	}
	rigid_statics.clear();
	for (auto it = items.begin(); it != items.end(); ++it) {
		if (*it != nullptr) {
			DeregisterRenderItem(*it);
		}
	}
	items.clear();
}
void Game::update(double t)
{
	//en la escena de nieve, cuando se llega al tiempo SPAWN_DIANA 
	//se llama al metodo que cambia la diana de posicion
	if (current == SNOW) {
		dianaElapsedTime += t;
		if (dianaElapsedTime >= SPAWN_DIANA) {
			setDiana();
			dianaElapsedTime = 0;	//reiniciar el tiempo a 0
		}
	}
	//si acaba de disparar espera el cooldown para volver a poder
	if (!can_shoot) {
		cooldownTime += t;
		if (cooldownTime >= COOLDOWN) {
			can_shoot = true; //puede volver a disparar
		}
	}
	//si esta muriendo se espera a llegar al tiempo RESPAWN_TIME para cambiar a la escena correspondiente
	if (dying) {
		respawnTime += t;
		if (respawnTime >= RESPAWN_TIME) {
			dying = false;
			changeScene();
		}

	}
	//si esta esperando a respawnear espera al tiempo RESPAWN_TIME para cambiar a la escena correspondiente
	if (wait_restart) {
		respawnTime += t;
		if (respawnTime >= RESPAWN_TIME) {
			wait_restart = false;
			changeScene();
		}

	}
	//update de particulas, sistemas, solidos rigidos dinamicos/estaticos
	for (auto it = systems.begin(); it != systems.end(); ++it) {
		if (*it != nullptr) {
			(*it)->update(t);
		}
	}
	for (auto it = rigid_systems.begin(); it != rigid_systems.end(); ++it) {
		if (*it != nullptr) {
			(*it)->update(t);
		}
	}
	for (auto it = particles.begin(); it != particles.end();) {
		(*it)->integrate(t);
		(*it)->update(t);
		//comprobar si siguen vivas, si no es asi se elimina
		if (!(*it)->isAlive()) {
			delete (*it);
			it = particles.erase(it);
		}
		else {
			++it;
		}
	}
	for (auto it = rigid_solids.begin(); it != rigid_solids.end();) {
		(*it)->integrate(t);
		(*it)->update(t);
		//comprobar si siguen vivos, si no es asi se elimina
		if (!(*it)->isAlive()) {
			delete (*it);
			it = rigid_solids.erase(it);
		}
		else {
			++it;
		}
	}
}
void Game::titleScene()
{
	//inicializar los textos de la escena
	title_text = "CHAOS RUN";
	next_text = "Pulse c para continuar";
}
void Game::introScene()
{
	//inicializar los textos de la escena
	intro_text = "Enhorabuena eres el/la finalista de CHAOS RUN. Ahora tienes que ser capaz de superar la prueba final para ganar los 6 creditos";
	intro_text2 = "Para determinar tus vidas para la prueba final tienes que acertar el mayor numero de dianas. Por cada 10 puntos conseguiras una vida.";
}
void Game::snowScene() {

	//inicializar los textos de la escena
	points_text = "Puntos: " + to_string(points);
	//reiniciar variables de spawn y contador de dianas
	cont_dianas = 0;
	dianaElapsedTime = 0;
	//cambiar la camara de posicion
	if (cam != nullptr) {
		cam->setTransform(PxTransform({ 100, 54, 2 }), PxVec3(-1, 0, 0));
	}
	//solido estatico suelo blanco
	RigidStatic* suelo = new RigidStatic(gPhysics, gScene, Vector3(0, 0, 0), Vector3(200, 0.1, 200), Vector4(1.0, 1.0, 1.0, 1.0), "BOX", "suelo");
	rigid_statics.push_back(suelo);
	//sistema de particulas con distribuciones uniformes, con gravedad, que simula nieve
	ParticlesSystem* snow_system = new ParticlesSystem(Vector4(1.0, 1.0, 1.0, 1.0), Vector3(0.0, 80.0, 0), Vector3(0.0, -2.0, 0.0), 1, 0.5f, 0.5f, 0.0f, 0.0001f);
	snow_system->set_u_Distribution(true);
	snow_system->setUniformDistribPos(-100.0, 100.0);
	snow_system->setUniformDistribVel(1.0, 3.0);
	snow_system->setNormalDistribLifeTime(5.0, 3.0);
	snow_system->setGravityForce();
	systems.push_back(snow_system);
}
void Game::intro2Scene()
{
	//se calculan las vidas, cada 10 puntos 1 vida
	lives = points / 10;
	//si no se han conseguido sufieciente puntos, se da 1 vida
	if (lives == 0) lives = 1;
	//inicializar los textos de la escena
	next_text = "Pulse c para continuar";
	intro_text3 = "Lo has hecho genial, por hacer " + to_string(points) + " puntos tienes " + to_string(lives) + " vida(s).";
	intro_text4 = "Ahora tienes que superar los 3 niveles finales. Esquiva los obstaculos y evita caer a la lava.";
	input_text = "Con 'W' saltas, con 'D' avanzas y con 'A' retrocedes.";
}
void Game::level1Scene()
{
	//inicializar los textos de la escena
	lives_text = "Vidas: " + to_string(lives);
	//se cambia la posicion de la camara a la inicial
	if (cam != nullptr) {
		cam->resetInitialTransform();
	}
	//solido rigido estatico que simula lava
	RigidStatic* lava = new RigidStatic(gPhysics, gScene, Vector3(0, 0, 0), Vector3(200, 0.1, 200), Vector4(1.0, 0.1, 0.0, 1.0), "BOX", "lava");
	rigid_statics.push_back(lava);
	//crea el jugador
	createPlayer();
	//se crea el escenario con solidos rigidos estaticos
	RigidStatic* plataforma = new RigidStatic(gPhysics, gScene, Vector3(0, 30, 0), Vector3(100, 10, 4), Vector4(0.0, 0.6, 0.8, 1), "BOX", "plataforma");
	rigid_statics.push_back(plataforma);
	RigidStatic* plataforma2 = new RigidStatic(gPhysics, gScene, Vector3(0, 30, -50), Vector3(100, 10, 4), Vector4(0.0, 0.0, 0.0, 1), "BOX", "plataforma");
	rigid_statics.push_back(plataforma2);
	RigidStatic* flag = new RigidStatic(gPhysics, gScene, Vector3(40, 40, -0), Vector3(3, 0.1, 3), Vector4(0.0, 1.0, 0.0, 1), "BOX", "flag");
	rigid_statics.push_back(flag);
	RigidStatic* canion1 = new RigidStatic(gPhysics, gScene, Vector3(-60, 43, -50), Vector3(3, 3, 3), Vector4(0.4, 0.4, 0.4, 1), "BOX", "objeto");
	rigid_statics.push_back(canion1);
	RigidStatic* canion2 = new RigidStatic(gPhysics, gScene, Vector3(-30, 43, -50), Vector3(3, 3, 3), Vector4(0.4, 0.4, 0.4, 1), "BOX", "objeto");
	rigid_statics.push_back(canion2);
	RigidStatic* canion3 = new RigidStatic(gPhysics, gScene, Vector3(0, 43, -50), Vector3(3, 3, 3), Vector4(0.4, 0.4, 0.4, 1), "BOX", "objeto");
	rigid_statics.push_back(canion3);

	//sistemas de solidos rigidos dinamicos con distribuciones normales, 
	//sin gravedad, con diferentes velocidades lineales en el eje z
	RigidSolidSystem* s1 = new RigidSolidSystem(gPhysics, gScene, Vector3(-60, 42, -45), Vector3(0, 0, 100), Vector3(0, 0, 0),
		Vector3(1, 1, 1), Vector4(1.0f, 0.0f, 0.0f, 1.0f), 1, 0.01, 2000.0f, "SPHERE");
	s1->quitGravity();
	s1->set_u_Distribution(false);
	s1->setNormalDistribPos(0.01, 0.01);
	s1->setNormalDistribAngularVel(0.01, 0.01);
	s1->setNormalDistribLinealVel(0.01, 0.01);
	s1->setNormalDistribLifeTime(2.0, 0.1);
	rigid_systems.push_back(s1);

	RigidSolidSystem* s2 = new RigidSolidSystem(gPhysics, gScene, Vector3(-30, 42, -45), Vector3(0, 0, 150), Vector3(0, 0, 0),
		Vector3(1, 1, 1), Vector4(0.0f, 1.0f, 0.0f, 1.0f), 1, 0.01, 2000.0f, "SPHERE");
	s2->quitGravity();
	s2->set_u_Distribution(false);
	s2->setNormalDistribPos(0.01, 0.01);
	s2->setNormalDistribAngularVel(0.01, 0.01);
	s2->setNormalDistribLinealVel(0.01, 0.01);
	s2->setNormalDistribLifeTime(2.0, 0.1);
	rigid_systems.push_back(s2);

	RigidSolidSystem* s3 = new RigidSolidSystem(gPhysics, gScene, Vector3(0, 42, -45), Vector3(0, 0, 200), Vector3(0, 0, 0),
		Vector3(1, 1, 1), Vector4(0.0f, 0.0f, 1.0f, 1.0f), 1, 0.01, 2000.0f, "SPHERE");
	s3->quitGravity();
	s3->set_u_Distribution(false);
	s3->setNormalDistribPos(0.01, 0.01);
	s3->setNormalDistribAngularVel(0.01, 0.01);
	s3->setNormalDistribLinealVel(0.01, 0.01);
	s3->setNormalDistribLifeTime(2.0, 0.1);
	rigid_systems.push_back(s3);
}
void Game::level2Scene()
{
	//inicializar los textos de la escena
	lives_text = "Vidas: " + to_string(lives);
	//solido rigido estatico que simula lava
	RigidStatic* lava = new RigidStatic(gPhysics, gScene, Vector3(0, 0, 0), Vector3(200, 0.1, 200), Vector4(1.0, 0.1, 0.0, 1.0), "BOX", "lava");
	rigid_statics.push_back(lava);
	//se crea al jugador
	createPlayer();
	//se crea el escenario con solidos rigidos estaticos
	RigidStatic* plataforma = new RigidStatic(gPhysics, gScene, Vector3(-80, 28, 0), Vector3(10, 5, 5), Vector4(1.0, 0.0, 0.0, 1), "SPHERE", "plataforma");
	rigid_statics.push_back(plataforma);
	RigidStatic* plataforma2 = new RigidStatic(gPhysics, gScene, Vector3(-30, 28, 0), Vector3(10, 5, 5), Vector4(1.0, 0.0, 0.0, 1), "SPHERE", "plataforma");
	rigid_statics.push_back(plataforma2);
	RigidStatic* plataforma3 = new RigidStatic(gPhysics, gScene, Vector3(20, 28, 0), Vector3(10, 5, 5), Vector4(1.0, 0.0, 0.0, 1), "SPHERE", "plataforma");
	rigid_statics.push_back(plataforma3);
	RigidStatic* flag = new RigidStatic(gPhysics, gScene, Vector3(20, 38, -0), Vector3(4, 0.1, 4), Vector4(0.0, 1.0, 0.0, 1), "BOX", "flag"); 
	rigid_statics.push_back(flag); 

	//sistemas de solidos rigidos dinamicos, con distribuciones normales, 
	//sin gravedad, que lleva integrado el generador de fuerzas de un torbellino
	//uno son esferas y otro cubos
	RigidSolidSystem* whirlwind_solid_system = new RigidSolidSystem(gPhysics, gScene, 
		Vector3(-50, 30, 0), Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(0.5, 1, 1), Vector4(1, 1, 0, 1), 
		1, 0.1f, 5.0f, "SPHERE"); 
	whirlwind_solid_system->set_u_Distribution(false); 
	whirlwind_solid_system->quitGravity(); 
	whirlwind_solid_system->setNormalDistribPos(3.0, 1.0); 
	whirlwind_solid_system->setNormalDistribLinealVel(5.0, 3.0); 
	whirlwind_solid_system->setNormalDistribAngularVel(0.0, 0.01); 
	whirlwind_solid_system->setNormalDistribLifeTime(3.0, 2.0); 
	whirlwind_solid_system->setWhirlWindForce(50.0f, 1.0f, 1.0f);
	rigid_systems.push_back(whirlwind_solid_system); 

	RigidSolidSystem* whirlwind_solid_system2 = new RigidSolidSystem(gPhysics, gScene,
		Vector3(0, 30, 0), Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(0.5, 0.5, 0.5), Vector4(0, 0, 1, 1),
		1, 0.1f, 5.0f, "BOX");
	whirlwind_solid_system2->set_u_Distribution(false);
	whirlwind_solid_system2->quitGravity();
	whirlwind_solid_system2->setNormalDistribPos(3.0, 1.0);
	whirlwind_solid_system2->setNormalDistribLinealVel(8.0, 3.0);
	whirlwind_solid_system2->setNormalDistribAngularVel(0.0, 0.01);
	whirlwind_solid_system2->setNormalDistribLifeTime(3.0, 2.0);
	whirlwind_solid_system2->setWhirlWindForce(50.0f, 1.0f, 1.0f);
	rigid_systems.push_back(whirlwind_solid_system2);
}
void Game::level3Scene()
{	
	//inicializar los textos de la escena
	lives_text = "Vidas: " + to_string(lives);
	//solido rigido estatico que simula lava
	RigidStatic* lava = new RigidStatic(gPhysics, gScene, Vector3(0, 0, 0), Vector3(200, 0.1, 200), Vector4(1.0, 0.1, 0.0, 1.0), "BOX", "lava");
	rigid_statics.push_back(lava);
	//se crea el escenario con solidos rigidos estaticos
	RigidStatic* plataforma = new RigidStatic(gPhysics, gScene, Vector3(0, 30, -3), Vector3(100, 10, 6), Vector4(0.0, 0.6, 0.8, 1), "BOX", "plataforma");
	rigid_statics.push_back(plataforma);
	RenderItem* pared = new RenderItem(CreateShape(physx::PxBoxGeometry(200, 50, 5)),
		new physx::PxTransform(0, 30, -12), Vector4(0.0, 0.0, 0.0, 1));
	items.push_back(pared);
	RigidStatic* flag = new RigidStatic(gPhysics, gScene, Vector3(40, 40, -3), Vector3(3, 0.1, 3), Vector4(0.0, 1.0, 0.0, 1), "BOX", "flag");
	rigid_statics.push_back(flag);
	//se crea el jugador
	createPlayer();
	//se crean diferentes muelles
	createDock(Vector3(-64.0, 43.0, -5.0), 2500.0, Vector4(1.0, 0.0, 0.0, 1), 0.5);
	createDock(Vector3(-44.0, 52.0, -5.0), 2750.0, Vector4(1.0, 0.5, 0.0, 1),0.5);
	createDock(Vector3(-34.0, 43.0, -5.0), 3000.0, Vector4(0.0, 1.0, 0.0, 1), 0.5);
	createDock(Vector3(-14.0, 52.0, -5.0), 3250.0, Vector4(0.0, 1.0, 0.5, 1), 0.5);
	createDock(Vector3(4.0, 43.0, -5.0), 3500.0, Vector4(0.0, 0.0, 1.0, 1), 0.5);
}
void Game::respawnScene()
{
	//inicializar los textos de la escena
	respawn_text = "Eso ha quemado. No te preocupes, aun te queda(n) " + to_string(lives) + " vida(s).";
	//reiniciar variables de respawn
	wait_restart = true;
	respawnTime = 0;
}
void Game::lostScene()
{
	lives = 0;
	points = 0;
	//inicializar los textos de la escena
	lost_text = "Has perdido todas tus vidas :( No te rindas, seguro que completas la carrera en otra convocatoria.";
	next_text = "Pulse c para repetir";
	exit_text = "Pulse q para salir";
}
void Game::winScene()
{
	//inicializar los textos de la escena
	win_text = "Enhorabuena, has completado CHAOS RUN con exito.";
	exit_text = "Pulse q para salir";
	// 
	//los dos primeros la fuerza de explosion tienen la misma k pero diferente masa 
	//todo el sistema con masa 1
	ParticlesSystem* system1 = new ParticlesSystem(Vector4(1.0, 0.0, 0.0, 1.0), Vector3(-70.0f, 30.0f, 30.0f), Vector3(0.0, 0.0, 0.0), 1, 0.2f, 0.2f, 0.0f, 1.0f);
	system1->set_u_Distribution(true); 
	system1->setUniformDistribPos(-3.0, 3.0); 
	system1->setUniformDistribVel(0.0, 0.0); 
	system1->setNormalDistribLifeTime(2.0, 1.0); 
	system1->setExplosionForce(10.0f, 1000.0, 0.5f); 
	//todo el sistema con masa 10
	ParticlesSystem* system2 = new ParticlesSystem(Vector4(0.0, 0.0, 1.0, 1.0), Vector3(50.0f, 40.0f, -10.0f), Vector3(0.0, 0.0, 0.0), 1, 0.2f, 0.2f, 0.0f, 10.0f);
	system2->set_u_Distribution(true); 
	system2->setUniformDistribPos(-3.0, 3.0); 
	system2->setUniformDistribVel(0.0, 0.0); 
	system2->setNormalDistribLifeTime(2.0, 1.0); 
	system2->setExplosionForce(10.0f, 1000.0, 0.5f); 
	//gestioanan la masa con una distribucion normal
	ParticlesSystem* system3 = new ParticlesSystem(Vector4(1.0, 0.0, 1.0, 1.0), Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0, 0.0, 0.0), 1, 0.2f, 0.2f, 0.0f, 0.0f);
	system3->set_u_Distribution(true);
	system3->setUniformDistribPos(-3.0, 3.0);
	system3->setUniformDistribVel(0.0, 0.0);
	system3->setNormalDistribLifeTime(2.0, 1.0);
	system3->setRandomMass(3.0, 10.0);
	system3->setExplosionForce(10.0f, 500.0, 0.5f);
	ParticlesSystem* system4 = new ParticlesSystem(Vector4(1.0, 0.5, 0.0, 1.0), Vector3(0.0f, 60.0f, 0.0f), Vector3(0.0, 0.0, 0.0), 1, 0.2f, 0.2f, 0.0f, 0.0f);
	system4->set_u_Distribution(true);
	system4->setUniformDistribPos(-3.0, 3.0);
	system4->setUniformDistribVel(0.0, 0.0);
	system4->setNormalDistribLifeTime(2.0, 1.0);
	system4->setRandomMass(3.0, 10.0);
	system4->setExplosionForce(10.0f, 1000.0, 0.5f);

	systems.push_back(system1);
	systems.push_back(system2);
	systems.push_back(system3);
	systems.push_back(system4);
}
void Game::clearScene()
{
	//limpia todos los elementos de la escena
	for (auto it = systems.begin(); it != systems.end(); ++it) {
		if (*it != nullptr) {
			delete* it;
		}
	}
	systems.clear();
	for (auto it = rigid_systems.begin(); it != rigid_systems.end(); ++it) {
		if (*it != nullptr) {
			delete* it;
		}
	}
	rigid_systems.clear();
	for (auto it = particles.begin(); it != particles.end(); ++it) {
		if (*it != nullptr) {
			delete* it;
		}
	}
	particles.clear();
	for (auto it = rigid_solids.begin(); it != rigid_solids.end(); ++it) {
		if (*it != nullptr) {
			delete* it;
		}
	}
	rigid_solids.clear();
	for (auto it = rigid_statics.begin(); it != rigid_statics.end(); ++it) {
		if (*it != nullptr) {
			delete* it;
		}
	}
	rigid_statics.clear();
	for (auto it = items.begin(); it != items.end(); ++it) {
		if (*it != nullptr) {
			DeregisterRenderItem(*it);
		}
	}
	items.clear();
}
void Game::changeScene()
{
	//gestiona el cambio de escenas
	//limpia la escena, actualiza a la escena actual, limpia los textos, llama al metodo de la escena que se debe instanciar
	clearScene();
	switch (current)
	{
	case START:
	{
		title_text = "";
		current = INTRO;
		introScene();
		break;
	}
	case INTRO:
	{
		intro_text = "";
		intro_text2 = "";
		next_text = "";
		current = SNOW;
		snowScene();
		break;
	}
	case SNOW:
	{
		points_text = "";
		if (diana) {
			delete diana;
			diana = nullptr;
		}
		current = INTRO2;
		intro2Scene();
		break;
	}
	case INTRO2:
	{
		next_text = "";
		intro_text3 = "";
		intro_text4 = "";
		input_text = "";
		current = LEVEL1;
		level1Scene();
		break;
	}
	case LEVEL1:
	{
		lives_text = "";
		//si no tiene vidas, la escena de derrota es la siguiente  
		if (lives <= 0) {
			current = LOST;
			lostScene();
		}
		//si debe pasar de nivel se pasa al segundo
		else if (next_level) {
			next_level = false;
			current = LEVEL2;
			level2Scene();
		}
		//si aun tiene vidas pero ha muerto, guarda el nivel actual para reaparecer
		else {
			last_scene = LEVEL1;
			current = RESPAWN;
			respawnScene();
		}
		break;
	}
	case LEVEL2:
	{
		lives_text = "";
		//si no tiene vidas, la escena de derrota es la siguiente  
		if (lives <= 0) {
			current = LOST;
			lostScene();
		}
		//si debe pasar de nivel se pasa al tercero
		else if (next_level) {
			next_level = false;
			current = LEVEL3;
			level3Scene();
		}
		//si aun tiene vidas pero ha muerto, guarda el nivel actual para reaparecer
		else {
			last_scene = LEVEL2;
			current = RESPAWN;
			respawnScene();
		}
		break;
	}
	case LEVEL3:
	{
		lives_text = "";
		//si no tiene vidas, la escena de derrota es la siguiente  
		if (lives <= 0) {
			current = LOST;
			lostScene();
		}
		//si ha pasado de nivel ha ganado
		else if (next_level) {
			next_level = false;
			current = WIN;
			winScene();
		}
		//si aun tiene vidas pero ha muerto, guarda el nivel actual para reaparecer
		else {
			last_scene = LEVEL3;
			current = RESPAWN;
			respawnScene();
		}
		break;
	}
	case RESPAWN:
	{
		respawn_text = "";
		current = last_scene;
		//reaparece en el nivel guardado como el ultimo
		if (current == LEVEL1) {
			level1Scene();
		}
		if (current == LEVEL2) {
			level2Scene();
		}
		if (current == LEVEL3) {
			level3Scene();
		}
		break;
	}
	case LOST:
	{
		//en la escena de derrota la siguiente vuelve a ser la del titulo
		lost_text = "";
		exit_text = "";
		current = START;
		titleScene();
		break;
	}
	default:
		break;
	}
}
void Game::setDiana()
{
	//aun no se ha llegado al maximo de dianas a spawnear
	if (cont_dianas < TOTAL_DIANAS) {
		//posicion aleatoria
		std::uniform_real_distribution<double> pos_z(MIN_Z, MAX_Z);
		std::uniform_real_distribution<double> pos_y(MIN_Y, MAX_Y);

		Vector3 pos = Vector3(-20, pos_y(mt), pos_z(mt));

		//si es la primera se crea
		if (diana == nullptr) {
			diana = new Diana(gPhysics, gScene, pos);
		}
		//las siguientes veces solo cambia de posicion
		else {
			diana->setTransform(pos);
		}
		//aumenta el contador
		cont_dianas++;
	}
	else {
		
		//ya han spawneado TOTAL_DIANAS, se reinicia las variables y se cambia de escena
		dianaElapsedTime = 0;
		changeScene();
	}
	
}
void Game::createPlayer()
{
	//crea el jugador como un solido rigido dinamico
	player = new RigidSolid(gPhysics, gScene, gMaterial,
		Vector3(-80, 45, 0), Vector3(0.0, 0.0, 0), Vector3(0, 0, 0), Vector3(2, 2, 2), Vector4(1.0, 0.0, 1.0, 1),
		10.0, 500, "BOX", "player");

	//se ajustan las propiedades del material
	player->setMaterialProperties(0.0f, 0.2f, 0.3f);
	GravityForceGenerator* g = new GravityForceGenerator(Vector3(0, -9.8, 0));
	player->addForceGenerator(g);
	rigid_solids.push_back(player);
}
void Game::createDock(const Vector3& pos, double k, const Vector4& color, double length)
{
	//se crea el cubo de anclaje
	RenderItem* cube = new RenderItem(CreateShape(physx::PxBoxGeometry(2, 2, 2)),
		new physx::PxTransform(pos), color);
	items.push_back(cube);

	Vector3 pos2 = pos;
	pos2.z += 10;
	//se crea el solido rigido al que le va a afectar la fuerza del muelle
	RigidSolid* s = new RigidSolid(gPhysics, gScene, gMaterial,
		pos2, Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(1.5, 1, 1), color,
		7, 500, "SPHERE", "muelle");
	rigid_solids.push_back(s);
	//generador de fuerza del muelle
	SpringForceGenerator* spring_generator = new SpringForceGenerator(k, length, pos);
	s->addForceGenerator(spring_generator);
	//bloquea el movimiento del solido los ejes x e y 
	s->getSolid()->setRigidDynamicLockFlags(
		PxRigidDynamicLockFlag::eLOCK_LINEAR_X |
		PxRigidDynamicLockFlag::eLOCK_LINEAR_Y
	);
}
void Game::handleMouse(int button, int state, int x, int y)
{
	//si puede disparar, se encuentra en la escena de nieve y se pulsa el boton izquierdo
	if (can_shoot && current == SNOW && button == 0) {
		if (cam != nullptr) {
			//se obtiene la posicion de la camara y donde apunta el raton
			Vector3 pos = cam->getEye();
			Vector3 dir = cam->getMousePos(x,y);

			float speed = 200.0f;
			//se calcula la velocidad
			physx::PxVec3 vel = dir * speed;
			//se crea un solido rigido dinamico que va a ser la bala/laser que se dispara desde la camara hacia donde ha clickado el raton
			RigidSolid* s = new RigidSolid(
				gPhysics, gScene, gMaterial,
				pos, vel, Vector3(0, 0, 0), Vector3(1, 1, 1), Vector4(0.0f, 0.0f, 0.0f, 1.0f), 2500.0f, 1.0f, "SPHERE", "laser");
			rigid_solids.push_back(s);
			//activa el cooldown 
			can_shoot = false;
			cooldownTime = 0;
		}
	}
}
void Game::keyPress(unsigned char key)
{
	switch (toupper(key))
	{
	//cambia de escena
	case 'C':
	{
		if (current != SNOW && current != LEVEL1 && current != LEVEL2 && current != LEVEL3) {
			changeScene();
		}
		
		break;
	}
	//el jugador avanza
	case 'D':
	{
		if (player && !dying) {
			Vector3 pos = player->getPosition();
			pos.x += 1.0f;
			player->setPosition(pos);
		}
		break;
	}
	//el jugador retrocede
	case 'A':
	{
		if (player && !dying) {
			Vector3 pos = player->getPosition();
			pos.x -= 1.0f;
			player->setPosition(pos);
		}
		break;
	}
	//el jugador salta
	case 'W':
	{
		if (player && canJump && !dying) {
			canJump = false;
			Vector3 vel = player->getLinealVel();
			vel.y = 20.0f;
			//puede avanzar o retroceder a la vez que salta
			if (GetAsyncKeyState('D')) {
				vel.x += 5.0f;
			}
			else if (GetAsyncKeyState('A')) {
				vel.x -= 5.0f;
			}
			player->setLinearVel(vel);
			
		}
		break;
	}
	default:
		break;
	}

}
void Game::onCollision(physx::PxActor* actor1, physx::PxActor* actor2)
{
	const char* name1 = actor1->getName();
	const char* name2 = actor2->getName();

	//gestiona colision entre el laser y la diana/centro de la diana
	if ((name1 && strcmp(name1, "laser") == 0 && name2 && strcmp(name2, "Diana") == 0) ||
		(name1 && strcmp(name1, "Diana") == 0 && name2 && strcmp(name2, "laser") == 0) ||
		(name1 && strcmp(name1, "laser") == 0 && name2 && strcmp(name2, "Diana_center") == 0) ||
		(name1 && strcmp(name1, "Diana_center") == 0 && name2 && strcmp(name2, "laser") == 0)) {

		if (name1 && strcmp(name1, "laser") == 0) {
			RigidSolid* s1 = static_cast<RigidSolid*>(actor1->userData);
			if (s1) s1->die();
		}
		if (name2 && strcmp(name2, "laser") == 0) {
			RigidSolid* s2 = static_cast<RigidSolid*>(actor2->userData);
			if (s2) s2->die();
		}
		//se suman los puntos correspondientes por acertar
		points += DIANA_POINTS;
		//se actualiza el texto que lleva el contador de puntos
		points_text = "Puntos: " + to_string(points);
	}
	//gestiona colision entre el jugador y las plataformas para saber si puede saltar
	if ((name1 && strcmp(name1, "player") == 0 && name2 && strcmp(name2, "plataforma") == 0) ||
		(name1 && strcmp(name1, "plataforma") == 0 && name2 && strcmp(name2, "player") == 0)) {
		
		canJump = true;
	}

	//si no esta muriendo ya, gestiona colision entre el jugador y la lava
	if (!dying && (name1 && strcmp(name1, "player") == 0 && name2 && strcmp(name2, "lava") == 0) ||
		(name1 && strcmp(name1, "lava") == 0 && name2 && strcmp(name2, "player") == 0)) {

		respawnTime = 0;	//empieza el contador de respawn
		dying = true;		//empieza a morir
		lives--;			//pierde una vida
		next_level = false;	//no pasa de nivel

		//se crea un sitema de particulas con distribuciones normales que simula al jugador quemandose en la lava
		ParticlesSystem* fire_system = new ParticlesSystem(Vector4(1.0, 0.5, 0.0, 1.0), player->getPosition(), Vector3(0, 0, 0), 20, 1.5f, 0.8f, 0.0f, 0.0f);
		fire_system->set_u_Distribution(false);
		fire_system->setNormalDistribPos(1.0, 0.5);
		fire_system->setNormalDistribVel(1.0, 0.5);
		fire_system->setNormalDistribLifeTime(1.0, 0.5);
		systems.push_back(fire_system);

		//hace al jugador inisible
		player->setInvisible();
		player->die();
	}
	//gestiona colision entre el jugador y la bandera que indica que ha superado el nivel y pasa al siguiente
	if ( !next_level && (name1 && strcmp(name1, "player") == 0 && name2 && strcmp(name2, "flag") == 0) ||
		(name1 && strcmp(name1, "flag") == 0 && name2 && strcmp(name2, "player") == 0)) {

		respawnTime = 0;
		next_level = true;
		wait_restart = true;
	}
}